/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IDOMDOCUMENTQTIPL_H
#define IDOMDOCUMENTQTIPL_H

#include "IDomDocument.h"
#include <QDomDocument>
#include <memory>

class DomDocumentQt : public IDomDocument
{
private:
    sp<IDomElement>	documentElement() const;

    bool setContent(const std::string & text, bool namespaceProcessing, std::string & errorMsg, int & errorLine, int & errorColumn) override;
    bool setContent(const std::string & text, std::string & errorMsg, int & errorLine, int & errorColumn) override;

    sp<IDomElement> SelectSingleNode(const std::string &xPath) override;

// form IDomNode
    sp<IDomElement>     toElement() const override;

public:
    DomDocumentQt(const QDomDocument& other):impl_(other){}
    DomDocumentQt():impl_(){}

public:
    QDomDocument impl_;
//    friend class IDomDocument;
//    friend class DomNodeQt;
//    friend class DomAttributeQt;
//    friend class DomElementQt;

};

#endif // IDOMDOCUMENTQTIPL_H
