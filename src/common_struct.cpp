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
        if(cnt==0){
            return nullptr;
        }
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

bool DomEnablePage(dom_node* node){
    if(node->enable_page){
        return true;
    }else{
        std::vector<ge_common_struct::dom_node*> children=node->children;
        for(size_t i=0;i<children.size();i++){
            ge_common_struct::dom_node* child=children[i];
            bool enable=DomEnablePage(child);
            if(enable){
                return true;
            }
        }
    }
    return false;

}

key_event_type TranslateEventTypeName(std::string name){

    if(name.compare("KEY_UP")==0){
        return key_event_type::KEY_UP;
    }else if(name.compare("KEY_LEFT")==0){
        return key_event_type::KEY_LEFT;
    }else if(name.compare("KEY_DOWN")==0){
        return key_event_type::KEY_DOWN;
    }else if(name.compare("KEY_RIGHT")==0){
        return key_event_type::KEY_RIGHT;
    }else if(name.compare("KEY_ESC")==0){
        return key_event_type::KEY_ESC;
    }else if(name.compare("KEY_CANCLE")==0){
        return key_event_type::KEY_CANCLE;
    }else if(name.compare("KEY_CONFIRM")==0){
        return key_event_type::KEY_CONFIRM;
    }else if(name.compare("KEY_ENTER")==0){
        return key_event_type::KEY_ENTER;
    }else if(name.compare("KEY_SPACE")==0){
        return key_event_type::KEY_SPACE;
    }else{
        return key_event_type::NO_EVENT;
    }

}

}
