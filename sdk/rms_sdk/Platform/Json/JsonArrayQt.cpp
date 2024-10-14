/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

//#ifdef QTFRAMEWORK
#include "JsonArrayQt.h"
#include "JsonObjectQt.h"
//#include <QJsonDocument>

namespace rmscore { namespace platform { namespace json {

std::shared_ptr<IJsonArray> IJsonArray::Create()
{
    return std::make_shared<JsonArrayQt>();
}

uint32_t JsonArrayQt::Size()
{
    return static_cast<uint32_t>(this->impl_.size());
}

const std::string JsonArrayQt::GetStringAt(uint32_t index)
{
    const nlohmann::json& val = this->impl_[index];
    return val.is_string() ? val.template get<std::string>() : "";
}

std::shared_ptr<IJsonObject> JsonArrayQt::GetObjectAt(uint32_t index)
{
    const nlohmann::json& val = this->impl_[index];
    return std::make_shared<JsonObjectQt>(val);
}

void JsonArrayQt::Clear()
{
    impl_.clear();
}

void JsonArrayQt::Append(const IJsonObject& jsonObject)
{
    auto jo = static_cast<const JsonObjectQt&>(jsonObject);
    impl_.push_back(jo.impl());
}

void JsonArrayQt::Append(const std::string& name)
{
    nlohmann::json json = name;
    impl_.push_back(json);
}

common::ByteArray JsonArrayQt::Stringify()
{
    auto res = impl_.dump();
    return common::ByteArray(res.begin(), res.end());

}

}}} // namespace rmscore { namespace platform { namespace json {

