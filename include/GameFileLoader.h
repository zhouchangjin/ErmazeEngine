#ifndef SCENEFILELOADER_H
#define SCENEFILELOADER_H

#include <File_Util.h>
#include <common_struct.h>
#include <XML_Utilities.h>
#include <Str_Utilities.h>

namespace ge_fileutil{
    void parse_sheets(xmlutils::MyXMLNode xml_node,std::map<std::string,ge_common_struct::image_def>& images,std::string path="");
    void parse_resource(xmlutils::MyXMLNode xml_node,std::map<std::string,ge_common_struct::resource_def>& resources);
    void parse_sprites_action(xmlutils::MyXMLNode xml_node,std::vector<ge_common_struct::sprite_action>& actions);
    ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::dialog_style_node parse_dialog_style(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::ge_adv_color parse_rgba_color(std::string colorStr);
    ge_common_struct::ge_color parse_color(std::string colorStr);
    ge_common_struct::dom_node* parse_dom(xmlutils::MyXMLNode dom_node,ge_common_struct::dom_node* parent=nullptr);
    ge_common_struct::ui_layout str_to_layout(std::string layoutstr);

    void parse_chunk(xmlutils::MyXMLNode xml_node,ge_fileutil::chunk& chunk);
    ge_fileutil::chunk_type translate_datatype(std::string datatype);
}


#endif // SCENEFILELOADER_H
