/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef PLATFORMJSONOBJECTTEST
#define PLATFORMJSONOBJECTTEST
#include <QtTest>

class PlatformJsonObjectTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testGetNamedString(bool enabled = true);
    void testGetNamedString_data();

    void testGetNamedBool(bool enabled = true);
    void testGetNamedBool_data();

    void testGetNamedObject(bool enabled = true);
    void testGetNamedObject_data();

    void testStringify();
    void testStringify_data();

    void testSetNamedString();
    void testSetNamedObject();
    void testSetNamedArray();
};
#endif // PLATFORMJSONOBJECTTEST

