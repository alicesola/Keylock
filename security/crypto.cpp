#include "crypto.hpp"
#include <stdexcept>

Bytes deriveKey(const std::string &password, const std::array<uint8_t, kSaltLen> &salt)
{
    if (password.empty())
        throw std::invalid_argument("Password cannot be empty");
    Bytes key(kKeyLen);
    if (argon2id_hash_raw(3, 1 << 16, 1, password.data(), password.size(), salt.data(), salt.size(), key.data(), key.size()) != ARGON2_OK)
        throw std::runtime_error("Argon2 failed");
    return key;
}
Bytes encrypt(const std::string &plainText, const std::string &password)
{
    // 生成随机盐和向量等
    std::array<uint8_t, kSaltLen> salt;
    std::array<uint8_t, kIvLen> iv;
    RAND_bytes(salt.data(), salt.size());
    RAND_bytes(iv.data(), iv.size());

    // 派生密钥
    Bytes key = deriveKey(password, salt);

    // AES-256-GCM
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw std::runtime_error("CTX_new");
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("EncryptInit");

    Bytes cipher(plainText.size());
    int len;
    if (EVP_EncryptUpdate(ctx, cipher.data(), &len, reinterpret_cast<const uint8_t *>(plainText.data()), plainText.size()) != 1)
        throw std::runtime_error("EncryptUpdate");
    size_t cipher_len = len;
    if (EVP_EncryptFinal_ex(ctx, cipher.data() + cipher_len, &len) != 1)
        throw std::runtime_error("EncryptFinal");
    cipher_len += len;
    cipher.resize(cipher_len);

    Bytes tag(kTagLen);
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, kTagLen, tag.data()) != 1)
        throw std::runtime_error("GET_TAG");
    EVP_CIPHER_CTX_free(ctx);

    // 打包盐，向量，标签等
    Bytes out;
    out.reserve(kSaltLen + kIvLen + kTagLen + cipher.size());
    out.insert(out.end(), salt.begin(), salt.end());
    out.insert(out.end(), iv.begin(), iv.end());
    out.insert(out.end(), tag.begin(), tag.end());
    out.insert(out.end(), cipher.begin(), cipher.end());
    return out;
}
std::string decrypt(const Bytes &blob, const std::string &password)
{
    if (blob.size() < kSaltLen + kIvLen + kTagLen)
        throw std::runtime_error("file too small");
    // 解包（终于理解解包是什么意思啦）
    std::array<uint8_t, kSaltLen> salt;
    std::array<uint8_t, kIvLen> iv;
    std::array<uint8_t, kTagLen> tag;
    std::copy_n(blob.begin(), kSaltLen, salt.begin());
    std::copy_n(blob.begin() + kSaltLen, kIvLen, iv.begin());
    std::copy_n(blob.begin() + kSaltLen + kIvLen, kTagLen, tag.begin());
    Bytes cipher(blob.begin() + kSaltLen + kIvLen + kTagLen, blob.end());

    // 派生同一密钥
    Bytes key = deriveKey(password, salt);
    // 同算法解密
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw std::runtime_error("CTX_new");
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("DecryptInit");
    Bytes plain(cipher.size());
    int len;
    if (EVP_DecryptUpdate(ctx, plain.data(), &len, cipher.data(), cipher.size()) != 1)
        throw std::runtime_error("DecryptUpdate");
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, kTagLen,
                             const_cast<uint8_t *>(tag.data())))
        throw std::runtime_error("SET_TAG");

    if (EVP_DecryptFinal_ex(ctx, plain.data() + len, &len) != 1)
        throw std::runtime_error("DecryptFinal: wrong password or corrupted data");
    EVP_CIPHER_CTX_free(ctx);

    plain.resize(len);
    return std::string(plain.begin(), plain.end());
}