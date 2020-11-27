#ifndef SCENEFILELOADER_H
#define SCENEFILELOADER_H

#include <common_struct.h>
#include <XML_Utilities.h>
#include <Str_Utilities.h>

namespace ge_fileutil{
    ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::dialog_style_node parse_dialog_style(xmlutils::MyXMLNode dialog_node);
    ge_common_struct::ge_adv_color parse_rgba_color(std::string colorStr);
    ge_common_struct::ge_color parse_color(std::string colorStr);
}


#endif // SCENEFILELOADER_H
