/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifdef QTFRAMEWORK
#include "UriQt.h"
#include <regex>

namespace rmscore { namespace platform { namespace http {

#if 0
std::shared_ptr<IUri> IUri::Create(const std::string& uri)
{
    return std::make_shared<UriQt>(uri);
}
const std::string UriQt::GetScheme() const
{
    return this->pImpl_->scheme().toStdString();
}
const std::string UriQt::GetHost() const
{
    return this->pImpl_->host().toStdString();
}
int UriQt::GetPort() const
{
    return this->pImpl_->port();
}
const std::string UriQt::ToString()const
{
    return this->pImpl_->toString().toStdString();
}
#endif

std::shared_ptr<IUri> IUri::Create(const std::string& uri)
{
    return std::make_shared<UriQt>(uri);
}

UriQt::UriQt(const std::string& uri) : uri_(uri), port_(-1) {
    // 使用正则表达式解析URI
    std::regex uri_regex("^(?:([^:/?#]+):)?(?://([^/?#]*))?");
    std::smatch matches;
    
    if (std::regex_search(uri, matches, uri_regex)) {
        if (matches.size() > 1) scheme_ = matches[1];
        if (matches.size() > 2) {
            std::string authority = matches[2];
            auto pos = authority.find(':');
            if (pos != std::string::npos) {
                host_ = authority.substr(0, pos);
                try {
                    port_ = std::stoi(authority.substr(pos + 1));
                } catch (...) {
                    port_ = -1;
                }
            } else {
                host_ = authority;
            }
        }
    }
}

UriQt::~UriQt() = default;

const std::string UriQt::GetScheme() const {
    return scheme_;
}

const std::string UriQt::GetHost() const {
    return host_;
}

int UriQt::GetPort() const {
    return port_;
}

const std::string UriQt::ToString() const {
    return uri_;
}

}}} // namespace rmscore { namespace platform { namespace http {
#endif


