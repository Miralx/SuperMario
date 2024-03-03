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
	if (x < 0 && sprite_index == 1)    //x ��СΪ0    ֻ��mario�б߽�
		x = 0;
	if(x > SCREEN_W - agk::GetSpriteWidth(sprite_index) && sprite_index == 1)   //x���Ϊscreen_w - sprite_w
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
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //��ʼ�������
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
	//direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //��ʼ�������
	direction = 1;
	exist_state = 1;

	set_x(x);
	set_y(y);

	moved_dist = x - agk::GetSpriteX(TILE_SPRITE);

	if (kind == 1) {    //Ģ���ֵĳ�ʼ��
		agk::SetSpriteAnimation(get_sprite_index(), 60, 60, 3);
		agk::PlaySprite(get_sprite_index(), 3, 1, 1, 2);
	}
	else if (kind == 2) {   //�ڹ�ֵĳ�ʼ��
		agk::SetSpriteAnimation(get_sprite_index(), 60, 60, 5);
		if (direction == 1) {   //���ݵ�ǰ���򲥷Ŷ���
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
	g_state++;   //�л���Ϸ״̬ ��һ����
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
	g_state++;   //�л���Ϸ״̬ ��һ����
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
	agk::ResetTimer();   //���������ϴ�resetʱ�����̫Զ��game_over�������
}

void Game::display_prompt()
{
	score = 0;   //�������score��ʼ��һ��

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
	g_state = 3;    //��Ϸ��ʼ״̬
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
	g_state = 2;   //�л���Ϸ״̬����ʼ����
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
	if (mario.get_exist_state() == 3) {    //��Ϸʤ��ʱ��ӡ��Ϣ
		string info = "\tVictory!\nYour score:" + to_string(static_cast<long long>(score+100));   //long long ����long double    ͨ�ؼ�100��
		agk::SetTextString(summary_text_index, info.c_str());
	}
	else {   //��Ϸ��;��ӡ��Ϣ����������������
		string info = "Your health:" + to_string(static_cast<long long>(mario.get_health()));   //long long ����long double
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
		g_state = 2;   //�����Ϸʤ���л���Ϸ״̬����Ϸ��ʼ����     �����л�����Ϸplay״̬���ݶ��ص���ʼλ��  
	else {
		g_state = 3;
		display_tiles();    //���ص���������չʾ
		agk::SetSpriteVisible(mario.get_sprite_index(), SHOW);

		//agk::SetSpriteVisible(enemy.get_sprite_index(), SHOW);
		for (int i = 0; i < ENEMY_NUM; i++) {
			//û�����ĵ��˲�������ʾ
			if(enemys[i].get_exist_state() != 0)
				agk::SetSpriteVisible(enemys[i].get_sprite_index(), SHOW);
		}
		//��ʾaward
		for (int i = 0; i < AWARD_NUM; i++) {
			if (awards[i].get_exist_state() != 0)  //û���Ե�award������ʾ
				agk::SetSpriteVisible(awards[i].get_sprite_index(), SHOW);
		}
		//����trap
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
			//����ͼƬ��С
			if (tile_map[r][c] == hill) {     //hill�Ĵ�С��������
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
			agk::SetSpriteDepth(sprite_index, 20*tile_map[r][c]);       //��ȸ���img index��С���ã�indexԽ�����Խ��
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
			if (tile_map[r][c] == award1) {   //˳��ָ�һ�½���ש��
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
	//�ƶ��߼�
	if (agk::GetRawKeyState(AGK_KEY_LEFT)) {     //һֱ����Ҫһֱ�ƶ������Բ���state      ���pressed����released��ֻ���ƶ�һ��
		mario.set_direction(-1);
		int cur = mario.get_current_frame();  //��õ�ǰ֡
		if(mario.get_cnt() % 2 == 0)          //ÿ������һ֡
			mario.set_currrent_frame((cur + 1) % 10 + 1);   //���õ�ǰ֡Ϊ��һ֡
		if (cur < 6)   //1~5֡Ϊ�����ƶ�
			cur += 5;
		if (cur == 10)  //��ʮ֡Ϊ��Ծ
			cur = 6;
		agk::PlaySprite(mario.get_sprite_index(), 10, 1, cur, cur);    
		if (get_sprite_left_tile(mario) == sky) {     //û�������ϰ�����ǰ��
			if (agk::GetSpriteX(TILE_SPRITE) != 0 && mario.get_x() <= MARIO_X) {    //δ����߽����ƶ���ͼ����         �����ƶ��ı߽��ǵ�һ��tile��x != 0     ��������x������ڳ�ʼֵ���������ƶ������
				change_entire_tile_x(MOVE_DIST);
			}
			else {     //�����ƶ�mario����
				float x = mario.get_x();
				mario.set_x(x - MOVE_DIST);
			}
		}
	}
	else if (agk::GetRawKeyState(AGK_KEY_RIGHT)) {    
		mario.set_direction(1);
		int cur = mario.get_current_frame();  //��õ�ǰ֡
		if (mario.get_cnt() % 2 == 0)
			mario.set_currrent_frame((cur + 1) % 10 + 1);   //���õ�ǰ֡Ϊ��һ֡
		if (cur > 6)   //6~10֡Ϊ�����ƶ�
			cur -= 5;
		if (cur == 5)   //����֡Ϊ��Ծ
			cur = 1;
		agk::PlaySprite(mario.get_sprite_index(), 10, 1, cur, cur);      
		if (get_sprite_right_tile(mario) == sky) {      //û�������ϰ�����ǰ��
			if (agk::GetSpriteX(TILE_SPRITE + TILE_COLS - 1) != 1140 && mario.get_x() >= MARIO_X) {    //δ����߽����ƶ���ͼ����    �����ƶ��ı߽��ǵ�һ�����һ���tile��x != 1140   ��������x����С�ڳ�ʼֵ���������ƶ������
				change_entire_tile_x(-1 * MOVE_DIST);
			}
			else {     //�����ƶ�mario����
				float x = mario.get_x();
				mario.set_x(x + MOVE_DIST);
			}
		}
	}

	//��Ծ�߼�
	if (agk::GetRawKeyState(AGK_KEY_SPACE) && get_sprite_down_tile(mario) != sky) {  //ֻ��һ����Ծ��ɺ�ſ��ٴ���Ծ����ŵ㲻��sky����
		mario.set_move_state(1);    //����Ϊ��Ծ״̬
		mario.set_pre_y(mario.get_y());
	}

	if (agk::GetRawKeyReleased(AGK_KEY_SPACE)) {
		mario.set_move_state(2);   //�ɿ���Ծ�� ��ʼ����
	}

	if (mario.get_move_state() == 1) {   //=1��������״̬
		if(mario.get_direction() == 1)      //���������򲥷��������Ķ���
			agk::PlaySprite(mario.get_sprite_index(), 3, 1, 5, 5);
		else       //���������򲥷��������Ķ���
			agk::PlaySprite(mario.get_sprite_index(), 3, 1, 10, 10);

		float y = mario.get_y();
		if ((mario.get_pre_y() - y) < mario.get_jump_dist() && get_sprite_up_tile(mario) == sky)   //δ�ﵽԤ�������߶���һֱ�������������ײ������ҲҪֹͣ����(��mario�ϲ�Ϊsky��������)
			mario.set_y(y - 8);   //ÿ������8
		else
			mario.set_move_state(0);
	}
	else {        //���mario�Ƿ������䣬������yֵ   ���û����������һֱ����
		float y = mario.get_y();
		float mario_h = agk::GetSpriteHeight(mario.get_sprite_index());
		float mario_w = agk::GetSpriteWidth(mario.get_sprite_index());
		int tile_r = (int)((y + mario_h) / TILE_H);

		float x = mario.get_x();
		float offset = agk::GetSpriteX(TILE_SPRITE);
		int l_tile_c = (int)(x + fabs(offset) + mario_w / 6) / TILE_W; //ֱ�Ӽ�ⶥ������bug�����ȶ���������΢С�ĵ�
		int r_tile_c = (int)(x + fabs(offset) + mario_w / 1.5) / TILE_W;  //���·������·����к���ͬ���кŲ�ͬ

		if (tile_map[tile_r][l_tile_c] == sky && tile_map[tile_r][r_tile_c] == sky) {     //���mario��Χ�����·������·����������ڵ�tile��sky��һֱ����
			mario.set_y(mario.get_y() + 9);
			mario.set_move_state(2);
			//����״̬Ҳ������Ծ����
			if (mario.get_direction() == 1)      //���������򲥷��������Ķ���
				agk::PlaySprite(mario.get_sprite_index(), 3, 1, 5, 5);
			else       //���������򲥷��������Ķ���
				agk::PlaySprite(mario.get_sprite_index(), 3, 1, 10, 10);
		}
		else {    //����˵���Ѿ��ȵ��������ı��ƶ�״̬�������ʵ���Сmario  yֵ���������mario�����ذ�����(��y��ɹ̶�ֵ������ᶶ��)
			mario.set_y(tile_r * TILE_H - agk::GetSpriteHeight(mario.get_sprite_index()));
			mario.set_move_state(0);
		}
	}

	agk::Sync();
}

void Game::move_enemy(Enemy& enemy)    //ֻ���ƶ�����mushroom_monster
{
	enemy.set_x(agk::GetSpriteX(TILE_SPRITE) + enemy.get_moved_dist());

	//���û�ȶ�����һֱ����  ����˵���Ѿ��ȵ������������ʵ���С  yֵ��������ֿ����ذ�����(��y��ɹ̶�ֵ������ᶶ��)
	if (get_sprite_ld_tile(enemy) == sky && get_sprite_rd_tile(enemy) == sky) {
		enemy.set_y(enemy.get_y() + 5);
	}
	//�ƶ��߼�
	else if (enemy.get_exist_state() != 0) {  //û�������ƶ�  ��ز����ƶ�
		if (enemy.get_direction() == 1 && get_sprite_right_tile(enemy) == sky) {      //�������ң�û��ײ�������������ƶ�         ����ʱ���������ƶ���һ��ֻ����һ�������ϵ��˶�
			//enemy.set_x(enemy.get_x() + MOVE_DIST);
			enemy.set_moved_dist(enemy.get_moved_dist() + MOVE_DIST);
		}
		else if (enemy.get_direction() == -1 && get_sprite_left_tile(enemy) == sky) {    //��������û��ײ�������������ƶ�
			//enemy.set_x(enemy.get_x() - MOVE_DIST);
			enemy.set_moved_dist(enemy.get_moved_dist() - MOVE_DIST);
		}
		else {
			enemy.set_direction(enemy.get_direction() * -1);
		}
	}
	else if (enemy.get_exist_state() == 0 && agk::GetSpriteVisible(enemy.get_sprite_index())) {  //����֮��2s����sprite
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

		//���û�ȶ�����һֱ����  ����˵���Ѿ��ȵ������������ʵ���С  yֵ��������ֿ����ذ�����(��y��ɹ̶�ֵ������ᶶ��)
		if (get_sprite_ld_tile(enemys[i]) == sky && get_sprite_rd_tile(enemys[i]) == sky) {
			enemys[i].set_y(enemys[i].get_y() + 5);
		}
		//�ƶ��߼�
		else if (enemys[i].get_exist_state() != 0) {  //û�������ƶ�   ����Ҫ��غ�����ƶ�
			if (enemys[i].get_direction() == 1 && get_sprite_right_tile(enemys[i]) == sky) {      //�������ң�û��ײ�������������ƶ�         ����ʱ���������ƶ���һ��ֻ����һ�������ϵ��˶�
				//enemys[i].set_x(enemys[i].get_x() + MOVE_DIST);
				if (enemys[i].get_kind() == 1) {    //Ģ���ƶ�����̶�
					enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 1 * MOVE_DIST);
				}
				else if (enemys[i].get_kind() == 2) {   //�ڹ���Ҫ���²��Ŷ���    �ڷ���ı�ʱ���л������������л��ᵼ��"ƽ�ơ�
					if (enemys[i].get_exist_state() == 1) {
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 1.05 * MOVE_DIST);
					}
					else {    //�ڹ��ǹ��״̬
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() + 2.2 * MOVE_DIST);
					}
				}
			}
			else if (enemys[i].get_direction() == -1 && get_sprite_left_tile(enemys[i]) == sky) {    //��������û��ײ�������������ƶ�
				//enemys[i].set_x(enemys[i].get_x() - MOVE_DIST);
				if (enemys[i].get_kind() == 1) {    //Ģ���ƶ�����̶�
					enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 1 * MOVE_DIST);
				}
				else if (enemys[i].get_kind() == 2) {
					if (enemys[i].get_exist_state() == 1) {
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 1.05 * MOVE_DIST);
					}
					else {    //�ڹ��ǹ��״̬
						enemys[i].set_moved_dist(enemys[i].get_moved_dist() - 2.2 * MOVE_DIST);
					}
				}
			}
			else {
				enemys[i].set_direction(enemys[i].get_direction() * -1);
				if (enemys[i].get_kind() == 2 && enemys[i].get_exist_state() == 1) {   //�ڹ��ڷ���仯ʱ��Ҫ�л�����
					if (enemys[i].get_direction() == 1) {  //�л��������ݷ��򲥷Ŷ���
						agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 3, 4);
					}
					else {
						agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 1, 2);
					}
				}
			}
		}
		else if (enemys[i].get_exist_state() == 0 && agk::GetSpriteVisible(enemys[i].get_sprite_index())) {  //����֮��2s����sprite
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

		//���û�ȶ�����һֱ����  ����˵���Ѿ��ȵ������������ʵ���С  yֵ��������ֿ����ذ�����(��y��ɹ̶�ֵ������ᶶ��)
		if (awards[i].get_kind() == 2 && awards[i].get_exist_state() == 1) {        //���״̬��Ģ������Ҫ�ƶ�
			if (get_sprite_down_tile(awards[i]) == sky) {    //û��ؾ����䣬����˾�����һС�ξ���
				awards[i].set_y(awards[i].get_y() + 5);
			}
			else{   //Ģ����������
				awards[i].set_y(awards[i].get_y() - 1);
			}

			if (awards[i].get_direction() == 1 && get_sprite_right_tile(awards[i], 0.8) == sky) {      //�������ң�û��ײ�������������ƶ�         ����ʱ���������ƶ���һ��ֻ����һ�������ϵ��˶�
				awards[i].set_moved_dist(awards[i].get_moved_dist() + MOVE_DIST * 1.2);
			}
			else if (awards[i].get_direction() == -1 && get_sprite_left_tile(awards[i], 0.8) == sky) {    //��������û��ײ�������������ƶ�
				awards[i].set_moved_dist(awards[i].get_moved_dist() - MOVE_DIST * 1.2);
			}
			else {   //�ı��ƶ�����
				awards[i].set_direction(awards[i].get_direction() * -1);
			}
		}
	}
}

void Game::move_traps(Trap traps[])
{
	for (int i = 0; i < TRAP_NUM; i++) {
		traps[i].set_x(agk::GetSpriteX(TILE_SPRITE) + traps[i].get_moved_dist());   //��ͼ������ˣ�trap����ҲҪ����
		if (traps[i].get_exist_state() == 1) {   //״̬Ϊ����̬���ƶ�
			if (traps[i].get_kind() == 1) {   //kunkun���ƶ���ʽ   ����ƽ��
				if (traps[i].get_y() >= 390) {
					traps[i].set_y(traps[i].get_y() - MOVE_DIST * 5);
				}
			}
			else if (traps[i].get_kind() == 2) {  //������ƶ���ʽ   ����ƽ��
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

//int Game::get_mario_lu_tile(Mario& mario)    //���mario��Χ�����Ͻǵĵ�
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

int Game::get_sprite_right_tile(Sprite& sprite, float propotion)      //enemyҲ��Ҫ��ײ��⣬���Ըĳ�sprite
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
	//�������ʤ������
	int a = VICTORY_X;
	if (mario.get_x() >= VICTORY_X && mario.get_x() <= (VICTORY_X + 30) && (mario.get_y()+30) >= VICTORY_Y && mario.get_y() <= VICTORY_Y + 60) {  
		mario.set_exist_state(3);        //��Ϸʤ������״̬Ϊʤ���������Ӧ��Ϣ
		agk::ResetTimer();
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
		hide_tiles();
		g_state = 5;
	}

	//��� mario���䵽�ף�������ֵ��һ������ֵ��һ�󣬻ָ�����ʼλ�ÿ����ĵط�  
	if (mario.get_y() >= 15*TILE_H) {    
		mario.set_health(mario.get_health() - 1);
		mario.set_exist_state(1);
		agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
		mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //�ָ���Ծ����ʹ�С
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
		hide_tiles();
		mario.set_x(MARIO_X - 100);   //�ָ�����ʼλ�ÿ���ĵط�   ��ʵ���ǲ��ף�����bug
		mario.set_y(MARIO_Y - 200);

		if (mario.get_health() > 0) {   //����0���Ǵ��״̬����;��ӡsummary
			agk::ResetTimer();
			g_state = 5;
		}
	}

	//mario �� enemy������ײ  ***********************************************************************
	//if (check_collision_ways(mario, enemy) == 1 && enemy.get_exist_state() != 0) {     //ײ�����ˣ����˲�������״̬��������ֵ��һ
	//	mario.set_health(mario.get_health() - 1);
	//	agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
	//	hide_tiles();
	//	mario.set_x(MARIO_X - 200);   //�ָ�����ʼλ�ÿ���ĵط�   ��ʵ���ǲ��ף�����bug
	//	mario.set_y(MARIO_Y - 200);

	//	if (mario.get_health() > 0) {   //����0���Ǵ��״̬����;��ӡsummary
	//		agk::ResetTimer();
	//		g_state = 5;
	//	}
	//}
	//else if (check_collision_ways(mario, enemy) == 2 && enemy.get_exist_state() != 0) {   //�ȵ����ˣ��ѵ��˲���
	//	mario.set_y(mario.get_y() - 20);  //�ȵ����˻�����һ�ξ���

	//	enemy.set_exist_state(0);  //��������
	//	agk::PlaySprite(enemy.get_sprite_index(), 3, 1, 3, 3);

	//	time_t now = time(NULL);   //��������ʱ��
	//	tm* tm_t = localtime(&now);
	//	enemy.set_dead_time(tm_t->tm_sec);

	//	add_score(MUSHROOM_SCORE);  //����Ģ���ּӷ�
	//}
	if (mario.get_unmatched() > 0) {   //�����ʱ���޵�״̬������������ײ���
		mario.set_unmatched(mario.get_unmatched() - 1);
		agk::SetSpriteColorGreen(mario.get_sprite_index(), 100);
	}
	else {
		agk::SetSpriteColorGreen(mario.get_sprite_index(), 255);
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (check_collision_ways(mario, enemys[i]) == 1 && enemys[i].get_exist_state() != 0) {     //ײ�����ˣ����˲�������״̬��������ֵ��һ�� ����Ǳ��״̬���ȱ�С
				if (mario.get_exist_state() == 2) {    //�����ǻ�˲��������ײ���¿�Ѫ����Ҫ�����޵�֡
					mario.set_exist_state(1);
					agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
					mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //�ָ���Ծ����ʹ�С
					mario.set_unmatched(UNMATCHED_TIME);   //�����޵�ʱ��
				}
				else {
					mario.set_health(mario.get_health() - 1);
					agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
					hide_tiles();
					mario.set_x(MARIO_X - 200);   //�ָ�����ʼλ�ÿ���ĵط�   ��ʵ���ǲ��ף�����bug
					mario.set_y(MARIO_Y - 200);

					if (mario.get_health() > 0) {   //����0���Ǵ��״̬����;��ӡsummary
						agk::ResetTimer();
						g_state = 5;
					}
				}
			}
			else if (check_collision_ways(mario, enemys[i]) == 2 && enemys[i].get_exist_state() != 0) {   //�ȵ����ˣ��ѵ��˲���   �ڹ����ɹ��״̬
				mario.set_y(mario.get_y() - 40);  //�ȵ����˻�����һ�ξ���

				if (enemys[i].get_kind() == 1) {    //����Ģ����
					enemys[i].set_exist_state(0);  //��������
					agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 3, 3);
					time_t now = time(NULL);   //��������ʱ��
					tm* tm_t = localtime(&now);
					enemys[i].set_dead_time(tm_t->tm_sec);

					add_score(MUSHROOM_SCORE);  //����Ģ���ּӷ�
				}
				else if (enemys[i].get_kind() == 2) {   //�ȵ��ڹ�
					if (enemys[i].get_exist_state() == 1)   //ֻ�л��Ų����żӷ�
						add_score(TURTLE_SCORE);

					enemys[i].set_exist_state(2);    //��ɹ��
					agk::PlaySprite(enemys[i].get_sprite_index(), 3, 1, 5, 5);
				}
			}
		}
	}

	//������������ײ
	for (int i = 0; i < TRAP_NUM; i++) {
		if (traps[i].get_exist_state() == 0) {   //����״̬����Ƿ�ȵ�����
			if (fabs(mario.get_x() - agk::GetSpriteX(TILE_SPRITE) - traps[i].get_trigger_x()) <= 10 && mario.get_y() >= traps[i].get_trigger_y()) {  //����ȵ�����
			//if(mario.get_x() - agk::GetSpriteX(TILE_SPRITE) > traps[i].get_x() && mario.get_y() > traps[i].get_y()){
				float tmp;
				if (mario.get_health() < 3)
   					tmp = agk::GetSpriteX(TILE_SPRITE);
           		traps[i].set_exist_state(1);   //�������
				agk::SetSpriteVisible(traps[i].get_sprite_index(), SHOW);
			}
		}
		else if (traps[i].get_exist_state() == 1) {   //����̬�����ײ
			if (check_collision_ways(mario, traps[i]) > 0) {  //ײ�����Ѫ
				mario.set_health(mario.get_health() - 1);
				mario.set_exist_state(1);
				agk::SetSpriteScale(mario.get_sprite_index(), 1, 1);
				mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //�ָ���Ծ����ʹ�С
				agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
				hide_tiles();
				mario.set_x(MARIO_X - 100);   //�ָ�����ʼλ�ÿ���ĵط�   ��ʵ���ǲ��ף�����bug
				mario.set_y(MARIO_Y - 200);

				if (mario.get_health() > 0) {   //����0���Ǵ��״̬����;��ӡsummary
					agk::ResetTimer();
					g_state = 5;
				}
			}
		}
	}

	//����Ƿ�ײ������
	for (int i = 0; i < AWARD_NUM; i++) {
		if (check_collision_ways(mario, awards[i]) > 0 && awards[i].get_exist_state() != 0) {   //ײ��û���ԵĽ���   ������agk����ײ��⣬��Χ̫����
			if (awards[i].get_kind() == 1) {   //ײ�����
				awards[i].set_exist_state(0);
				agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);

				add_score(GOLD_SCORE);
			}
			else if (awards[i].get_kind() == 2) {   //ײ��Ģ��
				if (awards[i].get_exist_state() == 1) {
					awards[i].set_exist_state(0);
					agk::SetSpriteVisible(awards[i].get_sprite_index(), HIDE);

					agk::SetSpriteScale(mario.get_sprite_index(), 1.5, 1.5);   //���

					mario.set_jump_dist(agk::GetSpriteHeight(mario.get_sprite_index()) * 3 + 10);  //��Ծ����䳤

					add_score(GOLD_SCORE * 2);
				}
				else if (awards[i].get_exist_state() == -1) {   //ײ������״̬��Ģ������Ϊ���״̬
					awards[i].set_exist_state(1);

					awards[i].set_y(awards[i].get_y() - 5);  //��ֹ��һ��ײ��Ģ���ͱ����
					mario.set_exist_state(2);
				}
			}
		}
	}

	//mario ͷ��ײ����⣬����Ƿ�ײ������ש��******************************
	float y = mario.get_y();
	float sprite_h = agk::GetSpriteHeight(mario.get_sprite_index());
	float sprite_w = agk::GetSpriteWidth(mario.get_sprite_index());

	int tile_r = (int)((y) / TILE_H);
	float x = mario.get_x();
	float offset = agk::GetSpriteX(TILE_SPRITE);
	int tile_c = (int)(x + fabs(offset) + sprite_w / 2) / TILE_W;
	if (tile_map[tile_r][tile_c] == award1) {   //��������ש�飬�л�ͼƬ
		agk::SetSpriteImage(TILE_SPRITE + tile_r * TILE_COLS + tile_c, award0);
	}

	if (mario.get_health() <= 0) { 
		mario.set_exist_state(0);    //����ֵС��0����������mario
		agk::SetSpriteVisible(mario.get_sprite_index(), HIDE);
	}
}

int Game::check_collision_ways(Mario& mario, Sprite& sprite)		 //���mario�ǲȵ����黹��ײ������   �ȵ���2     ײ����1	û��������0
{  
	//mario ʵ�ʵ�λ�ô�Լռ��Χ���м�� 1/3
	//��mario ���Ҹ������������ж�  1/3 ~ 2/3���������ҵ�������������
	float x1 = agk::GetSpriteX(mario.get_sprite_index()) + agk::GetSpriteWidth(mario.get_sprite_index()) * 1 / 3;   //����֮һ����λ��
	float y1 = agk::GetSpriteY(mario.get_sprite_index());
	float x2 = agk::GetSpriteX(mario.get_sprite_index()) + agk::GetSpriteWidth(mario.get_sprite_index()) * 2 / 3;
	float y2 = agk::GetSpriteY(mario.get_sprite_index()) + agk::GetSpriteHeight(mario.get_sprite_index()) * 1 / 2;
	float y3 = agk::GetSpriteY(mario.get_sprite_index()) + agk::GetSpriteHeight(mario.get_sprite_index());   //��� 2 * 3 ������

	float t_x1 = sprite.get_x();
	float t_x2 = sprite.get_x() + agk::GetSpriteWidth(sprite.get_sprite_index());
	float t_y1 = sprite.get_y();
	float t_y2 = sprite.get_y() + agk::GetSpriteHeight(sprite.get_sprite_index());
	
	//if (agk::GetSpriteCollision(mario.get_sprite_index(), sprite.get_sprite_index())) {  //������agk::GetspriteCollision���ж� ��Χ̫����
	if( (x1-t_x1)*(x1-t_x2) < 0&&(y1-t_y1)*(y1-t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y1 - t_y1) * (y1 - t_y2) < 0 || (x1 - t_x1) * (x1 - t_x2) < 0 && (y2 - t_y1) * (y2 - t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y2 - t_y1) * (y2 - t_y2) < 0 || (x1 - t_x1) * (x1 - t_x2) < 0 && (y3 - t_y1) * (y3 - t_y2) < 0 || (x2 - t_x1) * (x2 - t_x2) < 0 && (y3 - t_y1) * (y3 - t_y2) < 0){
		//float mid_x = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) / 2;
		float r = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) * 2 / 3;   //������΢���м俿
		float l = mario.get_x() + agk::GetSpriteWidth(mario.get_sprite_index()) * 1 / 3;

		float l_x = sprite.get_x();
		float r_x = l_x + agk::GetSpriteWidth(sprite.get_sprite_index());

		float m_y = mario.get_y() + agk::GetSpriteHeight(mario.get_sprite_index());
		float s_y = sprite.get_y() + 20;

		if ((r > l_x && r < r_x || l > l_x && l < r_x) && m_y <= s_y) {   //m_y < s_y ��mario �� sprite����
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
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //��ʼ�������
	if (kind == 2) {   //Ģ��������Ϊ����״̬      ��һ����������Ϊ���̬�����������������Ϊ���̬��  ���³�ʼ��Ϊ1���в��Ծ���
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
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //��ʼ�������
	if (kind == 2) {   //Ģ��������Ϊ����״̬      ��һ����������Ϊ���̬�����������������Ϊ���̬��  ���³�ʼ��Ϊ1���в��Ծ���
		exist_state = -1;
	}
	else {
		exist_state = 1;
	}

	set_x(x);
	set_y(y);
	moved_dist = x - agk::GetSpriteX(TILE_SPRITE);

	agk::SetSpriteDepth(get_sprite_index(), 4 * 20 + 1);   //�����������שͷ����

	/*if (kind == 1) {   //��Ҵ�С����             Ϊʲôû�ã�
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
	direction = agk::Random(-5, 5) > 0 ? 1 : -1;   //��ʼ�������
	exist_state = 0;

	set_x(init_x);
	set_y(init_y);
	//moved_dist = init_x - agk::GetSpriteX(TILE_SPRITE);   //������;��ʼ����λ�þͱ���
	moved_dist = init_x;

	this->trigger_x = trigger_x;
	this->trigger_y = trigger_y;

	agk::SetSpriteVisible(get_sprite_index(), HIDE);
}
