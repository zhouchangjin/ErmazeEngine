#include "SceneFileLoader.h"
namespace ge_fileutil{


void parse_dialog_node(xmlutils::MyXMLNode xmlnode,ge_common_struct::dialog_tree_node* node){
    xmlutils::MyXMLNode text_node=xmlnode.Child("text");
    xmlutils::MyXMLNode opt_node=xmlnode.Child("option");
    xmlutils::MyXMLNode children_node=xmlnode.Child("children");
    if(text_node){
      xmlutils::MyXMLNode line_node=text_node.Child("line");
      for(;line_node;line_node=line_node.NextSlibing("line")){
        std::string line=line_node.valueStr();
        ge_str_utilities::replaceAll(line,"\t","");
        ge_str_utilities::replaceAll(line,"\n","");
        node->node_text.push_back(line);
      }
    }
    if(opt_node){
        node->option_name=opt_node.valueStr();
    }
    if(children_node){
        xmlutils::MyXMLNode child_node=children_node.Child("node");
        for(;child_node;child_node=child_node.NextSlibing("node")){
            ge_common_struct::dialog_tree_node* tree_node
            =new ge_common_struct::dialog_tree_node;
            node->children.push_back(tree_node);
            parse_dialog_node(child_node,tree_node);
        }
    }
}

ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node){
    ge_common_struct::dialog_tree_node* node=new ge_common_struct::dialog_tree_node;
    node->root_node=true;
    parse_dialog_node(dialog_node,node);
    return node;
}


}
