/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <TokenCache.h>

#define NOQT 1

#if NOQT
#include <vector>
#include <string>
#else
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#endif
namespace rmsauth {

#if NOQT
    static std::vector<uint8_t> Int2Uint8(int number)
    {
        std::vector<uint8_t> result;
        uint32_t temp;
        temp = static_cast<uint32_t>(number);
        for (int i = 0; i < 4; ++i)
        {
            result.push_back((number >> (24 - i * 8)) & 0xFF);
        }
        return result;
    }

    static std::vector<uint8_t> Str2Uint8(const std::string str)
    {
        size_t len = str.length();
        std::vector<uint8_t> size = Int2Uint8(static_cast<int>(len));
        std::vector<uint8_t> result(size.begin(), size.end());
        for (const char& ch : str)
        {
            result.push_back(static_cast<uint8_t>(ch));
        }
        return result;
    }

    static void ConcatenationVector(std::vector<uint8_t>& allinfo, const std::vector<uint8_t>& other)
    {
        for (const uint8_t& value : other)
        {
            allinfo.push_back(value);
        }
    }

    static uint32_t GetIntNumber(const std::vector<uint8_t>& info, int& indexPos)
    {
        uint32_t result = 0;
        for (int i = 0; i < 4; ++i)
        {
            result |= static_cast<uint32_t>(info[indexPos]) << (24 - i * 8);
            indexPos++;
        }
        return result;
    }

    static std::string GetString(const std::vector<uint8_t>& info, int& indexPos)
    {
        uint32_t len = GetIntNumber(info, indexPos);
        std::string result("");
        for (uint32_t i = indexPos; i < indexPos + len; ++i)
        {
            result.append(1, info[i]);
        }
        indexPos += len;
        return result;
    }
#endif
    /* ---------------------------------------------------------- */
    ByteArray TokenCache::serialize()
        {
        Logger::info(Tag(), "serialize start");
#if !NOQT
        QByteArray qba;

        QDataStream qds(&qba, QIODevice::WriteOnly);
#endif
        Logger::info(Tag(), "Serializing token cache with % items.", tokenCacheDictionary_.size());
#if !NOQT
        qds << SchemaVersion_;
        qds << count();
        for( auto kvp : tokenCacheDictionary_)
        {
            auto tokenCacheKey = kvp.first;
            AuthenticationResultPtr authenticationResultPtr = kvp.second;

            qds << QString::fromStdString(tokenCacheKey.authority());
            qds << QString::fromStdString(tokenCacheKey.resource());
            qds << QString::fromStdString(tokenCacheKey.clientId());
            qds << static_cast<int>(tokenCacheKey.tokenSubjectType());

            qds << QString::fromStdString(authenticationResultPtr->serialize());
        }

        ByteArray ba(qba.begin(), qba.end());
        return std::move(ba);
#endif
#if NOQT
        std::vector<uint8_t> vctVer = Int2Uint8(SchemaVersion_);
        std::vector<uint8_t> allInfo(vctVer.begin(), vctVer.end());
        std::vector<uint8_t> vctCnt = Int2Uint8(count());
        ConcatenationVector(allInfo, vctCnt);

        for (auto kvp : tokenCacheDictionary_)
        {
            auto tokenCacheKey = kvp.first;
            AuthenticationResultPtr authenticationResultPtr = kvp.second;

            std::vector<uint8_t> authority = Str2Uint8(tokenCacheKey.authority());
            std::vector<uint8_t> resource = Str2Uint8(tokenCacheKey.resource());
            std::vector<uint8_t> clientId = Str2Uint8(tokenCacheKey.clientId());
            std::vector<uint8_t> tokenSubjectType = Int2Uint8(static_cast<int>(tokenCacheKey.tokenSubjectType()));
            std::vector<uint8_t> authenticationResultString = Str2Uint8(authenticationResultPtr->serialize());
            ConcatenationVector(allInfo, authority);
            ConcatenationVector(allInfo, resource);
            ConcatenationVector(allInfo, clientId);
            ConcatenationVector(allInfo, tokenSubjectType);
            ConcatenationVector(allInfo, authenticationResultString);
        }
        ByteArray ba(allInfo.begin(), allInfo.end());
        Logger::info(Tag(), "serialize finished");
        return std::move(ba);
#endif
    }

    void TokenCache::deserialize(const ByteArray& state)
    {
        Logger::info(Tag(), "deserialize");
        if (state.empty())
        {
           tokenCacheDictionary_.clear();
           Logger::warning(Tag(), "The state empty.");
           return;
        }
#if !NOQT
        QByteArray qba(state.data(), (int)state.size());
        QDataStream qds(&qba, QIODevice::ReadOnly);

        int schemaVersion;
        qds >> schemaVersion;
        if (schemaVersion != SchemaVersion_)
        {
            Logger::warning(Tag(), "The version of the persistent state of the cache does not match the current schema, so skipping deserialization.");
            return;
        }

        int count;
        qds >> count;

        for(int i=0; i<count; ++i)
        {
            QString authority;
            QString resource;
            QString clientId;
            int tokenSubjectType;

            qds >> authority;
            qds >> resource;
            qds >> clientId;
            qds >> tokenSubjectType;

            QString authenticationResultString;
            qds >> authenticationResultString;
            AuthenticationResultPtr result = AuthenticationResult::deserialize(authenticationResultString.toStdString());

            if(result == nullptr)
            {
                throw RmsauthParsingException("AuthenticationResult::deserialize returned nullptr");
            }

            TokenCacheKey tokenCacheKey(
                authority.toStdString()
                ,resource.toStdString()
                ,clientId.toStdString()
                ,TokenSubjectType(tokenSubjectType)
                ,result->userInfo());


            tokenCacheDictionary_.insert(std::make_pair(std::move(tokenCacheKey), result));
        }
#endif
#if NOQT
        const std::vector<uint8_t> info(state.begin(), state.end());

        int curIndex = 0;
        uint32_t schemaVersion = GetIntNumber(info, curIndex);
        if (schemaVersion != SchemaVersion_)
        {
            Logger::warning(Tag(), "The version of the persistent state of the cache does not match the current schema, so skipping deserialization.");
            return;
        }
        uint32_t count = GetIntNumber(info, curIndex);
        for (uint32_t i = 0; i < count; ++i)
        {
            std::string authority = GetString(info, curIndex);
            std::string resource = GetString(info, curIndex);
            std::string clientId = GetString(info, curIndex);
            int tokenSubjectType = GetIntNumber(info, curIndex);
            std::string authenticationResultString = GetString(info, curIndex);
            AuthenticationResultPtr result = AuthenticationResult::deserialize(authenticationResultString);
            if (result == nullptr)
            {
                throw RmsauthParsingException("AuthenticationResult::deserialize returned nullptr");
            }
            TokenCacheKey tokenCacheKey(
                authority
                , resource
                , clientId
                , TokenSubjectType(tokenSubjectType)
                , result->userInfo());
            tokenCacheDictionary_.insert(std::make_pair(std::move(tokenCacheKey), result));
        }
#endif
        Logger::info(Tag(), "Deserialized % items to token cache.", this->count());
        if (count != this->count())
        {
            Logger::warning(Tag(), "Deserialized Token Not Fully Cached.");
        }
        Logger::info(Tag(), "deserialized finished");
    }

} // namespace rmsauth {
