/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef JSONUTILSQT_H
#define JSONUTILSQT_H

#include <types.h>
#include <Exceptions.h>
#include <nlohmann/json.hpp>

namespace rmsauth {

class JsonUtilsQt
{
    static const String Tag(){static const String tag = "JsonUtilsQt"; return tag;}
    template<typename P>
    using Deserializer = P (*)(const String& jsonString);

public:
    static String getStringOrDefault(const nlohmann::json& qobj, const String& key, const String defval = "");
    static int getIntOrDefault(const nlohmann::json& qobj, const String& key, int defval = 0 );
    static int getStringAsIntOrDefault(const nlohmann::json& qobj, const String& key, int defval = 0);
    static bool getBoolOrDefault(const nlohmann::json& qobj, const String& key, bool defval = false);
    static IntArray getIntArrayOrEmpty(const nlohmann::json& qobj, const String& key);
    template<typename T>
    static T parseObject(const nlohmann::json& qobj, const String& key, Deserializer<T> d);
    static void insertObject(nlohmann::json& qobj, const String& key, const String& jsonString);
    static void insertString(nlohmann::json& qobj, const String& key, const String& str);
};

template<typename P>
P JsonUtilsQt::parseObject(const nlohmann::json& qobj, const String& key, Deserializer<P> deserialize)
{
    if (!qobj.contains(key))
    {
        return nullptr;
    }

    return deserialize(qobj[key].dump());
}

} // namespace rmsauth {

#endif // JSONUTILSQT_H
