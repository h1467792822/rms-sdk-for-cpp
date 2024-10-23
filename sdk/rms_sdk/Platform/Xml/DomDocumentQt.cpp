/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifdef QTFRAMEWORK
#include "DomDocumentQt.h"
#include "DomElementQt.h"
#include <QXmlQuery>
#include <QBuffer>
#include "../Logger/Logger.h"

using namespace rmscore::platform::logger;

sp<IDomDocument> IDomDocument::create()
{
    return sp<IDomDocument>(new DomDocumentQt());
}
bool DomDocumentQt::setContent(const std::string & text, bool namespaceProcessing, std::string & errorMsg, int & errorLine, int & errorColumn)
{
    QString str;
    bool res = this->impl_.setContent(QString(text.c_str()), namespaceProcessing, &str, &errorLine, &errorColumn);
    errorMsg = str.toStdString();
    return res;
}
bool DomDocumentQt::setContent(const std::string & text, std::string & errorMsg, int & errorLine, int & errorColumn)
{
    QString str;
    bool res = this->impl_.setContent(QString(text.c_str()), &str, &errorLine, &errorColumn);
    errorMsg = str.toStdString();
    return res;
}

sp<IDomElement>	DomDocumentQt::documentElement() const
{
    QDomElement elem = this->impl_.documentElement();
    return std::make_shared<DomElementQt>(elem);

}

sp<IDomElement> DomDocumentQt::SelectSingleNode(const std::string &xPath)
{
    const QString defaultNsPattern = "declare default element namespace '%1'";
    const QString xpathReqPattern = "%1; %2/%3";

    QBuffer device;
    device.setData(this->impl_.toString().toUtf8());
    device.open(QIODevice::ReadOnly);
    QXmlQuery query;
    query.bindVariable("inputDocument", &device);

    auto defaultNs = this->impl_.documentElement().toElement().attribute("xmlns");
    Logger::Hidden("DomDocumentQt::SelectSingleNode: defaultNs: %s", defaultNs.toStdString().data());

    QString  xpathReq = xpathReqPattern.arg(defaultNsPattern.arg(defaultNs)).arg("doc($inputDocument)").arg(xPath.data());
    query.setQuery(xpathReq);
    Logger::Hidden("DomDocumentQt::SelectSingleNode: xpathReq: %s", xpathReq.toStdString().data());

    if ( !query.isValid())
    {
        Logger::Error("Error: DomDocumentQt::SelectSingleNode: invalid query.");
        return nullptr;
    }
    QString res;
    query.evaluateTo(&res);
    if(res.isEmpty())
    {
        Logger::Warning("DomDocumentQt::SelectSingleNode: result is empty.");
        return nullptr;
    }
    QDomDocument resDoc;
    resDoc.setContent("<result>" + res + "</result>");

    auto root = resDoc.documentElement();

    Logger::Hidden("DomDocumentQt::SelectSingleNode: resDoc: %s", res.toStdString().data());
    return std::make_shared<DomElementQt>(resDoc.documentElement());
}

// from IDomNode
sp<IDomElement>     DomDocumentQt::toElement() const
{
    QDomElement elem = this->impl_.toElement();
    return std::make_shared<DomElementQt>(elem);
}

#endif

