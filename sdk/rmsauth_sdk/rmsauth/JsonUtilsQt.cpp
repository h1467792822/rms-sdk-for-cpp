/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "JsonUtilsQt.h"
#include <Exceptions.h>
#include <Logger.h>
#include <cstdlib>
#include <exception>

namespace rmsauth {

String JsonUtilsQt::getStringOrDefault(const nlohmann::json& qobj, const String& key, const String defval)
{
    if (qobj.contains(key))
    {
        auto res = qobj[key];
        if(!res.is_string())
        {
            Logger::error(Tag(), "getStringOrDefault: The value for the key '%' is not a string", key);
            throw RmsauthJsonParsingException("JsonUtilsQt::getStringOrDefault", "value is not a string");
        }

        return res.template get<std::string>();
    }
    return defval;
}

int JsonUtilsQt::getIntOrDefault(const nlohmann::json& qobj, const String& key, int defval)
{
    if (qobj.contains(key))
    {
        auto res = qobj[key];
        if(!res.is_number())
        {
            Logger::error(Tag(), "getIntOrDefault: The value for the key '%' is not int", key);
            throw RmsauthJsonParsingException("JsonUtilsQt::getIntOrDefault", "value is not int");
        }

        return res.template get<int>();
    }
    return defval;
}

int JsonUtilsQt::getStringAsIntOrDefault(const nlohmann::json& qobj, const String& key, int defval)
{
    if (qobj.contains(key))
    {
        auto res = qobj[key];

        if(res.is_number())
        {
            return res.template get<int>();
        }

        if(!res.is_string())
        {
            Logger::error(Tag(), "getStringAsIntOrDefault: The value for the key '%' is not a string", key);
            throw RmsauthJsonParsingException("JsonUtilsQt::getStringAsIntOrDefault", "value is not a string");
        }

        auto strRes = res.template get<std::string>();

	char* endptr = "";
	auto intRes = strtol(strRes.c_str(), &endptr, 10);

        if(strRes.empty() || *endptr != 0)
        {
            Logger::error(Tag(), "The value '%' for the key '%' can't be converted to int", strRes, key);
            throw RmsauthJsonParsingException("JsonUtilsQt::getStringAsIntOrZero", "value can't be converted to int");
        }

        return intRes;
    }
    return defval;
}

bool JsonUtilsQt::getBoolOrDefault(const nlohmann::json& qobj, const String& key, bool defval)
{
    if (qobj.contains(key))
    {
        auto res = qobj[key];
        if(!res.is_boolean())
        {
            Logger::error(Tag(), "getBoolOrDefault: The value for the key '%' is not bool", key);
            throw RmsauthJsonParsingException("JsonUtilsQt::getBoolOrDefault", "value is not bool");
        }

        return res.template get<bool>();
    }

    return defval;
}

IntArray JsonUtilsQt::getIntArrayOrEmpty(const nlohmann::json& qobj, const String& key)
{
    IntArray sa;
    if (qobj.contains(key))
    {
        auto val =  qobj[key];
        if(val.is_array())
        {
            for( size_t i = 0; i < val.size(); ++i)
            {
		auto elem = val[i];
                if(elem.is_number())
                {
                    sa.push_back( elem.template get<int>());
                }
                else
                {
                    StringStream ss;
                    ss << "getIntArrayOrEmpty: the array element for the key '" <<  key << "' in not an int";
                    throw RmsauthException(ss.str());
                }
            }
        }
        else if(val.is_null())
        {
            return std::move(sa);
        }
        else
        {
            StringStream ss;
            ss << "getIntArrayOrEmpty: the value for the key '" <<  key << "' in not an array";
            throw RmsauthException(ss.str());
        }
    }
    return std::move(sa);
}

void JsonUtilsQt::insertObject(nlohmann::json& qobj, const String& key, const String& jsonString)
{
    try {
        auto jobj = nlohmann::json::parse(jsonString);
	qobj[key] = jobj;
    }catch(std::exception& e)
    {
        throw RmsauthException(String("AuthenticationResult::deserialize: ") + e.what());
    }
}

void JsonUtilsQt::insertString(nlohmann::json& qobj, const String& key, const String& str)
{
   qobj[key] = str;
}

} // namespace rmsauth {
