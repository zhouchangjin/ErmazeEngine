#ifndef SCENEFILELOADER_H
#define SCENEFILELOADER_H

#include <common_struct.h>
#include <XML_Utilities.h>
#include <Str_Utilities.h>

namespace ge_fileutil{

    ge_common_struct::dialog_tree_node* parse_dialog_tree(xmlutils::MyXMLNode dialog_node);


}


#endif // SCENEFILELOADER_H
