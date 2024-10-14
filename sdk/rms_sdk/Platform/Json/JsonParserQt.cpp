/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <nlohmann/json.hpp>
#include "JsonParserQt.h"
#include "JsonObjectQt.h"
#include "JsonArrayQt.h"
#include "../Logger/Logger.h"

using namespace rmscore::platform::logger;

namespace rmscore {
namespace platform {
namespace json {
std::shared_ptr<IJsonParser>IJsonParser::Create()
{
  return std::make_shared<JsonParserQt>();
}

std::shared_ptr<IJsonObject>JsonParserQt::Parse(
  const common::ByteArray& jsonObject)
{
  nlohmann::json val;
  try {
      val = nlohmann::json::parse(jsonObject);
  }
  catch(std::exception& e)
  {
    Logger::Error("JsonParserQt::Parse: %s", e.what());
    return nullptr;
  }

  if (!val.is_object())
  {
    Logger::Error("JsonParserQt::Parse: %s", "given json is not a json object");
    return nullptr;
  }
  return std::make_shared<JsonObjectQt>(val);
}

std::shared_ptr<IJsonArray>JsonParserQt::ParseArray(
  const common::ByteArray& jsonArray)
{
  nlohmann::json val;
  try {
      val = nlohmann::json::parse(jsonArray);
  }
  catch(std::exception& e)
  {
    Logger::Error("JsonParserQt::ParseArray: %s", e.what());
    return nullptr;
  }

  if (!val.is_array())
  {
    Logger::Error("JsonParserQt::Parse: %s", "given json is not a json array");
    return nullptr;
  }
  return std::make_shared<JsonArrayQt>(val);
}
}
}
} // namespace rmscore { namespace platform { namespace json {
