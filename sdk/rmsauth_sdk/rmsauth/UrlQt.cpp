/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "UrlQt.h"
#include <Url.h>
#include <regex>

using namespace std;
namespace rmsauth {

Url::Url() : pImpl(std::make_shared<UrlQt>())
{
}

Url::Url(const String& url) : pImpl(std::make_shared<UrlQt>(url.data()))
{
}

UrlQt::UrlQt()
{
}

UrlQt::UrlQt(const String& url) : url_(url.data())
{
}

void UrlQt::setUrl(const String& url)
{
    this->url_ = url;
}

String UrlQt::toString() const
{
    return this->url_;
}
String matches(const String& url, const String& regularExpression) 
{
    smatch matches;
    regex pattern(regularExpression);
    if (regex_search(url, matches, pattern) && matches.size() > 1) {
        return matches[1].str();
    }
    return "";
}
String UrlQt::scheme() const
{
    string regularExpression = R"(^([a-zA-Z]+):)";
    return matches(this->url_, regularExpression);
}
String UrlQt::authority() const
{
    string regularExpression = R"(^[^:]+://([^/]+)(/.*)?)";
    return matches(this->url_, regularExpression);
}
String UrlQt::host() const
{
    string regularExpression = R"(^[^:]+://([^/:]+)(:[\d]+)?([^#?\s]+))";
    return matches(this->url_, regularExpression);
}
String UrlQt::fragment() const
{
    string regularExpression = R"(#([^#?]*))";
    return matches(this->url_, regularExpression);
}
String UrlQt::path() const
{
    string regularExpression = R"(^[^:]+://[^\/]+(\/[^?#]*)(?:\?[^#]*)?(?:#.*)?$)";
    return matches(this->url_, regularExpression);
}
bool UrlQt::isValid() const
{
    regex pattern("^(https?://)?([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z0-9]{2,6}"
        "(:[0-9]{1,5})?(/.*)?$");
    return std::regex_match(this->url_, pattern);
}

} // namespace rmsauth {
