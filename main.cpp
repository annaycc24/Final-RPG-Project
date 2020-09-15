//collision fun要不要直接刪掉？ 可以刪
//第二關Boss主動攻擊的code要不要直接刪掉？ 可以刪
//typedef的位置可以往下移嗎？
//about的code能移到menu下嗎？
//確認Anna註解的code能刪掉嗎？

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define GAME_TERMINATE 666
#define GO_to_STARTGAME_player 1
#define GO_to_ABOUT_PAGE1 2
#define RETURN_to_MENU 3
#define GO_to_ABOUT_PAGE2 4
#define GO_to_ABOUT_PAGE3 5
#define GO_to_STARTGAME_stage1 6
#define LEAVE_GAME 7
#define GO_to_GAMEOVER 8
#define GO_to_STARTGAME_stage2 9
#define GO_to_NEXTSTAGE 10
#define GO_to_WINPAGE 11

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER* game_update_timer;
ALLEGRO_TIMER* game_stage3_timer;
ALLEGRO_COLOR color;
ALLEGRO_COLOR color_player_background_left;
ALLEGRO_COLOR color_player_background_right;
ALLEGRO_COLOR color_player_background_word_left;
ALLEGRO_COLOR color_player_background_word_right;

ALLEGRO_SAMPLE *song_menu;
ALLEGRO_SAMPLE *song_game;
ALLEGRO_SAMPLE *song_stage2;
ALLEGRO_SAMPLE *soundeffect;
ALLEGRO_SAMPLE *addblood;
ALLEGRO_SAMPLE *winsound;
ALLEGRO_SAMPLE *losesound;
ALLEGRO_SAMPLE_INSTANCE *songInstance_menu;
ALLEGRO_SAMPLE_INSTANCE *songInstance_game;
ALLEGRO_SAMPLE_INSTANCE *songInstance_stage2;

ALLEGRO_FONT *font;
ALLEGRO_FONT *font1;
ALLEGRO_FONT *font2;
ALLEGRO_FONT *font3;
ALLEGRO_FONT *font4;
ALLEGRO_FONT *font5;
ALLEGRO_FONT *font6;
ALLEGRO_BITMAP* img1 = NULL;   //初階劍士(about)
ALLEGRO_BITMAP* img2 = NULL;   //初階弓箭手(about)
ALLEGRO_BITMAP* img3 = NULL;   //高階劍士(第一階段->第二階段再呈現,與NPC相同頁面)
ALLEGRO_BITMAP* img4 = NULL;   //高階弓箭手(第一階段->第二階段再呈現,與NPC相同頁面)
ALLEGRO_BITMAP* img5 = NULL;   //A monster(about)
ALLEGRO_BITMAP* img6 = NULL;   //B monster(about)
ALLEGRO_BITMAP* img7 = NULL;   //C monster(about)
ALLEGRO_BITMAP* img8 = NULL;   //Boss(about)
ALLEGRO_BITMAP* img10 = NULL;   //background2(first stage)
ALLEGRO_BITMAP* img11 = NULL;   //background3(second stage)
ALLEGRO_BITMAP* img12 = NULL;   //background4(game over page)
ALLEGRO_BITMAP* img13 = NULL;   //background5(leave page)
ALLEGRO_BITMAP* img14 = NULL;   //background6(NPC page)
ALLEGRO_BITMAP* img20 = NULL;   //background12(about page)
ALLEGRO_BITMAP* img21 = NULL;   //background13(menu)
ALLEGRO_BITMAP* img22 = NULL;   //初階劍士(explore)
ALLEGRO_BITMAP* img23 = NULL;   //初階弓箭手(explore)
ALLEGRO_BITMAP* img24 = NULL;   //高階劍士(explore)
ALLEGRO_BITMAP* img25 = NULL;   //高階弓箭手(explore)
ALLEGRO_BITMAP* img26 = NULL;   //A monster(explore)
ALLEGRO_BITMAP* img27 = NULL;   //B monster(explore)
ALLEGRO_BITMAP* img28 = NULL;   //C monster(explore)
ALLEGRO_BITMAP* img29 = NULL;   //Boss(explore)
ALLEGRO_BITMAP* img30 = NULL;   //A monster(attack)
ALLEGRO_BITMAP* img31 = NULL;   //B monster(attack)
ALLEGRO_BITMAP* img32 = NULL;   //C monster(attack)
ALLEGRO_BITMAP* img33 = NULL;   //Boss(attack)
ALLEGRO_BITMAP* img34 = NULL;   //heart
ALLEGRO_BITMAP* img37 = NULL;   //background16(final win page)

const int width = 900;  //900 2400
const int height = 640;  //640 1500
int role_pos_x, role_pos_y;
bool* mouse_state;
int pos_x, pos_y;   //The position of rectangle's left-up corner.
int mouse_x, mouse_y;   //mouse position
int click_x, click_y;
int rec_x1, rec_x2, rec_y1, rec_y2;
bool end;
bool START = false, ROLE_LEFT = false, ROLE_RIGHT = false, ABOUT = false, MENU = false, NEXT1 = false, NEXT2 = false, LEAVE = false, GAMEOVER = false, STAGE2 = false, NEXTSTAGE = false, WINPAGE = false;  //按鈕是否被按下
bool startgame_player = false, startgame_stage1 = false, startgame_stage2 = false, startgame_stage3 = false, about = false, menu = true, leave = false, gameover = false, winpage = false;  //遊戲停留在哪一個畫面
int value;  //確認玩家是按yes還是no
int mouse_count_npc;
bool A1_add_blood, B1_add_blood, Boss_add_blood;
int A1_add_blood_x, A1_add_blood_y, B1_add_blood_x, B1_add_blood_y, Boss_add_blood_x, Boss_add_blood_y;
//關於音樂的
bool losesound_destroyed, winsound_destroyed;
//關於角色的bool
bool role_atk = false;  //角色發出攻擊時，role_atk會變true，讓角色切換成有爆炸的圖
bool left = false, right = false; //用來分別角色是往左還是往右，用來翻轉角色
bool role_begin = true;  //角色初始位置
float role_hp;
//關於小怪物的
int monster_movespeed = 3, role_movespeed = 4, stage2_role_movespeed = 8;  //怪物和角色的移動速度
bool A1_dir = true, A2_dir = true, A3_dir = true, A4_dir = true, B1_dir = true, B2_dir = true, B3_dir = false, C1_dir = true, C2_dir = true, A1_add_blood_dir = true, B1_add_blood_dir = true, Boss_dir = true, Boss_add_blood_dir = true;   //怪物移動方向
bool A1_to_calculate_attack, A2_to_calculate_attack, A3_to_calculate_attack, A4_to_calculate_attack, B1_to_calculate_attack, B2_to_calculate_attack, B3_to_calculate_attack, C1_to_calculate_attack, C2_to_calculate_attack, Boss_to_calculate_attack;   //如果已經計算過，就會變成false，否則會一直加分上去
bool Boss_atk = false;   //Boss主動發出攻擊時，Boss_atk會變true，讓Boss切換成有閃電攻擊圖(最後沒用到)

typedef struct {
    int monster_pos_x;
    int monster_pos_y;
    int monster_hp;
    bool die;
} Monster;
Monster A1, A2, A3, A4, B1, B2, B3, C1, C2, Boss;

bool next1 = false;   //for about_pages2(),遊戲停留在哪一個畫面
bool next2 = false;   //for about_pages3(),遊戲停留在哪一個畫面
void show_err_msg(int msg);
const int FPS = 60;
bool key_state[ALLEGRO_KEY_MAX];
int flag;  //用在main裡面，用來break兩層迴圈
int attack = 0;   //(第一階段)紀錄目前攻擊狀態(以分數直接代表狀態,總共有3種,10, 20, 40)
float calculate_status = 0;   //(第一階段)紀錄目前計分累計狀態
int my_posx, my_posy;   //for collision function(最後沒用到)
int counter_posx, counter_posy;   //for collision function(最後沒用到)

void game_init();   //initialize
void game_startgame_stage1_init();   //第一關初始化
void game_startgame_stage3_init();   //第二關初始化
void game_begin_menu();   //主選單
void game_begin_startgame_player();   //第一階段選擇角色畫面
void game_begin_startgame_stage1();   //第一階段遊戲畫面
void game_begin_startgame_stage2();   //中轉NPC畫面
void game_begin_startgame_stage3();   //第二階段遊戲畫面
void game_begin_about_page1();   //角色介紹第一頁
void game_begin_about_page2();   //角色介紹第二頁
void game_begin_about_page3();   //角色介紹第三頁 ＆ 遊戲進行方式說明
void game_begin_gameover();   //兩階段game over畫面
void game_begin_winpage();   //第二階段最後的獲勝畫面
void game_begin_leave();   //給玩家確認要離開遊戲還是回主畫面
int process_event_menu();   //處理menu event(mouse, keyboard, display)
int game_run_menu();   //處理main msg狀態
void game_destroy();   //free memory
void game_update_menu();   //處理menu mouse, keyboard位置＆狀態
void game_update_startgame_player();   //處理第一階段選角色畫面mouse, keyboard位置＆狀態
void game_update_startgame_stage1();
void game_update_startgame_stage2();
void game_update_startgame_stage3();
void game_update_about_page1();   //處理about mouse, keyboard位置＆狀態
void game_update_about_page2();
void game_update_about_page3();
void game_update_gameover();
void game_update_winpage();
void game_update_leave();
bool collision(int x, int y, int cx, int cy, int width, int height);  //x = my_pos_x, y = my_pos_y, cx= counter_pos_x, cy = counter_pos_y, width = al_get_bitmap_width(), height = al_get_bitmap_height()

bool cannot_go(int x, int y){   //fot stage1
    if (x >= width - 100 || x <= 0 || y >= height-105 || y <= 200) return true;
    else return false;
}

bool cannot_go2(int x, int y){   //fot stage2(character)
    if (x >= width - 100 || x <= 0 || y >= height-105 || y <= 0) return true;
    else return false;
}

bool cannot_go3(int x, int y){   //fot stage2(Boss)
    if (x >= width - 400 || x <= 0 || y >= height-105 || y <= 0) return true;
    else return false;
}

bool collision(int x, int y, int cx, int cy, int width, int height) {
    //x, y, cx, cy表示該image(0,0)位置; x, y表示我的位置/ cx, cy表示對方的位置
    if (x + width < cx || x > cx + width || y + height < cy || y > cy + height) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    int msg = 0;
    game_init();
    game_begin_menu();
    printf("Hello world!!!\n");
    pos_x = pos_y = mouse_x = mouse_y = 400;
    while (msg != GAME_TERMINATE) {
        flag = 0;
        msg = game_run_menu();
        if (msg == GAME_TERMINATE) {
            printf ("Your score is %f\n", calculate_status);
            //printf ("%f\n", calculate_status);
        }
        else if (msg == GO_to_STARTGAME_player) {   //START鍵
            MENU = false;  //到遊戲畫面時，主畫面就可以變false了
            game_begin_startgame_player();}
        else if (msg == GO_to_STARTGAME_stage1) {
            game_startgame_stage1_init();
            START = false;
            game_begin_startgame_stage1();  //ROLE_LEFT or ROLE_RIGHT is true, remember switch to false when entering to the next stage
            //else game_begin_gameover();
            //al_destroy_sample(losesound);
            //al_destroy_sample(soundeffect);
        }
        else if (msg == GO_to_STARTGAME_stage2) {
            al_play_sample(winsound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); //不知道為什麼加在game_begin_start_stage2裡面會一直重複跑
            mouse_count_npc = 0;
            STAGE2 = false;
            game_begin_startgame_stage2();
        }
        else if (msg == GO_to_NEXTSTAGE) {
            game_startgame_stage3_init();
            NEXTSTAGE = false;
            game_begin_startgame_stage3();
        }
        else if (msg == GO_to_ABOUT_PAGE1) {   //ABOUT鍵
            MENU = false;   //待確認->這裡要加，因為離開主畫面後，MENU按鈕應變成false。
            game_begin_about_page1();}
        else if (msg == GO_to_ABOUT_PAGE2) {   //NEXT鍵,進入第二頁
            ABOUT = false;
            game_begin_about_page2();
        }
        else if (msg == GO_to_ABOUT_PAGE3) {   //NEXT鍵,進入第三頁
            NEXT1 = false;
            game_begin_about_page3();
        }
        else if (msg == GO_to_WINPAGE) {
            al_play_sample(winsound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); //不知道為什麼加在game_begin_start_stage2裡面會一直重複跑
            WINPAGE = false;
            ROLE_LEFT = false;
            ROLE_RIGHT = false;
            game_begin_winpage();
            al_destroy_sample(soundeffect);
            al_destroy_sample(addblood);
        }
        else if (msg == GO_to_GAMEOVER) {
            //al_play_sample(losesound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            ROLE_LEFT = false;
            ROLE_RIGHT = false;
            //GAMEOVER = false;
            game_begin_gameover();
            al_destroy_sample(soundeffect);
            al_destroy_sample(addblood);
        }
        else if (msg == LEAVE_GAME) {
            ROLE_LEFT = false;
            ROLE_RIGHT = false;
            game_begin_leave();
        }
        else if (msg == RETURN_to_MENU) {   //返回MENU鍵
            START = false;
            ABOUT = false;
            LEAVE = false;
            NEXT1 = false;
            NEXT2 = false;
            GAMEOVER = false;
            WINPAGE = false;
            if (!losesound_destroyed) {
                al_destroy_sample(losesound);
                losesound_destroyed = true; // 已經destroy過了，避免重複destroy
            }
            if (!winsound_destroyed) {
                al_destroy_sample(winsound);
                winsound_destroyed = true;
            }
            game_begin_menu();
        }
    }
    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()) {
        show_err_msg(-1);
    }

    pos_x = width / 2;
    pos_y = height / 2;
    color = al_map_rgb(255, 0, 255);
    display = al_create_display(width, height);
    event_queue = al_create_event_queue();
    game_update_timer = al_create_timer(1.0f / FPS);
    
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-1);
    }

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    //所有字體先在這邊load，避免記憶體隨著迴圈一直跑爆掉。
    //字體的大小可能需要隨視窗比例更改
    font1 = al_load_ttf_font("pirulen.ttf", 45, 0);
    font2 = al_load_ttf_font("pirulen.ttf", 30, 0);
    font3 = al_load_ttf_font("kaiu.ttf", 25, 0);
    font4 = al_load_ttf_font("kaiu.ttf", 20, 0);
    font5 = al_load_ttf_font("kaiu.ttf", 55, 0);
    font6 = al_load_ttf_font("kaiu.ttf", 40, 0);
    
    al_init_image_addon();
    //所有圖片在這邊也是先load，否則迴圈每跑一次，就會多load一次，然後一直佔用更多記憶體，就算是load一樣的東西。
    img1 = al_load_bitmap("swordsman1.png");
    img2 = al_load_bitmap("archer1.png");
    img3 = al_load_bitmap("swordsman2.png");
    img4 = al_load_bitmap("archer2.png");
    
    img5 = al_load_bitmap("A.png");
    img6 = al_load_bitmap("B.png");
    img7 = al_load_bitmap("C.png");
    img8 = al_load_bitmap("Boss.png");
     
    img10 = al_load_bitmap("background2.png");   //first stage
    img11 = al_load_bitmap("background3.jpeg");   //second stage
    img12 = al_load_bitmap("background4.png");   //game over page
    img13 = al_load_bitmap("background5.jpg");   //leave(any leave)
    img14 = al_load_bitmap("background6.jpg");   //intermediate(1->2),NPC
    img20 = al_load_bitmap("background12.jpg");   //about
    img21 = al_load_bitmap("background13.jpg");   //menu
    img22 = al_load_bitmap("sw1_ex.png");
    img23 = al_load_bitmap("ar1_ex.png");
    img24 = al_load_bitmap("sw2_ex.png");
    img25 = al_load_bitmap("ar2_ex.png");
    img26 = al_load_bitmap("A_ex.png");
    img27 = al_load_bitmap("B_ex.png");
    img28 = al_load_bitmap("C_ex.png");
    img29 = al_load_bitmap("Boss_ex.png");
    img30 = al_load_bitmap("A_atk.png");
    img31 = al_load_bitmap("B_atk.png");
    img32 = al_load_bitmap("C_atk.png");
    img33 = al_load_bitmap("Boss_atk.png");
    img34 = al_load_bitmap("heart.png");
    img37 = al_load_bitmap("background16.jpeg");   //final win page
    al_convert_mask_to_alpha(img34, al_map_rgb(255, 255, 255));
    
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(7);
    song_menu = al_load_sample("music_menu.ogg");
    song_game = al_load_sample("music_game1.ogg");
    if (!(song_stage2 = al_load_sample("stage2.ogg"))) printf ("ERR");
    //soundeffect = al_load_sample("soundeffect.wav");
    //winsound = al_load_sample("win.wav");
    //losesound = al_load_sample("lose.wav");
    

    //if (!al_load_sample("music_first.ogg")) printf ("fail\n");
    songInstance_menu = al_create_sample_instance(song_menu);
    songInstance_game = al_create_sample_instance(song_game);
    songInstance_stage2 = al_create_sample_instance(song_stage2);

    al_attach_sample_instance_to_mixer(songInstance_menu, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(songInstance_game, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(songInstance_stage2, al_get_default_mixer());
    
    al_set_sample_instance_playmode(songInstance_menu, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(songInstance_game, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(songInstance_stage2, ALLEGRO_PLAYMODE_LOOP);
    
    al_install_keyboard();
    al_install_mouse();
    const unsigned m_buttons = al_get_mouse_num_buttons();
    mouse_state = static_cast<bool*>(malloc((m_buttons + 1) * sizeof(bool))); //看網路上說C++要加這個static_cast<bool*>, Synrax:static_cast <new_type>, Returns a value of type new_type, (轉換值)應該是只開一個新的空間,裡面要放boolean值
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_hide_mouse_cursor(display);//用來隱藏滑鼠
    
    al_start_timer(game_update_timer);//這一行的後面不要加東西，因為會影響timer，東西都加在上面就好。
}

void game_startgame_stage1_init(){
    //每次進到遊戲都要初始化這些，不然離開後再進入就會有問題
    //printf ("YO\n");
    role_pos_x = width * 3/4 + 30;
    role_pos_y = height/2;  //+ 60
    calculate_status = 0;  //角色計分要歸零。
    attack = 0;
    role_hp = 1000; //不然好快就死了哈哈哈哈哈哈
    role_begin = true;
    
    soundeffect = al_load_sample("soundeffect.wav");
    addblood = al_load_sample("add-blood.wav");
    winsound = al_load_sample("win.wav");
    winsound_destroyed = false;
    losesound = al_load_sample("lose.wav");
    losesound_destroyed = false;
    
    A1_add_blood = false;  //是否吃到愛心、補過血
    B1_add_blood = false;
    
    A1_dir = true;
    A2_dir = true;
    A3_dir = true;
    A4_dir = true;
    B1_dir = true;
    B2_dir = true;
    B3_dir = false;
    C1_dir = true;
    C2_dir = true;
    A1_add_blood_dir = true;
    B1_add_blood_dir = true;
    
    //愛心位置
    A1_add_blood_x = 50;
    A1_add_blood_y = height-150;
    B1_add_blood_x = width/2-180;
    B1_add_blood_y = height/2;
    
    //怪物血量位置設定
    A1.monster_hp = 100;
    A1.monster_pos_x = width/10 + 110;
    A1.monster_pos_y = height/3;
    //A1.die = false;
    
    A2.monster_hp = 100;
    A2.monster_pos_x = width/20 - 40;
    A2.monster_pos_y = height/3 + 80;
    //A2.die = false;
    
    A3.monster_hp = 100;
    A3.monster_pos_x = width/10 + 200;
    A3.monster_pos_y = height *3/4;
    //A3.die = false;
    
    A4.monster_hp = 100;
    A4.monster_pos_x = width/10 + 560;
    A4.monster_pos_y = height/3 - 10;
    //A4.die = false;
    
    B1.monster_hp = 100;
    B1.monster_pos_x = width/6;
    B1.monster_pos_y = height * 2/3;
    //B1.die = false;
    
    B2.monster_hp = 100;
    B2.monster_pos_x = width/6 + 260;
    B2.monster_pos_y = height/2 - 60;
    //B2.die = false;
    
    B3.monster_hp = 100;
    B3.monster_pos_x = width * 3/5 - 20;
    B3.monster_pos_y = height/2 + 60;
    //B3.die = false;
    
    C1.monster_hp = 100;
    C1.monster_pos_x = width/12;
    C1.monster_pos_y = height/3;
    //C1.die = false;

    C2.monster_hp = 100;
    C2.monster_pos_x = width * 2/5 - 66;
    C2.monster_pos_y = height/3 + 120;
    //C2.die = false;
    
    A1_to_calculate_attack = true;
    A2_to_calculate_attack = true;
    A3_to_calculate_attack = true;
    A4_to_calculate_attack = true;
    B1_to_calculate_attack = true;
    B2_to_calculate_attack = true;
    B3_to_calculate_attack = true;
    C1_to_calculate_attack = true;
    C2_to_calculate_attack = true;
}

void game_startgame_stage3_init(){  //第二階段遊戲畫面初始化
    game_stage3_timer = al_create_timer(10);
    //角色初始化
    role_begin = true;
    role_pos_x = width * 3/4 + 30;
    role_pos_y = height/2;
    attack = 0;

    if (role_hp <= 400) {  //過關獎勵hp+600(最高上限1000,此處未達1000)
        role_hp += 600;
    } else {  //過關獎勵hp+600(最高上限1000)
        role_hp = 1000;
    }
    
    Boss_dir = true;  //怪物移動方向
    
    //怪物血量＆位置設定(Boss會有自己專屬血量表)
    Boss.monster_hp = 600;   //Boss血量初始值設定為600
    Boss.monster_pos_x = width/10 + 110;
    Boss.monster_pos_y = height/3;
    //A1.die = false;
    
    Boss_to_calculate_attack = true;   //Boss血量統計(待確認)
    al_register_event_source(event_queue, al_get_timer_event_source(game_stage3_timer));
    al_start_timer(game_stage3_timer);
}

void game_begin_menu() {  //主選單
    menu = true;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    about = false;
    next1 = false; //這邊要加上，否則回到主畫面時next1和next2都還是true，這樣再按about時會直接跳到最後一頁。
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    
    al_draw_scaled_bitmap(img21, 0, 0, al_get_bitmap_width(img21), al_get_bitmap_height(img21), 0, 0, width, height, 0);   //背景
    al_draw_text(font1, al_map_rgb(100, 0, 0), width / 2, height / 10 + 80, ALLEGRO_ALIGN_CENTRE, "Welcome to My Game");
    al_draw_rectangle(width/2 - 400, height/10+70, width/2 + 400, height/10 + 140, al_map_rgb(0, 0, 50), 4);
    al_draw_text(font1, al_map_rgb(100, 0, 0), width / 2, height / 10 + 230, ALLEGRO_ALIGN_CENTRE, "START");
    al_draw_text(font1, al_map_rgb(100, 0, 0), width / 2, height / 10 + 310, ALLEGRO_ALIGN_CENTRE, "ABOUT");
    al_draw_text(font1, al_map_rgb(100, 0, 0), width / 2, height / 10 + 390, ALLEGRO_ALIGN_CENTRE, "EXIT");
    rec_x1 = width / 2 - 145;
    rec_x2 = width / 2 + 145;
    rec_y1 = height / 10 - 10;
    rec_y2 = height / 10 + 60;
    for (int i = 230 ; i <= 390; i = i + 80){
        al_draw_rectangle(rec_x1, rec_y1 + i, rec_x2, rec_y2+i, al_map_rgb(0, 0, 50), 2);
    }
    al_play_sample_instance(songInstance_menu);
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_stage2);
    //if (draw) al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_startgame_player() {   //遊戲開始選角色畫面
    startgame_player = true;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    menu = false;
    about = false;
    next1 = false;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;

    al_clear_to_color(al_map_rgb(0,0,0));   //保留黑底,比較容易辨識
    al_draw_filled_rectangle(110, 190, 360, 500, color_player_background_left);
    al_draw_filled_rectangle(width-380, 190, width-100, 500, color_player_background_right);
    al_draw_text(font2, al_map_rgb(255, 255, 255), width / 2, height / 6, ALLEGRO_ALIGN_CENTRE, "CHOOSE A PLAYER");
    //圖文
    al_draw_text(font3, color_player_background_word_left, width/12 + 150, height/4 + 30, 0, "劍士");
    al_draw_text(font3, color_player_background_word_right, width - 330, height/4 + 30, 0, "弓箭手");
    al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1), width/12 + 40, height/3 + 30, 250, 250, 0);
    al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2), width - 360, height/3 + 20, 275, 275, 0);
    al_draw_text(font2, al_map_rgb(255, 255, 255), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Menu");
    //al_draw_rectangle(rec_x1, rec_y1 + 300, rec_x2, rec_y2+300, al_map_rgb(0, 0, 50), 2);
    //mouse
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_startgame_stage1(){   //第一階段遊戲畫面
    startgame_stage1 = true;
    startgame_player = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    menu = false;
    about = false;
    next1 = false;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    al_stop_sample_instance(songInstance_stage2);
    al_stop_sample_instance(songInstance_menu);
    al_play_sample_instance(songInstance_game);
    //第一階段遊戲畫面
    al_draw_scaled_bitmap(img10, 0, 0, al_get_bitmap_width(img10), al_get_bitmap_height(img10), 0, 0, width, height, 0);
    //al_draw_bitmap(img34, width/2, height/2, 0);
    //al_draw_scaled_bitmap(img34, 0, 0, al_get_bitmap_width(img34), al_get_bitmap_height(img34), 0, 0, width/2, height/2, 0);
    
    //怪物A1
    if (A1.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        if (A1_dir){//往右
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A1.monster_pos_x, A1.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
            if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130 && A1.monster_pos_y +39 <= role_pos_y+150 && A1.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    A1.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);  //怪物A1
                }
                else {
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (A1.monster_pos_y +39 <= role_pos_y+150 && A1.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A1.monster_pos_y +39 <= role_pos_y+90 && A1.monster_pos_y+120 >= role_pos_y+60 && A1.monster_pos_x <= role_pos_x){
                            A1.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A1.monster_pos_y +39 <= role_pos_y+90 && A1.monster_pos_y+120 >= role_pos_y+60 && A1.monster_pos_x >= role_pos_x){
                            A1.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);  //怪物A1
                        }
                    }
                    //if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        else { //怪物A1 往左
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);
            if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130 && A1.monster_pos_y +39 <= role_pos_y+150 && A1.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    A1.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);  //怪物A1
                    //printf ("%d\n", A1.monster_hp);
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
            else if (A1.monster_pos_y +39 <= role_pos_y+150 && A1.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A1.monster_pos_y +39 <= role_pos_y+90 && A1.monster_pos_y+120 >= role_pos_y+60 && A1.monster_pos_x <= role_pos_x){
                            A1.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A1.monster_pos_y +39 <= role_pos_y+90 && A1.monster_pos_y+120 >= role_pos_y+60 && A1.monster_pos_x >= role_pos_x){
                            A1.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    //if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A1.monster_pos_x + 80 >= role_pos_x && A1.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A1.monster_pos_x, A1.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        if (A1_dir) A1.monster_pos_x += monster_movespeed;
        else if (!A1_dir) A1.monster_pos_x -= monster_movespeed;
            //A1.monster_pos_x += 1;
        if (cannot_go(A1.monster_pos_x, A1.monster_pos_y)) {
            if (A1_dir) A1.monster_pos_x -= monster_movespeed;
            else if (!A1_dir) A1.monster_pos_x += monster_movespeed;
            A1_dir = !A1_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {  //怪物hp <= 0 死掉
        if (A1_to_calculate_attack) {
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 10;
            calculate_status += attack;
        }
        A1_to_calculate_attack = false;
        //printf ("calculate status = %f\n", calculate_status);
    }
    
    //怪物A2
    if (A2.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        if (A2_dir){//往右
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
            if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130 && A2.monster_pos_y +39 <= role_pos_y+150 && A2.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){ //適用劍士
                if (role_atk) {
                    A2.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                    //printf ("hp = %d\n", role_hp);
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (A2.monster_pos_y +39 <= role_pos_y+150 && A2.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A2.monster_pos_y +39 <= role_pos_y+90 && A2.monster_pos_y+120 >= role_pos_y+60 && A2.monster_pos_x <= role_pos_x){
                            A2.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A2.monster_pos_y +39 <= role_pos_y+90 && A2.monster_pos_y+120 >= role_pos_y+60 && A2.monster_pos_x >= role_pos_x){
                            A2.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                        }
                    }
                    //if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        else {  //怪物A1 往左
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);
            if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130 && A2.monster_pos_y +39 <= role_pos_y+150 && A2.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){ //適用劍士
                if (role_atk) {
                    A2.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);   //怪物A1
                    //printf ("%d\n", A1.monster_hp);
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
            else if (A2.monster_pos_y +39 <= role_pos_y+150 && A2.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A2.monster_pos_y +39 <= role_pos_y+90 && A2.monster_pos_y+120 >= role_pos_y+60 && A2.monster_pos_x <= role_pos_x){
                            A2.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A2.monster_pos_y +39 <= role_pos_y+90 && A2.monster_pos_y+120 >= role_pos_y+60 && A2.monster_pos_x >= role_pos_x){
                            A2.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    //if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A2.monster_pos_x + 80 >= role_pos_x && A2.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A2.monster_pos_x, A2.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        if (A2_dir) A2.monster_pos_x += monster_movespeed;
        else if (!A2_dir) A2.monster_pos_x -= monster_movespeed;
            //A1.monster_pos_x += 1;
        if (cannot_go(A2.monster_pos_x, A2.monster_pos_y)) {
            if (A2_dir) A2.monster_pos_x -= monster_movespeed;
            else if (!A2_dir) A2.monster_pos_x += monster_movespeed;
            A2_dir = !A2_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {
        if (A2_to_calculate_attack) {
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 10;
            calculate_status += attack;
        }
        A2_to_calculate_attack = false;
        //printf ("calculate status = %f\n", calculate_status);
    }
        
    //怪物A3
    if (A3.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        if (A3_dir){//往右
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
            if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130 && A3.monster_pos_y +39 <= role_pos_y+150 && A3.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){ //適用劍士
                if (role_atk) {
                    A3.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                    //printf ("hp = %d\n", role_hp);
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (A3.monster_pos_y +39 <= role_pos_y+150 && A3.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A3.monster_pos_y +39 <= role_pos_y+90 && A3.monster_pos_y+120 >= role_pos_y+60 && A3.monster_pos_x <= role_pos_x){
                            A3.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                        }
                    }
                    else if (right){
                        if (A3.monster_pos_y +39 <= role_pos_y+90 && A3.monster_pos_y+120 >= role_pos_y+60 && A3.monster_pos_x >= role_pos_x){
                            A3.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                        }
                    }
                    //if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        else {  //怪物A1 往左
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);
            if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130 && A3.monster_pos_y +39 <= role_pos_y+150 && A3.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    A3.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);   //怪物A1
                    //printf ("%d\n", A1.monster_hp);
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
            else if (A3.monster_pos_y +39 <= role_pos_y+150 && A3.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A3.monster_pos_y +39 <= role_pos_y+90 && A3.monster_pos_y+120 >= role_pos_y+60 && A3.monster_pos_x <= role_pos_x){
                            A3.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A3.monster_pos_y +39 <= role_pos_y+90 && A3.monster_pos_y+120 >= role_pos_y+60 && A3.monster_pos_x >= role_pos_x){
                            A3.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    //if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A3.monster_pos_x + 80 >= role_pos_x && A3.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A3.monster_pos_x, A3.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        if (A3_dir) A3.monster_pos_x += monster_movespeed;
        else if (!A3_dir) A3.monster_pos_x -= monster_movespeed;
            //A1.monster_pos_x += 1;
        if (cannot_go(A3.monster_pos_x, A3.monster_pos_y)) {
            if (A3_dir) A3.monster_pos_x -= monster_movespeed;
            else if (!A3_dir) A3.monster_pos_x += monster_movespeed;
            A3_dir = !A3_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {
        if (A3_to_calculate_attack) {
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 10;
            calculate_status += attack;
        }
        A3_to_calculate_attack = false;
        //printf ("calculate status = %f\n", calculate_status);
    }
    
    //怪物A4
    if (A4.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        if (A4_dir){//往右
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
            if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130 && A4.monster_pos_y +39 <= role_pos_y+150 && A4.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    A4.monster_hp -= 40;
                    //role_hp += 2;
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                    //printf ("hp = %d\n", role_hp);
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (A4.monster_pos_y +39 <= role_pos_y+150 && A4.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A4.monster_pos_y +39 <= role_pos_y+90 && A4.monster_pos_y+120 >= role_pos_y+60 && A4.monster_pos_x <= role_pos_x){
                            A4.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                        }
                    }
                    else if (right){
                        if (A4.monster_pos_y +39 <= role_pos_y+90 && A4.monster_pos_y+120 >= role_pos_y+60 && A4.monster_pos_x >= role_pos_x){
                            A4.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                        }
                    }
                    //if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        else {  //怪物A1 往左
            al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);
            if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130 && A4.monster_pos_y +39 <= role_pos_y+150 && A4.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    A4.monster_hp -= 40;
                    //role_hp += 2;
                    //printf ("%d\n", A1.monster_hp);
                    al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);   //怪物A1
                }
                else {
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 2;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
            else if (A4.monster_pos_y +39 <= role_pos_y+150 && A4.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                if (role_atk) {
                    if (left){
                        if (A4.monster_pos_y +39 <= role_pos_y+90 && A4.monster_pos_y+120 >= role_pos_y+60 && A4.monster_pos_x <= role_pos_x){
                            A4.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    else if (right){
                        if (A4.monster_pos_y +39 <= role_pos_y+90 && A4.monster_pos_y+120 >= role_pos_y+60 && A4.monster_pos_x >= role_pos_x){
                            A4.monster_hp -= 40;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);   //怪物A1
                        }
                    }
                    //if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 4;
                }
                else if (A4.monster_pos_x + 80 >= role_pos_x && A4.monster_pos_x + 35 <= role_pos_x + 130){
                    //al_draw_scaled_bitmap(img26, 0, 0, al_get_bitmap_width(img26), al_get_bitmap_height(img26), A4.monster_pos_x, A4.monster_pos_y, 150, 150, 0);   //怪物A1
                    role_hp -= 4;
                    if (role_hp <= 0) GAMEOVER = true;
                }
            }
        }
        if (A4_dir) A4.monster_pos_x += monster_movespeed;
        else if (!A4_dir) A4.monster_pos_x -= monster_movespeed;
            //A1.monster_pos_x += 1;
        if (cannot_go(A4.monster_pos_x, A4.monster_pos_y)) {
            if (A4_dir) A4.monster_pos_x -= monster_movespeed;
            else if (!A4_dir) A4.monster_pos_x += monster_movespeed;
            A4_dir = !A4_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {
        if (A4_to_calculate_attack) {
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 10;
            calculate_status += attack;
        }
        A4_to_calculate_attack = false;
        //printf ("calculate status = %f\n", calculate_status);
    }
    
    //怪物B1 怪物被攻擊到才閃光
    if (B1.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        al_draw_scaled_bitmap(img6, 0, 0, al_get_bitmap_width(img6), al_get_bitmap_height(img6), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);   //怪物B1
        if (B1.monster_pos_x + 80 >= role_pos_x && B1.monster_pos_x + 35 <= role_pos_x + 130 && B1.monster_pos_y +39 <= role_pos_y+150 && B1.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){//適用劍士
            if (role_atk) {
                B1.monster_hp -= 20;
                //role_hp += 4;
                al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
            }
            else {
                //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
                role_hp -= 4;
                if (role_hp <= 0) GAMEOVER = true;
                //printf ("hp = %d\n", role_hp);
            }
        }
        else if (B1.monster_pos_y +39 <= role_pos_y+150 && B1.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
            if (role_atk) {
                if (left){  //若角色朝左，只能殺左邊的怪物
                    if (B1.monster_pos_y +39 <= role_pos_y+90 && B1.monster_pos_y+120 >= role_pos_y+60 && B1.monster_pos_x <= role_pos_x){
                        B1.monster_hp -= 20;
                        //role_hp += 8;
                        //printf ("%d\n", A1.monster_hp);
                        al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
                    }
                }
                else if (right){ //若角色朝右，只能殺右邊的怪物
                    if (B1.monster_pos_y +39 <= role_pos_y+90 && B1.monster_pos_y+120 >= role_pos_y+60 && B1.monster_pos_x >= role_pos_x){
                        B1.monster_hp -= 20;
                        //role_hp += 8;
                        //printf ("%d\n", A1.monster_hp);
                        al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
                    }
                }
                //if (B1.monster_pos_x + 80 >= role_pos_x && B1.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 8;
            }
            else if (B1.monster_pos_x + 80 >= role_pos_x && B1.monster_pos_x + 35 <= role_pos_x + 130){
                //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
                role_hp -= 8;
                if (role_hp <= 0) GAMEOVER = true;
            }
        }
        //控制怪物移動方向
        if (B1_dir) B1.monster_pos_y += monster_movespeed;
        else if (!B1_dir) B1.monster_pos_y -= monster_movespeed;
        if (cannot_go(B1.monster_pos_x, B1.monster_pos_y)) {
            if (B1_dir) B1.monster_pos_y -= monster_movespeed;
            else if (!B1_dir) B1.monster_pos_y += monster_movespeed;
            B1_dir = !B1_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {  //怪物死掉之後計分
        if (B1_to_calculate_attack) {
            //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 20;
            calculate_status += attack;
        }
        B1_to_calculate_attack = false;
        //printf ("calculate status = %d\n", calculate_status);
    }
    
    //怪物B2 怪物被攻擊到閃光
    if (B2.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        al_draw_scaled_bitmap(img6, 0, 0, al_get_bitmap_width(img6), al_get_bitmap_height(img6), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);   //怪物B1
        if (B2.monster_pos_x + 80 >= role_pos_x && B2.monster_pos_x + 35 <= role_pos_x + 130 && B2.monster_pos_y +39 <= role_pos_y+150 && B2.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
            if (role_atk) {
                B2.monster_hp -= 20;
                //role_hp += 4;
                al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
            }
            else {
                //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B1.monster_pos_x, B1.monster_pos_y, 100, 100, 0);
                role_hp -= 4;
                if (role_hp <= 0) GAMEOVER = true;
                //printf ("hp = %d\n", role_hp);
            }
        }
        else if (B2.monster_pos_y +39 <= role_pos_y+150 && B2.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
            if (role_atk) {
                if (left){  //若角色朝左，只能殺左邊的怪物
                    if (B2.monster_pos_y +39 <= role_pos_y+90 && B2.monster_pos_y+120 >= role_pos_y+60 && B2.monster_pos_x <= role_pos_x){
                        B2.monster_hp -= 20;
                        //role_hp += 8;
                        //printf ("%d\n", A1.monster_hp);
                        al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
                    }
                }
                else if (right){  //若角色朝右，只能殺右邊的怪物
                    if (B2.monster_pos_y +39 <= role_pos_y+90 && B2.monster_pos_y+120 >= role_pos_y+60 && B2.monster_pos_x >= role_pos_x){
                        B2.monster_hp -= 20;
                        //role_hp += 8;
                        //printf ("%d\n", A1.monster_hp);
                        al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
                    }
                }
                //if (B2.monster_pos_x + 80 >= role_pos_x && B2.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 8;
            }
            else if (B2.monster_pos_x + 80 >= role_pos_x && B2.monster_pos_x + 35 <= role_pos_x + 130){
                //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
                role_hp -= 8;
                if (role_hp <= 0) GAMEOVER = true;
            }
        }
        //控制怪物移動方向
        if (B2_dir) B2.monster_pos_y -= monster_movespeed;
        else if (!B2_dir) B2.monster_pos_y += monster_movespeed;
        if (cannot_go(B2.monster_pos_x, B2.monster_pos_y)) {
            if (B2_dir) B2.monster_pos_y += monster_movespeed;
            else if (!B2_dir) B2.monster_pos_y -= monster_movespeed;
            B2_dir = !B2_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {//怪物死掉之後計分
        if (B2_to_calculate_attack) {
            //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 20;
            calculate_status += attack;
        }
        B2_to_calculate_attack = false;
        //printf ("calculate status = %d\n", calculate_status);
    }
     
    //怪物B3 怪物被攻擊到時閃光
    if (B3.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
         if (B3_dir){//往右
             al_draw_scaled_bitmap(img6, 0, 0, al_get_bitmap_width(img6), al_get_bitmap_height(img6), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
             if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130 && B3.monster_pos_y +39 <= role_pos_y+150 && B3.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                 if (role_atk) {
                     B3.monster_hp -= 20;
                     //role_hp += 4;
                     al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                 }
                 else {
                     //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A1
                     //printf ("hp = %d\n", role_hp);
                     role_hp -= 4;
                     if (role_hp <= 0) GAMEOVER = true;
                     //printf ("hp = %d\n", role_hp);
                 }
             }
             else if (B3.monster_pos_y +39 <= role_pos_y+150 && B3.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                 if (role_atk) {
                     if (left){
                         if (B3.monster_pos_y +39 <= role_pos_y+90 && B3.monster_pos_y+120 >= role_pos_y+60 && B3.monster_pos_x <= role_pos_x){
                             B3.monster_hp -= 20;
                             //role_hp += 8;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                         }
                     }
                     else if (right){
                         if (B3.monster_pos_y +39 <= role_pos_y+90 && B3.monster_pos_y+120 >= role_pos_y+60 && B3.monster_pos_x >= role_pos_x){
                             B3.monster_hp -= 20;
                             //role_hp += 8;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                         }
                     }
                     //if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 8;
                 }
                 else if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130){
                     //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物A3
                     role_hp -= 8;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
         }
         else {  //怪物A1 往左
             al_draw_scaled_bitmap(img6, 0, 0, al_get_bitmap_width(img6), al_get_bitmap_height(img6), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);
             if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130 && B3.monster_pos_y +39 <= role_pos_y+150 && B3.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){ //適用劍士
                 if (role_atk) {
                     B3.monster_hp -= 20;
                     //role_hp += 4;
                     al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);
                     //printf ("%d\n", A1.monster_hp);
                 }
                 else {
                     //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);   //怪物A1
                     role_hp -= 4;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
             else if (B3.monster_pos_y +39 <= role_pos_y+150 && B3.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){ //適用弓箭手
                 if (role_atk) {
                     if (left){
                         if (B3.monster_pos_y +39 <= role_pos_y+90 && B3.monster_pos_y+120 >= role_pos_y+60 && B3.monster_pos_x <= role_pos_x){
                             B3.monster_hp -= 20;
                             //role_hp += 8;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);
                         }
                     }
                     else if (right){
                         if (B3.monster_pos_y +39 <= role_pos_y+90 && B3.monster_pos_y+120 >= role_pos_y+60 && B3.monster_pos_x >= role_pos_x){
                             B3.monster_hp -= 20;
                             //role_hp += 8;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);
                         }
                     }
                     //if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 8;
                 }
                 else if (B3.monster_pos_x + 80 >= role_pos_x && B3.monster_pos_x + 35 <= role_pos_x + 130){
                     //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B3.monster_pos_x, B3.monster_pos_y, 100, 100, 0);   //怪物A1
                     role_hp -= 8;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
         }
         if (B3_dir) B3.monster_pos_x -= monster_movespeed;
         else if (!B3_dir) B3.monster_pos_x += monster_movespeed;
             //A1.monster_pos_x += 1;
         if (cannot_go(B3.monster_pos_x, B3.monster_pos_y)) {
             if (B3_dir) B3.monster_pos_x += monster_movespeed;
             else if (!B3_dir) B3.monster_pos_x -= monster_movespeed;
             B3_dir = !B3_dir;//怪物轉向
             //printf ("%d\n", dir);
         }
     }
     else {
         if (B3_to_calculate_attack) {
             al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
             attack = 20;
             calculate_status += attack;
         }
         B3_to_calculate_attack = false;
         //printf ("calculate status = %f\n", calculate_status);
     }
    
    //怪物C1
    if (C1.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
         if (C1_dir){  //往右
             al_draw_scaled_bitmap(img7, 0, 0, al_get_bitmap_width(img7), al_get_bitmap_height(img7), C1.monster_pos_x, C1.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C1
             if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130 && C1.monster_pos_y +39 <= role_pos_y+150 && C1.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
                 if (role_atk) {
                     C1.monster_hp -= 10;
                     //role_hp += 8;
                     al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C1
                 }
                 else {
                     role_hp -= 8;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
             else if (C1.monster_pos_y +39 <= role_pos_y+150 && C1.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                 if (role_atk) {
                     if (left){
                         if (C1.monster_pos_y +39 <= role_pos_y+90 && C1.monster_pos_y+120 >= role_pos_y+60 && C1.monster_pos_x <= role_pos_x){
                             C1.monster_hp -= 10;
                             //role_hp += 16;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C1
                         }
                     }
                     else if (right){
                         if (C1.monster_pos_y +39 <= role_pos_y+90 && C1.monster_pos_y+120 >= role_pos_y+60 && C1.monster_pos_x >= role_pos_x){
                             C1.monster_hp -= 10;
                             //role_hp += 16;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C1
                         }
                     }
                     //if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 16;
                 }
                 else if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130){
                     //al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C1
                     role_hp -= 16;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
         }
         else {  //怪物A1 往左
             al_draw_scaled_bitmap(img7, 0, 0, al_get_bitmap_width(img7), al_get_bitmap_height(img7), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
             if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130 && C1.monster_pos_y +39 <= role_pos_y+150 && C1.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){ //適用劍士
                 if (role_atk) {
                     C1.monster_hp -= 10;
                     //role_hp += 8;
                     al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
                 }
                 else {
                     //al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
                     role_hp -= 8;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
             else if (C1.monster_pos_y +39 <= role_pos_y+150 && C1.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
                 if (role_atk) {
                     if (left){
                         if (C1.monster_pos_y +39 <= role_pos_y+90 && C1.monster_pos_y+120 >= role_pos_y+60 && C1.monster_pos_x <= role_pos_x){
                             C1.monster_hp -= 10;
                             //role_hp += 16;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
                         }
                     }
                     else if (right){
                         if (C1.monster_pos_y +39 <= role_pos_y+90 && C1.monster_pos_y+120 >= role_pos_y+60 && C1.monster_pos_x >= role_pos_x){
                             C1.monster_hp -= 10;
                             //role_hp += 16;
                             //printf ("%d\n", A1.monster_hp);
                             al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
                         }
                     }
                     //if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 16;
                 }
                 else if (C1.monster_pos_x + 80 >= role_pos_x && C1.monster_pos_x + 35 <= role_pos_x + 130){
                     //al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C1.monster_pos_x, C1.monster_pos_y, 100, 100, 0);
                     role_hp -= 16;
                     if (role_hp <= 0) GAMEOVER = true;
                 }
             }
         }
         if (C1_dir) C1.monster_pos_x += monster_movespeed;
         else if (!C1_dir) C1.monster_pos_x -= monster_movespeed;
             //A1.monster_pos_x += 1;
         if (cannot_go(C1.monster_pos_x, C1.monster_pos_y)) {
             if (C1_dir) C1.monster_pos_x -= monster_movespeed;
             else if (!C1_dir) C1.monster_pos_x += monster_movespeed;
             C1_dir = !C1_dir;//怪物轉向
             //printf ("%d\n", dir);
         }
     }
     else {
         if (C1_to_calculate_attack) {
             al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
             attack = 40;
             calculate_status += attack;
         }
         C1_to_calculate_attack = false;
     }
    
    //怪物C2
    if (C2.monster_hp > 0){  //若hp<0，這裡就直接不用跑了
        al_draw_scaled_bitmap(img7, 0, 0, al_get_bitmap_width(img7), al_get_bitmap_height(img7), C2.monster_pos_x, C2.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);   //怪物C2
        if (C2.monster_pos_x + 80 >= role_pos_x && C2.monster_pos_x + 35 <= role_pos_x + 130 && C2.monster_pos_y +39 <= role_pos_y+150 && C2.monster_pos_y+120 >= role_pos_y && ROLE_LEFT){  //適用劍士
            if (role_atk) {
                C2.monster_hp -= 10;
                //role_hp += 8;
                al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C2.monster_pos_x, C2.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);
            }
            else {
                role_hp -= 8;
                if (role_hp <= 0) GAMEOVER = true;
            }
        }
        else if (C2.monster_pos_y +39 <= role_pos_y+150 && C2.monster_pos_y+120 >= role_pos_y && ROLE_RIGHT){  //適用弓箭手
            if (role_atk) {
                if (left){  //若角色朝左，只能殺左邊的怪物
                    if (C2.monster_pos_y +39 <= role_pos_y+90 && C2.monster_pos_y+120 >= role_pos_y+60 && C2.monster_pos_x <= role_pos_x){
                        C2.monster_hp -= 10;
                        //role_hp += 16;
                        al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C2.monster_pos_x, C2.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);
                    }
                }
                else if (right){ //若角色朝右，只能殺右邊的怪物
                    if (C2.monster_pos_y +39 <= role_pos_y+90 && C2.monster_pos_y+120 >= role_pos_y+60 && C2.monster_pos_x >= role_pos_x){
                        C2.monster_hp -= 10;
                        //role_hp += 16;
                        //printf ("%d\n", A1.monster_hp);
                        al_draw_scaled_bitmap(img28, 0, 0, al_get_bitmap_width(img28), al_get_bitmap_height(img28), C2.monster_pos_x, C2.monster_pos_y, 100, 100, ALLEGRO_FLIP_HORIZONTAL);
                    }
                }
                //if (C2.monster_pos_x + 80 >= role_pos_x && C2.monster_pos_x + 35 <= role_pos_x + 130) role_hp += 16;
            }
            else if (C2.monster_pos_x + 80 >= role_pos_x && C2.monster_pos_x + 35 <= role_pos_x + 130){
                role_hp -= 16;
                if (role_hp <= 0) GAMEOVER = true;
            }
        }
        //控制怪物移動方向
        if (C2_dir) C2.monster_pos_y -= monster_movespeed;
        else if (!C2_dir) C2.monster_pos_y += monster_movespeed;
        if (cannot_go(C2.monster_pos_x, C2.monster_pos_y)) {
            if (C2_dir) C2.monster_pos_y += monster_movespeed;
            else if (!C2_dir) C2.monster_pos_y -= monster_movespeed;
            C2_dir = !C2_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {//怪物死掉之後計分
        if (C2_to_calculate_attack) {
            //al_draw_scaled_bitmap(img27, 0, 0, al_get_bitmap_width(img27), al_get_bitmap_height(img27), B2.monster_pos_x, B2.monster_pos_y, 100, 100, 0);
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            attack = 40;
            calculate_status += attack;
        }
        C2_to_calculate_attack = false;
        //printf ("calculate status = %d\n", calculate_status);
    }
    
    //殺死A1後可以吃愛心補充血量
    if (A1.monster_hp <= 0){
        if (!A1_add_blood){
            al_draw_bitmap(img34, A1_add_blood_x, A1_add_blood_y, 0);
            if (A1_add_blood_x + 80 >= role_pos_x && A1_add_blood_x + 35 <= role_pos_x + 130 && A1_add_blood_y +39 <= role_pos_y+150 && A1_add_blood_y + 120 >= role_pos_y){
                A1_add_blood = true;
                al_play_sample(addblood, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                if (role_hp <= 900) role_hp += 100;
                else role_hp = 1000;
                
            }
            if (A1_add_blood_dir) A1_add_blood_x += 8;
            else if (!A1_add_blood_dir) A1_add_blood_x -= 8;
            if (cannot_go(A1_add_blood_x, A1_add_blood_y)) {
                if (A1_add_blood_dir) A1_add_blood_x -= 8;
                else if (!A1_add_blood_dir) A1_add_blood_x += 8;
                A1_add_blood_dir = !A1_add_blood_dir;//愛心轉向
            }
        }
    }
    if (B1.monster_hp <= 0){
        if (!B1_add_blood){
            al_draw_bitmap(img34, B1_add_blood_x, B1_add_blood_y, 0);
            if (B1_add_blood_x + 80 >= role_pos_x && B1_add_blood_x + 35 <= role_pos_x + 130 && B1_add_blood_y +39 <= role_pos_y+150 && B1_add_blood_y+120 >= role_pos_y){
                B1_add_blood = true;
                al_play_sample(addblood, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                if (role_hp <= 900) role_hp += 100;
                else role_hp = 1000;
            }
            if (B1_add_blood_dir) B1_add_blood_y += 8;
            else if (!B1_add_blood_dir) B1_add_blood_y -= 8;
            if (cannot_go(B1_add_blood_x, B1_add_blood_y)) {
                if (B1_add_blood_dir) B1_add_blood_y -= 8;
                else if (!B1_add_blood_dir) B1_add_blood_y += 8;
                B1_add_blood_dir = !B1_add_blood_dir;//愛心轉向
            }
        }
    }
    
    //角色
    if (ROLE_LEFT) {   //選左邊角色的話
        if (role_begin)al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1), role_pos_x, role_pos_y, 150, 150, 0);  //角色
        else if (left){
            if (role_atk){
                al_draw_scaled_bitmap(img22, 0, 0, al_get_bitmap_width(img22), al_get_bitmap_height(img22), role_pos_x, role_pos_y, 200, 160, 0);
                role_atk = false;  //角色
            }
            else al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1), role_pos_x, role_pos_y, 150, 150, 0);  //角色
        }
        else if (right){
            if (role_atk){
                al_draw_scaled_bitmap(img22, 0, 0, al_get_bitmap_width(img22), al_get_bitmap_height(img22), role_pos_x, role_pos_y, 160, 120, ALLEGRO_FLIP_HORIZONTAL);
                role_atk = false;//角色
            }
            else al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);  //角色
        }
        al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1)/2, 210, 15, 40, 30, 0);  //計分表角色標示(小頭圖示)
    }
    else if (ROLE_RIGHT) {   //選右邊角色的話
        if (role_begin) al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);
        else if (left){
            if (role_atk){
                al_draw_scaled_bitmap(img23, 0, 0, al_get_bitmap_width(img23), al_get_bitmap_height(img23), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);
                role_atk = false;
            }
            else al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);  //角色
        }
        else if (right){
            if (role_atk){
                al_draw_scaled_bitmap(img23, 0, 0, al_get_bitmap_width(img23), al_get_bitmap_height(img23), role_pos_x, role_pos_y, 150, 150, 0);
                role_atk = false;
            }
            else al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2), role_pos_x, role_pos_y, 150, 150, 0);  //角色
        }
        
        al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2) * 3/5, 210, 10, 50, 35, 0);  //計分表角色標示(小頭圖示)
    }
    
    /*
    以下code(if else)為角色計分表(if else部分為疊加上去的動態圖示)
    attack 紀錄目前攻擊狀態(以分數直接代表攻擊狀態,總共有3種,10, 20, 40)
    calculate_status 紀錄目前計分累計狀態
    */
    al_draw_filled_rectangle(50, 20, 200, 45, al_map_rgb(120, 120, 120));  //角色計分表(灰底)
    //printf ("attack = %d\n", attack);
    //以下積分動態變動以紅底標示
    if (calculate_status >= 100) {   //計分表累計達100分,將進入第二關
        //calculate_status = calculate_status + attack;
        al_draw_filled_rectangle(50, 20, 50 + calculate_status * (3.f/2.f), 45, al_map_rgb(0, 0, 255));
        STAGE2 = true;
        //al_draw_filled_rectangle(50, 20, 200, 45, al_map_rgb(255, 0, 0));  //角色計分表(灰底)
        //printf ("Hooray!\n");
        //待補(進入第二關/中轉頁面)
        
    } //else if(calculate_status == 0) {   //計分表降至0, game over
        //calculate_status = calculate_status + attack;
        //al_draw_filled_rectangle(50, 20, 50 + calculate_status, 45, al_map_rgb(255, 0, 0));
        //待補(進入game over畫面)
    //}
    
    else if(calculate_status > 0 && calculate_status < 100) {   //動態變動
        if (attack == 10) {   //攻擊到A怪物,玩家積分+10
            //calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status*(3.f/2.f), 45, al_map_rgb(0, 0, 255));
        } else if (attack == 20) {   //攻擊到B怪物,玩家積分+20
            //calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status*(3.f/2.f), 45, al_map_rgb(0, 0, 255));
        } else if (attack == 40) {   //攻擊到C怪物,玩家積分+40
            //calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status*(3.f/2.f), 45, al_map_rgb(0, 0, 255));
        }
        /*else if (attack == -10) {   //被A怪物攻擊,玩家積分-10
            calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status, 45, al_map_rgb(0, 0, 255));
        } else if (attack == -20) {   //被B怪物攻擊,玩家積分-20
            calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status, 45, al_map_rgb(0, 0, 255));
        } else if (attack == -40) {   //被C怪物攻擊,玩家積分-40
            calculate_status += attack;
            al_draw_filled_rectangle(50, 20, 50 + calculate_status, 45, al_map_rgb(0, 0, 255));
        }
        */
    }
    
    /*以下紀錄血量*/
    al_draw_filled_rectangle(50, 50, 200, 75, al_map_rgb(120, 120, 120));  //血量表(灰底)
    al_draw_filled_rectangle(50, 50, 50 + role_hp*(3.f/20.f), 75, al_map_rgb(255, 0, 0)); //血量表(紅底)
    al_draw_text(font2, al_map_rgb(0, 0, 0), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Leave");
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);  //游標
    al_flip_display();
}

void game_begin_startgame_stage2(){  //NPC頁面(這邊的next鍵用鍵盤好像沒作用,待修)
    about = false;
    menu = false;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = true;
    startgame_stage3 = false;
    next1 = false;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    //al_clear_to_color(al_map_rgb(100,100,255));
    al_draw_scaled_bitmap(img14, 0, 0, al_get_bitmap_width(img14), al_get_bitmap_height(img14), 0, 0, width, height, 0);   //背景
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_stage2);
    //al_play_sample(winsound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
    al_draw_text(font2, al_map_rgb(255, 255, 255), width/2+10, height/6-20, ALLEGRO_ALIGN_CENTRE, "<-- I am NPC !!!");
    al_draw_text(font2, al_map_rgb(255, 255, 255), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Leave");
    if (mouse_count_npc == 0) {
        al_draw_text(font3, al_map_rgb(255, 255, 255), width/2, height/2, 0, "點擊畫面繼續");
    }
    else if (mouse_count_npc == 1) {
        al_draw_text(font3, al_map_rgb(255, 255, 255), width/2, height/2, 0, "NPC會指引你");
    }
    else {
        al_draw_text(font3, al_map_rgb(255, 255, 255), width/2, height-50, 0, "按NEXT繼續到下一關");
        al_draw_text(font3, al_map_rgb(0, 0, 0), width/10, height/2, 0, "恭喜您一路過關斬將至此，第二關將要面對終");
        al_draw_text(font3, al_map_rgb(0, 0, 0), width/10, height/2 + 35, 0, "極Boss!!接下來將不再計分，僅計算血量且");
        al_draw_text(font3, al_map_rgb(0, 0, 0), width/10, height/2 + 70, 0, "限時，若角色血量降至0或時限內未殺死Boss，");
        al_draw_text(font3, al_map_rgb(0, 0, 0), width/10, height/2 + 105, 0, "則Game over。升級獎勵為補血600滴，惟含");
        al_draw_text(font3, al_map_rgb(0, 0, 0), width/10, height/2 + 140, 0, "第一關剩餘血量，最高加至1000滴。");

        if (ROLE_LEFT) {
            al_draw_scaled_bitmap(img3, 0, 0, al_get_bitmap_width(img3), al_get_bitmap_height(img3), width*3/5 + 30, height/3, 300, 300, 0);
        } else if (ROLE_RIGHT) {
            al_draw_scaled_bitmap(img4, 0, 0, al_get_bitmap_width(img4), al_get_bitmap_height(img4), width*3/5 + 40, height/3, 260, 320, 0);
        }
        al_draw_text(font2, al_map_rgb(255, 255, 255), width-90, height-50, ALLEGRO_ALIGN_CENTRE, "Next");  //Next鍵
    }
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_startgame_stage3(){  //第二階段遊戲畫面
    /*
      第二關玩家不再計分,但被怪物攻擊到三次或時限內未殺死怪物則game over
      玩家在進入第二關前會有升級獎勵(補血),補600,但含第一關剩餘的血量,最多加至1000
      目前先設定Boss血量為600(會有Boss專屬血量計分表)
     */
    about = false;
    menu = false;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = true;
    next1 = false;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    al_draw_scaled_bitmap(img11, 0, 0, al_get_bitmap_width(img11), al_get_bitmap_height(img11), 0, 0, width, height, 0);   //背景
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_menu);
    al_play_sample_instance(songInstance_stage2);
    
    //角色
    if (ROLE_LEFT){  //高階劍士
        if (role_begin) al_draw_scaled_bitmap(img3, 0, 0, al_get_bitmap_width(img3), al_get_bitmap_height(img3), role_pos_x, role_pos_y, 150, 150, 0);
        else if (left){
            al_draw_scaled_bitmap(img3, 0, 0, al_get_bitmap_width(img3), al_get_bitmap_height(img3), role_pos_x, role_pos_y, 150, 150, 0);  //角色
        }
        else if (right){
            al_draw_scaled_bitmap(img3, 0, 0, al_get_bitmap_width(img3), al_get_bitmap_height(img3), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);  //角色
        }
        al_draw_scaled_bitmap(img3, 0, 0, al_get_bitmap_width(img3), al_get_bitmap_height(img3)*2/3, 210, 15, 40, 30, 0);  //計分表角色標示(小頭圖示)
    }
    else if (ROLE_RIGHT) {  //高階弓箭手
        if (role_begin) al_draw_scaled_bitmap(img4, 0, 0, al_get_bitmap_width(img4), al_get_bitmap_height(img4), role_pos_x, role_pos_y, 120, 180, 0);
        else if (left){
            //if (role_atk){
              /*
                if (role_atk_fail) al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, 0);
               */
                //role_atk = false;
            //}
            //else
            al_draw_scaled_bitmap(img4, 0, 0, al_get_bitmap_width(img4), al_get_bitmap_height(img4), role_pos_x, role_pos_y, 120, 180, 0);  //角色
        }
        else if (right){
            /*if (role_atk){
                role_atk_fail = true;
                if (role_atk_fail) al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, ALLEGRO_FLIP_HORIZONTAL);
                role_atk_fail = false;
                //role_atk = false;
            }*/
            //else
                al_draw_scaled_bitmap(img4, 0, 0, al_get_bitmap_width(img4), al_get_bitmap_height(img4), role_pos_x, role_pos_y, 120, 180, ALLEGRO_FLIP_HORIZONTAL);  //角色
        }
        al_draw_scaled_bitmap(img4, 0, 0, al_get_bitmap_width(img4)*3/4, al_get_bitmap_height(img4)*3/5, 215, 5, 30, 40, 0);  //計分表角色標示(小頭圖示)
    }

    //Boss(Boss爆炸圖表示Boss被攻擊到,角色爆炸圖表示角色發出攻擊)-->待處理(目前Boss只會左右一直走)
    //al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, 0);
    if (Boss.monster_hp > 0){   //若hp<0，這裡就直接不用跑了
        if (Boss_dir){   //Boss往右
            al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, 0);   //Boss原圖
            if (Boss.monster_pos_x +400 >= role_pos_x && Boss.monster_pos_x <= role_pos_x + 130 && Boss.monster_pos_y +85 <= role_pos_y+150 && Boss.monster_pos_y+365 >= role_pos_y && ROLE_LEFT){  //適用劍士(上下左右皆可攻擊,所以血量增減range較弓箭手小)
                if (role_atk) {
                    if (left)al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 200, 160, 0);
                    else if (right)al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 200, 160, ALLEGRO_FLIP_HORIZONTAL);
                    Boss.monster_hp -= 80;
                    role_atk = false;
                    //role_hp += 50;
                    al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, 0);   //Boss被擊中
                }
                /*else if (Boss_atk) {  //Boss主動出手攻擊
                    role_hp -= 20;
                    //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                    al_draw_scaled_bitmap(img33, 0, 0, al_get_bitmap_width(img33), al_get_bitmap_height(img33), role_pos_x, role_pos_y, 450, 450, 0);  //Boss主動攻擊閃電畫面
                    //al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 150, 150, 0);   //劍士被擊中(劍士爆炸表示角色發出攻擊)
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }*/
                else {   //Boss出手攻擊
                    role_hp -= 20;
                    //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                    //al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 150, 150, 0);   //劍士被擊中(劍士爆炸表示角色發出攻擊)
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (Boss.monster_pos_y +85 <= role_pos_y+150 && Boss.monster_pos_y+365 >= role_pos_y + 30 && ROLE_RIGHT) {  //適用弓箭手(橫排攻擊,血量增減range較劍士大)
                if (role_atk) {  //角色出手攻擊
                    if (left){  //往左橫排攻擊
                        //Boss.monster_pos_y +85 <= role_pos_y+150 && Boss.monster_pos_y+365 >= role_pos_y+30 &&
                        if (Boss.monster_pos_x <= role_pos_x && (role_pos_x - Boss.monster_pos_x <= 500)){
                            al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, 0);
                            Boss.monster_hp -= 50;
                            role_atk = false;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, 0);  //Boss被擊中
                        }
                    }
                    else if (right){   //往右橫排攻擊
                        //Boss.monster_pos_y +85 >= role_pos_y+90 && Boss.monster_pos_y+365 <= role_pos_y+60 &&
                        if (Boss.monster_pos_x >= role_pos_x && (Boss.monster_pos_x - role_pos_x <= 500)){
                            al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, ALLEGRO_FLIP_HORIZONTAL);
                            Boss.monster_hp -= 50;
                            role_atk = false;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, 0);  //Boss被擊中
                        }
                    }
                    /*if (Boss.monster_pos_x + 80 >= role_pos_x && Boss.monster_pos_x + 35 <= role_pos_x + 130) {  //公因式(角色出手,角色hp +100)
                        role_hp += 100;
                    }*/
                }
                /*else if (Boss_atk) {  //Boss主動出手攻擊
                    if (Boss.monster_pos_x + 80 >= role_pos_x && Boss.monster_pos_x + 35 <= role_pos_x + 130) {
                        role_hp -= 40;
                        //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                        al_draw_scaled_bitmap(img33, 0, 0, al_get_bitmap_width(img33), al_get_bitmap_height(img33), role_pos_x, role_pos_y, 450, 450, 0);  //Boss主動攻擊閃電畫面
                        //al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 150, 150, 0);   //角色被擊中(弓箭手爆炸表示角色發出攻擊)
                        if (role_hp <= 0) GAMEOVER = true;
                    }
                }*/
                else {   //Boss攻擊
                    if (Boss.monster_pos_x + 400 >= role_pos_x && Boss.monster_pos_x <= role_pos_x + 130) {
                        role_hp -= 40;
                        //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                        //al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 150, 150, 0);   //角色被擊中(弓箭手爆炸表示角色發出攻擊)
                        if (role_hp <= 0) GAMEOVER = true;
                    }
                }
            }
        }
        else {  //Boss 往左
            al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);
            if (Boss.monster_pos_x +400 >= role_pos_x && Boss.monster_pos_x <= role_pos_x + 130 && Boss.monster_pos_y +85 <= role_pos_y+150 && Boss.monster_pos_y+365 >= role_pos_y && ROLE_LEFT){  //適用劍士
                if (role_atk) {
                    if (left)al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 200, 160, 0);
                    else if (right)al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 200, 160, ALLEGRO_FLIP_HORIZONTAL);
                    Boss.monster_hp -= 80;
                    role_atk = false;
                    //role_hp += 50;
                    al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);   //Boss被擊中
                }
                /*else if (Boss_atk) {  //Boss主動出手攻擊
                    role_hp -= 20;
                    //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                    al_draw_scaled_bitmap(img33, 0, 0, al_get_bitmap_width(img33), al_get_bitmap_height(img33), role_pos_x, role_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);  //Boss主動攻擊閃電畫面
                    //al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 150, 150, 0);   //劍士被擊中(劍士爆炸圖代表劍士攻擊)
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }*/
                else {   //Boss攻擊
                    role_hp -= 20;
                    //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                    //al_draw_scaled_bitmap(img24, 0, 0, al_get_bitmap_width(img24), al_get_bitmap_height(img24), role_pos_x, role_pos_y, 150, 150, 0);   //劍士被擊中(劍士爆炸圖代表劍士攻擊)
                    if (role_hp <= 0) GAMEOVER = true;
                    //printf ("hp = %d\n", role_hp);
                }
            }
            else if (Boss.monster_pos_y +85 <= role_pos_y+150 && Boss.monster_pos_y+365 >= role_pos_y+30 && ROLE_RIGHT){  //適用弓箭手
                if (role_atk) {
                    if (left){  //往左橫排攻擊
                        //Boss.monster_pos_y +85 <= role_pos_y+90 && Boss.monster_pos_y+365 >= role_pos_y+60 &&
                        if (Boss.monster_pos_x <= role_pos_x && (role_pos_x - Boss.monster_pos_x <= 500)){
                            al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, 0);
                            Boss.monster_hp -= 50;
                            role_atk = false;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);  //Boss被擊中
                        }
                    }
                    else if (right){   //往右橫排攻擊
                        //Boss.monster_pos_y +85 <= role_pos_y+90 && Boss.monster_pos_y+365 >= role_pos_y+60 &&
                        if (Boss.monster_pos_x >= role_pos_x && (Boss.monster_pos_x - role_pos_x <= 500)){
                            al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 120, 180, ALLEGRO_FLIP_HORIZONTAL);
                            Boss.monster_hp -= 50;
                            role_atk = false;
                            //role_hp += 4;
                            //printf ("%d\n", A1.monster_hp);
                            al_draw_scaled_bitmap(img29, 0, 0, al_get_bitmap_width(img29), al_get_bitmap_height(img29), Boss.monster_pos_x, Boss.monster_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);  //Boss被擊中
                        }
                    }
                    /*if (Boss.monster_pos_x + 80 >= role_pos_x && Boss.monster_pos_x + 35 <= role_pos_x + 130) {  //公因式(角色出手,角色hp +100)
                        role_hp += 100;
                    }*/
                }
                /*else if (Boss_atk) {  //Boss主動出手攻擊
                    if (Boss.monster_pos_x + 80 >= role_pos_x && Boss.monster_pos_x + 35 <= role_pos_x + 130) {
                        role_hp -= 40;
                        //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                        al_draw_scaled_bitmap(img33, 0, 0, al_get_bitmap_width(img33), al_get_bitmap_height(img33), role_pos_x, role_pos_y, 450, 450, ALLEGRO_FLIP_HORIZONTAL);  //Boss主動攻擊閃電畫面
                        //al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 150, 150, ALLEGRO_FLIP_HORIZONTAL);   //角色被擊中(弓箭手爆炸圖代表劍士攻擊)
                        if (role_hp <= 0) GAMEOVER = true;
                    }
                }*/
                else {   //Boss攻擊
                    if (Boss.monster_pos_x + 400 >= role_pos_x && Boss.monster_pos_x <= role_pos_x + 130) {
                        role_hp -= 40;
                        //Boss.monster_hp += 30;  //再調整,如果太難玩,就不讓怪物血量增加
                        //al_draw_scaled_bitmap(img25, 0, 0, al_get_bitmap_width(img25), al_get_bitmap_height(img25), role_pos_x, role_pos_y, 150, 150, 0);   //角色被擊中(弓箭手爆炸表示角色發出攻擊)
                        if (role_hp <= 0) GAMEOVER = true;
                    }
                }
            }
        }
        if (Boss_dir) Boss.monster_pos_x += monster_movespeed;
        else if (!Boss_dir) Boss.monster_pos_x -= monster_movespeed;
            //A1.monster_pos_x += 1;
        if (cannot_go3(Boss.monster_pos_x, Boss.monster_pos_y)) {
            if (Boss_dir) Boss.monster_pos_x -= monster_movespeed;
            else if (!Boss_dir) Boss.monster_pos_x += monster_movespeed;
            Boss_dir = !Boss_dir;//怪物轉向
            //printf ("%d\n", dir);
        }
    }
    else {  //怪物hp <= 0 死掉
        //if (Boss_to_calculate_attack) {
            al_play_sample(soundeffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            //attack = 10;
            //calculate_status += attack;
        WINPAGE = true;
        //}
        //Boss_to_calculate_attack = false;
        //printf ("calculate status = %f\n", calculate_status);
    }
 
    //printf ("%d\n", Boss.monster_hp);
    
    //以下為血量表(角色＆Boss)
    al_draw_text(font2, al_map_rgb(0, 0, 0), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Leave");
    al_draw_filled_rectangle(50, 20, 200, 45, al_map_rgb(120, 120, 120));  //角色血量表(灰底)
    al_draw_filled_rectangle(50, 20, 50 + role_hp*(3.f/20.f), 45, al_map_rgb(255, 0, 0)); //角色血量線(紅)
    al_draw_filled_rectangle(50, 50, 200, 75, al_map_rgb(120, 120, 120));  //Boss血量表(灰底)
    al_draw_filled_rectangle(50, 50, 50 + Boss.monster_hp*(3.f/12.f), 75, al_map_rgb(160, 50, 255));  //Boss血量線(紫)
    
    al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8)*4/5, 210, 45, 45, 30, 0);  //計分表Boss標示(小頭圖示)
    
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_about_page1() {   //about畫面-->角色介紹第一頁(可控角色)
    about = true;
    menu = false;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    next1 = false;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    
    //基本框架
//    al_clear_to_color(al_map_rgb(100,100,255));  //藍底(勿刪,漂亮的藍色～～～,最後沒用到再刪)
    al_draw_scaled_bitmap(img20, 0, 0, al_get_bitmap_width(img20), al_get_bitmap_height(img20), 0, 0, width, height, 0);  //背景
    al_draw_text(font2, al_map_rgb(0, 0, 0), width/2, height/6, ALLEGRO_ALIGN_CENTRE, "About this game");  //title
    al_draw_text(font2, al_map_rgb(255, 255, 255), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Menu");  //返回Menu鍵
    al_draw_text(font2, al_map_rgb(255, 255, 255), width-90, height-50, ALLEGRO_ALIGN_CENTRE, "Next");  //Next鍵
    
    //文(顯示中文,直接用kaiu.ttf,已加入project file)
    al_draw_text(font3, al_map_rgb(0, 0, 0), width/12 + 50, height/4, 0, "角色介紹：");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12+ 50, height/4 + 40, 0, "兩種可操控角色：劍士及弓箭手, 攻擊力及防禦力皆不同, ");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12+ 50, height/4 + 70, 0, "劍士以近身接觸方式攻擊，弓箭手為遠程(橫排)攻擊。積分");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12+ 50, height/4 + 100, 0, "初始值均為0分，玩家攻擊到A, B, C三種怪物時，分別獲得");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12+ 50, height/4 + 130, 0, "10, 20, 40分，累計積分達100分則進入第二關。");
    
    //圖
    al_draw_scaled_bitmap(img1, 0, 0, al_get_bitmap_width(img1), al_get_bitmap_height(img1), width/4 - 70, height/2 - 5, 220, 220, 0);
    al_draw_scaled_bitmap(img2, 0, 0, al_get_bitmap_width(img2), al_get_bitmap_height(img2), width/2 + 70, height/2 - 25, 240, 240, 0);
    
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_about_page2() {   //about畫面-->角色介紹第二頁(monster)
    about = false;  //改了這邊
    menu = false;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    next1 = true;
    next2 = false;
    gameover = false;
    winpage = false;
    leave = false;
    
    al_draw_scaled_bitmap(img20, 0, 0, al_get_bitmap_width(img20), al_get_bitmap_height(img20), 0, 0, width, height, 0);
    al_draw_text(font2, al_map_rgb(0, 0, 0), width/2, height/6, ALLEGRO_ALIGN_CENTRE, "About this game");
    al_draw_text(font2, al_map_rgb(255, 255, 255), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Menu");  //返回Menu鍵
    al_draw_text(font2, al_map_rgb(255, 255, 255), width-90, height-50, ALLEGRO_ALIGN_CENTRE, "Next");  //Next鍵
    
    al_draw_text(font3, al_map_rgb(0, 0, 0), width/12 + 50, height/4, 0, "角色介紹：");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 40, 0, "怪物：分別為A, B, C三種怪物，能力等級和長相均不同，惟血");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12+ 50, height/4 + 70, 0, "      量起始值均為100分，血量降至0則死亡。");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 180, height/4 + 120, 0, "A怪物：共4隻，攻擊到玩家，劍士血量減少2分，弓");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 180, height/4 + 150, 0, "       箭手少4分，被玩家攻擊則血量損失40分。");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 260, height/4 + 190, 0, "B怪物：共3隻，攻擊到玩家，劍士血量");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 260, height/4 + 220, 0, "       減少4分，弓箭手少8分，被玩家");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 260, height/4 + 250, 0, "       攻擊則血量損失20分。");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 220, height/4 + 290, 0, "C怪物：共2隻，攻擊到玩家，劍士血量減少8分，");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 220, height/4 + 320, 0, "       弓箭手少16分，被玩家攻擊則血量損失10分。");
    
    al_draw_scaled_bitmap(img5, 0, 0, al_get_bitmap_width(img5), al_get_bitmap_height(img5), width/12 + 20, height/3 - 10, 200, 200, 0);
    al_draw_scaled_bitmap(img6, 0, 0, al_get_bitmap_width(img6), al_get_bitmap_height(img6), width* 3/4 + 15, height/2 - 15, 150, 150, ALLEGRO_FLIP_HORIZONTAL);
    al_draw_scaled_bitmap(img7, 0, 0, al_get_bitmap_width(img7), al_get_bitmap_height(img7), width/12 + 60, height/2 + 70, 150, 150, ALLEGRO_FLIP_HORIZONTAL);
    
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_about_page3() {   //about畫面-->角色介紹第三頁(Boss) & 遊戲進行方式
    about = false;  //改了這邊
    menu = false;
    startgame_player = false;
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    next1 = false;
    next2 = true;
    gameover = false;
    winpage = false;
    leave = false;
    
    al_draw_scaled_bitmap(img20, 0, 0, al_get_bitmap_width(img20), al_get_bitmap_height(img20), 0, 0, width, height, 0);
    al_draw_text(font2, al_map_rgb(0, 0, 0), width/2, height/6, ALLEGRO_ALIGN_CENTRE, "About this game");
    al_draw_text(font2, al_map_rgb(255, 255, 255), 90, height-50, ALLEGRO_ALIGN_CENTRE, "Menu");  //返回Menu鍵
    
    //角色介紹(Boss)
    al_draw_text(font3, al_map_rgb(0, 0, 0), width/12 + 50, height/4, 0, "角色介紹：");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 40, 0, "終極Boss (一隻)。");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 70, 0, "Boss：玩家被Boss攻擊至血量降至0或時限內(10秒)");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 100, 0, "      未殺死Boss即Game Over。");
    al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8), width * 3/4 - 130, height/4 - 70, 350, 350, ALLEGRO_FLIP_HORIZONTAL);
    
    //遊戲進行方式
    al_draw_text(font3, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 140, 0, "遊戲進行方式：");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 180, 0, "選擇角色 (劍士及弓箭手二選一)。");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 210, 0, "藉由攻擊特定怪物來得分。第一階段得分累計達100分即升");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 240, 0, "級(角色外形改變，技能升級，攻擊路徑變密集)，進入第二階");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 270, 0, "段攻打終極Boss，需於時限內殺死Boss ; 任一階段未達目標");
    al_draw_text(font4, al_map_rgb(0, 0, 0), width/12 + 50, height/4 + 300, 0, "則Game Over。進入第二階段前，NPC會給予提示。");
    
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_winpage() {
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    startgame_player = false;
    menu = false;
    about = false;
    next1 = false;
    next2 = false;
    leave = false;
    gameover = false;
    winpage = true;
    al_draw_scaled_bitmap(img37, 0, 0, al_get_bitmap_width(img37), al_get_bitmap_height(img37), 0, 0, width, height, 0);   //背景
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_stage2);
    
    al_draw_text(font1, al_map_rgb(0, 0, 255), width / 2, height / 10 + 80, ALLEGRO_ALIGN_CENTRE, "YOU WIN!!");
    al_draw_text(font6, al_map_rgb(0, 0, 255), width/12 + 150, height/2, 0, "離開遊戲");
    al_draw_text(font6, al_map_rgb(0, 0, 255), width - 330, height/2, 0, "回主畫面");
    
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

void game_begin_gameover(){
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    startgame_player = false;
    menu = false;
    about = false;
    next1 = false;
    next2 = false;
    leave = false;
    gameover = true;
    winpage = false;
    al_draw_scaled_bitmap(img12, 0, 0, al_get_bitmap_width(img12), al_get_bitmap_height(img12), 0, 0, width, height, 0);   //背景
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_stage2);
    al_play_sample(losesound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
    al_draw_text(font1, al_map_rgb(255, 0, 0), width / 2, height / 10 + 80, ALLEGRO_ALIGN_CENTRE, "Game Over");
    //printf ("Your score is %d\n", calculate_status);//不知道怎麼顯示在畫面上
    al_draw_text(font6, al_map_rgb(255, 0, 0), width/12 + 150, height/2, 0, "離開遊戲");
    al_draw_text(font6, al_map_rgb(255, 0, 0), width - 330, height/2, 0, "回主畫面");
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    //gameover音樂
    
    al_flip_display();
}

void game_begin_leave(){  //第一階段遊戲離開畫面
    startgame_stage1 = false;
    startgame_stage2 = false;
    startgame_stage3 = false;
    startgame_player = false;
    menu = false;
    about = false;
    next1 = false;
    next2 = false;
    gameover = false;
    leave = true;
    winpage = false;
    
    al_draw_scaled_bitmap(img13, 0, 0, al_get_bitmap_width(img13), al_get_bitmap_height(img13), 0, 0, width, height, 0);
    al_draw_text(font5, al_map_rgb(0, 0, 0), width/12 + 230, height/4 + 30, 0, "確定要離開嗎?");
    al_draw_text(font6, al_map_rgb(0, 0, 0), width/12 + 150, height/2, 0, "離開遊戲");
    al_draw_text(font6, al_map_rgb(0, 0, 0), width - 330, height/2, 0, "回主畫面");
    al_stop_sample_instance(songInstance_game);
    al_stop_sample_instance(songInstance_stage2);
    al_play_sample_instance(songInstance_menu);
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, color);
    al_flip_display();
}

int process_event_menu() {
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return GAME_TERMINATE;  //這裡會return到game_run裡面，把值丟給process去接收。
    
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        // Event for mouse key down.
        mouse_state[event.mouse.button] = true;
        click_x = event.mouse.x;
        click_y = event.mouse.y;
        //printf ("queue is empty = %d\n", al_is_event_queue_empty(event_queue));
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        mouse_state[event.mouse.button] = false;
        //printf ("queue is empty = %d\n", al_is_event_queue_empty(event_queue));
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        if (event.mouse.dx != 0 || event.mouse.dy != 0){
            mouse_x = event.mouse.x;
            mouse_y = event.mouse.y;
        } else if (event.mouse.dz != 0) {}  //這裡是要做什麼？->我猜可能是滑鼠的滾輪？不確定
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN){   //鍵盤中 某個鍵被按下
        key_state[event.keyboard.keycode] = true;   //key_state[按鍵名稱]，見下方game_update
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return GAME_TERMINATE;   //設定按ESC關閉視窗
        }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        key_state[event.keyboard.keycode] = false;
    }
    
    if (event.type == ALLEGRO_EVENT_TIMER) {   //這裡不知道用else if或if有沒有差別
        if (event.timer.source == game_update_timer) {
            if (startgame_player){
                game_update_startgame_player();
                if (MENU) return RETURN_to_MENU;
                if (ROLE_LEFT || ROLE_RIGHT) return GO_to_STARTGAME_stage1;
                //if (ROLE_RIGHT) return GO_to_STARTGAME_stage1;
                game_begin_startgame_player();
            }
            else if (startgame_stage1){
                game_update_startgame_stage1();
                if (LEAVE) return LEAVE_GAME;
                game_begin_startgame_stage1();
                if (GAMEOVER) return GO_to_GAMEOVER;
                if (STAGE2) return GO_to_STARTGAME_stage2;
            }
            else if (startgame_stage2){
                game_update_startgame_stage2();
                if (LEAVE) return LEAVE_GAME;
                if (NEXTSTAGE) return GO_to_NEXTSTAGE;
                game_begin_startgame_stage2();
            }
            else if (startgame_stage3){
                //printf ("here");
                game_update_startgame_stage3();
                if (LEAVE) return LEAVE_GAME;
                game_begin_startgame_stage3();
                if (GAMEOVER) return GO_to_GAMEOVER;
                if (WINPAGE) return GO_to_WINPAGE;
            }
            else if (winpage){//類似gameover頁面設定
                game_update_winpage();
                if (MENU) return RETURN_to_MENU;
                else if (end == true) return GAME_TERMINATE;
                game_begin_winpage();
            }
            else if (about){   //進入about第一頁
                game_update_about_page1();
                if (MENU) return RETURN_to_MENU;
                else if (NEXT1) {
                        return GO_to_ABOUT_PAGE2;
                }
                game_begin_about_page1();
            }//加了以下
            else if (next1){
                game_update_about_page2();
                if (MENU) return RETURN_to_MENU;
                if (NEXT2) return GO_to_ABOUT_PAGE3;
                game_begin_about_page2();
                
            }
            else if (next2){
                game_update_about_page3();
                if (MENU) return RETURN_to_MENU;
                game_begin_about_page3();
            }
            else if (gameover){
                game_update_gameover();
                if (MENU) return RETURN_to_MENU;
                else if (end == true) return GAME_TERMINATE;
                game_begin_gameover();
            }
            else if (leave){
                game_update_leave();
                if (MENU) return RETURN_to_MENU;
                else if (end == true) return GAME_TERMINATE;
                game_begin_leave();
            }
            else {
                game_update_menu();
                if (START) return GO_to_STARTGAME_player;
                else if (ABOUT) return GO_to_ABOUT_PAGE1;
                else if (end == true) return GAME_TERMINATE;
                game_begin_menu();
            }
        }
        else if (event.timer.source == game_stage3_timer && startgame_stage3){
            GAMEOVER = true;
        }
    }
    return 0;
}

void game_update_menu() {  //如果要用滑鼠來讓box消失在出現，以及右鍵結束遊戲的話，就無法同時變換滑鼠顏色(下方註解的那邊)
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= rec_x1 && click_x <= rec_x2 && click_y >= rec_y1+230 && click_y <= rec_y2+230){
            START = true;
            mouse_state[1] = false;
        }
        else if (click_x >= rec_x1 && click_x <= rec_x2 && click_y >= rec_y1+310 && click_y <= rec_y2+310) {
            ABOUT = true;
            mouse_state[1] = false;
        }
        else if (click_x >= rec_x1 && click_x <= rec_x2 && click_y >= rec_y1+390 && click_y <= rec_y2+390) {
            end = true;
            mouse_state[1] = false;
        }
        //mouse_state[1] = false;
    }
    else if (mouse_state[2]){ //如果沒加else的話mouse_state[1]的顏色都不會變，還不知道為什麼
        color = al_map_rgb(255, 0, 0);
    }
    else if ((mouse_x >= rec_x1 && mouse_x <= rec_x2 && mouse_y >= rec_y1+230 && mouse_y <= rec_y2+230) || (mouse_x >= rec_x1 && mouse_x <= rec_x2 && mouse_y >= rec_y1+310 && mouse_y <= rec_y2+310) || (mouse_x >= rec_x1 && mouse_x <= rec_x2 && mouse_y >= rec_y1+390 && mouse_y <= rec_y2+390)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_S]){ //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        START = true;
        key_state[ALLEGRO_KEY_S] = false;
    }
    else if (key_state[ALLEGRO_KEY_A]){
        ABOUT = true;
        key_state[ALLEGRO_KEY_A] = false;}
}

void game_update_startgame_player() {
    pos_x = mouse_x;
    pos_y = mouse_y;
    
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            MENU = true;
            mouse_state[1] = false;
        }
        else if (click_x >= 100 && click_x <= 350 && click_y >= 200 && click_y <= 500){
            ROLE_LEFT = true;
            mouse_state[1] = false;
        }
        else if (click_x >= width - 380 && click_x <= width - 100 && click_y >= 200 && click_y <= 500){
            ROLE_RIGHT = true;
            mouse_state[1] = false;
        }
    }
    else if (mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height){
        color = al_map_rgb(0, 0, 255);
    }
    else if (mouse_x >= 110 && mouse_x <= 360 && mouse_y >= 190 && mouse_y <= 500){
        color = al_map_rgb(0, 0, 255);
        color_player_background_left = al_map_rgb(230,230,200);
        color_player_background_word_left = al_map_rgb(0, 0, 0);
    }
    else if (mouse_x <= width - 100 && mouse_x >= width - 380 && mouse_y >= 200 && mouse_y <= 500){
        color = al_map_rgb(0, 0, 255);
        color_player_background_right = al_map_rgb(230,230,200);
        color_player_background_word_right = al_map_rgb(0, 0, 0);
    }
    else{
        color = al_map_rgb(255, 255, 255);
        color_player_background_left = al_map_rgb(0, 0, 0);
        color_player_background_right = al_map_rgb(0, 0, 0);
        color_player_background_word_left = al_map_rgb(255, 255, 255);
        color_player_background_word_right = al_map_rgb(255, 255, 255);
    }
    
    if (key_state[ALLEGRO_KEY_M]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;
    }
    else if (key_state[ALLEGRO_KEY_LEFT]){
        ROLE_LEFT = true;
        key_state[ALLEGRO_KEY_LEFT] = false;
    }
    else if (key_state[ALLEGRO_KEY_RIGHT]){
        ROLE_RIGHT = true;
        //key_state[ALLEGRO_KEY_RIGHT] = false;
    }
}

void game_update_startgame_stage1() {
    pos_x = mouse_x;
    pos_y = mouse_y;
    //int vx, vy;
    //vx = vy = 0;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            //value = al_show_native_message_box(display, "QUIT", "Do you want to leave the game?", "Once you click 'NO', your score will become zoro. Think twice!!", NULL, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
            //if (value == 1) LEAVE = true;
            LEAVE = true;
            mouse_state[1] = false;//一定要記得加
        }
        
    }
    else if (mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_L]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        //value = al_show_native_message_box(display, "QUIT", "Do you want to leave the game?", "Once you click 'NO', your score will become zoro. Think twice!!", NULL, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
        //if (value == 1) LEAVE = true;
        //value = 0;
        LEAVE = true;
        key_state[ALLEGRO_KEY_L] = false;
        //printf ("value = %d\n", value);
    }
   
    if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W]){ //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        //vy -= 1;
        
        role_pos_y -= role_movespeed;
        if (cannot_go(role_pos_x, role_pos_y)) role_pos_y += role_movespeed;
    }
        //dir = Up;
    if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S]){
        //vy += 1;
        role_pos_y += role_movespeed;
        if (cannot_go(role_pos_x, role_pos_y)) role_pos_y -= role_movespeed;
    }
        //dir = Down;
    if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A]){
        //vx -= 1;
        role_begin = false;
        right = false;
        left = true;
        role_pos_x -= role_movespeed;
        if (cannot_go(role_pos_x, role_pos_y)) role_pos_x += role_movespeed;
    }
        //dir = Left;
    if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D]){
        //vx += 1;
        role_begin = false;
        left = false;
        right = true;
        role_pos_x += role_movespeed;
        if (cannot_go(role_pos_x, role_pos_y)) role_pos_x -= role_movespeed;
    }
    if (key_state[ALLEGRO_KEY_Z] || key_state[ALLEGRO_KEY_Z]){
        role_atk = true;
        key_state[ALLEGRO_KEY_Z] = false;
        //key_state[ALLEGRO_KEY_Z] = false;
    }
        //dir = Right;
    //role_pos_y += vy * (vx ? 3.55f : 5);
    //role_pos_x += vx * (vy ? 3.55f : 5);
}

void game_update_startgame_stage2(){  //NPC
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
          //draw = !draw; //按左鍵box消失，再按一次會出現
        mouse_count_npc++;
        mouse_state[1] = false;
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            LEAVE = true;
            //mouse_state[1] = false;//一定要記得加
        }
        else if(click_x >= width-160 && click_x <= width && click_y >= height-80 && click_y <= height) {   //改next鍵位置
            NEXTSTAGE = true;
            mouse_state[1] = false;
        }
        
    }
    else if ((mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height) || ((mouse_x >= width-160 && mouse_x <= width && mouse_y >= height-80 && mouse_y <= height) && (mouse_count_npc > 1))){
        color = al_map_rgb(0, 0, 255);
    }
    else color = al_map_rgb(255, 255, 255);
    if (key_state[ALLEGRO_KEY_L]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        //value = al_show_native_message_box(display, "QUIT", "Do you want to leave the game?", "Once you click 'NO', your score will become zoro. Think twice!!", NULL, ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
        //if (value == 1) LEAVE = true;
        //value = 0;
        LEAVE = true;
        key_state[ALLEGRO_KEY_L] = false;
        //printf ("value = %d\n", value);
    }
    if (key_state[ALLEGRO_KEY_N]){
        NEXTSTAGE = true;
        key_state[ALLEGRO_KEY_N] = false;
    }
}

void game_update_startgame_stage3(){
     pos_x = mouse_x;
     pos_y = mouse_y;
     //int vx, vy;
     //vx = vy = 0;
     if (mouse_state[1]){
         color = al_map_rgb(0, 0, 0);
         if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
             LEAVE = true;
             mouse_state[1] = false;//一定要記得加
         }
         
     }
     else if (mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height){
         color = al_map_rgb(0, 0, 255);
     }
    /*
    else if (mouse_x >= role_pos_x +10 && mouse_x <= role_pos_x+90 && mouse_y >= role_pos_y +30 && mouse_y <= role_pos_y +150){
        color = al_map_rgb(0, 0, 255);
    }*/
    //al_draw_scaled_bitmap(img8, 0, 0, al_get_bitmap_width(img8), al_get_bitmap_height(img8), Boss.monster_pos_x, Boss.monster_pos_y, 150, 150, 0);
    //Boss.monster_pos_y +85 >= role_pos_y+90 && Boss.monster_pos_y+365 <= role_pos_y+60 && Boss.monster_pos_x <= role_pos_x
     else
         color = al_map_rgb(255, 255, 255);
     
     if (key_state[ALLEGRO_KEY_L]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
         LEAVE = true;
         key_state[ALLEGRO_KEY_L] = false;
     }
    
     if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W]){ //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
         //vy -= 1;
         
         role_pos_y -= stage2_role_movespeed;
         if (cannot_go2(role_pos_x, role_pos_y)) role_pos_y += stage2_role_movespeed;
     }
         //dir = Up;
     if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S]){
         //vy += 1;
         role_pos_y += stage2_role_movespeed;
         if (cannot_go2(role_pos_x, role_pos_y)) role_pos_y -= stage2_role_movespeed;
     }
         //dir = Down;
     if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A]){
         //vx -= 1;
         role_begin = false;
         right = false;
         left = true;
         role_pos_x -= stage2_role_movespeed;
         if (cannot_go2(role_pos_x, role_pos_y)) role_pos_x += stage2_role_movespeed;
     }
         //dir = Left;
     if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D]){
         //vx += 1;
         role_begin = false;
         left = false;
         right = true;
         role_pos_x += stage2_role_movespeed;
         if (cannot_go2(role_pos_x, role_pos_y)) role_pos_x -= stage2_role_movespeed;
     }
     if (key_state[ALLEGRO_KEY_Z] || key_state[ALLEGRO_KEY_Z]){
         role_atk = true;
         key_state[ALLEGRO_KEY_Z] = false;
         //key_state[ALLEGRO_KEY_Z] = false;
     }
         //dir = Right;
     //role_pos_y += vy * (vx ? 3.55f : 5);
     //role_pos_x += vx * (vy ? 3.55f : 5);
}

void game_update_about_page1() {
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            MENU = true;
            mouse_state[1] = false;
        } else if(click_x >= width-160 && click_x <= width && click_y >= height-80 && click_y <= height) {   //改next鍵位置
            NEXT1 = true;
            mouse_state[1] = false;
        }
    }
    else if ((mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height) || (mouse_x >= width-160 && mouse_x <= width && mouse_y >= height-80 && mouse_y <= height)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
    else if (key_state[ALLEGRO_KEY_N]){
        NEXT1 = true;
        key_state[ALLEGRO_KEY_N] = false;}//防止timer和queue的更新時間錯開，在NEXT1=true之後趕快把這個變成false>。
}

void game_update_about_page2() {
    //printf ("NEXT2 = %d\n", NEXT2);
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            MENU = true;
            mouse_state[1] = false;
        } else if(click_x >= width-160 && click_x <= width && click_y >= height-80 && click_y <= height) {   //改next鍵位置
            NEXT2 = true;
            mouse_state[1] = false;
        }
    }
    else if ((mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height) || (mouse_x >= width-160 && mouse_x <= width && mouse_y >= height-80 && mouse_y <= height)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
    else if (key_state[ALLEGRO_KEY_N]){
        //printf ("here\n");
        NEXT2 = true;
        key_state[ALLEGRO_KEY_N] = false;
    }
}

void game_update_about_page3() {
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= 0 && click_x <= 160 && click_y >= height-80 && click_y <= height){
            MENU = true;
            mouse_state[1] = false;
        }
    }
    else if (mouse_x >= 0 && mouse_x <= 160 && mouse_y >= height-80 && mouse_y <= height){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
}

void game_update_winpage(){
    pos_x = mouse_x;
    pos_y = mouse_y;
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= width - 350 && click_x <= width - 180 && click_y >= 300 && click_y <= 350){
            MENU = true;
            mouse_state[1] = false;
        }
        else if (click_x >= 200 && click_x <= 390 && click_y >= 300 && click_y <= 350){
            end = true;
            mouse_state[1] = false;
        }
    }
    else if ((mouse_x >= 200 && mouse_x <= 390 && mouse_y >= 300 && mouse_y <= 350) || (mouse_x <= width - 180 && mouse_x >= width - 350 && mouse_y >= 300 && mouse_y <= 350)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
}

void game_update_gameover(){
    pos_x = mouse_x;
    pos_y = mouse_y;
    //width/12 + 150, height/2, 離開
    //width - 330, height/2, 回主畫面
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= width - 350 && click_x <= width - 180 && click_y >= 300 && click_y <= 350){
            MENU = true;
            mouse_state[1] = false;
        }
        else if (click_x >= 200 && click_x <= 390 && click_y >= 300 && click_y <= 350){
            end = true;
            mouse_state[1] = false;
        }
    }
    else if ((mouse_x >= 200 && mouse_x <= 390 && mouse_y >= 300 && mouse_y <= 350) || (mouse_x <= width - 180 && mouse_x >= width - 350 && mouse_y >= 300 && mouse_y <= 350)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
}

void game_update_leave() {
    pos_x = mouse_x;
    pos_y = mouse_y;
    //width/12 + 150, height/2, 離開
    //width - 330, height/2, 回主畫面
    if (mouse_state[1]){
        color = al_map_rgb(0, 0, 0);
        //draw = !draw; //按左鍵box消失，再按一次會出現
        if (click_x >= width - 350 && click_x <= width - 180 && click_y >= 300 && click_y <= 350){
            MENU = true;
            mouse_state[1] = false;
        }
        else if (click_x >= 200 && click_x <= 390 && click_y >= 300 && click_y <= 350){
            end = true;
            mouse_state[1] = false;
        }
    }    
    else if ((mouse_x >= 200 && mouse_x <= 390 && mouse_y >= 300 && mouse_y <= 350) || (mouse_x <= width - 180 && mouse_x >= width - 350 && mouse_y >= 300 && mouse_y <= 350)){
        color = al_map_rgb(0, 0, 255);
    }
    else
        color = al_map_rgb(255, 255, 255);
    
    if (key_state[ALLEGRO_KEY_M]){  //ALLEGRO_KEY_UP代表往上的鍵，ALLEGRO_KEY_W代表W鍵
        MENU = true;
        key_state[ALLEGRO_KEY_M] = false;}
}

int game_run_menu() {
    int error = 0;
    if (!al_is_event_queue_empty(event_queue)) {
        error = process_event_menu();
    }
    return error;
}


void game_destroy() {
    al_destroy_bitmap(img1);
    al_destroy_bitmap(img2);
    al_destroy_bitmap(img3);
    al_destroy_bitmap(img4);
    al_destroy_bitmap(img5);
    al_destroy_bitmap(img6);
    al_destroy_bitmap(img7);
    al_destroy_bitmap(img8);
    al_destroy_bitmap(img10);
    al_destroy_bitmap(img11);
    al_destroy_bitmap(img12);
    al_destroy_bitmap(img13);
    al_destroy_bitmap(img14);
    al_destroy_bitmap(img20);
    al_destroy_bitmap(img21);
    al_destroy_bitmap(img22);
    al_destroy_bitmap(img23);
    al_destroy_bitmap(img24);
    al_destroy_bitmap(img25);
    al_destroy_bitmap(img26);
    al_destroy_bitmap(img27);
    al_destroy_bitmap(img28);
    al_destroy_bitmap(img29);
    al_destroy_bitmap(img30);
    al_destroy_bitmap(img31);
    al_destroy_bitmap(img32);
    al_destroy_bitmap(img33);
    al_destroy_bitmap(img34);
    al_destroy_bitmap(img37);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_timer(game_update_timer);
    al_destroy_timer(game_stage3_timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(song_menu);
    al_destroy_sample(song_game);
    al_destroy_sample(song_stage2);
    //al_destroy_sample(soundeffect);
    //al_destroy_sample(winsound);
    //al_destroy_sample(losesound);
    al_destroy_sample_instance(songInstance_menu);
    al_destroy_sample_instance(songInstance_game);
    al_destroy_sample_instance(songInstance_stage2);
    al_destroy_display(display);
    free(mouse_state);
}


/*
copy the loaded bitmap to the resized bmp
void al_draw_scaled_bitmap(ALLEGRO_BITMAP *bitmap, float sx, float sy, float sw, float sh,
float dx, float dy, float dw, float dh, int flags)

al_draw_scaled_bitmap(loaded_bmp,
   0, 0,                                // source origin
   al_get_bitmap_width(loaded_bmp),     // source width
   al_get_bitmap_height(loaded_bmp),    // source height
   0, 0,                                // target origin
   w, h,                                // target dimensions
   0                                    // flags
);
*/
