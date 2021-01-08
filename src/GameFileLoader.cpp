#include "GameFileLoader.h"
namespace ge_fileutil
{

std::vector<int> parse_color_to_vector(std::string colorStr)
{
    size_t start_pos=colorStr.find("(");
    size_t end_pos=colorStr.find(")");
    if(start_pos!=std::string::npos && end_pos!=std::string::npos)
    {
        std::string substring=colorStr.substr(start_pos+1,end_pos-1);
        //GE_LOG("color=%s\n",substring.c_str());
        std::vector<int> colors=ge_str_utilities::SplitStrToIntArray(substring,',');
        return colors;
    }
    else
    {
        std::vector<int> colors;
        return colors;
    }

}

ge_common_struct::ge_color parse_color(std::string colorStr)
{

    ge_common_struct::ge_color color;
    std::vector<int> colors=parse_color_to_vector(colorStr);
    if(colors.size()==3)
    {
        color.r=colors[0];
        color.g=colors[1];
        color.b=colors[2];
    }
    return color;
}
ge_common_struct::ge_adv_color parse_rgba_color(std::string colorStr)
{
    ge_common_struct::ge_adv_color color;
    std::vector<int> colors=parse_color_to_vector(colorStr);
    if(colors.size()>=3)
    {
        color.r=colors[0];
        color.g=colors[1];
        color.b=colors[2];
    }
    if(colors.size()==4)
    {
        color.a=colors[3];
    }
    else
    {
        color.a=255;
    }
    return color;
}

void parse_exp_node(xmlutils::MyXMLNode& xml_node,ge_common_struct::exp_node* exp_node)
{
    xmlutils::MyXMLNode lexp_node=xml_node.Child("lexp");
    xmlutils::MyXMLNode rexp_node=xml_node.Child("rexp");
    std::string typestr=xml_node.Child("type").ValueStr();
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
        std::string attname=xml_node.Child("name").ValueStr();
        std::string valueStr=xml_node.Child("value").ValueStr();
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
            std::string line=line_node.ValueStr();
            ge_str_utilities::ReplaceAll(line,"\t","");
            ge_str_utilities::ReplaceAll(line,"\n","");
            node->node_text.push_back(line);
        }
    }
    if(opt_node)
    {
        node->option_name=opt_node.ValueStr();
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



ge_common_struct::box_style parse_window_style(xmlutils::MyXMLNode
        window_style_node,ge_common_struct::dom_node* parent=nullptr)
{
    ge_common_struct::box_style window_style;
    if(parent)
    {
        ge_common_struct::box_style parent_style=parent->style;
        window_style.background_color=parent_style.background_color;
        window_style.font_color=parent_style.font_color;
        window_style.border_color=parent_style.border_color;
        if(parent->child_layout==ge_common_struct::ui_layout::VERVICAL_LAYOUT)
        {
            //只是为了初始化(y,w无意义,h有意义),是百分比还是数无所谓
            window_style.client_rect.x=0; //relative;
            window_style.client_rect.y=0;
            window_style.client_rect.w=100;
            window_style.is_percentage=true;
            window_style.position_is_absolute=false;
        }
        else if(parent->child_layout==ge_common_struct::ui_layout::HORIZONTAL_LAYOUT)
        {
            window_style.client_rect.x=0;
            window_style.client_rect.y=0; //relative;
            window_style.client_rect.h=100;
            window_style.is_percentage=true;
            window_style.position_is_absolute=false;
        }
        else if(parent->child_layout==ge_common_struct::ui_layout::GRID_LAYOUT)
        {
            //x,y,w,h都在绘画时才能确定，且无法设置
            window_style.position_is_absolute=false;
            window_style.is_percentage=true;
            window_style.client_rect.x=0;
            window_style.client_rect.y=0;
            int row=parent->row;
            int col=parent->col;
            window_style.client_rect.w=100/col;
            window_style.client_rect.h=100/row;
        }
        else if(parent->child_layout==ge_common_struct::ui_layout::FLOW_LAYOUT)
        {
            //w,h都是读取的
            window_style.position_is_absolute=false;
            window_style.is_percentage=false;
        }
        else if(parent->child_layout==ge_common_struct::ui_layout::BORDER_LAYOUT)
        {
            //x,y无用只有width和height有用
            window_style.position_is_absolute=false;
            window_style.is_percentage=false;
        }
        else if(parent->child_layout==ge_common_struct::ui_layout::NULL_LAYOUT)
        {
            window_style.position_is_absolute=false;
            window_style.is_percentage=false;
        }
    }
    xmlutils::MyXMLNode rect_node=window_style_node.Child("rect");
    if(rect_node)
    {
        if(rect_node.HasAttribute("num_type"))
        {
            std::string num_type=rect_node.StrAttribute("num_type");
            if(num_type.compare("percentage")==0)
            {
                window_style.is_percentage=true;
            }
            else
            {
                window_style.is_percentage=false;
            }

        }
        if(rect_node.HasAttribute("radius"))
        {
            int radius= rect_node.IntAttribute("radius");
            if(radius>0)
            {
                window_style.out_radius=radius;
                window_style.draw_shape=true;
            }
        }
        if(rect_node.HasAttribute("x"))
        {
            int x=rect_node.IntAttribute("x");
            window_style.client_rect.x=x;
        }
        if(rect_node.HasAttribute("y"))
        {
            int y=rect_node.IntAttribute("y");
            window_style.client_rect.y=y;
        }
        if(rect_node.HasAttribute("h"))
        {
            int h=rect_node.IntAttribute("h");
            window_style.client_rect.h=h;
        }
        if(rect_node.HasAttribute("w"))
        {
            int w=rect_node.IntAttribute("w");
            window_style.client_rect.w=w;
        }

        xmlutils::MyXMLNode background_node=rect_node.Child("background");
        xmlutils::MyXMLNode border_node=rect_node.Child("border");
        xmlutils::MyXMLNode font_node=rect_node.Child("font");

        if(background_node)
        {
            if(background_node.HasAttribute("color"))
            {
                std::string colorStr=background_node.StrAttribute("color");
                ge_common_struct::ge_adv_color color=parse_rgba_color(colorStr);
                window_style.background_color=color;
            }
        }
        if(border_node)
        {
            int border_width=border_node.IntAttribute("width");
            window_style.border_width=border_width;
            std::string type=border_node.StrAttribute("type");
            std::string colorStr=border_node.StrAttribute("color");
            ge_common_struct::ge_adv_color color=parse_rgba_color(colorStr);
            window_style.border_color=color;
        }
        if(font_node)
        {
            if(font_node.HasAttribute("color"))
            {

                std::string colorStr=font_node.StrAttribute("color");
                ge_common_struct::ge_color color=parse_color(colorStr);
                window_style.font_color=color;
            }
        }

    }

    //后面去掉
    std::string node_name=window_style_node.Name();
    if(node_name.compare("button")==0)
    {
        window_style.align=ge_common_struct::text_align::CENTER;
        window_style.border_width=2;
        window_style.border_color.r=255;
        window_style.border_color.g=255;
        window_style.border_color.b=255;
        window_style.border_color.a=80;
    }else if(node_name.compare("icon")==0){
        if(window_style.client_rect.h==0){
            window_style.client_rect.h=28;
            window_style.client_rect.w=68;
            window_style.padding.top=2;
            window_style.padding.bottom=2;
            window_style.padding.left=20;
            window_style.padding.right=20;
            window_style.is_icon=true;
        }
    }else if(node_name.compare("text")==0){
        if(window_style.client_rect.h==0){
            window_style.client_rect.h=window_style.line_height;
        }
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
        ge_common_struct::box_style window_style=parse_window_style(window);
        if(type.compare("main")==0)
        {
            style_node.main_window=window_style;
        }
        else if(type.compare("choice")==0)
        {
            style_node.choice_window=window_style;
        }
        else if(type.compare("title")==0)
        {
            style_node.choice_window=window_style;
        }
    }
    return style_node;
}

void parse_icons(xmlutils::MyXMLNode xml_node,
                 std::map<std::string,ge_common_struct::icon_def>& icons)
{
    xmlutils::MyXMLNode icon_node=xml_node.Child("icon");
    for(; icon_node; icon_node=icon_node.NextSlibing("icon"))
    {
        std::string ref_id=icon_node.StrAttribute("ref_sheet");
        std::string name=icon_node.StrAttribute("name");
        int id=icon_node.IntAttribute("id");
        int dir=icon_node.IntAttribute("direction");
        ge_common_struct::icon_def icon_def;
        icon_def.direction=dir;
        icon_def.id=id;
        icon_def.icon_name=name;
        icon_def.resource_id=ref_id;
        icons[name]=icon_def;
    }
}

void parse_sheets(xmlutils::MyXMLNode xml_node,
                  std::map<std::string,ge_common_struct::image_def>& images,
                  std::string path)
{
    xmlutils::MyXMLNode sheet_node=xml_node.Child("spritesheet");

    for(; sheet_node; sheet_node=sheet_node.NextSlibing("spritesheet"))
    {
        std::string node_id =sheet_node.StrAttribute("id");
        std::string final_path=path+sheet_node.StrAttribute("path");
        int width=sheet_node.IntAttribute("width");
        int height=sheet_node.IntAttribute("height");
        int col=sheet_node.IntAttribute("col");
        int row=sheet_node.IntAttribute("row");
        ge_common_struct::image_def image_def;
        image_def.path=final_path;
        image_def.id=node_id;
        image_def.col=col;
        image_def.row=row;
        image_def.width=width;
        image_def.height=height;
        images[image_def.id]=image_def;
    }
}

ge_common_struct::dom_node* parse_dom(xmlutils::MyXMLNode xml_node,
                                     ge_common_struct::dom_node* parent)
{
    ge_common_struct::dom_node* node=new ge_common_struct::dom_node();
    node->parent_node=parent;
    std::string elename=xml_node.Name();
    node->ele_name=elename;
    if(xml_node.HasAttribute("id"))
    {
        node->node_id=xml_node.StrAttribute("id");
    }
    ge_common_struct::box_style style=parse_window_style(xml_node,parent);
    node->style=style;

    xmlutils::MyXMLNode children_node=xml_node.Child("rect").Child("container");
    xmlutils::MyXMLNode template_node=xml_node.Child("template");
    if(children_node)
    {
        std::string layoutstr=children_node.StrAttribute("layout");
        ge_common_struct::ui_layout layout=str_to_layout(layoutstr);
        if(children_node.HasAttribute("row")){
            node->row=children_node.IntAttribute("row");
        }else{
            node->row=1;
        }
        if(children_node.HasAttribute("col")){
            node->col=children_node.IntAttribute("col");
        }else{
            node->col=1;
        }
        node->child_layout=layout;
        xmlutils::MyXMLNode child=children_node.FirstChild();
        for(; child; child=child.NextSlibing())
        {
            ge_common_struct::dom_node* child_node=parse_dom(child,node);
            node->children.push_back(child_node);
        }

    }
    if(template_node){
        ge_common_struct::dom_node* template_dom=new ge_common_struct::dom_node();
        std::string list_name=xml_node.StrAttribute("list");
        template_dom->ele_name="template_"+list_name;
        template_dom->list_name=list_name;
        template_dom->parent_node=node;
        template_dom->style=node->style;
        node->list_template=template_dom;
        std::string layoutstr=template_node.StrAttribute("layout");
        ge_common_struct::ui_layout layout=str_to_layout(layoutstr);
        template_dom->child_layout=layout;
        xmlutils::MyXMLNode child=template_node.FirstChild();
        for(;child;child=child.NextSlibing()){
            ge_common_struct::dom_node* child_node=parse_dom(child,template_dom);
            template_dom->children.push_back(child_node);
        }
    }
    if(!children_node && !template_node)
    {
        std::string value=xml_node.ValueStr();
        ge_str_utilities::ReplaceAll(value,"\t","");
        ge_str_utilities::ReplaceAll(value,"\n","");
        if(ge_str_utilities::Contains(value,"{")){
            node->use_template=true;
            node->template_text=value;
        }else{
           node->use_template=false;
           node->text=value;
        }
    }
    return node;
}

ge_common_struct::ui_layout str_to_layout(std::string layoutstr){
    if(layoutstr.compare("grid")==0){
        return ge_common_struct::ui_layout::GRID_LAYOUT;
    }else if(layoutstr.compare("border")==0){
        return ge_common_struct::ui_layout::BORDER_LAYOUT;
    }else if(layoutstr.compare("flow")==0){
        return ge_common_struct::ui_layout::FLOW_LAYOUT;
    }else if(layoutstr.compare("horizontal")==0){
        return ge_common_struct::ui_layout::HORIZONTAL_LAYOUT;
    }else if(layoutstr.compare("vertical")==0){
        return ge_common_struct::ui_layout::VERVICAL_LAYOUT;
    }else{
        return ge_common_struct::ui_layout::NULL_LAYOUT;
    }
}

}
