#include "my_macro.h"
#include "template.h"
#include<string>
#include<time.h>
using namespace std;


//********************************************************************************
//********************************************************************************
//class Sprite
Sprite::Sprite()
{
	sprite_index = 0;
	img_file = "";
}

Sprite::Sprite(int index, string file_name)
{
	sprite_index = index;
	img_file = file_name;
}

Sprite::~Sprite()
{

}

int Sprite::get_sprite_index()
{
	return sprite_index;
}

string Sprite::get_img_file()
{
	return img_file;
}

float Sprite::get_x()
{
	return agk::GetSpriteX(sprite_index);
}

float Sprite::get_y()
{
	return agk::GetSpriteY(sprite_index);
}

void Sprite::create_sprite()
{
	agk::CreateSprite(sprite_index, img_file.c_str());
	agk::SetSpriteVisible(sprite_index, HIDE);
}

void Sprite::set_position(float x, float y)
{
	agk::SetSpritePosition(sprite_index, x, y);
}

void Sprite::set_x(float x)
{
	if (x < 0 && sprite_index == 1)    //x 最小为0    只有mario有边界
		x = 0;
	if(x > SCREEN_W - agk::GetSpriteWidth(sprite_index) && sprite_index == 1)   //x最大为screen_w - sprite_w
		x = SCREEN_W - agk::GetSpriteWidth(sprite_index);
	agk::SetSpriteX(sprite_index, x);
}

void Sprite::set_y(float y)
{
	agk::SetSpriteY(sprite_index, y);
}

//********************************************************************************
//********************************************************************************
//class Mario
Mario::Mario()
	:Sprite(1, "./Mario/mario.png")   
{
	move_state = 0;
	direction = 1;
	health = 3;
	exist_state = 1;
	jump_dist = 190;
	armed = false;
	current_frame = 1;
	cnt = 0;
	pre_y = TILE_H * 11;
	unmatched = 0;
}

Mario::~Mario()
{
}

int Mario::get_exist_state()
{
	return exist_state;
}

int Mario::get_direction()
{
	return direction;
}

int Mario::get_current_frame()
{
	return current_frame;
}

int Mario::get_cnt()
{
	int tmp = cnt;
	cnt = (cnt + 1)%10000000;
	return tmp;
}

int Mario::get_move_state()
{
	return move_state;
}

int Mario::get_health()
{
	return health;
}

int Mario::get_unmatched()
{
	return unmatched;
}

float Mario::get_pre_y()
{
	return pre_y;
}

float Mario::get_jump_dist()
{
	return jump_dist;
}

void Mario::set_direction(int direction)
{
	this->direction = direction;
}

void Mario::set_currrent_frame(int frame)
{
	current_frame = frame;
}

void Mario::set_move_state(int state)
{
	move_state = state;
}

void Mario::set_pre_y(float y)
{
	pre_y = y;
}

void Mario::set_health(int h)
{
	health = h;
}

void Mario::set_exist_state(int state)
{
	exist_state = state;
}

void Mario::init_mario()
{
	direction = 1;
	move_state = 0;
	health = 3;
	exist_state = 1;
	set_x(MARIO_X);
	set_y(MARIO_Y);
	agk::SetSpriteAnimation(get_sprite_index(), 60, 60, 10);
	agk::PlaySprite(get_sprite_index(), 10, 1, current_frame, current_frame);
	agk::SetSpriteScale(get_sprite_index(), 1, 1);
	jump_dist = agk::GetSpriteHeight(get_sprite_index()) * 3 + 10;
	unmatched = 0;
	agk::SetSpriteColor(get_sprite_index(), 255, 255, 255, 255);
}

void Mario::set_jump_dist(float dist)
{
	jump_dist = dist;
}

void Mario::set_unmatched(int time)
{
	unmatched = time;
}

//********************************************************************************
//********************************************************************************
//class Enemy
Enemy::Enemy(int index, int kind, string filename)
	:Sprite(index, filename)
{
	this->kind = kind;
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //初始随机方向
	exist_state = 1;
	dead_time = -1;
}

Enemy::~Enemy()
{

}

int Enemy::get_direction()
{
	return direction;
}

int Enemy::get_exist_state()
{
	return exist_state;
}

int Enemy::get_dead_time()
{
	return dead_time;
}

int Enemy::get_kind()
{
	return kind;
}

float Enemy::get_moved_dist()
{
	return moved_dist;
}

void Enemy::set_direction(int d)
{
	direction = d;
}

void Enemy::set_exist_state(int state)
{
	exist_state = state;
}

void Enemy::init_enemy(float x, float y)
{
	//direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //初始随机方向
	direction = 1;
	exist_state = 1;

	set_x(x);
	set_y(y);

	moved_dist = x - agk::GetSpriteX(TILE_SPRITE);

	if (kind == 1) {    //蘑菇怪的初始化
		agk::SetSpriteAnimation(get_sprite_index(), 60, 60, 3);
		agk::PlaySprite(get_sprite_index(), 3, 1, 1, 2);
	}
	else if (kind == 2) {   //乌龟怪的初始化
		agk::SetSpriteAnimation(get_sprite_index(), 60, 60, 5);
		if (direction == 1) {   //根据当前方向播放动画
			agk::PlaySprite(get_sprite_index(), 3, 1, 3, 4);
		}
		else {
			agk::PlaySprite(get_sprite_index(), 3, 1, 1, 2);
		}
	}
}

void Enemy::set_moved_dist(float dist)
{
	moved_dist = dist;
}

void Enemy::set_dead_time(int time)
{
	dead_time = time;
}




//********************************************************************************
//********************************************************************************
//class Game
Game::Game()
{
	g_state = 0;
	score = 0;

	//text index
	info_text_index = 1;
	rule_text_index = 2;
	prompt_text_index = 3;
	game_over_text_index = 4;
	summary_text_index = 5;

	//tile map img index
	brick = 1;
	pipe = 2;
	award1 = 3;
	award0 = 4;
	box = 5;
	castle = 7;
	cloud = 8;
	hill = 9;
	sky = 10;

	//init tile map
	int tmp_map[TILE_ROWS][TILE_COLS] =        //16 * 20*2
	{																    //10																  //20																  //30																	//40																  //50																    //60																  //70																	//80
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, cloud, cloud, cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},//1
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, cloud, cloud, sky,   sky,   sky,   cloud, cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{hill,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   cloud, cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   cloud, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   box,   box,   sky,   sky,   sky,   box,   box,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   brick, brick, brick, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   box,   box,   sky,   sky,   sky,   box,   box,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},//6
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   box,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   brick, brick, brick, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   castle,sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   brick, award1,brick, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   brick, brick, brick, brick, brick, sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,	sky,   sky,   sky,   pipe,  sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},//11
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,	sky,   sky,   sky,   pipe,  sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   pipe,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky},
		{brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   brick,brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   sky,   brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick},
		{brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   brick,brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   sky,   brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick},
		{brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   brick,brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   sky,   brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick},
		{brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   brick,brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, sky,   sky,   sky,   brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick, brick},//16
		{sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,	sky,   sky,   sky,   sky,   sky,   sky,   sky,  sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky,   sky}
	};
	for (int i = 0; i < TILE_ROWS; i++) {
		for (int j = 0; j < TILE_COLS; j++) {
			tile_map[i][j] = tmp_map[i][j];
		}
	}
}

Game::~Game()
{
}

int Game::get_g_state()
{
	return g_state;
}

int Game::get_info_text_index()
{
	return info_text_index;
}

int Game::get_rule_text_index()
{
	return rule_text_index;
}

int Game::get_prompt_text_index()
{
	return prompt_text_index;
}

int Game::get_game_over_text_index()
{
	return game_over_text_index;
}

int Game::get_summary_text_index()
{
	return summary_text_index;
}

int Game::get_score()
{
	return score;
}

void Game::display_info()
{
	//set visible
	if (!agk::GetTextVisible(info_text_index)) {  
		agk::SetTextVisible(info_text_index, SHOW);
	}
	agk::Sync();

	//play music
}

void Game::hide_info()
{
	agk::SetTextVisible(info_text_index, HIDE);
	agk::ResetTimer();
	g_state++;   //切换游戏状态 加一即可
}

void Game::display_rules()
{
	//set visible
	if (!agk::GetTextVisible(rule_text_index)) {
		agk::SetTextVisible(rule_text_index, SHOW);
	}

	//play music
}

void Game::hide_rules()
{
	agk::SetTextVisible(rule_text_index, HIDE);
	agk::ResetTimer();
	g_state++;   //切换游戏状态 加一即可
}

void Game::play_game(Mario &mario, Enemy enemys[], Award awards[], Trap traps[])
{
	agk::SetSpriteVisible(mario.get_sprite_index(), SHOW);
	move_mario(mario);
	//move_enemy(enemy);
	move_enemys(enemys);
	move_awards(awards);
	move_traps(traps);
	check_for_collision(mario, enemys, awards, traps);
}

void Game::hide_game(Enemy enemys[], Award awards[], Trap traps[])
{
	hide_tiles();
	//agk::SetSpriteVisible(enemy.get_sprite_index(), HIDE);
	for (int i = 0; i < ENEMY_NUM; i++) {
		agk::SetSpriteVisible(enemys[i].get_sprite_index(), HIDE);
	}
	//hide award
	for (int i = 0; i < AWARD_NUM; i++) {
		agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);
	}
	//hide trap
	for (int i = 0; i < TRAP_NUM; i++) {
		agk::SetSpriteVisible(traps[i].get_sprite_index(), HIDE);
	}
	g_state = 4;
	agk::ResetTimer();   //否则会造成上次reset时间距离太远，game_over不会输出
}

void Game::display_prompt()
{
	score = 0;   //在这里对score初始化一下

	//set visible
	if (!agk::GetTextVisible(prompt_text_index)) {
		agk::SetTextVisible(prompt_text_index, SHOW);
	}

	//play music
}

void Game::hide_prompt()
{
	agk::SetTextVisible(prompt_text_index, HIDE);
	agk::ResetTimer();
	g_state = 3;    //游戏开始状态
}

void Game::load_tile_img()
{
	//load img
	agk::LoadImage(brick, "./Mario/my_mario/brick.png");
	agk::LoadImage(sky, "./Mario/my_mario/sky.png");
	agk::LoadImage(castle, "./Mario/castle.png");
	agk::LoadImage(cloud, "./Mario/cloud.png");
	agk::LoadImage(hill, "./Mario/hill.png");
	agk::LoadImage(pipe, "./Mario/ob7.png");
	agk::LoadImage(award1, "./Mario/ob5.png");
	agk::LoadImage(award0, "./Mario/ob3.png");
	agk::LoadImage(box, "./Mario/ob11.png");
}

void Game::display_game_over()
{
	//set visible
	if (!agk::GetTextVisible(game_over_text_index)) {
		agk::SetTextVisible(game_over_text_index, SHOW);
	}
	//play music
}

void Game::hide_game_over()
{
	agk::SetTextVisible(game_over_text_index, HIDE);
	agk::ResetTimer();
	g_state = 2;   //切换游戏状态到开始界面
}

void Game::display_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[])
{
	agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
	hide_tiles();
	//hide enemy
	//agk::SetSpriteVisible(enemy.get_sprite_index(), HIDE);
	for (int i = 0; i < ENEMY_NUM; i++) {
		agk::SetSpriteVisible(enemys[i].get_sprite_index(), HIDE);
	}
	//hide award
	for (int i = 0; i < AWARD_NUM; i++) {
		agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);
	}
	//hide trap
	for (int i = 0; i < TRAP_NUM; i++) {
		agk::SetSpriteVisible(traps[i].get_sprite_index(), HIDE);
	}

	//summary text
	if (mario.get_exist_state() == 3) {    //游戏胜利时打印信息
		string info = "\tVictory!\nYour score:" + to_string(static_cast<long long>(score+100));   //long long 或者long double    通关加100分
		agk::SetTextString(summary_text_index, info.c_str());
	}
	else {   //游戏中途打印信息，先隐藏其他物体
		string info = "Your health:" + to_string(static_cast<long long>(mario.get_health()));   //long long 或者long double
		agk::SetTextString(summary_text_index, info.c_str());
	}
	
	//set visible
	if (!agk::GetTextVisible(summary_text_index)) {
		agk::SetTextVisible(summary_text_index, SHOW);
	}

	//play music
}

void Game::hide_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[])
{
	extern int trap_pos[TRAP_NUM][4];

	agk::SetTextVisible(summary_text_index, HIDE);
	agk::ResetTimer();
	if (mario.get_exist_state() == 3)
		g_state = 2;   //如果游戏胜利切换游戏状态到游戏开始界面     否则切换到游戏play状态，暂定回到初始位置  
	else {
		g_state = 3;
		display_tiles();    //隐藏的物体重新展示
		agk::SetSpriteVisible(mario.get_sprite_index(), SHOW);

		//agk::SetSpriteVisible(enemy.get_sprite_index(), SHOW);
		for (int i = 0; i < ENEMY_NUM; i++) {
			//没有死的敌人才重新显示
			if(enemys[i].get_exist_state() != 0)
				agk::SetSpriteVisible(enemys[i].get_sprite_index(), SHOW);
		}
		//显示award
		for (int i = 0; i < AWARD_NUM; i++) {
			if (awards[i].get_exist_state() != 0)  //没被吃的award重新显示
				agk::SetSpriteVisible(awards[i].get_sprite_index(), SHOW);
		}
		//重置trap
		for (int i = 0; i < TRAP_NUM; i++) {
			traps[i].init_trap(trap_pos[i][0], trap_pos[i][1], trap_pos[i][2], trap_pos[i][3]);
		}
	}

}



void Game::init_tiles()
{
	float x = 0, y = 0;
	int sprite_index = TILE_SPRITE;

	for (int r = 0; r < TILE_ROWS; r++) {
		x = 0;
		for (int c = 0; c < TILE_COLS; c++) {
			agk::CreateSprite(sprite_index, tile_map[r][c]);
			//调整图片大小
			if (tile_map[r][c] == hill) {     //hill的大小另外设置
				agk::SetSpriteScale(sprite_index, 900 / agk::GetSpriteWidth(sprite_index), 600 / agk::GetSpriteHeight(sprite_index));
			}
			else if(tile_map[r][c] == castle) {
				agk::SetSpriteScale(sprite_index, TILE_W * 4 / agk::GetSpriteWidth(sprite_index), TILE_H * 4 / agk::GetSpriteHeight(sprite_index));
			}
			else if (tile_map[r][c] == cloud) {
				agk::SetSpriteScale(sprite_index, TILE_W * 2 / agk::GetSpriteWidth(sprite_index), TILE_H * 2 / agk::GetSpriteHeight(sprite_index));
			}
			else {
				agk::SetSpriteScale(sprite_index, TILE_W / agk::GetSpriteWidth(sprite_index), TILE_H / agk::GetSpriteHeight(sprite_index));
			}

			agk::SetSpritePosition(sprite_index, x, y);
			agk::SetSpriteVisible(sprite_index, HIDE);
			agk::SetSpriteDepth(sprite_index, 20*tile_map[r][c]);       //深度根据img index大小设置，index越大，深度越深
			x += TILE_W;
			sprite_index += 1;
		}
		y += TILE_H;
	}
}

void Game::init_tile_position()
{
	float x = 0, y = 0;
	int sprite_index = TILE_SPRITE;

	for (int r = 0; r < TILE_ROWS; r++) {
		x = 0;
		for (int c = 0; c < TILE_COLS; c++) {
			agk::SetSpritePosition(sprite_index, x, y);
			agk::SetSpriteVisible(sprite_index, HIDE);
			x += TILE_W;
			sprite_index += 1;
			if (tile_map[r][c] == award1) {   //顺便恢复一下奖励砖块
				agk::SetSpriteImage(TILE_SPRITE + r * TILE_COLS + c, award1);
			}
		}
		y += TILE_H;
	}
}

void Game::display_tiles()
{
	int sprite_index = TILE_SPRITE;
	for (int r = 0; r < TILE_ROWS; r++) {
		for (int c = 0; c < TILE_COLS; c++) {
			agk::SetSpriteVisible(sprite_index, SHOW);
			sprite_index++;
		}
	}
}

void Game::hide_tiles()
{
	int sprite_index = TILE_SPRITE;
	for (int r = 0; r < TILE_ROWS; r++) {
		for (int c = 0; c < TILE_COLS; c++) {
			agk::SetSpriteVisible(sprite_index, HIDE);
			sprite_index++;
		}
	}
}

void Game::change_entire_tile_x(float offset)
{
	float x;
	int sprite_index = TILE_SPRITE;
	for (int r = 0; r < TILE_ROWS; r++) {
		for (int c = 0; c < TILE_COLS; c++) {
			x = agk::GetSpriteX(sprite_index);
			agk::SetSpriteX(sprite_index, x + offset);
			sprite_index++;
		}
	}
}

void Game::move_mario(Mario& mario)
{
	//移动逻辑
	if (agk::GetRawKeyState(AGK_KEY_LEFT)) {     //一直按就要一直移动，所以采用state      如果pressed或者released，只会移动一次
		mario.set_direction(-1);
		int cur = mario.get_current_frame();  //获得当前帧
		if(mario.get_cnt() % 2 == 0)          //每两次切一帧
			mario.set_currrent_frame((cur + 1) % 10 + 1);   //设置当前帧为下一帧
		if (cur < 6)   //1~5帧为向右移动
			cur += 5;
		if (cur == 10)  //第十帧为跳跃
			cur = 6;
		agk::PlaySprite(mario.get_sprite_index(), 10, 1, cur, cur);    
		if (get_sprite_left_tile(mario) == sky) {     //没有碰到障碍才能前进
			if (agk::GetSpriteX(TILE_SPRITE) != 0 && mario.get_x() <= MARIO_X) {    //未到达边界则移动地图坐标         向左移动的边界是第一块tile的x != 0     如果马里奥x坐标大于初始值，则优先移动马里奥
				change_entire_tile_x(MOVE_DIST);
			}
			else {     //否则移动mario坐标
				float x = mario.get_x();
				mario.set_x(x - MOVE_DIST);
			}
		}
	}
	else if (agk::GetRawKeyState(AGK_KEY_RIGHT)) {    
		mario.set_direction(1);
		int cur = mario.get_current_frame();  //获得当前帧
		if (mario.get_cnt() % 2 == 0)
			mario.set_currrent_frame((cur + 1) % 10 + 1);   //设置当前帧为下一帧
		if (cur > 6)   //6~10帧为向左移动
			cur -= 5;
		if (cur == 5)   //第五帧为跳跃
			cur = 1;
		agk::PlaySprite(mario.get_sprite_index(), 10, 1, cur, cur);      
		if (get_sprite_right_tile(mario) == sky) {      //没有碰到障碍才能前进
			if (agk::GetSpriteX(TILE_SPRITE + TILE_COLS - 1) != 1140 && mario.get_x() >= MARIO_X) {    //未到达边界则移动地图坐标    向右移动的边界是第一行最后一块的tile的x != 1140   如果马里奥x坐标小于初始值，则优先移动马里奥
				change_entire_tile_x(-1 * MOVE_DIST);
			}
			else {     //否则移动mario坐标
				float x = mario.get_x();
				mario.set_x(x + MOVE_DIST);
			}
		}
	}

	//跳跃逻辑
	if (agk::GetRawKeyState(AGK_KEY_SPACE) && get_sprite_down_tile(mario) != sky) {  //只有一次跳跃完成后才可再次跳跃，落脚点不是sky即可
		mario.set_move_state(1);    //设置为跳跃状态
		mario.set_pre_y(mario.get_y());
	}

	if (agk::GetRawKeyReleased(AGK_KEY_SPACE)) {
		mario.set_move_state(2);   //松开跳跃键 则开始下落
	}

	if (mario.get_move_state() == 1) {   //=1则处于上升状态
		if(mario.get_direction() == 1)      //方向向右则播放向右跳的动画
			agk::PlaySprite(mario.get_sprite_index(), 3, 1, 5, 5);
		else       //方向向左则播放向左跳的动画
			agk::PlaySprite(mario.get_sprite_index(), 3, 1, 10, 10);

		float y = mario.get_y();
		if ((mario.get_pre_y() - y) < mario.get_jump_dist() && get_sprite_up_tile(mario) == sky)   //未达到预定上升高度则一直上升，如果上面撞到物体也要停止上升(即mario上部为sky才能上升)
			mario.set_y(y - 8);   //每次上升8
		else
			mario.set_move_state(0);
	}
	else {        //检测mario是否在下落，更新其y值   如果没碰到东西则一直下落
		float y = mario.get_y();
		float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
		float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
		int tile_r = (int)((y + mario_h) / TILE_H);

		float x = mario.get_x();
		float offset = agk::GetSpriteX(TILE_SPRITE);
		int l_tile_c = (int)(x + fabs(offset) + mario_w / 6) / TILE_W; //直接检测顶点会存在bug，检测比顶点坐标稍微小的点
		int r_tile_c = (int)(x + fabs(offset) + mario_w / 1.5) / TILE_W;  //左下方和右下方的行号相同，列号不同

		if (tile_map[tile_r][l_tile_c] == sky && tile_map[tile_r][r_tile_c] == sky) {     //如果mario包围框左下方和右下方的坐标所在的tile是sky则一直下落
			mario.set_y(mario.get_y() + 9);
			mario.set_move_state(2);
			//下落状态也播放跳跃动画
			if (mario.get_direction() == 1)      //方向向右则播放向右跳的动画
				agk::PlaySprite(mario.get_sprite_index(), 3, 1, 5, 5);
			else       //方向向左则播放向左跳的动画
				agk::PlaySprite(mario.get_sprite_index(), 3, 1, 10, 10);
		}
		else {    //否则说明已经踩到东西，改变移动状态，可以适当减小mario  y值，避免出现mario卡进地板的情况(将y设成固定值，否则会抖动)
			mario.set_y(tile_r * TILE_H - agk::GetSpriteHeight(mario.get_sprite_index()));
			mario.set_move_state(0);
		}
	}

	agk::Sync();
}

void Game::move_enemy(Enemy& enemy)    //只能移动单个mushroom_monster
{
	enemy.set_x(agk::GetSpriteX(TILE_SPRITE) + enemy.get_moved_dist());

	//如果没踩东西则一直下落  否则说明已经踩到东西，可以适当减小  y值，避免出现卡进地板的情况(将y设成固定值，否则会抖动)
	if (get_sprite_ld_tile(enemy) == sky && get_sprite_rd_tile(enemy) == sky) {
		enemy.set_y(enemy.get_y() + 5);
	}
	//移动逻辑
	else if (enemy.get_exist_state() != 0) {  //没死才能移动  落地才能移动
		if (enemy.get_direction() == 1 && get_sprite_right_tile(enemy) == sky) {      //方向向右，没有撞到东西，向右移动         下落时不能左右移动，一次只处理一个方向上的运动
			//enemy.set_x(enemy.get_x() + MOVE_DIST);
			enemy.set_moved_dist(enemy.get_moved_dist() + MOVE_DIST);
		}
		else if (enemy.get_direction() == -1 && get_sprite_left_tile(enemy) == sky) {    //方向向左，没有撞到东西，向左移动
			//enemy.set_x(enemy.get_x() - MOVE_DIST);
			enemy.set_moved_dist(enemy.get_moved_dist() - MOVE_DIST);
		}
		else {
			enemy.set_direction(enemy.get_direction() * -1);
		}
	}
	else if (enemy.get_exist_state() == 0 && agk::GetSpriteVisible(enemy.get_sprite_index())) {  //死了之后2s隐藏sprite
		time_t now = time(NULL);
		tm* tm_t = localtime(&now);
		if (tm_t->tm_sec >= enemy.get_dead_time() + 2) {
			agk::SetSpriteVisible(enemy.get_sprite_index(), HIDE);
		}
	}
}

void Game::move_enemys(Enemy enemys[]) 
{
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemys[i].set_x(agk::GetSpriteX(TILE_SPRITE) + enemys[i].get_moved_dist());

		//如果没踩东西则一直下落  否则说明已经踩到东西，可以适当减小  y值，避免出现卡进地板的情况(将y设成固定值，否则会抖动)
		if (get_sprite_ld_tile(enemys[i]) == sky && get_sprite_rd_tile(enemys[i]) == sky) {
			enemys[i].set_y(enemys[i].get_y() + 5);
		}
		//移动逻辑
		else if (enemys[i].get_exist_state() != 0) {  //没死才能移动   怪物要落地后才能移动
			if (enemys[i].get_direction() == 1 && get_sprite_right_tile(enemys[i]) == sky) {      //方向向右，没有撞到东西，向右移动         下落时不能左右移动，一次只处理一个方向上的运动
				//enemys[i].set_x(enemys[i].get_x() + MOVE_DIST);
				if (enemys[i].get_kind() == 1) {    //蘑菇移动距离固定
					enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 1 * MOVE_DIST);
				}
				else if (enemys[i].get_kind() == 2) {   //乌龟需要重新播放动画    在方向改变时再切换动画，这里切换会导致"平移”
					if (enemys[i].get_exist_state() == 1) {
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 1.05 * MOVE_DIST);
					}
					else {    //乌龟是龟壳状态
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 2.2 * MOVE_DIST);
					}
				}
			}
			else if (enemys[i].get_direction() == -1 && get_sprite_left_tile(enemys[i]) == sky) {    //方向向左，没有撞到东西，向左移动
				//enemys[i].set_x(enemys[i].get_x() - MOVE_DIST);
				if (enemys[i].get_kind() == 1) {    //蘑菇移动距离固定
					enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 1 * MOVE_DIST);
				}
				else if (enemys[i].get_kind() == 2) {
					if (enemys[i].get_exist_state() == 1) {
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 1.05 * MOVE_DIST);
					}
					else {    //乌龟是龟壳状态
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 2.2 * MOVE_DIST);
					}
				}
			}
			else {
				enemys[i].set_direction(enemys[i].get_direction() * -1);
				if (enemys[i].get_kind() == 2 && enemys[i].get_exist_state() == 1) {   //乌龟在方向变化时需要切换动画
					if (enemys[i].get_direction() == 1) {  //切换方向后根据方向播放动画
						agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 3, 4);
					}
					else {
						agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 1, 2);
					}
				}
			}
		}
		else if (enemys[i].get_exist_state() == 0 && agk::GetSpriteVisible(enemys[i].get_sprite_index())) {  //死了之后2s隐藏sprite
			time_t now = time(NULL);
			tm* tm_t = localtime(&now);
			if (tm_t->tm_sec >= enemys[i].get_dead_time() + 2) {
				agk::SetSpriteVisible(enemys[i].get_sprite_index(), HIDE);
			}
		}
	}
}

void Game::move_awards(Award awards[])
{
	for (int i = 0; i < AWARD_NUM; i++) {
		awards[i].set_x(agk::GetSpriteX(TILE_SPRITE) + awards[i].get_moved_dist());

		//如果没踩东西则一直下落  否则说明已经踩到东西，可以适当减小  y值，避免出现卡进地板的情况(将y设成固定值，否则会抖动)
		if (awards[i].get_kind() == 2 && awards[i].get_exist_state() == 1) {        //存活状态的蘑菇才需要移动
			if (get_sprite_down_tile(awards[i]) == sky) {    //没落地就下落，落地了就上升一小段距离
				awards[i].set_y(awards[i].get_y() + 5);
			}
			else{   //蘑菇慢慢上升
				awards[i].set_y(awards[i].get_y() - 1);
			}

			if (awards[i].get_direction() == 1 && get_sprite_right_tile(awards[i], 0.8) == sky) {      //方向向右，没有撞到东西，向右移动         下落时不能左右移动，一次只处理一个方向上的运动
				awards[i].set_moved_dist(awards[i].get_moved_dist() + MOVE_DIST * 1.2);
			}
			else if (awards[i].get_direction() == -1 && get_sprite_left_tile(awards[i], 0.8) == sky) {    //方向向左，没有撞到东西，向左移动
				awards[i].set_moved_dist(awards[i].get_moved_dist() - MOVE_DIST * 1.2);
			}
			else {   //改变移动方向
				awards[i].set_direction(awards[i].get_direction() * -1);
			}
		}
	}
}

void Game::move_traps(Trap traps[])
{
	for (int i = 0; i < TRAP_NUM; i++) {
		traps[i].set_x(agk::GetSpriteX(TILE_SPRITE) + traps[i].get_moved_dist());   //地图坐标变了，trap坐标也要更新
		if (traps[i].get_exist_state() == 1) {   //状态为激活态才移动
			if (traps[i].get_kind() == 1) {   //kunkun的移动方式   向上平移
				if (traps[i].get_y() >= 390) {
					traps[i].set_y(traps[i].get_y() - MOVE_DIST * 5);
				}
			}
			else if (traps[i].get_kind() == 2) {  //篮球的移动方式   向左平移
				traps[i].set_moved_dist(traps[i].get_moved_dist() - MOVE_DIST * 5);
			}
		}
	}
}

void Game::add_score(int s)
{
	score += s;
}

int Game::get_sprite_lu_tile(Sprite& sprite)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset)) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_lu_tile(Mario& mario)    //检测mario包围盒左上角的点
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)y / TILE_H;
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset)) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_ru_tile(Sprite& sprite)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_ru_tile(Mario& mario)
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)y / TILE_H;
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset) + mario_w) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_ld_tile(Sprite& sprite)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y + sprite_h) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset)) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_ld_tile(Mario& mario)
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)((y + mario_h) / TILE_H);
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset)) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_rd_tile(Sprite& sprite)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y + sprite_h) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_rd_tile(Mario& mario)
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)((y + mario_h) / TILE_H);
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset) + mario_w) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_up_tile(Sprite& sprite, float propotion)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w * propotion) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_up_tile(Mario& mario)
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)((y) / TILE_H);
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset) + mario_w / 2) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_down_tile(Sprite& sprite, float propotion)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y + sprite_h) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w * propotion) / TILE_W;
	return tile_map[tile_r][tile_c];
}

//int Game::get_mario_down_tile(Mario& mario)
//{
//	float y = mario.get_y();
//	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
//	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
//
//	int tile_r = (int)((y + mario_h) / TILE_H);
//	float x = mario.get_x();
//	float offset = agk::GetSpriteX(TILE_SPRITE);
//	int tile_c = (int)(x + fabs(offset) + mario_w / 2) / TILE_W;
//	return tile_map[tile_r][tile_c];
//}

int Game::get_sprite_left_tile(Sprite& sprite, float propotion)
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y + sprite_h * propotion) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset)) / TILE_W;
	return tile_map[tile_r][tile_c];
}

/*int Game::get_mario_left_tile(Mario& mario)
{
	float y = mario.get_y();
	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());

	int tile_r = (int)((y + mario_h / 2) / TILE_H);
	float x = mario.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset)) / TILE_W;
	return tile_map[tile_r][tile_c];
}*/

int Game::get_sprite_right_tile(Sprite& sprite, float propotion)      //enemy也需要碰撞检测，所以改成sprite
{
	float y = sprite.get_y();
	float sprite_h = agk::GetSpriteHeight(sprite.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(sprite.get_sprite_index());

	int tile_r = (int)((y + sprite_h * propotion) / TILE_H);
	float x = sprite.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w) / TILE_W;
	return tile_map[tile_r][tile_c];
}

/*int Game::get_mario_right_tile(Mario& mario)
{
	float y = mario.get_y();
	float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
	float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());

	int tile_r = (int)((y + mario_h / 2) / TILE_H);
	float x = mario.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + mario_w) / TILE_W;
	return tile_map[tile_r][tile_c];
}*/

void Game::check_for_collision(Mario& mario, Enemy enemys[], Award awards[], Trap traps[])
{
	//如果到达胜利区域
	int a = VICTORY_X;
	if (mario.get_x() >= VICTORY_X && mario.get_x() <= (VICTORY_X + 30) && (mario.get_y()+30) >= VICTORY_Y && mario.get_y() <= VICTORY_Y + 60) {  
		mario.set_exist_state(3);        //游戏胜利设置状态为胜利，输出相应信息
		agk::ResetTimer();
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
		hide_tiles();
		g_state = 5;
	}

	//如果 mario下落到底，则生命值减一，生命值减一后，恢复到初始位置靠左点的地方  
	if (mario.get_y() >= 15*TILE_H) {    
		mario.set_health(mario.get_health() - 1);
		mario.set_exist_state(1);
		agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
		mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //恢复跳跃距离和大小
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
		hide_tiles();
		mario.set_x(MARIO_X - 100);   //恢复到初始位置靠左的地方   其实还是不妥，会有bug
		mario.set_y(MARIO_Y - 200);

		if (mario.get_health() > 0) {   //大于0则还是存活状态，中途打印summary
			agk::ResetTimer();
			g_state = 5;
		}
	}

	//mario 和 enemy发生碰撞  ***********************************************************************
	//if (check_collision_ways(mario, enemy) == 1 && enemy.get_exist_state() != 0) {     //撞到敌人，敌人不是死亡状态，则生命值减一
	//	mario.set_health(mario.get_health() - 1);
	//	agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
	//	hide_tiles();
	//	mario.set_x(MARIO_X - 200);   //恢复到初始位置靠左的地方   其实还是不妥，会有bug
	//	mario.set_y(MARIO_Y - 200);

	//	if (mario.get_health() > 0) {   //大于0则还是存活状态，中途打印summary
	//		agk::ResetTimer();
	//		g_state = 5;
	//	}
	//}
	//else if (check_collision_ways(mario, enemy) == 2 && enemy.get_exist_state() != 0) {   //踩到敌人，把敌人踩死
	//	mario.set_y(mario.get_y() - 20);  //踩到敌人会上升一段距离

	//	enemy.set_exist_state(0);  //踩死敌人
	//	agk::PlaySprite(enemy.get_sprite_index(), 3, 1, 3, 3);

	//	time_t now = time(NULL);   //设置死亡时间
	//	tm* tm_t = localtime(&now);
	//	enemy.set_dead_time(tm_t->tm_sec);

	//	add_score(MUSHROOM_SCORE);  //踩死蘑菇怪加分
	//}
	if (mario.get_unmatched() > 0) {   //如果此时是无敌状态，不做怪物碰撞检测
		mario.set_unmatched(mario.get_unmatched() - 1);
		agk::SetSpriteColorGreen(mario.get_sprite_index(), 100);
	}
	else {
		agk::SetSpriteColorGreen(mario.get_sprite_index(), 255);
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (check_collision_ways(mario, enemys[i]) == 1 && enemys[i].get_exist_state() != 0) {     //撞到敌人，敌人不是死亡状态，则生命值减一， 如果是变大状态则先变小
				if (mario.get_exist_state() == 2) {    //但还是会瞬间两次碰撞导致扣血，需要设置无敌帧
					mario.set_exist_state(1);
					agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
					mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //恢复跳跃距离和大小
					mario.set_unmatched(UNMATCHED_TIME);   //设置无敌时间
				}
				else {
					mario.set_health(mario.get_health() - 1);
					agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
					hide_tiles();
					mario.set_x(MARIO_X - 200);   //恢复到初始位置靠左的地方   其实还是不妥，会有bug
					mario.set_y(MARIO_Y - 200);

					if (mario.get_health() > 0) {   //大于0则还是存活状态，中途打印summary
						agk::ResetTimer();
						g_state = 5;
					}
				}
			}
			else if (check_collision_ways(mario, enemys[i]) == 2 && enemys[i].get_exist_state() != 0) {   //踩到敌人，把敌人踩死   乌龟则变成龟壳状态
				mario.set_y(mario.get_y() - 40);  //踩到敌人会上升一段距离

				if (enemys[i].get_kind() == 1) {    //踩死蘑菇怪
					enemys[i].set_exist_state(0);  //踩死敌人
					agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 3, 3);
					time_t now = time(NULL);   //设置死亡时间
					tm* tm_t = localtime(&now);
					enemys[i].set_dead_time(tm_t->tm_sec);

					add_score(MUSHROOM_SCORE);  //踩死蘑菇怪加分
				}
				else if (enemys[i].get_kind() == 2) {   //踩到乌龟
					if (enemys[i].get_exist_state() == 1)   //只有活着踩死才加分
						add_score(TURTLE_SCORE);

					enemys[i].set_exist_state(2);    //变成龟壳
					agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 5, 5);
				}
			}
		}
	}

	//检测与陷阱的碰撞
	for (int i = 0; i < TRAP_NUM; i++) {
		if (traps[i].get_exist_state() == 0) {   //休眠状态检查是否踩到机关
			if (fabs(mario.get_x() - agk::GetSpriteX(TILE_SPRITE) - traps[i].get_trigger_x()) <= 10 && mario.get_y() >= traps[i].get_trigger_y()) {  //如果踩到机关
			//if(mario.get_x() - agk::GetSpriteX(TILE_SPRITE) > traps[i].get_x() && mario.get_y() > traps[i].get_y()){
				float tmp;
				if (mario.get_health() < 3)
   					tmp = agk::GetSpriteX(TILE_SPRITE);
           		traps[i].set_exist_state(1);   //激活机关
				agk::SetSpriteVisible(traps[i].get_sprite_index(), SHOW);
			}
		}
		else if (traps[i].get_exist_state() == 1) {   //激活态检测碰撞
			if (check_collision_ways(mario, traps[i]) > 0) {  //撞到则扣血
				mario.set_health(mario.get_health() - 1);
				mario.set_exist_state(1);
				agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
				mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //恢复跳跃距离和大小
				agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
				hide_tiles();
				mario.set_x(MARIO_X - 100);   //恢复到初始位置靠左的地方   其实还是不妥，会有bug
				mario.set_y(MARIO_Y - 200);

				if (mario.get_health() > 0) {   //大于0则还是存活状态，中途打印summary
					agk::ResetTimer();
					g_state = 5;
				}
			}
		}
	}

	//检测是否撞到奖励
	for (int i = 0; i < AWARD_NUM; i++) {
		if (check_collision_ways(mario, awards[i]) > 0 && awards[i].get_exist_state() != 0) {   //撞到没被吃的奖励   不能用agk的碰撞检测，范围太大了
			if (awards[i].get_kind() == 1) {   //撞到金币
				awards[i].set_exist_state(0);
				agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);

				add_score(GOLD_SCORE);
			}
			else if (awards[i].get_kind() == 2) {   //撞到蘑菇
				if (awards[i].get_exist_state() == 1) {
					awards[i].set_exist_state(0);
					agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);

					agk::SetSpriteScale(mario.get_sprite_index(), 1.5, 1.5);   //变大

					mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //跳跃距离变长

					add_score(GOLD_SCORE * 2);
				}
				else if (awards[i].get_exist_state() == -1) {   //撞到隐藏状态的蘑菇先设为存活状态
					awards[i].set_exist_state(1);

					awards[i].set_y(awards[i].get_y() - 5);  //防止第一次撞到蘑菇就变大了
					mario.set_exist_state(2);
				}
			}
		}
	}

	//mario 头部撞击检测，检测是否撞到奖励砖块******************************
	float y = mario.get_y();
	float sprite_h = agk::GetSpriteHeight(mario.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(mario.get_sprite_index());

	int tile_r = (int)((y) / TILE_H);
	float x = mario.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w / 2) / TILE_W;
	if (tile_map[tile_r][tile_c] == award1) {   //碰到奖励砖块，切换图片
		agk::SetSpriteImage(TILE_SPRITE + tile_r * TILE_COLS + tile_c, award0);
	}

	if (mario.get_health() <= 0) { 
		mario.set_exist_state(0);    //生命值小于0则死亡隐藏mario
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
	}
}

int Game::check_collision_ways(Mario& mario, Sprite& sprite)		 //检查mario是踩到精灵还是撞到精灵   踩到：2     撞到：1	没有碰到：0
{  
	//mario 实际的位置大约占包围盒中间的 1/3
	//用mario 左右各个三个点来判断  1/3 ~ 2/3的区域左右的上中下三个点
	float x1 = agk::GetSpriteX(mario.get_sprite_index()) + agk::GetSpriteWidth(mario.get_sprite_index()) * 1 / 3;   //三分之一处的位置
	float y1 = agk::GetSpriteY(mario.get_sprite_index());
	float x2 = agk::GetSpriteX(mario.get_sprite_index()) + agk::GetSpriteWidth(mario.get_sprite_index()) * 2 / 3;
	float y2 = agk::GetSpriteY(mario.get_sprite_index()) + agk::GetSpriteHeight(mario.get_sprite_index()) * 1 / 2;
	float y3 = agk::GetSpriteY(mario.get_sprite_index()) + agk::GetSpriteHeight(mario.get_sprite_index());   //检测 2 * 3 六个点

	float t_x1 = sprite.get_x();
	float t_x2 = sprite.get_x() + agk::GetSpriteWidth(sprite.get_sprite_index());
	float t_y1 = sprite.get_y();
	float t_y2 = sprite.get_y() + agk::GetSpriteHeight(sprite.get_sprite_index());
	
	//if (agk::GetSpriteCollision(mario.get_sprite_index(), sprite.get_sprite_index())) {  //不能用agk::GetspriteCollision来判断 范围太宽了
	if( (x1-t_x1)*(x1-t_x2) < 0&&(y1-t_y1)*(y1-t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y1 - t_y1) * (y1 - t_y2) < 0 || (x1 - t_x1) * (x1 - t_x2) < 0 && (y2 - t_y1) * (y2 - t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y2 - t_y1) * (y2 - t_y2) < 0 || (x1 - t_x1) * (x1 - t_x2) < 0 && (y3 - t_y1) * (y3 - t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y3 - t_y1) * (y3 - t_y2) < 0){
		//float mid_x = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) / 2;
		float r = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) * 2 / 3;   //监测点稍微往中间靠
		float l = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) * 1 / 3;

		float l_x = sprite.get_x();
		float r_x = l_x + agk::GetSpriteWidth(sprite.get_sprite_index());

		float m_y = mario.get_y() + agk::GetSpriteHeight(mario.get_sprite_index());
		float s_y = sprite.get_y() + 20;

		if ((r > l_x && r < r_x || l > l_x && l < r_x) && m_y <= s_y) {   //m_y < s_y 即mario 在 sprite上面
			return 2;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
}


//************************************************************************************
//************************************************************************************
//class Award
Award::Award(int index, int kind, string filename)
	:Sprite(index, filename)
{
	moved_dist = 0;
	this->kind = kind;
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //初始随机方向
	if (kind == 2) {   //蘑菇先设置为隐藏状态      第一次碰到后设为存活态还是碰到奖励块后设为存活态？  等下初始化为1进行测试就行
		exist_state = -1;
	}
	else {
		exist_state = 1;
	}
}

Award::~Award()
{

}

int Award::get_exist_state()
{
	return exist_state;
}

int Award::get_direction()
{
	return direction;
}

int Award::get_kind()
{
	return kind;
}

float Award::get_moved_dist()
{
	return moved_dist;
}

void Award::init_award(float x, float y)
{
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //初始随机方向
	if (kind == 2) {   //蘑菇先设置为隐藏状态      第一次碰到后设为存活态还是碰到奖励块后设为存活态？  等下初始化为1进行测试就行
		exist_state = -1;
	}
	else {
		exist_state = 1;
	}

	set_x(x);
	set_y(y);
	moved_dist = x - agk::GetSpriteX(TILE_SPRITE);

	agk::SetSpriteDepth(get_sprite_index(), 4 * 20 + 1);   //将深度设置在砖头后面

	/*if (kind == 1) {   //金币大小设置             为什么没用？
		agk::SetSpriteScale(get_sprite_index(), 0.5 * TILE_W / agk::GetSpriteWidth(get_sprite_index()), 0.5 * TILE_H / agk::GetSpriteHeight(get_sprite_index()));
	}
	else {
		agk::SetSpriteScale(get_sprite_index(), TILE_W / agk::GetSpriteWidth(get_sprite_index()), TILE_H / agk::GetSpriteHeight(get_sprite_index()));
	}*/
}

void Award::set_exist_state(int state)
{
	exist_state = state;
}

void Award::set_direction(int d)
{
	direction = d;
}

void Award::set_moved_dist(float dist)
{
	moved_dist = dist;
}


//***********************************************************************************
//***********************************************************************************
//class trap

Trap::Trap(int index, int kind, string filename)
	:Sprite(index, filename)
{
	moved_dist = 0;
	exist_state = 0;
	direction = 1;
	this->kind = kind;
	trigger_x = 0;
	trigger_y = 0;
}

Trap::~Trap()
{
}

int Trap::get_exist_state()
{
	return exist_state;
}

int Trap::get_direction()
{
	return direction;
}

int Trap::get_kind()
{
	return kind;
}

float Trap::get_moved_dist()
{
	return moved_dist;
}

float Trap::get_trigger_x()
{
	return trigger_x;
}

float Trap::get_trigger_y()
{
	return trigger_y;
}

void Trap::set_exist_state(int state)
{
	exist_state = state;
}

void Trap::set_direction(int d)
{
	direction = d;
}

void Trap::set_moved_dist(float dist)
{
	moved_dist = dist;
}

void Trap::init_trap(float init_x, float init_y, float trigger_x, float trigger_y)
{
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //初始随机方向
	exist_state = 0;

	set_x(init_x);
	set_y(init_y);
	//moved_dist = init_x - agk::GetSpriteX(TILE_SPRITE);   //否则中途初始化后位置就变了
	moved_dist = init_x;

	this->trigger_x = trigger_x;
	this->trigger_y = trigger_y;

	agk::SetSpriteVisible(get_sprite_index(), HIDE);
}
