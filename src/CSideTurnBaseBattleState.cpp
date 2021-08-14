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
    uint32_t cmd_size=cmd_list.size();
    if(cmd_size>0)
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
            else if(menu_name.compare("attack_comand")==0)
            {
                if(obj_id>0)
                {
                    item.center_target_obj_id=obj_id; //4
                }
                std::vector<int> ids=m_database->GetListObjectIds("players");
                int player_id=ids[m_current_command_player];
                int eid=m_database->GetObjectData(player_id,"equipment");
                int wid=m_database->GetObjectData(eid,"lefthand");
                item.using_obj_id=wid;
            }
        }
        m_seq_command_list.push_back(item);
        m_current_command_player++;
    }
    else
    {
        if(m_seq_command_list.size()>0)
        {
            m_current_command_player--;
            m_seq_command_list.pop_back();
        }
    }

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

    if(m_substate==substate::BATTLE_INIT_STATE)
    {
        GE_LOG("Prepare battle scene....\n");
        int draw_x=m_player_rect.x;
        ge_common_struct::ge_rect rect=sdlutil2::LoadWindowRect(m_context);
        int screenx=rect.w*draw_x/100;
        int speed=m_player_accerleration*m_batterprepare_time;
        for(size_t i=0; i<m_players.size(); i++)
        {
            int x=m_players[i].GetX();
            x-=speed;
            if(x>=screenx)
            {
                m_players[i].SetX(x);
            }
            else
            {
                m_players[i].SetX(screenx);
            }

        }
        if(m_frame>m_batterprepare_time)
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
    else if(m_substate==substate::BATTLE_STATE)
    {
        UpdateBattle();
    }else if(m_substate==substate::ENDING_STATE){
        Accounting();
    }
    m_animation_manager.Update();
    UpdatePlayer();


}

void CSideTurnBaseBattleState::Draw()
{
    //SDL_Renderer * renderer=sdlutil2::GetRenderer(m_context);
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    //绘制界面
    m_ui_manager.Draw();
    //绘制sprites
    DrawPlayer();
    DrawEnemy();

    //绘制特效
    m_animation_manager.Draw();

    //绘制菜单
    sdlutil2::RenderPresent(m_context);
    m_frame++;

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
}

void CSideTurnBaseBattleState::DrawEnemy()
{
    for(size_t i=0; i<m_enemies.size(); i++)
    {
        if(m_enemy_alive[i])
        {
            CSpriteGameObject& obj=m_enemies[i];
            int screenx=obj.GetX();
            int screeny=obj.GetY();
            obj.Play();
            int frameidx=obj.GetFrameIdx();
            float alpha=obj.GetAlpha();
            int iAlpha=std::round(alpha*255.0f);
            //if(iAlpha<255)
            //{
            //      GE_LOG("%d===%f==%f===\n",iAlpha,alpha*255.0f,alpha);
            //}
            sdlutil2::RenderSprite(m_context,obj.GetSprite(),screenx,
                                   screeny,frameidx,obj.GetRenderScale(),iAlpha);
        }else{

            //GE_LOG("enemy %zu is dead \n",i);
        }
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
        if(i==m_current_command_player && m_substate==substate::COMMAND_STATE)
        {
            screenx-=80;
        }
        int frameidx=obj.GetFrameIdx();
        sdlutil2::RenderSprite(m_context,sprite,screenx,screeny,frameidx,m_player_scale);
    }
}

void CSideTurnBaseBattleState::UpdatePlayer()
{
    if(m_frame%5==0)
    {
        for(size_t i=0; i<m_players.size(); i++)
        {
            m_players[i].Step();
        }
    }
}


void CSideTurnBaseBattleState::LoadSprites()
{
    //int draw_x=m_player_rect.x;
    int draw_y=m_player_rect.y;
    ge_common_struct::ge_rect rect=sdlutil2::LoadWindowRect(m_context);
    std::vector<int> ids=m_database->GetListObjectIds("players");
    std::vector<int> ids_ene=m_database->GetListObjectIds("battle");

    //hard coding
    //int screenx=rect.w*draw_x/100;
    int screenx=rect.w;
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
        int hp_max=m_database->GetObjectData(id,"hpmax");
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
        m_enemy_hps.push_back(hp_max);
        m_enemy_ids.push_back(id);
        m_enemy_alive.push_back(true);
    }
}

void CSideTurnBaseBattleState::SortCommand()
{

    for(size_t i=0; i<m_seq_command_list.size(); i++)
    {
        m_seq_command_list[i].sort_num=i; //注意引用和赋值
    }
    std::sort(m_seq_command_list.begin(),m_seq_command_list.end());

}


void CSideTurnBaseBattleState::ProcessBattle()
{

    //prepare
    SortCommand();
    m_substate=substate::BATTLE_STATE;
}

bool CSideTurnBaseBattleState::IsPast(uint32_t time)
{
    //由于uint32有循环特质，防止超越边界
    if(m_frame-time>=0 && m_frame-time<TIMECHECK)
    {
        //当前时间距离结束帧不能太久
        return true;
    }
    else
    {
        return false;
    }

}

void CSideTurnBaseBattleState::UpdateBattle()
{
    //两种策略，一种策略是上一个行动结束后，才能进行下一个行动，需要获取行动结束标志
    //另一种策略根据行动时间添加动画，多个行动可以并行，需要复杂的数据结构
    //指令结束后，需要根据状态进入结算还是下一行动

    for(size_t i=0; i<m_timer_event.size(); i++)
    {
        timer_event event= m_timer_event[i];
        if(m_frame-event.frame<=10)
        {
            ProcessTimerEvent(event);
            m_timer_event.erase(m_timer_event.begin()+i);
            i--;
        }
    }
    //结束战斗
    //if enemy is dead into ending phase
    bool flag=true;
    for(size_t i=0;i<m_enemy_alive.size();i++){
        if(m_enemy_alive[i]){
            flag=false;
        }
    }
    if(flag){
        //没有活着的敌人
        m_substate=substate::ENDING_STATE;
        for(size_t i=0;i<m_players.size();i++){
            //TODO 死了的人保持死亡状态
            m_players[i].UpdateDirection("win");
        }
    }
    //标记去世的对象为死亡状态
    if(m_seq_command_list.size()>0)
    {
        ge_common_struct::command_item& item=m_seq_command_list.back();
        if(item.prossesed)
        {
            if(IsPast(item.end_frame))
            {
                m_seq_command_list.pop_back();
            }
            else
            {
                //do nothing except update painting data
            }
        }
        else
        {
            item.prossesed=true;
            //Calculate damage only once
            //添加动画
            int obj_id=item.source_obj_id;
            std::string obj_type=m_database->GetObjectType(obj_id);
            if(obj_type.compare("player")==0)
            {
                std::vector<int>ids=m_database->GetListObjectIds("players");
                int player_no=0;
                for(size_t i=0; i<ids.size(); i++)
                {
                    if(obj_id==ids[i])
                    {
                        player_no=i;
                        break;
                    }

                }
                MoveForwardPlayer(player_no);
                if(item.group_target)
                {
                    //群体攻击

                }
                else
                {
                    //单体攻击
                    int tar_obj_id=item.center_target_obj_id;
                    if(tar_obj_id>-1)
                    {
                        //有具体对象
                        object_type type=GetObjectType(tar_obj_id);
                        if(type==object_type::ENEMY)
                        {
                            int enemy_seq=-1;
                            for(size_t i=0; i<m_enemy_ids.size(); i++)
                            {
                                if(tar_obj_id==m_enemy_ids[i])
                                {
                                    enemy_seq=i;
                                    //Choose another enemy is enemy is dead;
                                    break;
                                }
                            }
                            HitEnemy(player_no,enemy_seq,item.using_obj_id);
                        }
                    }
                }


                item.end_frame=m_frame+30;

            }
            else if(obj_type.compare("enemy")==0)
            {



            }
        }
    }
    else
    {

        m_substate=substate::COMMAND_INIT_STATE;
        m_current_command_player=0;
    }

}

void CSideTurnBaseBattleState::MoveForwardPlayer(int player_no)
{
    CSpriteGameObject* player=&m_players[player_no];
    CAnimationItem move_player;
    move_player.SetAnimateType(CAnimationItem::AnimateType::MOVE_SPRITE);
    move_player.SetActionName("leftward");
    move_player.SetStartFrame(0);
    move_player.SetEndFrame(4);
    move_player.SetObject(player);
    move_player.SetEndLoc(ge_common_struct::ge_point(
                              player->GetX()-50,player->GetY()));
    move_player.SetResetPosition(true);
    m_animation_manager.AddAnimateItem(move_player);

}

CSideTurnBaseBattleState::object_type
CSideTurnBaseBattleState::GetObjectType(int obj_id)
{
    std::string obj_type=m_database->GetObjectType(obj_id);
    if(obj_type.compare("player")==0)
    {
        return object_type::PLAYER;
    }
    else if(obj_type.compare("enemy")==0)
    {
        return object_type::ENEMY;
    }
    else
    {
        return object_type::OTHER;
    }
}

void CSideTurnBaseBattleState::HitEnemy(int player_no,int enemy_no
                                        ,int with_object)
{
    int enemy_id=m_enemy_ids[enemy_no];
    if(m_enemy_hps[enemy_no]>0)
    {
        CAnimationItem hit_enemy;
        hit_enemy.SetAnimateType(CAnimationItem::AnimateType::FLASH_SPRITE);
        hit_enemy.SetStartFrame(3);
        hit_enemy.SetEndFrame(10);
        hit_enemy.SetActionName("stand");
        hit_enemy.SetResetPosition(true);
        hit_enemy.SetObject(&m_enemies[enemy_no]);
        m_animation_manager.AddAnimateItem(hit_enemy);

        int num=rand()%100;
        CAnimationItem text_item;
        text_item.SetAnimateType(CAnimationItem::AnimateType::TEXT_MOTION);
        text_item.SetStartFrame(11);
        text_item.SetEndFrame(20);
        text_item.SetObject(&m_enemies[enemy_no]);
        text_item.SetText(std::to_string(num));
        ge_common_struct::ge_color color;
        color.r=255;
        color.g=255;
        color.b=255;
        text_item.SetFontColor(color);
        m_animation_manager.AddAnimateItem(text_item);
        m_enemy_hps[enemy_no]-=num;

    }
    else
    {
        //change target
        std::vector<int> ids_ene=m_database->GetListObjectIds("battle");
        size_t s=ids_ene.size();
        if(s>0)
        {
            int rand_enemy=rand()%s;
            int e_id=ids_ene[rand_enemy];
            for(size_t i=0; i<m_enemy_ids.size(); i++)
            {
                if(e_id==m_enemy_ids[i])
                {
                    HitEnemy(player_no,i,with_object);
                    return;
                }
            }
        }
        else
        {

            //TODO wining state
            return;
        }

    }

    if(with_object>-1)
    {
        std::string otype= m_database->GetObjectType(with_object);
        if(otype.compare("weapon")==0)
        {
            std::string animation_type=m_database->
                                       GetObjectText(with_object,"animation");
            if(animation_type.compare("bullet")==0)
            {
                CAnimationItem shoot;
                shoot.SetAnimateType(CAnimationItem::AnimateType::PROJECTILE);
                shoot.SetStartFrame(3);
                shoot.SetEndFrame(10);
                shoot.SetSpriteName("bullet");
                shoot.SetObject(&m_players[player_no]);
                shoot.SetTargetObject(&m_enemies[enemy_no]);
                m_animation_manager.AddAnimateItem(shoot);

            }
        }
    }




    if(m_enemy_hps[enemy_no]<0)
    {
        GE_LOG("enemy %d died \n",enemy_no);
        std::vector<int> ids=m_database->GetListObjectIds("battle");
        for(size_t i=0; i<ids.size(); i++)
        {
            int obj_id=ids[i];
            if(obj_id==enemy_id)
            {
                m_database->RemoveObjectFromList("battle",obj_id);
                DisappearEnemy(enemy_no);
                break;
            }
        }
    }

}


void CSideTurnBaseBattleState::DisappearEnemy(int enemy_no)
{
    CAnimationItem item;
    item.SetAnimateType(CAnimationItem::AnimateType::DISAPPEAR);
    item.SetStartFrame(20);
    item.SetEndFrame(25);
    item.SetObject(&m_enemies[enemy_no]);
    item.SetResetPosition(false);
    item.SetActionName(m_enemies[enemy_no].GetCurrentAction());
    m_animation_manager.AddAnimateItem(item);

    timer_event event;
    event.object_no=enemy_no;
    event.event_type=event_type::ENEMY_DIED;
    event.frame=m_frame+25;
    event.object_type=object_type::ENEMY;
    m_timer_event.push_back(event);
}

void CSideTurnBaseBattleState::ProcessTimerEvent(CSideTurnBaseBattleState
        ::timer_event event)
{
    if(event.event_type==CSideTurnBaseBattleState::event_type::ENEMY_DIED){
        m_enemy_alive[event.object_no]=false;
    }
}

void CSideTurnBaseBattleState::Accounting(){


}
