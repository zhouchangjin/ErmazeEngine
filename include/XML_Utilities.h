#ifndef XML_UTILITIES_HPP_INCLUDED
#define XML_UTILITIES_HPP_INCLUDED
#include "../libs/pugixml/pugixml.hpp"
#include <string>


namespace xmlutils
{

class MyXMLDoc;

class MyXMLNode;

class MyXMLAttribute;


MyXMLDoc LoadXML(std::string filepath);

class MyXMLNode
{

public:
    MyXMLNode();
    void SetNode(pugi::xml_node node)
    {
        m_node=node;
    };
    bool HasAttribute(std::string attname);
    MyXMLNode FirstChild();
    MyXMLNode NextSlibing();
    MyXMLNode Child(std::string nodename);
    MyXMLNode NextSlibing(std::string slibingname);
    MyXMLAttribute FirstAttribute();
    int IntAttribute(std::string attributename);
    std::string StrAttribute(std::string attributename);
    std::string ValueStr();
    std::string Name()
    {
        return m_node.name();
    };
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
    std::string GetStrAttribute(std::string xpath);
    std::string GetStr(std::string xpath);
    MyXMLNode GetNode(std::string xpath);
    pugi::xml_document* GetDocument()
    {
        return &m_doc;
    };
private:
    pugi::xml_document m_doc;
    pugi::xml_parse_result m_parsed_content;
};

class MyXMLAttribute
{
public:
    MyXMLAttribute();
    MyXMLAttribute NextAttribute();
    std::string StrValue();
    std::string Name();
    void SetAttribute(pugi::xml_attribute attribute)
    {
        m_attribute=attribute;
    }
    explicit operator bool();
private:
    pugi::xml_attribute m_attribute;


};

}

#endif // XML_UTILITIES_HPP_INCLUDED
