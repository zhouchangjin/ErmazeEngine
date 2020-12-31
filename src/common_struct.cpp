#include "common_struct.h"
namespace ge_common_struct
{
void FreeDomNode(dom_node* node)
{
    std::vector<dom_node*> children=node->children;
    if(children.size()>0){
       FreeDomVector(children);
    }
    if(node->list_template!=nullptr)
    {
        FreeDomNode(node->list_template);
        delete node->list_template;
        node->list_template=nullptr;
    }
    node->parent_node=nullptr;
}

void FreeDomVector(std::vector<dom_node*>& nodes){

    for(size_t i=0; i<nodes.size(); i++)
    {
        dom_node* node=nodes[i];
        if(node!=nullptr)
        {
            FreeDomNode(node);
            delete node;
            node=nullptr;
        }
    }
    nodes.clear();
}

}
