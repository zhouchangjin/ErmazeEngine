#include "CSideTurnBaseBattleState.h"

CSideTurnBaseBattleState::CSideTurnBaseBattleState()
{
    //ctor
}

CSideTurnBaseBattleState::CSideTurnBaseBattleState(CGameContext* context)
    :CGameState(context)
{

}

CSideTurnBaseBattleState::~CSideTurnBaseBattleState()
{
    //dtor release m_panels m_menu
}

void CSideTurnBaseBattleState::LogCommand(std::vector<ge_common_struct
        ::menu_command> &cmd)
{
    for(size_t i=0; i<cmd.size(); i++)
    {
        ge_common_struct::menu_command mcmd=cmd[i];
        std::string mname=mcmd.menu_name;
        std::string cname=mcmd.command_name;
        int order=mcmd.menu_order;
        int obj_id=mcmd.obj_id;
        GE_LOG("=>%s#%s(%d) ",mname.c_str(),cname.c_str(),order);
        if(obj_id>-1)
        {
            GE_LOG(" target obj ($%d)",obj_id);
        }
    }
    GE_LOG("\n");
}

void CSideTurnBaseBattleState::TranslateCommand(std::vector<ge_common_struct
        ::menu_command> &cmd_list)
{
    ge_common_struct::command_item item;
    int player_order=m_current_command_player;
    std::vector<int> ids=m_database->GetListObjectIds("players");
    int pid=ids[player_order];

    item.source_obj_id=pid; //1
    for(size_t i=0; i<cmd_list.size(); i++)
    {
        ge_common_struct::menu_command menu_cmd=cmd_list[i];
        std::string menu_name=menu_cmd.menu_name;
        std::string cmd_name=menu_cmd.command_name;
        int obj_id=menu_cmd.obj_id;
        if(menu_name.compare("battle_command")==0)
        {
            if(cmd_name.compare("")!=0)
            {
                item.command_name=cmd_name; //2
            }
        }
        else if(menu_name.compare("item_comand")==0)
        {
            if(obj_id>0)
            {
                item.using_obj_id=obj_id; //3
            }
        }
        else if(menu_name.compare("player_list")==0)
        {
            if(obj_id>0)
            {
                item.center_target_obj_id=obj_id;//4
            }
        }
        else if(menu_name.compare("enemy_list")==0){
            if(obj_id>0){
                item.center_target_obj_id=obj_id; //4
            }
        }
    }
    m_seq_command_list.push_back(item);
}

void CSideTurnBaseBattleState::Init()
{
    LoadComponents();

    m_ui_manager.SetGameContext(m_context);
    m_ui_manager.Init();
    m_ui_manager.SetMenuPointerName(m_menu_pointer);
    m_event_manager.EventSubscribe(&m_ui_manager,CUIManager::ProcessInput);
    m_animation_manager.SetGameContext(m_context);
    m_animation_manager.Init();
    //0代表状态不改变
    m_state_value=0;

    m_player_rect.x=80;
    m_player_rect.y=20;
    m_enemy_rect.x=10;
    m_enemy_rect.y=20;
    m_enemy_rect.w=50;
    m_enemy_rect.h=50;

    LoadUIDef();
    LoadSprites();

}

void CSideTurnBaseBattleState::Cleanup()
{

}

void CSideTurnBaseBattleState::Pause()
{

}

void CSideTurnBaseBattleState::Resume()
{
    m_state_value=0;
}

void CSideTurnBaseBattleState::HandleEvent(ge_common_struct::input_event event)
{
    if(event.get_top_event().key!=ge_common_struct::key_event_type::NO_EVENT)
    {

        CMenuInputEvent inputevent;
        inputevent.SetInputEvent(event);
        if(m_substate==substate::COMMAND_INIT_STATE)
        {

        }
        else if(m_substate==substate::COMMAND_STATE)
        {
            inputevent.SetCurrentSubState(1);
        }
        else if(m_substate==substate::BATTLE_STATE)
        {
            inputevent.SetCurrentSubState(2);
        }
        m_event_manager.EventPublish(inputevent);
    }
}

void CSideTurnBaseBattleState::Update()
{
    m_frame++;
    if(m_substate==substate::BATTLE_INIT_STATE)
    {
        GE_LOG("Prepare battle scene....\n");
        if(m_frame>20)
        {
            GE_LOG("Prepare battle scene done.\n");
            m_substate=substate::COMMAND_INIT_STATE;
        }
    }
    else if(m_substate==substate::COMMAND_STATE)
    {

        //检查命令是否完备，如果完备则转下一个状态
        if(m_ui_manager.IsPopPanelHidden())
        {
            std::vector<ge_common_struct::menu_command> clist
                =m_ui_manager.GetCommand();
            //LogCommand(clist);
            TranslateCommand(clist);
            if(m_current_command_player>3)
            {
                //全部完毕
                m_substate=substate::BATTLE_COMMAND_PREPARE_STATE;
                m_last_timer=m_frame;
            }
            else
            {
                m_substate=substate::COMMAND_INIT_STATE;
            }
        }
    }
    else if(m_substate==substate::COMMAND_INIT_STATE)
    {
        ge_common_struct::input_event event;
        ge_common_struct::key_event_type etype=ge_common_struct
                                               ::key_event_type::KEY_CONFIRM;
        event.add_event(etype);
        InitMenu(event);
    }
    else if(m_substate==substate::BATTLE_COMMAND_PREPARE_STATE)
    {
        ProcessBattle();//only once change state
    }
    else if(m_substate==substate::BATTLE_STATE){
        UpdateBattle();
    }
    m_animation_manager.Update();
    UpdatePlayer();


}

void CSideTurnBaseBattleState::Draw()
{
    //绘制界面
    m_ui_manager.Draw();
    //绘制sprites
    DrawPlayer();
    DrawEnemy();

    //绘制特效
    m_animation_manager.Draw();

    //绘制菜单
    sdlutil2::RenderPresent(m_context);

}

int CSideTurnBaseBattleState::GetStateValue()
{
    return this->m_state_value;
}

void CSideTurnBaseBattleState::PrepareData()
{

}

void CSideTurnBaseBattleState::LoadComponents()
{
    m_sprite_db=CServiceLocator::GetService<CSpriteDB>
                (CServiceLocator::ServiceID::SPRITE_DB);
    m_database=CServiceLocator::GetService<CGameDatabase>
               (CServiceLocator::ServiceID::DATABASE);
    //Test code
    /**
    CBaseParticleEmitter* emmiter=
        new CBaseParticleEmitter(m_particle_system.GetParticlePool());
    m_particle_system.AddEmitter(emmiter);
    CProjectileEmitter* projectile_emitter=new CProjectileEmitter();
    m_particle_system.AddEmitter(projectile_emitter);
    CProjectile pj;
    pj.SetPosX(800);
    pj.SetPosY(120);
    pj.SetVelocityX(-3);
    pj.SetVelocityY(3);
    pj.SetAX(-3);
    pj.SetAY(1);
    pj.SetLife(30);
    pj.SetTextureName("bullet");
    projectile_emitter->SetProjectile(pj);
    **/
}

void CSideTurnBaseBattleState::LoadUIDef()
{

    xmlutils::MyXMLDoc doc=xmlutils::LoadXML("./ui/battle.ui");
    xmlutils::MyXMLNode node=doc.GetNode("ui");
    xmlutils::MyXMLNode hud=node.Child("hud");
    xmlutils::MyXMLNode menu=node.Child("menu");

    xmlutils::MyXMLNode panel=hud.Child("panel");
    xmlutils::MyXMLNode window=menu.Child("window");

    int id=0;
    std::string idStr="";
    for(; panel; panel=panel.NextSlibing("panel"))
    {
        id++;
        ge_common_struct::dom_node* dom_node_ptr=ge_fileutil::parse_dom(panel);
        if(panel.HasAttribute("id"))
        {
            idStr=panel.StrAttribute("id");
        }
        else
        {
            idStr+=std::to_string(id);
        }
        m_ui_manager.AddPanel(idStr,dom_node_ptr);
    }
    idStr="";
    for(; window; window=window.NextSlibing("window"))
    {
        id++;
        ge_common_struct::dom_node* dom_node_ptr=ge_fileutil::parse_dom(window);
        if(window.HasAttribute("id"))
        {
            idStr=window.StrAttribute("id");
        }
        else
        {
            idStr+=std::to_string(id);
        }
        m_ui_manager.AddPanel(idStr,dom_node_ptr,true,true);
    }

}

void CSideTurnBaseBattleState::InitMenu(ge_common_struct::input_event& event)
{
    GE_LOG("player %d\n",m_current_command_player);
    CMenuInputEvent inputevent;
    inputevent.SetInputEvent(event);
    inputevent.SetCurrentSubState(0);
    inputevent.setMenuInitPanel("battle_command");
    m_event_manager.EventPublish(inputevent);
    m_substate=substate::COMMAND_STATE;
    m_current_command_player++;
}

void CSideTurnBaseBattleState::DrawEnemy()
{
    for(size_t i=0; i<m_enemies.size(); i++)
    {
        CSpriteGameObject& obj=m_enemies[i];
        int screenx=obj.GetX();
        int screeny=obj.GetY();
        obj.Play();
        int frameidx=obj.GetFrameIdx();
        sdlutil2::RenderSprite(m_context,obj.GetSprite(),screenx,
                               screeny,frameidx,obj.GetRenderScale());
    }

}

void CSideTurnBaseBattleState::DrawPlayer()
{

    for(size_t i=0; i<m_players.size(); i++)
    {
        CSpriteGameObject& obj=m_players[i];
        CSprite* sprite=obj.GetSprite();
        int screenx=obj.GetX();
        int screeny=obj.GetY();
        obj.Play();
        if(i==m_current_command_player-1 && m_substate!=substate::BATTLE_STATE)
        {
            screenx-=80;
        }
        int frameidx=obj.GetFrameIdx();
        sdlutil2::RenderSprite(m_context,sprite,screenx,screeny,frameidx,m_player_scale);
    }
}

void CSideTurnBaseBattleState::UpdatePlayer()
{
    if(m_frame%10==0)
    {
        for(size_t i=0; i<m_players.size(); i++)
        {
            m_players[i].Step();
        }
    }
}


void CSideTurnBaseBattleState::LoadSprites()
{
    int draw_x=m_player_rect.x;
    int draw_y=m_player_rect.y;
    ge_common_struct::ge_rect rect=sdlutil2::LoadWindowRect(m_context);
    std::vector<int> ids=m_database->GetListObjectIds("players");
    std::vector<int> ids_ene=m_database->GetListObjectIds("battle");

    //hard coding
    int screenx=rect.w*draw_x/100;
    int screeny=rect.h*draw_y/100;
    int height=80;


    for(size_t i=0; i<ids.size(); i++)
    {
        int id=ids[i];
        std::string sprite_name= m_database->GetObjectText(id,"sprite");
        CSprite* sprite=m_sprite_db->GetSprite(sprite_name);
        CSpriteGameObject player(sprite);
        player.UpdateDirection("leftward");//TODO 增加方法名称
        int x=screenx;
        int y=screeny+i*height;
        player.SetX(x);
        player.SetY(y);
        m_players.push_back(player);
    }
    int startx=m_enemy_rect.x*rect.w/100;
    int starty=m_enemy_rect.y*rect.h/100;
    int maxw=m_enemy_rect.w*rect.w/100;
    //int maxh=m_enemy_rect.h*rect.h/100;
    int maxx=startx+maxw;
    //int maxy=starty+maxh;
    int curx=startx;
    int cury=starty;
    for(size_t i=0; i<ids_ene.size(); i++)
    {
        int id=ids_ene[i];
        std::string sprite_name=m_database->GetObjectText(id,"sprite");
        CSprite* sprite=m_sprite_db->GetSprite(sprite_name);
        CSpriteGameObject enemy(sprite);
        enemy.UpdateDirection("stand");
        enemy.SetX(curx);
        enemy.SetY(cury);
        enemy.SetRenderScale(m_enemy_scale);
        int xnew=curx+sprite->GetSpriteSheet()->GetSpriteWidth()*m_enemy_scale;
        if(xnew<maxx)
        {
            curx=xnew;
        }
        else
        {
            curx=startx;
            cury=cury+sprite->GetSpriteSheet()->GetSpriteHeight()*m_enemy_scale;
        }
        m_enemies.push_back(enemy);
    }
}


void CSideTurnBaseBattleState::ProcessBattle(){

    //prepare
    m_substate=substate::BATTLE_STATE;
}

void CSideTurnBaseBattleState::UpdateBattle(){
    //添加动画 流程

    //两种策略，一种策略是上一个行动结束后，才能进行下一个行动，需要获取行动结束标志

    //另一种策略根据行动时间来确定下一行动，添加动画

    //判断是否所有行动动画播放完毕，结束战斗循环进入指令状态或结算状态
}
