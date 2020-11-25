#include "SceneFileLoader.h"
namespace ge_fileutil
{

void parse_exp_node(xmlutils::MyXMLNode& xml_node,ge_common_struct::exp_node* exp_node)
{
    xmlutils::MyXMLNode lexp_node=xml_node.Child("lexp");
    xmlutils::MyXMLNode rexp_node=xml_node.Child("rexp");
    std::string typestr=xml_node.Child("type").valueStr();
    if(lexp_node && rexp_node)
    {
        if(typestr.compare("AND")==0)
        {
            exp_node->type=ge_common_struct::exp_node_type::AND;
            exp_node->left=new ge_common_struct::exp_node;
            exp_node->right=new ge_common_struct::exp_node;
            parse_exp_node(lexp_node,exp_node->left);
            parse_exp_node(rexp_node,exp_node->right);
        }
        else if(typestr.compare("OR")==0)
        {
            exp_node->type=ge_common_struct::exp_node_type::OR;
            exp_node->left=new ge_common_struct::exp_node;
            exp_node->right=new ge_common_struct::exp_node;
            parse_exp_node(lexp_node,exp_node->left);
            parse_exp_node(rexp_node,exp_node->right);

        }else{
            exp_node->type=ge_common_struct::exp_node_type::NO_CONDITION;
        }
    }
    else
    {
        exp_node->type=ge_common_struct::exp_node_type::CONDITION;
        std::string attname=xml_node.Child("name").valueStr();
        std::string valueStr=xml_node.Child("value").valueStr();
        exp_node->cond.attribute_name=attname;
        exp_node->cond.attribute_value=atoi(valueStr.c_str());
        if(typestr.compare("EQUAL")==0){
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL;
        }else if(typestr.compare("GREATER")==0){
            exp_node->cond.type=ge_common_struct::condition_type::GREATER;
        }else if(typestr.compare("EQUAL_GREATER")==0){
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL_GREATER;
        }else if(typestr.compare("LESS")==0){
            exp_node->cond.type=ge_common_struct::condition_type::LESS;

        }else if(typestr.compare("EQUAL_LESS")==0){
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL_LESS;
        }else if(typestr.compare("UNEQUAL")==0){
            exp_node->cond.type=ge_common_struct::condition_type::UNEQUAL;
        }else{
            exp_node->type=ge_common_struct::exp_node_type::NO_CONDITION;
        }
    }

}

void parse_dialog_node(xmlutils::MyXMLNode xmlnode,ge_common_struct::dialog_tree_node* node)
{
    xmlutils::MyXMLNode text_node=xmlnode.Child("text");
    xmlutils::MyXMLNode opt_node=xmlnode.Child("option");
    xmlutils::MyXMLNode children_node=xmlnode.Child("children");
    xmlutils::MyXMLNode exp_node=xmlnode.Child("exp");
    if(text_node)
    {
        xmlutils::MyXMLNode line_node=text_node.Child("line");
        for(; line_node; line_node=line_node.NextSlibing("line"))
        {
            std::string line=line_node.valueStr();
            ge_str_utilities::replaceAll(line,"\t","");
            ge_str_utilities::replaceAll(line,"\n","");
            node->node_text.push_back(line);
        }
    }
    if(opt_node)
    {
        node->option_name=opt_node.valueStr();
    }
    if(children_node)
    {
        std::string children_type=children_node.StrAttribute("type");
        if(children_type.compare("options")==0)
        {
            node->has_options=true;
        }
        else
        {
            node->has_options=false;
        }
        xmlutils::MyXMLNode child_node=children_node.Child("node");
        for(; child_node; child_node=child_node.NextSlibing("node"))
        {
            ge_common_struct::dialog_tree_node* tree_node
                =new ge_common_struct::dialog_tree_node;
            node->children.push_back(tree_node);
            parse_dialog_node(child_node,tree_node);
        }
    }
    if(exp_node)
    {
        parse_exp_node(exp_node,&(node->expression));
    }
}

ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node)
{
    ge_common_struct::dialog_tree_node* node=new ge_common_struct::dialog_tree_node;
    node->root_node=true;
    parse_dialog_node(dialog_node,node);
    return node;
}


}
