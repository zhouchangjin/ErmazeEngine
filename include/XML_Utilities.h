#ifndef XML_UTILITIES_HPP_INCLUDED
#define XML_UTILITIES_HPP_INCLUDED
#include "../libs/pugixml/pugixml.hpp"
#include <string>


namespace xmlutils
{

class MyXMLDoc;

class MyXMLNode;


MyXMLDoc LoadXML(std::string filepath);

class MyXMLNode
{

public:
    MyXMLNode();
    void SetNode(pugi::xml_node node)
    {
        m_node=node;
    };
    MyXMLNode Child(std::string nodename);
    MyXMLNode NextSlibing(std::string slibingname);
    int IntAttribute(std::string attributename);
    std::string StrAttribute(std::string attributename);
    std::string valueStr();
    explicit operator bool();

private:
    pugi::xml_node m_node;

};

class MyXMLDoc
{
public:
    MyXMLDoc();
    MyXMLDoc(std::string path);
    int GetIntAttribute(std::string xpath);
    MyXMLNode GetNode(std::string xpath);
    pugi::xml_document* GetDocument()
    {
        return &m_doc;
    };
private:
    pugi::xml_document m_doc;
    pugi::xml_parse_result m_parsed_content;
};
}

#endif // XML_UTILITIES_HPP_INCLUDED
