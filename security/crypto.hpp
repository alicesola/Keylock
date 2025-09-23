#pragma once
#include <argon2.h>
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
