
/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "CryptoHash.h"
#include <stdexcept> // 添加此头文件以使用 std::runtime_error

namespace rmscrypto {
namespace platform {
namespace crypto {

CryptoHash::CryptoHash(const EVP_MD* hashAlgorithm)
    : m_hashAlgorithm(hashAlgorithm), m_ctx(EVP_MD_CTX_new()) {
    if (!m_ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX"); // 使用 std::runtime_error 抛出异常
    }
    EVP_DigestInit_ex(m_ctx, m_hashAlgorithm, nullptr);
}

CryptoHash::~CryptoHash() {
    EVP_MD_CTX_free(m_ctx); // 释放上下文
}

size_t CryptoHash::GetOutputSize() {
    return EVP_MD_size(m_hashAlgorithm); // 获取哈希输出大小
}

void CryptoHash::Hash(const uint8_t *pbIn, uint32_t cbIn,
                      uint8_t *pbOut, uint32_t &cbOut) {
    // 更新哈希数据
    if (EVP_DigestUpdate(m_ctx, pbIn, cbIn) != 1) {
        throw exceptions::RMSCryptoInvalidArgumentException("Failed to update hash");
    }

    // 完成哈希计算
    if (EVP_DigestFinal_ex(m_ctx, pbOut, nullptr) != 1) {
        throw exceptions::RMSCryptoInvalidArgumentException("Failed to finalize hash");
    }

    cbOut = GetOutputSize(); // 设置输出大小

    // 重新初始化上下文以便进行下一次哈希操作
    EVP_DigestInit_ex(m_ctx, m_hashAlgorithm, nullptr);
}

} // namespace crypto
} // namespace platform
} // namespace rmscrypto
