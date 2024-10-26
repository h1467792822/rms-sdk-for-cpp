/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <RequestParameters.h>
// #include <QUrl>
#include <string>

static unsigned char toHex(unsigned char num) {
    return num >9 ? num + 55 : num + 48;
}

static std::string encode(const std::string& str) {
    std::string tmp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char)str[i]) ||
        (str[i] == '-') ||
        (str[i] == '_') ||
        (str[i] == '.') ||
        (str[i] == '~')) {
            tmp += str[i];
        } else {
            tmp += '%';
            tmp += toHex((unsigned char)str[i] >> 4);
            tmp += toHex((unsigned char)str[i] % 16);
        }
    }
    return tmp;
}

namespace rmsauth {

String RequestParameters::uriEncode(const String& value)
{
    // auto encodedData = QUrl::toPercentEncoding(QString::fromStdString(value));
    // return String(encodedData.data(), encodedData.size());
    return encode(value);
}

} // namespace rmsauth {
