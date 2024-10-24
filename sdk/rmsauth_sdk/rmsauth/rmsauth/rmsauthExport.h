/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef RMSAUTHEXPORT
#define RMSAUTHEXPORT

//#include <QtGlobal>
// 无需考虑windows
#if defined(RMSAUTH_LIBRARY)
#  define RMSAUTH_EXPORT __attribute__((visibility("default")))
#else
#  define RMSAUTH_EXPORT __attribute__((visibility("default")))
#endif

#endif  // RMSAUTHEXPORT
