#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

#include <vector>
#include <string>
#include <queue>

#ifdef GE_DEBUG
#define GE_LOG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define GE_LOG(format, ...)
#endif


namespace ge_common_struct
{

typedef std::vector<std::string> StringList;

typedef std::vector<int> ROW_IDX;

typedef std::vector<ROW_IDX> LAYER_IDX;

enum text_align{
    LEFT,
    CENTER,
    RIGHT
};


enum action_source{
    NO,
    FACE_UP,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT
};

enum action_type
{
    NO_MOVE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
};

enum npc_move_type
{
    NPC_STILL,
    NPC_HALT,
    NPC_RANDOM
};

enum layer_type
{
    OBJECT_LAYER,
    TILED_LAYER,
    PICTURE_LAYER
};

enum grid_type
{
    NORMAL,
    EMPTY,
    COLLISION,
    LADDER,
    DOOR,
    TRANSFER,
    CUSTOM
};

enum input_event
{
    NO_EVENT,
    QUIT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_SPACE,
    KEY_ESC,
    KEY_ENTER

};

enum condition_type{
    EQUAL,
    UNEQUAL,
    GREATER,
    EQUAL_GREATER,
    LESS,
    EQUAL_LESS
};

enum exp_node_type{
    NO_CONDITION,
    CONDITION,
    AND,
    OR
};


struct ge_point
{
    int x;
    int y;
    ge_point(int tx=0,int ty=0){
        x=tx;
        y=ty;
    };
};

struct ge_rect
{
    int x;
    int y;
    int w;
    int h;
};

struct ge_triangle
{
    ge_point p0;
    ge_point p1;
    ge_point p2;
    ge_triangle(int x,int y,int x1,int y1,int x2,int y2)
    {
        p0=ge_point(x,y);
        p1=ge_point(x1,y1);
        p2=ge_point(x2,y2);
    }
};

struct ge_color
{
    int r;
    int g;
    int b;
};

struct ge_adv_color
{
    int r;
    int g;
    int b;
    int a;
};


struct action_log
{
    std::string action;
    int move_x;
    int move_y;
    action_log operator=(action_log& copylog)
    {
        action=copylog.action;
        move_x=copylog.move_x;
        move_y=copylog.move_y;
        return *this;
    };
    action_log()
    {
    };
    action_log(const action_log& other)
    {
        action=other.action;
        move_x=other.move_x;
        move_y=other.move_y;
    };
};

struct start_point2d
{
    int x=-1;
    int y=-1;
    int gridx=-1;
    int gridy=-1;
    int layer=1;
    std::string start_scene="01";
    std::string direction="upward";

    start_point2d operator=(start_point2d& copyone)
    {
        x=copyone.x;
        y=copyone.y;
        gridx=copyone.gridx;
        gridy=copyone.gridy;
        layer=copyone.layer;
        start_scene=copyone.start_scene;
        direction=copyone.direction;
        return *this;
    }
};

struct condition{
    condition_type type;
    std::string attribute_name;
    int attribute_value;
};

struct exp_node{
    exp_node_type type=exp_node_type::NO_CONDITION;
    condition cond;
    exp_node* left=nullptr;//必须用指针，不用指针就是无限死循环。
    exp_node* right=nullptr;//必须用指针
    ~exp_node(){
        if(left){
            delete left;
            left=nullptr;
        }
        if(right){
            delete right;
            right=nullptr;
        }
    }
};

struct dialog_tree_node{
    int id;
    bool root_node;
    bool has_options;
    std::string option_name;
    StringList node_text;
    exp_node expression;
    std::vector<dialog_tree_node*> children; //可以不用指针，用指针安全

    ~dialog_tree_node(){
        GE_LOG("delete node");
        for(size_t i=0;i<children.size();i++){
            dialog_tree_node* node=children[i];
            delete node;
            node=nullptr;
        }
        children.clear();
    }

    int get_size(){
        return node_text.size();
    }

    std::string get_text(int line){
        return node_text[line];
    }

    int get_child_size(){
        return children.size();
    }

    dialog_tree_node* get_child(int idx){
        return children[idx];
    }
};


struct box_style{
    bool is_percentage=false;
    bool is_round_box=false;
    bool visibility=true;
    ge_rect client_rect;
    ge_rect padding_rect;
    ge_adv_color background_color;
    ge_adv_color border_color;
    ge_color font_color;
    int border_width;
    int texture_id=-1;
    text_align align=text_align::LEFT;
};

struct dom_node{
    std::string node_id;
    box_style style;
    std::string text;
    std::vector<dom_node> children;
};


struct dialog_style_node{
    box_style main_window;
    box_style choice_window;
    box_style title_window;
};




}


#endif // COMMON_STRUCT_H_INCLUDED
