/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#include "JsonObjectQt.h"
#include "JsonArrayQt.h"
#include "JsonParserQt.h"

#include "../../ModernAPI/RMSExceptions.h"
#include "../Logger/Logger.h"

using namespace rmscore::platform::logger;

namespace rmscore {
namespace platform {
namespace json {
std::shared_ptr<IJsonObject>IJsonObject::Create()
{
  return std::make_shared<JsonObjectQt>();
}

bool JsonObjectQt::IsNull(const std::string& name)
{
  return impl_.contains(name) ? impl_[name].is_null()
         : throw exceptions::RMSInvalidArgumentException(
                 "JsonObjectQt::IsNull: Name doesn't exist");
}

std::shared_ptr<IJsonArray>JsonObjectQt::GetArray()
{
  if (!this->impl_.is_array())
  {
    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetArray: The object is not an array");
  }
  return std::shared_ptr<IJsonArray>(impl_);
}

bool JsonObjectQt::HasName(const std::string& name)
{
  return impl_.contains(name);
}

std::string JsonObjectQt::GetNamedString(const std::string& name,
                                         const std::string& defaultValue)
{
  if (!impl_.contains(name)) {
     return defaultValue;
  }
  auto val = impl_[name];
  if (val.is_null()) {
     return defaultValue;
  }
  if (val.is_string()) {
     return val.template get<std::string>();
  }

  throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedString: convertion error");
}

void JsonObjectQt::SetNamedString(const std::string& name,
                                  const std::string& value)
{
  impl_[name] = value;
}

bool JsonObjectQt::GetNamedBool(const std::string& name, bool bDefaultValue)
{
  if (!impl_.contains(name)) {
     return bDefaultValue;
  }
  auto val = impl_[name];
  if (val.is_null()) {
     return bDefaultValue;
  }
  if (val.is_boolean()) {
     return val.template get<bool>();
  }

    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedBool: convertion error");
}

void JsonObjectQt::SetNamedBool(const std::string& name, bool bValue)
{
  impl_[name] = bValue;
}

double JsonObjectQt::GetNamedNumber(const std::string& name, double fDefaultValue)
{
  if (!impl_.contains(name)) {
     return fDefaultValue;
  }
  auto val = impl_[name];
  if (val.is_null()) {
     return fDefaultValue;
  }
  if (val.is_number()) {
     return val.template get<double>();
  }

  throw exceptions::RMSInvalidArgumentException(
        "JsonObjectQt::GetNamedNumber: convertion error");
}

void JsonObjectQt::SetNamedNumber(const std::string& name, double fValue)
{
  impl_[name] = fValue;
}

std::shared_ptr<IJsonObject>JsonObjectQt::GetNamedObject(const std::string& name)
{
  if (!impl_.contains(name)) {
     return nullptr;
  }
  auto val = impl_[name];
  if (!val.is_object())
  {
    throw exceptions::RMSInvalidArgumentException(
            "JsonObjectQt::GetNamedObject: convertion error");
  }

  return std::make_shared<JsonObjectQt>(val);
}

void JsonObjectQt::SetNamedObject(const std::string& name,
                                  const IJsonObject& jsonObject)
{
  auto jo = static_cast<const JsonObjectQt&>(jsonObject);
  impl_[name] = jo.impl();
}

std::shared_ptr<IJsonArray>JsonObjectQt::GetNamedArray(const std::string& name)
{
  if (!impl_.contains(name)) {
     return nullptr;
  }
  auto val = impl_[name];
  if (!val.is_array())
  {
    throw exceptions::RMSInvalidArgumentException("the value is not an array");
  }

  return std::make_shared<JsonArrayQt>(val);
}

void JsonObjectQt::SetNamedArray(const std::string& name,
                                 const IJsonArray & jsonArray)
{
  auto jo = static_cast<const JsonArrayQt&>(jsonArray);
  impl_[name] = jo.impl();
}

void JsonObjectQt::SetNamedValue(const std::string      & name,
                                 const common::ByteArray& value) {
  impl_[name] = value;
}

common::ByteArray JsonObjectQt::GetNamedValue(const std::string& name)
{
  // nlohmann的is_string只支持utf8
  if (!impl_.contains(name)) {
     return common::ByteArray();
  }

  auto val = impl_[name];
  if (val.is_string() || val.is_binary()) {
     return common::ByteArray(val.begin(), val.end());
  }

  return common::ByteArray();
}

StringArray JsonObjectQt::GetNamedStringArray(const std::string& name)
{
  if (!impl_.contains(name))
  {
    Logger::Warning(
      "JsonObjectQt::GetNamedStringArray: Can't find the key named '%s'",
      name.c_str());
    return std::vector<std::string>();
  }

  auto array = impl_[name];

  if (!array.is_array())
  {
    throw exceptions::RMSInvalidArgumentException("the value is not an array");
  }

  StringArray list;

  for (size_t i = 0; i < array.size(); ++i)
  {
    //QJsonValue::toString: if !isString return null QString
    auto val = array[i];
    list.push_back(val.is_string() ? val.template get<std::string>() : std::string());
  }

  return list;
}

modernapi::AppDataHashMap JsonObjectQt::ToStringDictionary()
{
  modernapi::AppDataHashMap result;

  if (!impl_.is_object()) {
      return result;
  }

  for (auto it = impl_.begin(); it != impl_.end(); ++it) {
      auto val = it.value();
    //QJsonValue::toString: if !isString return null QString
      if (val.is_string()) {
          result.insert(std::make_pair(it.key(), val.template get<std::string>()));
      } else {
          result.insert(std::make_pair(it.key(), std::string()));
      }
  }

  return result;
}

common::ByteArray JsonObjectQt::Stringify()
{
  auto res = impl_.dump();

  return common::ByteArray(res.begin(), res.end());
}
}
}
} // namespace rmscore { namespace platform { namespace json {
