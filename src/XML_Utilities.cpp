#include "XML_Utilities.h"
namespace xmlutils{

    MyXMLNode::MyXMLNode(){
    }

    MyXMLNode::operator bool(){
        return m_node;
    }

    bool MyXMLNode::HasAttribute(std::string attname){
        return m_node.attribute(attname.c_str());
    }

    MyXMLNode MyXMLNode::FirstChild(){
        MyXMLNode node;
        node.SetNode(m_node.first_child());
        return node;
    }

    MyXMLNode MyXMLNode::NextSlibing(){
        MyXMLNode node;
        node.SetNode(m_node.next_sibling());
        return node;
    }

    MyXMLNode MyXMLNode::Child(std::string nodename){
        MyXMLNode node;
        node.SetNode(m_node.child(nodename.c_str()));
        return node;
    }

    MyXMLNode MyXMLNode::NextSlibing(std::string nodename){
        MyXMLNode node;
        node.SetNode(m_node.next_sibling(nodename.c_str()));
        return node;
    }

    int MyXMLNode::IntAttribute(std::string attribute_name){

       int num= m_node.attribute(attribute_name.c_str()).as_int();
       return num;
    }

    std::string MyXMLNode::StrAttribute(std::string attribute_name){
       std::string value= m_node.attribute(attribute_name.c_str()).value();
       return value;
    }

    std::string MyXMLNode::ValueStr(){
        std::string value=m_node.text().as_string();
        return value;
    }

    MyXMLAttribute MyXMLNode::FirstAttribute(){
        pugi::xml_attribute attribute=m_node.first_attribute();
        MyXMLAttribute xmlattribute;
        xmlattribute.SetAttribute(attribute);
        return xmlattribute;
    }

    MyXMLDoc::MyXMLDoc(std::string path){
        m_parsed_content=m_doc.load_file(path.c_str());
    }

    int MyXMLDoc::GetIntAttribute(std::string xpath){
        int int_att=m_doc.select_node(xpath.c_str()).attribute().as_int();
        return int_att;
    }

    std::string MyXMLDoc::GetStrAttribute(std::string xpath){
        std::string str_att=m_doc.select_node(xpath.c_str())
        .attribute().as_string();
        return str_att;
    }

    std::string MyXMLDoc::GetStr(std::string xpath){
        std::string strvalue=m_doc.select_node(xpath.c_str()).node().text().as_string();
        return strvalue;
    }

    MyXMLNode MyXMLDoc::GetNode(std::string xpath){
        pugi::xml_node node=m_doc.select_node(xpath.c_str()).node();
        MyXMLNode myxmlnode;
        myxmlnode.SetNode(node);
        return myxmlnode;
    }

    MyXMLDoc LoadXML(std::string path){
        MyXMLDoc doc(path);
        return doc;
    }

    MyXMLAttribute::MyXMLAttribute(){
    }

    MyXMLAttribute::operator bool(){
        return m_attribute;
    }

    MyXMLAttribute MyXMLAttribute::NextAttribute(){
        MyXMLAttribute nextxmlattr;
        pugi::xml_attribute attribute=m_attribute.next_attribute();
        nextxmlattr.SetAttribute(attribute);
        return nextxmlattr;
    }

    std::string   MyXMLAttribute::Name(){

        return m_attribute.name();
    }

    std::string MyXMLAttribute::StrValue(){
        return m_attribute.value();
    }

}
