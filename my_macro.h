#ifndef MY_MACRO
#define MY_MACRO

#define HIDE 0
#define SHOW 1
#define TILE_ROWS 16+1    //tile_map   rows   最下面垫一层天空
#define TILE_COLS 20*4	//tile_map   cols
#define TILE_W 60.0      //tile width and height  60*60
#define TILE_H 60.0
#define SCREEN_W 1200
#define SCREEN_H 960
#define TILE_SPRITE 1000   //tile map的精灵起始号   1000~1000+16*40
#define MOVE_DIST 5.0
#define MARIO_X 300.0    //mario的起始坐标
#define MARIO_Y 11*TILE_H
#define VICTORY_X 1040    //胜利区域的判定
#define VICTORY_Y 660
#define MUSHROOM_SCORE 20  //踩死蘑菇怪得分
#define TURTLE_SCORE 40   //乌龟得分
#define GOLD_SCORE 20
#define ENEMY_NUM 6
#define ENEMY_SPRITE 2   //enemy sprite起始序号
#define AWARD_SPRITE 200   //award sprite起始序号
#define AWARD_NUM 18
#define TRAP_SPRITE 400    //trap起始序号
#define TRAP_NUM 2
#define UNMATCHED_TIME 30

int enemy_pos[ENEMY_NUM][2] = { {1200,600}, {780,600}, {880, 500}, {2400, 400}, {2300, 600}, {3800, 660} };   //enemy 初始坐标
int award_pos[AWARD_NUM][2] = { {880,600},{800,600},{2460,540},{1920,170},{1990,170},{2060,170},{3130,660},{3160,600},{3190,540},{3230,480},{3260,540},{3290,600},{3320,660},{3780,660},{3840,660},{3960,660},{4020,660},{3900,660} };
//trap初始化需要两组坐标
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

	void create_sprite();   //创建后设置为不可见
	void set_position(float x, float y);
	void set_x(float x);
	void set_y(float y);
};

class Mario : public Sprite
{
private:
	int move_state;   //移动状态：  -1：移动     0：静止     1：跳跃
	int direction;   //-1左， 1右
	int health;      //生命值初始化为3
	int exist_state;  //存在状态   1：存活    0：死亡     2：变大
	int current_frame;   //当前的帧数   1~10
	int cnt;     //通过计数简单设置帧率
	int unmatched;
	float jump_dist;  //跳跃距离
	float pre_y;    //跳跃之前的y值
	bool armed;   //是否有武器

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
	int kind;    //种类   1：小蘑菇
	int direction;   //当前移动方向   怪物是一直在移动的
	int exist_state;   //存在状态   1：存活    0：死亡    2：其他（乌龟）
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
	int exist_state;	//存在状态  -1：隐藏    0：被mario吃掉   1：存活状态
	int direction;		//一些奖励可能会移动，需要方向
	int kind;			//种类   1：金币   2：蘑菇
	float moved_dist;   //相对于第一个tile的移动距离

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
	int g_state;    //游戏状态
	int score;		//得分
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
	int hill;     //9           900W*600H  需要15*10个tile，可以将图片调小一点
	int sky;      //10
public:
	Game();    //无参构造函数  初始化
	~Game();

	int get_g_state();
	int get_info_text_index();
	int get_rule_text_index();
	int get_prompt_text_index();
	int get_game_over_text_index();
	int get_summary_text_index();
	int get_score();
	int check_collision_ways(Mario& mario, Sprite& enemy);
	//int get_mario_lu_tile(Mario& mario);      //检测mario包围盒四个顶点
	int get_sprite_lu_tile(Sprite& sprite);
	//int get_mario_ru_tile(Mario& mario);
	int get_sprite_ru_tile(Sprite& sprite);
	//int get_mario_ld_tile(Mario& mario);
	int get_sprite_ld_tile(Sprite& sprite);
	//int get_mario_rd_tile(Mario& mario);
	int get_sprite_rd_tile(Sprite& sprite);
	//int get_mario_up_tile(Mario& mario);  //检测mario包围盒上下左右四个中点
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
	void display_tiles();   //每次重复创建的话会闪烁,并且索引号会用完,所以先init,每次设置visible就行了
	void hide_tiles();
	void display_prompt();
	void hide_prompt();
	void display_game_over();
	void hide_game_over();
	void display_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);
	void hide_summary(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);
	void change_entire_tile_x(float offset);   //移动offset 的距离
	void move_mario(Mario& mario);   //移动逻辑涉及马里奥坐标和tile_map坐标
	void check_for_collision(Mario& mario, Enemy enemys[], Award awards[], Trap traps[]);   //检测mario的碰撞物体
	void init_tile_position();
	void move_enemy(Enemy& enemy);
	void move_enemys(Enemy enemys[]);
	void add_score(int s);
	void move_awards(Award awards[]);
	void move_traps(Trap traps[]);
};

#endif   //MY_MACRO