#ifndef MY_MACRO
#define MY_MACRO

#define HIDE 0
#define SHOW 1
#define TILE_ROWS 16+1    //tile_map   rows   �������һ�����
#define TILE_COLS 20*4	//tile_map   cols
#define TILE_W 60.0      //tile width and height  60*60
#define TILE_H 60.0
#define SCREEN_W 1200
#define SCREEN_H 960
#define TILE_SPRITE 1000   //tile map�ľ�����ʼ��   1000~1000+16*40
#define MOVE_DIST 5.0
#define MARIO_X 300.0    //mario����ʼ����
#define MARIO_Y 11*TILE_H
#define VICTORY_X 1040    //ʤ��������ж�
#define VICTORY_Y 660
#define MUSHROOM_SCORE 20  //����Ģ���ֵ÷�
#define TURTLE_SCORE 40   //�ڹ�÷�
#define GOLD_SCORE 20
#define ENEMY_NUM 6
#define ENEMY_SPRITE 2   //enemy sprite��ʼ���
#define AWARD_SPRITE 200   //award sprite��ʼ���
#define AWARD_NUM 18
#define TRAP_SPRITE 400    //trap��ʼ���
#define TRAP_NUM 2
#define UNMATCHED_TIME 30

int enemy_pos[ENEMY_NUM][2] = { {1200,600}, {780,600}, {880, 500}, {2400, 400}, {2300, 600}, {3800, 660} };   //enemy ��ʼ����
int award_pos[AWARD_NUM][2] = { {880,600},{800,600},{2460,540},{1920,170},{1990,170},{2060,170},{3130,660},{3160,600},{3190,540},{3230,480},{3260,540},{3290,600},{3320,660},{3780,660},{3840,660},{3960,660},{4020,660},{3900,660} };
//trap��ʼ����Ҫ��������
int trap_pos[TRAP_NUM][4] = { {3230,1000, 3230, 300}, {4500, 600, 3900, 550} };


class Sprite
{
private:
	int sprite_index;
	string img_file;
public:
	Sprite();
	Sprite(int index, string file_name);
	~Sprite();

	int get_sprite_index();
	string get_img_file();
	float get_x();
	float get_y();

	void create_sprite();   //����������Ϊ���ɼ�
	void set_position(float x, float y);
	void set_x(float x);
	void set_y(float y);
};

class Mario : public Sprite
{
private:
	int move_state;   //�ƶ�״̬��  -1���ƶ�     0����ֹ     1����Ծ
	int direction;   //-1�� 1��
	int health;      //����ֵ��ʼ��Ϊ3
	int exist_state;  //����״̬   1�����    0������     2�����
	int current_frame;   //��ǰ��֡��   1~10
	int cnt;     //ͨ������������֡��
	int unmatched;
	float jump_dist;  //��Ծ����
	float pre_y;    //��Ծ֮ǰ��yֵ
	bool armed;   //�Ƿ�������

public:
	Mario();
	~Mario();

	int get_exist_state(); 
	int get_direction();
	int get_current_frame();
	int get_cnt();
	int get_move_state();
	int get_health();
	int get_unmatched();
	float get_pre_y();
	float get_jump_dist();

	void move_left();
	void move_right();
	void jump_left();
	void jump_right();
	void attack();
	void set_direction(int direction);
	void set_currrent_frame(int frame);
	void set_move_state(int state);
	void set_pre_y(float y);
	void set_health(int h);
	void set_exist_state(int state);
	void init_mario();
	void set_jump_dist(float dist);
	void set_unmatched(int time);
};

class Enemy :public Sprite 
{
private:
	int kind;    //����   1��СĢ��
	int direction;   //��ǰ�ƶ�����   ������һֱ���ƶ���
	int exist_state;   //����״̬   1�����    0������    2���������ڹ꣩
	int dead_time;
	float moved_dist;

public:
	Enemy(int index, int kind, string filename);
	~Enemy();

	int get_direction();
	int get_exist_state();
	int get_dead_time();
	int get_kind();
	float get_moved_dist();

	void set_direction(int d);
	void set_exist_state(int state);
	void init_enemy(float x, float y);
	void set_moved_dist(float dist);
	void set_dead_time(int time);
};

class Award : public Sprite
{
private:
	int exist_state;	//����״̬  -1������    0����mario�Ե�   1�����״̬
	int direction;		//һЩ�������ܻ��ƶ�����Ҫ����
	int kind;			//����   1�����   2��Ģ��
	float moved_dist;   //����ڵ�һ��tile���ƶ�����

public:

	Award(int index, int kind, string filename);
	~Award();

	int get_exist_state();
	int get_direction();
	int get_kind();
	float get_moved_dist();

	void init_award(float x, float y);
	void set_exist_state(int state);
	void set_direction(int d);
	void set_moved_dist(float dist);
};

class Trap :public Sprite 
{
private:
	int exist_state;
	int direction;
	int kind;
	float moved_dist;
	float trigger_x;
	float trigger_y;

public:

	Trap(int index, int kind, string filename);
	~Trap();

	int get_exist_state();
	int get_direction();
	int get_kind();
	float get_moved_dist();
	float get_trigger_x();
	float get_trigger_y();

	void set_exist_state(int state);
	void set_direction(int d);
	void set_moved_dist(float dist);
	void init_trap(float init_x, float init_y, float trigger_x, float trigger_y);
};

class Game
{
private:
	int g_state;    //��Ϸ״̬
	int score;		//�÷�
	int tile_map[TILE_ROWS][TILE_COLS];
	//text index
	int info_text_index;   //1
	int rule_text_index;   //2
	int prompt_text_index;   //3
	int game_over_text_index;   // 4
	int summary_text_index;    //5
	//tile map img index
	int brick;    //1
	int pipe;     //2
	int award1;    //3
	int award0;   //4
	int box;      //5
	int castle;   //7
	int cloud;    //8
	int hill;     //9           900W*600H  ��Ҫ15*10��tile�����Խ�ͼƬ��Сһ��
	int sky;      //10
public:
	Game();    //�޲ι��캯��  ��ʼ��
	~Game();

	int get_g_state();
	int get_info_text_index();
	int get_rule_text_index();
	int get_prompt_text_index();
	int get_game_over_text_index();
	int get_summary_text_index();
	int get_score();
	int check_collision_ways(Mario& mario, Sprite& enemy);
	//int get_mario_lu_tile(Mario& mario);      //���mario��Χ���ĸ�����
	int get_sprite_lu_tile(Sprite& sprite);
	//int get_mario_ru_tile(Mario& mario);
	int get_sprite_ru_tile(Sprite& sprite);
	//int get_mario_ld_tile(Mario& mario);
	int get_sprite_ld_tile(Sprite& sprite);
	//int get_mario_rd_tile(Mario& mario);
	int get_sprite_rd_tile(Sprite& sprite);
	//int get_mario_up_tile(Mario& mario);  //���mario��Χ�����������ĸ��е�
	int get_sprite_up_tile(Sprite& sprite, float propotion = 0.5);
	//int get_mario_down_tile(Mario& mario);
	int get_sprite_down_tile(Sprite& sprite, float propotion = 0.5);
	//int get_mario_left_tile(Mario& mario);
	int get_sprite_left_tile(Sprite& sprite, float propotion = 0.5);
	//int get_mario_right_tile(Mario& mario);
	int get_sprite_right_tile(Sprite& sprite, float propotion = 0.5);

	void display_info();
	void hide_info();
	void display_rules();
	void hide_rules();
	void play_game(Mario &mario, Enemy enemys[], Award awards[], Trap traps[]);
	void hide_game(Enemy enemys[], Award awards[], Trap traps[]);
	void load_tile_img();
	void init_tiles();
	void display_tiles();   //ÿ���ظ������Ļ�����˸,���������Ż�����,������init,ÿ������visible������
	void hide_tiles();
	void display_prompt();
	void hide_prompt();
	void display_game_over();
	void hide_game_over();
	void display_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);
	void hide_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);
	void change_entire_tile_x(float offset);   //�ƶ�offset �ľ���
	void move_mario(Mario& mario);   //�ƶ��߼��漰����������tile_map����
	void check_for_collision(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);   //���mario����ײ����
	void init_tile_position();
	void move_enemy(Enemy& enemy);
	void move_enemys(Enemy enemys[]);
	void add_score(int s);
	void move_awards(Award awards[]);
	void move_traps(Trap traps[]);
};

#endif   //MY_MACRO