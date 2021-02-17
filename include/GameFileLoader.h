#ifndef SCENEFILELOADER_H
#define SCENEFILELOADER_H

#include <common_struct.h>
#include <XML_Utilities.h>
#include <Str_Utilities.h>

namespace ge_fileutil{
    void parse_sheets(xmlutils::MyXMLNode xml_node,std::map<std::string,ge_common_struct::image_def>& images,std::string path="");
    void parse_icons(xmlutils::MyXMLNode xml_node,std::map<std::string,ge_common_struct::icon_def>& icons);
    ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::dialog_style_node parse_dialog_style(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::ge_adv_color parse_rgba_color(std::string colorStr);
    ge_common_struct::ge_color parse_color(std::string colorStr);
    ge_common_struct::dom_node* parse_dom(xmlutils::MyXMLNode dom_node,ge_common_struct::dom_node* parent=nullptr);
    ge_common_struct::ui_layout str_to_layout(std::string layoutstr);
}


#endif // SCENEFILELOADER_H
