#include "XML_Utilities.h"
namespace xmlutils{

    MyXMLNode::MyXMLNode(){
    }

    MyXMLNode::operator bool(){
        return m_node;
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

    std::string MyXMLNode::valueStr(){
        std::string value=m_node.value();
        return value;
    }

    MyXMLDoc::MyXMLDoc(std::string path){
        m_parsed_content=m_doc.load_file(path.c_str());
    }

    int MyXMLDoc::GetIntAttribute(std::string xpath){
        int int_att=m_doc.select_node(xpath.c_str()).attribute().as_int();
        return int_att;
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

}
