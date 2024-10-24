/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "PlatformXmlTest.h"
#include "../../Platform/Logger/Logger.h"
// #include "../../Platform/Xml/IDomDocument.h"
// #include "../../Platform/Xml/IDomNode.h"
// #include "../../Platform/Xml/IDomElement.h"
#include <filesystem>
#include <fstream>

#include "../../../rmsutils/RMSXmlParser.h"

using namespace std;
using namespace rmscore::platform::logger;


PlatformXmlTest::PlatformXmlTest()
{
}
void PlatformXmlTest::testSelectSingleNode_data()
{
    QTest::addColumn<QString>("xmlAsString");
    QTest::addColumn<QString>("xPathRequest");
    QTest::addColumn<QString>("expectedResult");
     
    string path1 = string(SRCDIR) + "data/testXPath1.xml";
    ifstream file1(path1, ios::in | ios::binary);
    QVERIFY(file1.is_open());
    QTest::newRow("xpath1")
        << QString::fromStdString(string((istreambuf_iterator<char>(file1)), istreambuf_iterator<char>()))
        << "kml/Document/Placemark[last()]/GeometryCollection/LineString/coordinates"
        << "0.000010,0.000020,0.000030";
    file1.close();

    string path2 = string(SRCDIR) + "data/testXPath2.xml";
    ifstream file2(path2, ios::in | ios::binary);
    QVERIFY(file2.is_open());
    auto file2AsString = QString::fromStdString(string((istreambuf_iterator<char>(file2)), istreambuf_iterator<char>()));
    QTest::newRow("xpath2")
        << file2AsString
        << "bookstore/book/author[last-name = \"Bob\" and first-name = \"Joe\"]/award"
        << "Trenton Literary Review Honorable Mention";
    file2.close();
}
void PlatformXmlTest::testSelectSingleNode(bool enabled)
{
    if (!enabled) return;

    // auto doc = IDomDocument::create();
    // std::string errorMsg;
    // int errorLine = 0;
    // int errorColumn = 0;

    QFETCH(QString, xmlAsString);
    QFETCH(QString, xPathRequest);
    QFETCH(QString, expectedResult);

    // auto ok = doc->setContent(xmlAsString.toStdString(), errorMsg, errorLine, errorColumn);
    // QVERIFY2(ok, errorMsg.data());

    // auto pnode = doc->SelectSingleNode(xPathRequest.toStdString());
    // auto realResult = pnode->toElement()->text();

    rmsutils::RMSXmlParser parser;
    parser.setXmlRoot(xmlAsString.toStdString());
    auto realResult = parser.SelectSingleNode(xPathRequest.toStdString());

    Logger::Hidden("expc: %s", expectedResult.toStdString().data());
    Logger::Hidden("real: %s", realResult.data());
    QVERIFY(realResult == expectedResult.toStdString());
}
