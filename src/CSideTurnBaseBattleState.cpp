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

void CSideTurnBaseBattleState::PreviousCommandPlayer(){
   //找前一个命令不能用-1的办法,存在前面所有人员都失能
   //所以得从命令栈里出栈
   if(m_seq_command_list.size()>0){
        ge_common_struct::command_item item=m_seq_command_list.back();
        m_current_command_player=item.source_obj_id;
        m_seq_command_list.pop_back();
   }
}

void CSideTurnBaseBattleState::FirstFunctionalPlayer(){
    if(m_player_data.size()>0 && m_player_data[0].IsAlive()){
        m_current_command_player=0;
    }else{
        m_current_command_player=0;
        NextFunctionalPlayer();
    }
}

void CSideTurnBaseBattleState::NextFunctionalPlayer(){
    m_current_command_player++;
    if(m_current_command_player==m_player_data.size()){
        //do nothing
    }else if(!m_player_data[m_current_command_player].IsAlive()){
        NextFunctionalPlayer();
    }
}

void CSideTurnBaseBattleState::TranslateCommand(std::vector<ge_common_struct
        ::menu_command> &cmd_list)
{
    uint32_t cmd_size=cmd_list.size();
    if(cmd_size>0)
    {
        ge_common_struct::command_item item;
        int player_order=m_current_command_player;
        int player_id=m_player_data[player_order].GetObjectId();
        item.source_obj_no=player_order;
        item.source_obj_id=player_id; //1
        item.sort_num=m_player_data[player_order].GetSpeed();
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
                int eid=m_database->GetObjectData(player_id,"equipment");
                int wid=m_database->GetObjectData(eid,"lefthand");
                item.using_obj_id=wid;
            }
        }
        m_seq_command_list.push_back(item);
        NextFunctionalPlayer();
    }
    else
    {
        if(m_seq_command_list.size()>0)
        {
            PreviousCommandPlayer();
        }
    }

}

void CSideTurnBaseBattleState::Init()
{
    GE_LOG("init battle state\n");
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

}

void CSideTurnBaseBattleState::Cleanup()
{

}

void CSideTurnBaseBattleState::Pause()
{

}

void CSideTurnBaseBattleState::ResetBattle()
{
    m_players.clear();
    m_enemies.clear();
    m_enemy_data.clear();
    m_player_data.clear();
    m_defeat_enemies.clear();


    if(m_timer_event.size()>0)
    {
        GE_LOG("Abnormal event size ================\n");
    }
    if(m_log_text.size()>0)
    {
        GE_LOG("Abnormal log size ================\n");
    }
    if(m_seq_command_list.size()>0)
    {
        GE_LOG("Abnormal cmd size ================\n");
    }
    m_seq_command_list.clear();
    m_timer_event.clear();
    m_log_text.clear();

    m_frame=0;
    m_last_timer=0;
    m_current_command_player=0;
    m_substate=substate::BATTLE_INIT_STATE;

}

void CSideTurnBaseBattleState::Resume()
{
    m_state_value=0;
    ResetBattle();
    LoadSprites();
    FirstFunctionalPlayer();
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
        else if(m_substate==substate::ENDING_STATE)
        {
            ge_common_struct::key_event kevent=event.get_top_event();
            if(m_log_text.size()>0 &&
                    kevent.press==ge_common_struct::key_press_type::KEY_PRESS)
            {
                //持续按键的事件过滤掉
                m_log_text.pop_back();
                if(m_log_text.size()==0)
                {
                    m_database->SetTextData("log","  ");
                }
            }
        }
        m_event_manager.EventPublish(inputevent);
    }
}

void CSideTurnBaseBattleState::Update()
{
    CheckEvent();

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
            if(m_current_command_player>=m_player_data.size()){
                //开始战斗
                m_substate=substate::BATTLE_COMMAND_PREPARE_STATE;
                m_last_timer=m_frame;
            }
            else if(m_player_data[m_current_command_player].IsAlive()){
                 //当前玩家还活着
                 std::vector<ge_common_struct::menu_command> clist
                =m_ui_manager.GetCommand();
                TranslateCommand(clist);
                m_substate=substate::COMMAND_INIT_STATE;
            }else{

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
    }
    else if(m_substate==substate::ENDING_STATE)
    {
        Accounting();
    }
    else if(m_substate==substate::FAIL_STATE){
        this->m_state_value=4;
    }
    else if(m_substate==substate::EXIT_STATE)
    {

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
    //std::vector 不会检查数组越界当数组越界时返回结果未知。
    if(m_current_command_player>=m_player_data.size())
    {
        //数组越界代表所有人都命令执行完毕
    }
    else
    {
        bool alive=m_player_data[m_current_command_player].IsAlive();
        if(alive)
        {
            CMenuInputEvent inputevent;
            inputevent.SetInputEvent(event);
            inputevent.SetCurrentSubState(0);
            inputevent.setMenuInitPanel("battle_command");
            m_event_manager.EventPublish(inputevent);

        }
        else
        {
            GE_LOG("EXCEPTION dead command occurs\n");
        }
    }
    //不论什么情况都进入command translation
    m_substate=substate::COMMAND_STATE;

}

void CSideTurnBaseBattleState::DrawEnemy()
{
    for(size_t i=0; i<m_enemies.size(); i++)
    {
        if(m_enemy_data[i].IsAlive())
        {
            CSpriteGameObject& obj=m_enemies[i];
            int screenx=obj.GetX();
            int screeny=obj.GetY();
            obj.Play();
            int frameidx=obj.GetFrameIdx();
            float alpha=obj.GetAlpha();
            int iAlpha=std::round(alpha*255.0f);
            ge_common_struct::ge_color black;
            black.r=0;
            black.g=0;
            black.b=0;
            if(obj.IsEnableMask())
            {
                sdlutil2::MaskSprite(m_context,
                                     obj.GetSprite(),
                                     screenx,
                                     screeny,
                                     frameidx,
                                     obj.GetRenderScale(),
                                     obj.GetMaskColor(),black);
            }
            else
            {
                sdlutil2::RenderSprite(m_context,obj.GetSprite(),screenx,
                                       screeny,frameidx,obj.GetRenderScale(),iAlpha);
            }

        }
        else
        {

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
            CRPGPlayerData &player=m_player_data[i];
            int hp=player.GetHp();
            int id=player.GetObjectId();
            if(!player.IsAlive())
            {
                m_players[i].UpdateDirection("dead");
            }
            m_database->SetObjectData(id,"hp",hp);
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
        int hp_max=m_database->GetObjectData(id,"hpmax");
        int hp=m_database->GetObjectData(id,"hp");
        int tp=m_database->GetObjectData(id,"tp");
        int tpmax=m_database->GetObjectData(id,"tpmax");
        int strength=m_database->GetObjectData(id,"strength");
        int con=m_database->GetObjectData(id,"con");
        int inteli=m_database->GetObjectData(id,"int");
        int dex=m_database->GetObjectData(id,"dex");
        int spd=m_database->GetObjectData(id,"speed");
        CSprite* sprite=m_sprite_db->GetSprite(sprite_name);
        CSpriteGameObject player(sprite);
        player.UpdateDirection("leftward");//TODO 增加方法名称
        int x=screenx;
        int y=screeny+i*height;
        player.SetX(x);
        player.SetY(y);
        m_players.push_back(player);

        CRPGPlayerData playerdata;
        playerdata.SetHpmax(hp_max);
        playerdata.SetHp(hp);
        playerdata.SetStrength(strength);
        playerdata.SetConstitution(con);
        playerdata.SetInteligence(inteli);
        playerdata.SetDexterity(dex);
        playerdata.SetSpeed(spd);

        if(hp<0)
        {
            playerdata.SetAlive(false);
        }
        else
        {
            playerdata.SetAlive(true);
        }
        playerdata.SetObjectId(id);
        m_player_data.push_back(playerdata);

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
        int strength=m_database->GetObjectData(id,"strength");
        int con=m_database->GetObjectData(id,"con");
        int inteli=m_database->GetObjectData(id,"int");
        int dex=m_database->GetObjectData(id,"dex");
        int spd=m_database->GetObjectData(id,"speed");
        int gold=m_database->GetObjectData(id,"gold");
        int exp=m_database->GetObjectData(id,"exp");

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

        CRPGEnemyData enemydata;
        enemydata.SetAlive(true);
        enemydata.SetHp(hp_max);
        enemydata.SetHpmax(hp_max);
        enemydata.SetStrength(strength);
        enemydata.SetInteligence(inteli);
        enemydata.SetConstitution(con);
        enemydata.SetDexterity(dex);
        enemydata.SetObjectId(id);
        enemydata.SetSpeed(spd);
        enemydata.SetExp(exp);
        enemydata.SetGold(gold);
        m_enemy_data.push_back(enemydata);

    }
}

void CSideTurnBaseBattleState::SortCommand()
{

    for(size_t i=0; i<m_seq_command_list.size(); i++)
    {
        ge_common_struct::command_item& item=m_seq_command_list[i];
        item.sort_num=item.sort_num+(rand()%(item.sort_num+1));
        //GE_LOG("item %lld speed  %d\n",i,m_seq_command_list[i].sort_num);
    }
    std::sort(m_seq_command_list.begin(),m_seq_command_list.end());

}

void CSideTurnBaseBattleState::GetAlivePlayer(std::vector<int>& alive_players)
{
    for(size_t i=0; i<m_player_data.size(); i++)
    {
        if(m_player_data[i].IsAlive())
        {
            alive_players.push_back(i);
        }
    }

}

void CSideTurnBaseBattleState::AddEnemyCommand()
{

    std::vector<int> alive_players;
    GetAlivePlayer(alive_players);
    int alive_size=alive_players.size();
    auto roll=ge_helper::GetDiceRoller(0,alive_size-1);
    for(size_t i=0; i<m_enemy_data.size(); i++)
    {
        if(m_enemy_data[i].IsAlive())
        {
            ge_common_struct::command_item item;
            CRPGEnemyData& data=m_enemy_data[i];
            int spd=data.GetSpeed();
            item.sort_num=spd+rand()%(spd+1);
            item.command_name="attack";
            item.source_obj_no=i;
            item.source_obj_id=m_enemy_data[i].GetObjectId();
            item.using_obj_id=-1;

            int roll_result=roll();
            //GE_LOG("===auto attack= player=%d  initiaty %d\n",roll_result,item.sort_num);
            int player_no=alive_players[roll_result];
            item.center_target_obj_id=-1;
            item.center_target_no=player_no;
            m_seq_command_list.push_back(item);
        }
    }

}


void CSideTurnBaseBattleState::ProcessBattle()
{

    //prepare
    AddEnemyCommand();
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

void CSideTurnBaseBattleState::CheckEvent()
{

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

}

void CSideTurnBaseBattleState::UpdateBattle()
{
    //两种策略，一种策略是上一个行动结束后，才能进行下一个行动，需要获取行动结束标志
    //另一种策略根据行动时间添加动画，多个行动可以并行，需要复杂的数据结构
    //指令结束后，需要根据状态进入结算还是下一行动

    //结束战斗
    //if enemy is dead into ending phase
    bool flag=true;
    bool playerfail_flag=true;
    for(size_t i=0; i<m_enemy_data.size(); i++)
    {
        if(m_enemy_data[i].IsAlive())
        {
            flag=false;
            break;
        }
    }

    for(size_t i=0;i<m_player_data.size();i++){
        if(m_player_data[i].IsAlive()){
            playerfail_flag=false;
            break;
        }
    }

    if(playerfail_flag){
        //玩家战场全部阵亡
        m_substate=substate::FAIL_STATE;
        return;
    }

    if(flag)
    {
        //没有活着的敌人
        m_substate=substate::ENDING_STATE;
        for(size_t i=0; i<m_players.size(); i++)
        {
            //TODO 死了的人保持死亡状态
            m_players[i].UpdateDirection("win");
        }
        int gold=0;
        int exp=0;
        for(size_t i=0; i<m_defeat_enemies.size(); i++)
        {
            gold+=m_defeat_enemies[i].GetGold();
            exp+=m_defeat_enemies[i].GetExp();
        }
        std::string exp_log="";
        exp_log+=u8"获得"+std::to_string(exp)+u8"经验";
        std::string gold_log="";
        gold_log+=u8"获得"+std::to_string(gold)+u8"金钱";
        m_log_text.push_back(gold_log);
        m_log_text.push_back(exp_log);
        return;
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
            int obj_no=item.source_obj_no;
            int tar_no=item.center_target_no;
            object_type obj_type=GetObjectType(obj_id);
            if(obj_type==object_type::PLAYER)
            {
                //int player_no=item.source_obj_no;
                MoveForwardPlayer(obj_no);
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
                            for(size_t i=0; i<m_enemy_data.size(); i++)
                            {
                                if(tar_obj_id==m_enemy_data[i].GetObjectId())
                                {
                                    enemy_seq=i;
                                    //Choose another enemy is enemy is dead;
                                    break;
                                }
                            }
                            HitEnemy(obj_no,enemy_seq,item.using_obj_id);
                        }
                    }
                }


                item.end_frame=m_frame+30;

            }
            else if(obj_type==object_type::ENEMY)
            {
                FlashEnemy(obj_no);
                HitPlayer(obj_no,tar_no);
                item.end_frame=m_frame+30;
            }
        }
    }
    else
    {

        m_substate=substate::COMMAND_INIT_STATE;
        FirstFunctionalPlayer();
    }

}

void CSideTurnBaseBattleState::FlashEnemy(int enemy_no)
{
    CSpriteGameObject* object=&m_enemies[enemy_no];
    CAnimationItem flash_enemy;
    flash_enemy.SetAnimateType(CAnimationItem::AnimateType::SHINE_SPRITE);
    flash_enemy.SetAnimateName("flash_enemy");
    flash_enemy.SetActionName("stand");
    flash_enemy.SetStartFrame(0);
    flash_enemy.SetEndFrame(10);
    flash_enemy.SetObject(object);
    flash_enemy.SetResetPosition(true);
    m_animation_manager.AddAnimateItem(flash_enemy);

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
    int enemy_id=m_enemy_data[enemy_no].GetObjectId();
    int hp_now=m_enemy_data[enemy_no].GetHp();
    if(hp_now>0)
    {
        CAnimationItem hit_enemy;
        hit_enemy.SetAnimateType(CAnimationItem::AnimateType::FLASH_SPRITE);
        hit_enemy.SetStartFrame(3);
        hit_enemy.SetEndFrame(10);
        hit_enemy.SetActionName("stand");
        hit_enemy.SetResetPosition(true);
        hit_enemy.SetObject(&m_enemies[enemy_no]);
        m_animation_manager.AddAnimateItem(hit_enemy);

        int damage=CalcDamage(player_no,enemy_no,with_object);

        CAnimationItem text_item;
        text_item.SetAnimateType(CAnimationItem::AnimateType::TEXT_MOTION);
        text_item.SetStartFrame(11);
        text_item.SetEndFrame(20);
        text_item.SetObject(&m_enemies[enemy_no]);
        text_item.SetText(std::to_string(damage));
        ge_common_struct::ge_color color;
        color.r=255;
        color.g=255;
        color.b=255;
        text_item.SetFontColor(color);
        m_animation_manager.AddAnimateItem(text_item);

        m_enemy_data[enemy_no].SetHp(hp_now-damage);

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
            for(size_t i=0; i<m_enemy_data.size(); i++)
            {
                if(m_enemy_data[i].GetHp()>0 &&
                        e_id==m_enemy_data[i].GetObjectId())
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
        else
        {

        }
    }
    else
    {

        CAnimationItem slash;
        slash.SetActionName("slash");
        slash.SetActionName("slash");
        slash.SetAnimateType(CAnimationItem::AnimateType::SHOW_VFX);
        slash.SetStartFrame(6);
        slash.SetEndFrame(16);
        slash.SetSpriteName("vfx_slash_sword");
        slash.SetObject(&m_enemies[enemy_no]);
        slash.SetFrameRate(3);
        slash.SetLoop(false);
        m_animation_manager.AddAnimateItem(slash);

    }




    if(m_enemy_data[enemy_no].GetHp()<=0)
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
    if(event.event_type==CSideTurnBaseBattleState::event_type::ENEMY_DIED)
    {
        m_enemy_data[event.object_no].SetAlive(false);
        m_defeat_enemies.push_back(m_enemy_data[event.object_no]);
    }
    else if(event.event_type==CSideTurnBaseBattleState::event_type::ACCOUNT_END)
    {
        //quit state
        this->m_state_value=2;
    }
    else if(event.event_type==CSideTurnBaseBattleState::event_type::PLAYER_DIED)
    {
        m_player_data[event.object_no].SetAlive(false);
    }
}

void CSideTurnBaseBattleState::Accounting()
{
    if(m_log_text.size()>0)
    {
        std::string log_text=m_log_text.back();
        std::string log=m_database->GetTextData("log");
        if(log.compare(log_text)==0)
        {

        }
        else
        {
            m_database->SetTextData("log",log_text);
        }
    }
    else
    {
        timer_event event;
        event.event_type=event_type::ACCOUNT_END;
        event.frame=m_frame+30;
        m_timer_event.push_back(event);
        m_substate=substate::EXIT_STATE;
    }

}

int CSideTurnBaseBattleState::CalcEnemyDamage(int ene_no,int play_no)
{
    CRPGEnemyData edata=m_enemy_data[ene_no];
    CRPGPlayerData pdata=m_player_data[play_no];
    int strength=edata.GetStrength();
    int defence=pdata.GetConstitution();
    int damage=strength/2-defence/2;
    if(damage<=0)
    {
        damage=1;
    }
    return damage;

}

int CSideTurnBaseBattleState::CalcDamage(int player_no,
        int enemy_no,int with_obj)
{

    //int player_id=m_player_data[player_no].GetObjectId();
    //int enemy_id=m_enemy_data[enemy_no].GetObjectId();
    //int strength=m_enemy_data[enemy_no].GetStrength();
    int strength=m_player_data[player_no].GetStrength();
    int defence=m_enemy_data[enemy_no].GetConstitution();
    int attack_point=0;
    if(with_obj>-1)
    {
        attack_point+=m_database->GetObjectData(with_obj,"attack");
    }

    int damage=attack_point+strength/2-defence/2;
    if(damage<=0)
    {
        damage=1;
    }
    return damage+10;
}

int CSideTurnBaseBattleState::ChangeTargetofEnemy()
{
    std::vector<int> alive_players;
    GetAlivePlayer(alive_players);
    int size_of_player=alive_players.size();
    if(size_of_player>0)
    {
        auto roll=ge_helper::GetDiceRoller(0,size_of_player-1);
        int pos=roll();
        int target=alive_players[pos];
        return target;
    }
    else
    {
        return -1;
    }

}


void CSideTurnBaseBattleState::HitPlayer(int enemy_no,int player_no)
{
    //CRPGEnemyData& enemy_data=m_enemy_data[enemy_no];
    //CRPGPlayerData& player_data=m_player_data[player_no];
    //int ene_id=enemy_data.GetObjectId();
    //int player_id=player_data.GetObjectId();
    int target=player_no;
    int hp=m_player_data[target].GetHp();

    if(hp<=0)
    {
        target=ChangeTargetofEnemy();
        GE_LOG("target changes from %d to %d\n",player_no,target);
        //update target hp because target has changed;
        hp=m_player_data[target].GetHp();
    }
    if(target<0)
    {
        return;
    }
    //target=3;//hard coding
    //hp=m_player_data[target].GetHp();//add

    CSpriteGameObject* player_sprite=&m_players[target];
    int curx=player_sprite->GetX();
    int cury=player_sprite->GetY();
    ge_common_struct::ge_point p(curx+20,cury);
    CAnimationItem item;
    item.SetAnimateType(CAnimationItem::AnimateType::MOVE_SPRITE);
    item.SetStartFrame(5);
    item.SetEndFrame(9);
    item.SetObject(player_sprite);
    item.SetActionName("hit");
    item.SetResetPosition(true);
    item.SetAnimateName("hitplayer");
    item.SetEndLoc(p);
    m_animation_manager.AddAnimateItem(item);


    int damage=CalcEnemyDamage(enemy_no,target);
    CAnimationItem text_item;
    text_item.SetAnimateType(CAnimationItem::AnimateType::TEXT_MOTION);
    text_item.SetStartFrame(7);
    text_item.SetEndFrame(14);
    text_item.SetObject(player_sprite);
    text_item.SetText(std::to_string(damage));

    ge_common_struct::ge_color color;
    color.r=255;
    color.g=255;
    color.b=255;
    text_item.SetFontColor(color);
    m_animation_manager.AddAnimateItem(text_item);


    int hpnew=hp-damage-35;
    m_player_data[target].SetHp(hpnew);


    if(hpnew<=0)
    {
        timer_event event;
        event.object_no=target;
        event.event_type=event_type::PLAYER_DIED;
        event.frame=m_frame+7;
        event.object_type=object_type::PLAYER;
        m_timer_event.push_back(event);
    }

}
