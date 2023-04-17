#ifndef COMMON_STRUCT_H_INCLUDED
#define COMMON_STRUCT_H_INCLUDED

#include <map>
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

enum text_align
{
    LEFT,
    CENTER,
    RIGHT
};

enum ui_layout
{
    VERVICAL_LAYOUT,
    HORIZONTAL_LAYOUT,
    GRID_LAYOUT,
    BORDER_LAYOUT,
    FLOW_LAYOUT,
    NULL_LAYOUT,
};


enum action_source
{
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

enum key_event_type
{
    NO_EVENT,
    QUIT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_SPACE,
    KEY_ESC,
    KEY_ENTER,
    KEY_CONFIRM,
    KEY_CANCLE

};

enum key_press_type
{
    KEY_PRESS,
    KEY_PRESSED,
    KEY_RELEASE
};

enum condition_type
{
    EQUAL,
    UNEQUAL,
    GREATER,
    EQUAL_GREATER,
    LESS,
    EQUAL_LESS
};

enum exp_node_type
{
    NO_CONDITION,
    CONDITION,
    AND,
    OR
};

enum val_type
{
    INT_VAL,
    DECIMAL_VAL,
    STRING_VAL
};


struct ge_point
{
    int x;
    int y;
    ge_point(int tx=0,int ty=0)
    {
        x=tx;
        y=ty;
    };
};

struct ge_rect
{
    int x=0;
    int y=0;
    int w=0;
    int h=0;
};

struct ge_sides
{
    int top=0;
    int left=0;
    int bottom=0;
    int right=0;
    ge_sides()
    {
    };
    ge_sides(int width)
    {
        top=width;
        left=width;
        bottom=width;
        right=width;
    };
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
    int a=255;
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

struct condition
{
    condition_type type;
    std::string attribute_name;
    int attribute_value;
};

struct exp_node
{
    exp_node_type type=exp_node_type::NO_CONDITION;
    condition cond;
    exp_node* left=nullptr;//必须用指针，不用指针就是无限死循环。
    exp_node* right=nullptr;//必须用指针
    ~exp_node()
    {
        if(left)
        {
            delete left;
            left=nullptr;
        }
        if(right)
        {
            delete right;
            right=nullptr;
        }
    }
};

struct dialog_tree_node
{
    int id;
    bool root_node;
    bool has_options;
    std::string option_name;
    StringList node_text;
    exp_node expression;
    std::vector<dialog_tree_node*> children; //可以不用指针，用指针安全

    ~dialog_tree_node()
    {
        //GE_LOG("delete node");
        for(size_t i=0; i<children.size(); i++)
        {
            dialog_tree_node* node=children[i];
            delete node;
            node=nullptr;
        }
        children.clear();
    }

    int get_size()
    {
        return node_text.size();
    }

    std::string get_text(int line)
    {
        return node_text[line];
    }

    int get_child_size()
    {
        return children.size();
    }

    dialog_tree_node* get_child(int idx)
    {
        return children[idx];
    }
};

struct attribute
{
    std::string attribute_name;
    val_type val_type;
    int int_value;
    float f_value;
    std::string str_value;
};

typedef std::map<std::string,attribute> AttMap;

struct box_style
{
    bool is_icon=false;
    bool background_texture=false;
    bool draw_shape=false;
    bool position_is_absolute=false; //暂时没有用的属性
    bool is_percentage=false;
    int out_radius=0;
    bool visibility=true;
    ge_rect client_rect;
    ge_sides padding;
    ge_adv_color background_color;
    ge_adv_color border_color;
    ge_color font_color;
    int border_width=0;
    int font_size=24;
    int line_height=26;
    text_align align=text_align::LEFT;
    std::string texture_name;
    int texture_scale=1;
    bool scroll_x_enabled=false; //暂时不用scroll的方式载入全部数据
    bool scroll_y_enabled=false; //暂时不用的属性，采用伪scroll
    int  scroll_x=0;             // 即使不启用scroll，该属性也可以显示scroll位置
    int  scroll_y=0;              //即使不启用scroll，该属性也可以显示scroll位置
};

struct dom_node
{
    ui_layout child_layout=ui_layout::VERVICAL_LAYOUT;
    std::string node_id;
    std::string ele_name;
    box_style style;
    std::string text;
    std::string template_text;
    std::vector<dom_node*> children;
    AttMap attributes; //节点属性（非样式才有） attribute 和var_list是两个概念
    ge_rect box;  //真实box
    int row=1;
    int col=1;
    uint32_t frame=0;
    dom_node* parent_node=nullptr; //经过复制之后就是无效的属性了
    dom_node* list_template=nullptr; //经过复制后还是有效的
    std::string list_name;
    int child_seq_no=-1;
    int obj_id=-1;
    std::string action_name; //菜单跳转
    std::string action_type_name; //菜单类别
    std::string type; //菜单类型
    bool use_template=false;
    bool enable_page=false;
    int page_size=0;
    std::vector<std::string> var_list; //模板变量名称集合
    dom_node* template_node=nullptr; //拷贝模板
    bool has_attribute(std::string attr_name){
        if(attributes.find(attr_name)!=attributes.end()){
            return true;
        }else{
            return false;
        }
    }

    std::string attr_str(std::string attr_name){
        return attributes[attr_name].str_value;
    }

    void attr_str(std::string attr_name,std::string value){
        attribute attr;
        attr.attribute_name=attr_name;
        attr.val_type=val_type::STRING_VAL;
        attr.str_value=value;
        attributes[attr_name]=attr;
    }
};


struct dialog_style_node
{
    box_style main_window;
    box_style choice_window;
    box_style title_window;
};

struct key_event
{
    key_event_type key=key_event_type::NO_EVENT;
    key_press_type press=key_press_type::KEY_PRESSED;
    int keycode;
};

struct input_event
{
    std::map<key_event_type,key_event> key_events;
    key_event get_event(key_event_type key_type)
    {
        if(key_events.find(key_type)!=key_events.end())
        {

            return key_events[key_type];
        }
        else
        {
            key_event event;
            event.keycode=-1;
            return event;
        }

    };

    key_event get_top_event()
    {
        if(key_events.size()>0)
        {
            return key_events.begin()->second;
        }
        else
        {
            key_event event;
            event.keycode=-1;
            return event;
        }
    }

    key_event_type get_event_type(key_event_type keytype)
    {
        if(key_events.find(keytype)!=key_events.end())
        {
            return key_events[keytype].key;
        }
        else
        {
            return key_event_type::NO_EVENT;
        }
    }

    void add_event(key_event_type key_type,
                   key_press_type press_type=key_press_type::KEY_PRESS,
                   int keycode=-1)
    {
        if(key_events.find(key_type)==key_events.end())
        {

            key_event event= {key_type,press_type,keycode};
            key_events[key_type]=event;
        }
    }

    bool empty()
    {
        return key_events.empty();
    }
};

struct image_def
{
    std::string id;
    std::string path;
    int width;
    int height;
    int row;
    int col;
};

struct sprite_action
{
    std::string sheet_id;
    std::string sprite_id;
    std::string action_name;
    std::vector<int> ids;
};

struct resource_def
{
    std::string resource_id;
    std::string resource_name;
    int id;
    int direction;
};

struct menu_command
{
    std::string menu_name;
    std::string command_name;
    int obj_id;
    int menu_order;
};

struct command_item
{
    std::string command_name;  //attack,magic,item,etc
    int  source_obj_id;
    int  source_obj_no;
    int  center_target_obj_id=-1;
    int  center_target_no=-1;
    bool group_target=false;
    int  target_x;
    int  target_y;
    int  using_obj_id;       //translate to different animation
    int command_type;        // redundant
    int sort_num;           //for sort purpose
    uint32_t end_frame;    // end frame cannot use for sort
    bool prossesed=false;
    friend bool operator<(const command_item& l,const command_item& r)
    {
        return l.sort_num<r.sort_num;
    }
};

void FreeDomNode(dom_node* node);

void FreeDomVector(std::vector<dom_node*>& nodes);

int CntDomChild(dom_node* node);

dom_node* GetDomSelection(dom_node* select_root,int selection);

bool DomEnablePage(dom_node* node);

key_event_type TranslateEventTypeName(std::string name);

}


#endif // COMMON_STRUCT_H_INCLUDED
