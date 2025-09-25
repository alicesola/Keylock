#pragma once
#include <argon2.h>
#include <algorithm>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <array>
#include <string>
#include <vector>

//参数
constexpr std::size_t kKeyLen = 32; //AES-256
constexpr std::size_t kIvLen = 12;
constexpr std::size_t kTagLen = 16; //认证标签
constexpr std::size_t kSaltLen = 16;//盐长度

using Bytes = std::vector<unsigned char>;

Bytes deriveKey(const std::string &password, const std::array<uint8_t, kSaltLen> &salt);

Bytes encrypt(const std::string &planinText, const std::string &password);

std::string decrypt(const Bytes &blob, const std::string &password);
