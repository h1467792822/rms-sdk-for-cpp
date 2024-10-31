/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef _CRYPTO_STREAMS_LIB_CRYPTOHASH_
#define _CRYPTO_STREAMS_LIB_CRYPTOHASH_
#include <memory>
#include <openssl/evp.h>
#include "../../CryptoAPI/ICryptoHash.h"
#include "../../CryptoAPI/RMSCryptoExceptions.h"

namespace rmscrypto {
namespace platform {
namespace crypto {
  
class CryptoHash : public api::ICryptoHash {
public:
    // 构造函数，接受 OpenSSL 的哈希算法
    CryptoHash(const EVP_MD* hashAlgorithm);

    virtual size_t GetOutputSize() override;
    virtual void Hash(const uint8_t *pbIn,
                      uint32_t cbIn,
                      uint8_t *pbOut,
                      uint32_t &cbOut) override;

    ~CryptoHash(); // 析构函数

private:
    const EVP_MD* m_hashAlgorithm; // 存储哈希算法
    EVP_MD_CTX* m_ctx; // OpenSSL 哈希上下文
};
} // namespace crypto
} // namespace platform
} // namespace rmscrypto
#endif // _CRYPTO_STREAMS_LIB_CRYPTOHASH_

