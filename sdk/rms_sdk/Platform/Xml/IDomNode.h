/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#ifndef IDOMNODE_H
#define IDOMNODE_H

#include <string>
#include <memory>
#include "../../Common/CommonTypes.h"
#include "DomNamedNodeMap.h"
#include "DomNodeList.h"

template <typename T>
using sp = std::shared_ptr<T>;

class IDomElement;

class IDomNode
{
public:
    enum class NodeType
    {
        ElementNode = 1,
        AttributeNode = 2,
        TextNode = 3,
        DocumentNode = 10,
    };

public:
    virtual sp<IDomElement>     toElement() const = 0;
};

using DomNamedNodeMap = rmscore::modernapi::detail::HashMapString<sp<IDomNode>>;
#include <list>
using DomNodeList = std::list<sp<IDomNode>>;

#endif // IDOMNODE_H
