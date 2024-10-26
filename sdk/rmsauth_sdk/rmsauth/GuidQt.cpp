/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "GuidQt.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace rmsauth {

#if 0
Guid::Guid()
{
    this->pImpl = std::make_shared<GuidQt>();
}

Guid::Guid(const String& str)
{
    this->pImpl = std::make_shared<GuidQt>(QString::fromStdString(str));
}

Guid::Guid(ptr<IGuid> guid)
{
    this->pImpl = guid;
}

Guid Guid::newGuid()
{
    auto pImpl = std::make_shared<GuidQt>(QUuid::createUuid());
    return Guid(pImpl);
}

GuidQt::GuidQt()
{
}

GuidQt::GuidQt(const QUuid& uuid):uuid_(uuid)
{
}
GuidQt::GuidQt(const QString& str):uuid_(str)
{
}

String GuidQt::toString() const
{
    return this->uuid_.toString().toStdString();
}

 bool GuidQt::empty() const
 {
     return this->uuid_.isNull();
 }

#endif

Guid::Guid()
{
    this->pImpl = std::make_shared<GuidQt>();
}

Guid::Guid(const String& str)
{
    this->pImpl = std::make_shared<GuidQt>(str);
}

Guid::Guid(ptr<IGuid> guid)
{
    this->pImpl = guid;
}

Guid Guid::newGuid()
{
    auto pImpl = std::make_shared<GuidQt>(GuidQt::newGuid());
    return Guid(pImpl);
}

GuidQt::GuidQt()
{
    std::fill(data.begin(), data.end(), 0);
}

GuidQt::GuidQt(const std::string& str)
{
    parse(str);
}

void GuidQt::parse(const std::string& str)
{
    std::stringstream ss(str);
    int i = 0;
    unsigned int component;
    while (ss >> std::hex >> component) {
        if (i >= 16) break;
        data[i++] = static_cast<unsigned char>(component);
        if (ss.peek() == '-') ss.ignore();
    }
}

std::string GuidQt::toString() const
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (i == 4 || i == 6 || i == 8 || i == 10)
            ss << '-';
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

bool GuidQt::empty() const
{
    return std::all_of(data.begin(), data.end(), [](unsigned char c) { return c == 0; });
}

GuidQt GuidQt::newGuid()
{
    GuidQt guid;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (auto& byte : guid.data)
    {
        byte = static_cast<unsigned char>(dis(gen));
    }

    // Set version to 4
    guid.data[6] = (guid.data[6] & 0x0F) | 0x40;
    // Set variant to RFC4122
    guid.data[8] = (guid.data[8] & 0x3F) | 0x80;

    return guid;
}

} // namespace rmsauth {
