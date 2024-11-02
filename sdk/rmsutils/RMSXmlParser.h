#ifndef RMS_XML_PARSER_H_
#define RMS_XML_PARSER_H_

#include <string>
#include <vector>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "../rms_sdk/Platform/Logger/Logger.h"

using namespace rmscore::platform::logger;
using namespace std;

namespace rmsutils {
class RMSXmlParser {
    public:
        RMSXmlParser() {}

        void setXmlRoot(const std::string& root) {
            xmlRoot = root;
        }

        std::string SelectSingleNode(const std::string &xPath) {
            xmlDocPtr doc = xmlParseMemory(xmlRoot.c_str(), xmlRoot.length());
            if (doc == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode: %s", " get xml doc is empty.");
                return std::string();
            }

            xmlNodePtr rootNode = xmlDocGetRootElement(doc);
            if (rootNode == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode: %s", " get xml root node is empty.");
                return std::string();
            }

            // printNode(rootNode);

            const xmlChar* xmlns = (rootNode->ns == nullptr) ? nullptr : rootNode->ns->href;

            Logger::Info("RMSXmlParser::get xmlns: %s", xmlns);

            xmlXPathContextPtr context = xmlXPathNewContext(doc);
            if (context == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode:", " xmlXPathNewContext context is empty.");
                return std::string();
            }

            if (xmlns != nullptr) {
                int regRet = xmlXPathRegisterNs(context, BAD_CAST "ns", BAD_CAST xmlns);
                if (regRet != 0) {
                    Logger::Error("RMSXmlParser::SelectSingleNode: xmlXPathRegisterNs error %d.", regRet);
                    xmlXPathFreeContext(context);
                    return std::string();
                }
            }

            std::string xPathPattern("");
            if (xmlns != nullptr) {
                xPathPattern = "/" + constructXPathPattern(xPath, "/", "ns", "text()");
            } else {
                xPathPattern = "/" + xPath;
            }
            Logger::Error("RMSXmlParser::xPathPattern: %s", xPathPattern.c_str());
            xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xPathPattern.c_str(), context);
            if (result == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode:%s", " xmlXPathEvalExpression error.");
                xmlXPathFreeContext(context);
                return std::string();
            }

            // printxmlXPathObject(result);

            if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
                Logger::Error("RMSXmlParser::SelectSingleNode:%s", " xmlXPathNodeSetIsEmpty error.");
                xmlXPathFreeObject(result);
                xmlXPathFreeContext(context);
                return std::string();
            }

            std::string resultStr = std::string((char *)xmlNodeGetContent(result->nodesetval->nodeTab[0]));
            Logger::Info("RMSXmlParser::xmlNodeGetContent: %s", resultStr.c_str());

            xmlXPathFreeObject(result);
            xmlXPathFreeContext(context);

            return resultStr;
        }
    private:
        void printNode(const xmlNodePtr& node) {
            if (node == nullptr) {
                Logger::Error("printNode %s", "node null!");
                return;
            }

            Logger::Info("print node name: %s, associated ns: %s : %s, def ns: %s : %s",
            node->name, node->ns == nullptr ? (xmlChar *)"null" : node->ns->href, node->ns == nullptr ? (xmlChar *)"null" : node->ns->prefix,
            node->nsDef == nullptr ? (xmlChar *)"null" : node->nsDef->href, node->nsDef == nullptr ? (xmlChar *)"null" : node->nsDef->prefix);

            if (node->properties == nullptr) {
                Logger::Error("printNode %s", "properties null!");
                return;
            }

            xmlAttrPtr attr = node->properties;
            while (attr != nullptr) {
                char* attrName = (char *)attr->name;
                char* attrValue = (char *)xmlGetProp(node, attr->name);

                Logger::Info("print xmlAttrPtr name: %s, value: %s", attrName, attrValue);

                xmlFree(attrValue);

                attr = attr->next;
            }
        }

        void printxmlXPathObject(const xmlXPathObjectPtr& obj) {
            if (obj == nullptr) {
                Logger::Error("printxmlXPathObject %s", "obj null!");
                return;
            }

            Logger::Info("printxmlXPathObject type:%d, value: %s ", obj->type, obj->stringval);
        }

        string constructXPathPattern(string xPath, const string& split, const string& xmlNs, const string& exclude) {
            vector<string> splitStrs;
            splitString(xPath, split, splitStrs);

            string ret;
            for (size_t i = 0; i < splitStrs.size(); i++) {
                if (splitStrs[i] == exclude || splitStrs[i] == split) {
                    ret += splitStrs[i];
                } else {
                    ret += xmlNs + ":" + splitStrs[i];
                }
            }

            Logger::Info("constructXPathPattern string: %s ", ret.c_str());
            return ret;
        }

        void splitString(const string& str, const string& split, vector<string>& res) {
            if (str == "") {
                return;
            }

            string strs = str;
            size_t pos = strs.find(split);
            while (pos != strs.npos)
            {
                string tmp = strs.substr(0, pos);
                if (tmp == "") {
                    res.push_back(split);
                } else {
                    res.push_back(tmp);
                    res.push_back(split);
                }

                strs = strs.substr(pos + 1);
                pos = strs.find(split);
            }

            if (strs != split) {
                res.push_back(strs);
            }

            // for(size_t i = 0; i < res.size(); i++) {
            //     Logger::Info("split string: %s ", res[i].c_str());
            // }
        }

    private:
        std::string xmlRoot;
};
}
#endif