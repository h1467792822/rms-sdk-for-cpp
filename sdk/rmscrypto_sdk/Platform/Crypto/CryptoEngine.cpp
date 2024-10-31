/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <string>
#include <memory>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "CryptoEngine.h"
#include "../../CryptoAPI/RMSCryptoExceptions.h"
using namespace std;

namespace rmscrypto {
namespace api {
shared_ptr<ICryptoEngine>ICryptoEngine::Create() {
  return make_shared<platform::crypto::CryptoEngine>();
}
}
namespace platform {
namespace crypto {
static const EVP_MD* MapHashAlgorithm(api::CryptoHashAlgorithm algorithm) {
    switch (algorithm) {
    case api::CRYPTO_HASH_ALGORITHM_SHA1:
        return EVP_sha1();  // 返回 SHA1 的 OpenSSL 哈希方法

    case api::CRYPTO_HASH_ALGORITHM_SHA256:
        return EVP_sha256(); // 返回 SHA256 的 OpenSSL 哈希方法

    default:
        throw exceptions::RMSCryptoInvalidArgumentException("Invalid algorithm");
    }
}

shared_ptr<api::ICryptoKey>CryptoEngine::CreateKey(const uint8_t       *pbKey,
                                                   uint32_t             cbKey,
                                                   api::CryptoAlgorithm algorithm)
{
  if ((algorithm == api::CRYPTO_ALGORITHM_AES_ECB) ||
      (algorithm == api::CRYPTO_ALGORITHM_AES_CBC) ||
      (algorithm == api::CRYPTO_ALGORITHM_AES_CBC_PKCS7)) {
    return make_shared<AESCryptoKey>(pbKey, cbKey, algorithm);
  }

  // no algorithm
  throw exceptions::RMSCryptoInvalidArgumentException("Invalid algorithm");
}

shared_ptr<api::ICryptoHash> CryptoEngine::CreateHash(api::CryptoHashAlgorithm algorithm) {
    const EVP_MD* evpMd = MapHashAlgorithm(algorithm); // 获取 OpenSSL 哈希方法
    return make_shared<CryptoHash>(evpMd); // 创建 CryptoHash 对象，传入 OpenSSL 哈希方法
}

} // namespace crypto
} // namespace platform
} // namespace rmscrypto

