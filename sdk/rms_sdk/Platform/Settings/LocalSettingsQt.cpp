/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "LocalSettingsQt.h"

namespace rmscore {
namespace platform {
namespace settings {

std::shared_ptr<ILocalSettings> ILocalSettings::Create(const std::string& filename)
{
    return std::make_shared<LocalSettingsQt>(filename);
}

LocalSettingsQt::LocalSettingsQt(const std::string& filename) :
  impl_(filename) {}

std::string LocalSettingsQt::GetString(const std::string& container,
                                            const std::string& name,
                                            const std::string& defaultValue)
{
  std::string key(container.empty() ? name : container + "/" + name);

  return this->impl_.getValue(key, defaultValue);
}

bool LocalSettingsQt::GetBool(const std::string& container,
                                   const std::string& name,
                                   bool               bDefaultValue)
{
  std::string key(container.empty() ? name : container + "/" + name);

  return this->impl_.getBool(key, bDefaultValue);
}

void LocalSettingsQt::SetBool(const std::string& container,
                                   const std::string& name,
                                   bool               bValue)
{
  std::string key(container.empty() ? name : container + "/" + name);

  return this->impl_.setValue(key, bValue);
}

int LocalSettingsQt::GetInt(const std::string& container,
                                 const std::string& name,
                                 int                nDefaultValue)
{
  std::string key(container.empty() ? name : container + "/" + name);

  return this->impl_.getInt(key, nDefaultValue);
}

void LocalSettingsQt::SetInt(const std::string& container,
                                  const std::string& name,
                                  int                nValue)
{
  std::string key(container.empty() ? name : container + "/" + name);

  return this->impl_.setValue(key, nValue);
}
}
}
} // namespace rmscore { namespace platform { namespace settings {

