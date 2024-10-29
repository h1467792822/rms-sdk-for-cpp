/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef URLQTTEST_H
#define URLQTTEST_H
#include <QTest>

class UrlQtTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void setUrlTest();
    void schemeTest();
    void authorityTest();
    void urlTest();
    void fragmentTest();
    void pathTest();
    void isValidTest();
};

#endif // INTERACTIVETEST_H
