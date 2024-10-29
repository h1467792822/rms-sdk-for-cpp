/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "UrlQtTests.h"
#include "../rmsauth/UrlQt.h"
#include <FileCache.h>
#include <cassert>
#include <QDebug>

Q_DECLARE_METATYPE(rmsauth::String)

using namespace std;
using namespace rmsauth;

void UrlQtTests::setUrlTest() {
    UrlQt url("http://www.aspxfans.com:8080/news");
    url.setUrl("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String res = url.toString();
    assert(res == "http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
}
void UrlQtTests::schemeTest() {
    UrlQt url("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String scheme = url.scheme();
    assert(scheme == "http"); 
}
void UrlQtTests::authorityTest() {
    UrlQt url("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String authority = url.authority();
    assert(authority == "www.aspxfans.com:8080"); 
}
void UrlQtTests::urlTest() {
    UrlQt url("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String host = url.host();
    assert(host == "www.aspxfans.com"); 
}
void UrlQtTests::fragmentTest() {
    UrlQt url("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String fragment = url.fragment();
    assert(fragment == "name"); 
}
void UrlQtTests::pathTest() {
    UrlQt url("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    String path = url.path();
    qDebug() << QString::fromStdString(path);
    assert(path == "/news/index.asp"); 
}
void UrlQtTests::isValidTest() {
    UrlQt url1("http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    UrlQt url2("https://www.aspxfans.com:8080/news/index.aspboardID=5&ID=24618&page=1#name");
    UrlQt url3("//www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name");
    bool valid1 = url1.isValid();
    assert(valid1 == true); 
    bool valid2 = url2.isValid();
    assert(valid2 == true); 
    bool valid3 = url3.isValid();
    assert(valid3 == false); 
}
