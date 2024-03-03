// Includes, namespace and prototypes
#include "template.h"
#include<iostream>
using namespace std;
#include "my_macro.h"
#include "my_function.cpp"    
using namespace AGK;
app App;

extern int enemy_pos[ENEMY_NUM][2];    //外部变量，存储enemy初始坐标
extern int award_pos[AWARD_NUM][2];    //award初始坐标
extern int trap_pos[TRAP_NUM][4];

//create obj
Game game;
Mario mario;

//enemy obj
int enemy_sprite = ENEMY_SPRITE;
//Enemy enemy1(2, 1, "./Mario/monster2.png");
Enemy enemys[ENEMY_NUM] = { Enemy(enemy_sprite++, 1, "./Mario/monster2.png"),
							Enemy(enemy_sprite++, 1, "./Mario/monster2.png"),
							Enemy(enemy_sprite++, 2, "./Mario/turtle.png"),
							Enemy(enemy_sprite++, 1, "./Mario/monster2.png"),
							Enemy(enemy_sprite++, 2, "./Mario/turtle.png"),
							Enemy(enemy_sprite++, 1, "./Mario/monster2.png")};

//award obj
int award_sprite = AWARD_SPRITE;
Award awards[AWARD_NUM] = { Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"), 
							Award(award_sprite++, 2, "./Mario/my_mario/mushroom.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/gold.png"),
							Award(award_sprite++, 1, "./Mario/my_mario/chicken.png")};

//trap obj
int trap_sprite = TRAP_SPRITE;
Trap traps[TRAP_NUM] = { Trap(trap_sprite++, 1, "./Mario/my_mario/kunkun.png"),
						 Trap(trap_sprite++, 2, "./Mario/my_mario/basketball.png")};

// Begin app, called once at the start
void app::Begin( void )
{
	agk::SetVirtualResolution(SCREEN_W, SCREEN_H);
	agk::SetWindowTitle("Super Mario");
	
	//create mario   and   enemy   and  award   and trap
	mario.create_sprite();
	//多个enemy，可以统一create
	//enemy1.create_sprite();
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemys[i].create_sprite();
	}
	for (int i = 0; i < AWARD_NUM; i++) {
		awards[i].create_sprite();
	}
	for (int i = 0; i < TRAP_NUM; i++) {
		traps[i].create_sprite();
	}

	//load tile imgs
	game.load_tile_img();
	//create tile sprite
	game.init_tiles();

	//create text sprite,   set size,  set position
	//info_text
	float text_x, text_y;
	agk::CreateText(game.get_info_text_index(), "Producer:  logxy");
	agk::SetTextSize(game.get_info_text_index(), 40);
	text_x = SCREEN_W / 4;
	text_y = SCREEN_H / 2;
	agk::SetTextPosition(game.get_info_text_index(), text_x, text_y);
	agk::SetTextVisible(game.get_info_text_index(), HIDE);
	//rule_text
	agk::CreateText(game.get_rule_text_index(), "Press -> <- to move right and left\n\nPress space to jump\n");
	agk::SetTextSize(game.get_rule_text_index(), 40);
	text_x = SCREEN_W / 5;
	text_y = SCREEN_H / 4;
	agk::SetTextPosition(game.get_rule_text_index(), text_x, text_y);
	agk::SetTextVisible(game.get_rule_text_index(), HIDE);
	//prompt_text
	agk::CreateText(game.get_prompt_text_index(), "Press enter to continue");
	agk::SetTextSize(game.get_prompt_text_index(), 60);
	agk::SetTextColor(game.get_prompt_text_index(), 255, 255, 0, 255);
	text_x = SCREEN_W / 8;
	text_y = SCREEN_H / 2;
	agk::SetTextPosition(game.get_prompt_text_index(), text_x, text_y);
	agk::SetTextVisible(game.get_prompt_text_index(), HIDE);
	//game over text
	agk::CreateText(game.get_game_over_text_index(), "Game   Over");
	agk::SetTextSize(game.get_game_over_text_index(), 40);
	text_x = SCREEN_W / 4;
	text_y = SCREEN_H / 2;
	agk::SetTextPosition(game.get_game_over_text_index(), text_x, text_y);
	agk::SetTextVisible(game.get_game_over_text_index(), HIDE);
	//summary text需要打印分数，打印时更新text
	agk::CreateText(game.get_summary_text_index(), "");
	agk::SetTextSize(game.get_summary_text_index(), 40);
	text_x = SCREEN_W / 4;
	text_y = SCREEN_H / 2;
	agk::SetTextPosition(game.get_summary_text_index(), text_x, text_y);
	agk::SetTextVisible(game.get_summary_text_index(), HIDE);
}

// Main loop, called every frame
void app::Loop ( void )
{
	switch (game.get_g_state()) {
	case 0:
		game.display_info();
		if (agk::Timer() >= 2) 
			game.hide_info();
		break;

	case 1:
		game.display_rules();
		if (agk::Timer() >= 2)
			game.hide_rules();
		break;

	case 2:    //开始界面
		mario.init_mario();   //对游戏初始化
		game.init_tile_position();   
		game.display_tiles();
		game.display_prompt();
		//init enemy 有多个enemy需要初始化，可以把每个enemy初始坐标存起来
		//enemy1.init_enemy(400, 600);
		for (int i = 0; i < ENEMY_NUM; i++) {
			enemys[i].init_enemy(enemy_pos[i][0], enemy_pos[i][1]);
		}
		//init award
		for (int i = 0; i < AWARD_NUM; i++) {
			awards[i].init_award(award_pos[i][0], award_pos[i][1]);
		}
		//init trap
		for (int i = 0; i < TRAP_NUM; i++) {
			traps[i].init_trap(trap_pos[i][0], trap_pos[i][1], trap_pos[i][2], trap_pos[i][3]);
		}

		if (agk::GetRawKeyPressed(AGK_KEY_ENTER)) {
			game.hide_prompt();
			//显示enemy
			//agk::SetSpriteVisible(enemy1.get_sprite_index(), SHOW);    //也可以放在play_game里显示，但我觉得这里比较好，mario其实也放在这显示比较好，懒得改暂时
			for (int i = 0; i < ENEMY_NUM; i++) {
				agk::SetSpriteVisible(enemys[i].get_sprite_index(), SHOW);
			}
			//显示award
			for (int i = 0; i < AWARD_NUM; i++) {
				agk::SetSpriteVisible(awards[i].get_sprite_index(), SHOW);
			}
			//显示trap  其实不用显示，只是用来测试
			/*for (int i = 0; i < TRAP_NUM; i++) {
				agk::SetSpriteVisible(traps[i].get_sprite_index(), SHOW);
			}*/
		}
		break;

	case 3:    //play 状态
		game.play_game(mario, enemys, awards, traps);  //玩游戏
		if(mario.get_exist_state() == 0)   //死亡状态结束游戏
			game.hide_game(enemys, awards, traps);	//游戏结束  
		break;

	case 4:    //game over输出相应信息
		game.display_game_over();
		if (agk::Timer() >= 2)
			game.hide_game_over();
		break;

	case 5:    //summary
		game.display_summary(mario, enemys, awards, traps);
		if (agk::Timer() >= 2)
			game.hide_summary(mario, enemys, awards, traps);    
		break;

	default:   //其他状态退出游戏
		break;
	}

	agk::Sync();
}

// Called when the app ends
void app::End ( void )
{
}
