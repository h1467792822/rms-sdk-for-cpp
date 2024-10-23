/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include <RmsauthIdHelper.h>
// #include <QSysInfo>
// #include <QString>
#include <string>

namespace rmsauth {

const String RmsauthIdHelper::getProcessorArchitecture()
{
// #ifdef QT_VER_LESS_THEN_54
//     return "unk";
// #else
//     return QSysInfo::currentCpuArchitecture().toStdString();
// #endif
    return std::string("arm64");
}
const String RmsauthIdHelper::getOSVersion()
{
// #ifdef QT_VER_LESS_THEN_54
//     return "unk";
// #else
//     return QSysInfo::kernelType().toStdString();
// #endif
    return std::string("HongMeng");
}

} // namespace rmsauth {
