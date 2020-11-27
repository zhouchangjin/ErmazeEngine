#include "GameFileLoader.h"
namespace ge_fileutil
{

std::vector<int> parse_color_to_vector(std::string colorStr){
    size_t start_pos=colorStr.find("(");
    size_t end_pos=colorStr.find(")");
    if(start_pos!=std::string::npos && end_pos!=std::string::npos){
       std::string substring=colorStr.substr(start_pos+1,end_pos-1);
       //GE_LOG("color=%s\n",substring.c_str());
       std::vector<int> colors=ge_str_utilities::SplitStrToIntArray(substring,',');
       return colors;
    }else{
        std::vector<int> colors;
        return colors;
    }

}

ge_common_struct::ge_color parse_color(std::string colorStr){

    ge_common_struct::ge_color color;
    std::vector<int> colors=parse_color_to_vector(colorStr);
    if(colors.size()==3){
        color.r=colors[0];
        color.g=colors[1];
        color.b=colors[2];
    }
    return color;
}
ge_common_struct::ge_adv_color parse_rgba_color(std::string colorStr){
    ge_common_struct::ge_adv_color color;
    std::vector<int> colors=parse_color_to_vector(colorStr);
    if(colors.size()>=3){
        color.r=colors[0];
        color.g=colors[1];
        color.b=colors[2];
    }
    if(colors.size()==4){
        color.a=colors[3];
    }else{
        color.a=255;
    }
    return color;
}

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

        }
        else
        {
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
        if(typestr.compare("EQUAL")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL;
        }
        else if(typestr.compare("GREATER")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::GREATER;
        }
        else if(typestr.compare("EQUAL_GREATER")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL_GREATER;
        }
        else if(typestr.compare("LESS")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::LESS;

        }
        else if(typestr.compare("EQUAL_LESS")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::EQUAL_LESS;
        }
        else if(typestr.compare("UNEQUAL")==0)
        {
            exp_node->cond.type=ge_common_struct::condition_type::UNEQUAL;
        }
        else
        {
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


ge_common_struct::window_style parse_window_style(xmlutils::MyXMLNode
        window_style_node)
{
    xmlutils::MyXMLNode rect_node=window_style_node.Child("rect");
    ge_common_struct::window_style window_style;
    std::string num_type=rect_node.StrAttribute("num_type");
    int x=rect_node.IntAttribute("x");
    int y=rect_node.IntAttribute("y");
    int h=rect_node.IntAttribute("h");
    int w=rect_node.IntAttribute("w");
    if(num_type.compare("percentage")==0){
        window_style.is_percentage=true;
    }else{
        window_style.is_percentage=false;
    }
    window_style.client_rect.x=x;
    window_style.client_rect.y=y;
    window_style.client_rect.h=h;
    window_style.client_rect.w=w;

    xmlutils::MyXMLNode background_node=rect_node.Child("background");
    xmlutils::MyXMLNode border_node=rect_node.Child("border");
    xmlutils::MyXMLNode font_node=rect_node.Child("font");

    if(background_node){

        std::string colorStr=background_node.StrAttribute("color");
        ge_common_struct::ge_adv_color color=parse_rgba_color(colorStr);
        window_style.background_color=color;
    }
    if(border_node){
        int border_width=border_node.IntAttribute("width");
        window_style.border_width=border_width;
        std::string type=border_node.StrAttribute("type");
        std::string colorStr=border_node.StrAttribute("color");
        ge_common_struct::ge_color color=parse_color(colorStr);
        window_style.border_color=color;
    }
    if(font_node){
        std::string colorStr=font_node.StrAttribute("color");
        ge_common_struct::ge_color color=parse_color(colorStr);
        window_style.font_color=color;
    }

    return window_style;
}

ge_common_struct::dialog_style_node parse_dialog_style(xmlutils::MyXMLNode
        dialog_style_node)
{
    ge_common_struct::dialog_style_node style_node;
    xmlutils::MyXMLNode window=dialog_style_node.Child("window");
    for(; window; window=window.NextSlibing("window"))
    {
        std::string type=window.StrAttribute("type");
        ge_common_struct::window_style window_style=parse_window_style(window);
        if(type.compare("main")==0){
            style_node.main_window=window_style;
        }else if(type.compare("choice")==0){
            style_node.choice_window=window_style;
        }else if(type.compare("title")==0){
            style_node.choice_window=window_style;
        }
    }
    return style_node;
}


}
