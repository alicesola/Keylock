#include "crypto.hpp"
#include <stdexcept>


Bytes deriveKey(const std::string&password,const std::array<uint8_t,kSaltLen>&salt){
    Bytes key(kKeyLen);
    if(argon2id_hash_raw(3,1<<16,1,password.data(),password.size(),salt.data(),salt.size(),key.data(),key.size())!=ARGON2_OK)
        throw std::runtime_error("Argon2 failed");
    return key;
}