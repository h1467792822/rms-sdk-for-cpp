/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <utils.h>
#include <openssl/sha.h>
#include <string>

                  
namespace rmsauth {

String HashUtils::createSha256Hash(const String& token)
{
   
    // 存储 SHA-256 哈希值的数组
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    // 初始化并计算 SHA-256 哈希
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, token.data(), token.size());
    SHA256_Final(hash, &sha256);

    // 将哈希结果转为 String 类型返回
    return String(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

}

} // namespace rmsauth