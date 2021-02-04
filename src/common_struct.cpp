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

dom_node* GetDomSelection(dom_node* select_root,int selection){
    if((size_t)selection<select_root->children.size()){
        ge_common_struct::dom_node* sel_node=select_root->children[selection];
        if(sel_node->children.size()==0){
            return sel_node;
        }else{
            return sel_node->children[0];
        }
    }else{
        size_t cnt=select_root->children.size();
        //TODO 如果是有行列排列则下面代码才有效
        int p_parent=selection%cnt;
        ge_common_struct::dom_node* sub_node=select_root->children[p_parent];
        size_t ccnt=sub_node->children.size();
        size_t idx=selection/cnt;
        if(idx<ccnt){
            ge_common_struct::dom_node* sel_node=sub_node->children[idx];
            return sel_node;
        }else{
            return nullptr;
        }

    }
}

}
