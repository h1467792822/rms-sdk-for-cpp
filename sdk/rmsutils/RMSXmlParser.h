#ifndef RMS_XML_PARSER_H_
#define RMS_XML_PARSER_H_

#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "../../rms_sdk/Platform/Logger/Logger.h"

using namespace rmscore::platform::logger;

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

            std::string nameStr("xmlns");
            xmlChar* xmlns = xmlGetProp(rootNode, (xmlChar *)nameStr.c_str());
            if (xmlns == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode: %s", "get xmlns result is empty.");
                return std::string();
            }

            xmlXPathContextPtr context = xmlXPathNewContext(doc);
            if (context == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode:", " xmlXPathNewContext context is empty.");
                return std::string();
            }

            int regRet = xmlXPathRegisterNs(context, BAD_CAST "ns", BAD_CAST xmlns);
            if (regRet != 0) {
                Logger::Error("RMSXmlParser::SelectSingleNode: xmlXPathRegisterNs error %d.", regRet);
                xmlXPathFreeContext(context);
                return std::string();
            }

            xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xPath.c_str(), context);
            if (result == nullptr) {
                Logger::Error("RMSXmlParser::SelectSingleNode:%s", " xmlXPathEvalExpression error.");
                xmlXPathFreeContext(context);
                return std::string();
            }

            if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
                Logger::Error("RMSXmlParser::SelectSingleNode:%s", " xmlXPathNodeSetIsEmpty error.");
                xmlXPathFreeObject(result);
                xmlXPathFreeContext(context);
                return std::string();
            }

            std::string resultStr = std::string((char *)xmlNodeGetContent(result->nodesetval->nodeTab[0]));
            xmlXPathFreeObject(result);
            xmlXPathFreeContext(context);

            return resultStr;
        }

    private:
        std::string xmlRoot;
};
}
#endif