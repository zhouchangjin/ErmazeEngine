#include "common_struct.h"
namespace ge_common_struct
{
void FreeDomNode(dom_node* node)
{
    std::vector<dom_node*> children=node->children;
    if(children.size()>0)
    {
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

void FreeDomVector(std::vector<dom_node*>& nodes)
{

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

int CntDomChild(dom_node* node)
{

    size_t cnt=node->children.size();
    int total=0;
    for(size_t i=0; i<cnt; i++)
    {
        dom_node* child=node->children[i];
        if(child->list_template){
            total+=child->children.size();
        }else{
            total+=1;
        }
    }
    return total;
}

}
