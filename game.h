#ifndef GAME_H_
#define GAME_H_

#define Lcd_W 				320
#define Lcd_H 				240

#define PLANE_W 			28
#define PLANE_H 			24
#define PLANE_COLOR			0x55D

#define MISSILE_W			6
#define MISSILE_H			6
#define MISSILE_COLOR		0xFBE4

#define BULLET_W			6
#define BULLET_H			6
#define BULLET_COLOR        0xFFFF

#define DIGIT_WIDTH         10
#define DIGIT_HEIGHT        14

#define MAX_MISSILE         20
#define MISSILE_SPEED       5

#define MAX_BULLET          1
#define BULLET_SPEED        5

typedef struct {
    unsigned short x, y;
    unsigned short prev_x, prev_y;
    unsigned short width, height;
    const int **image;
    unsigned char is_used;
} GameObject;

typedef enum {
    GAME_USER_WAIT = 0,
    GAME_READY_TO_PLAY =1,
    GAME_PLAYING = 2,
    GAME_OVER = 3
} GameState;

typedef enum {
    KEY_MINIMUM =0,
    KEY_PLANE_LEFT = 1,
    KEY_PLANE_RIGHT = 2,
    KEY_PLANE_BACK = 3,
    KEY_PLANE_FORWARD = 4,
    KEY_PLANE_BULLET = 5,
    KEY_GAME_RESET = 6,
    KEY_MAX_VALUE =7,
} KeyMap;

void _Delay(int ms);

void Draw_Image(GameObject *obj);
void Clear_Image(GameObject *obj);

void Game_Init(void);

void Game_Missile_Generation(void);
void Game_Bullet_Generation(void);

void Game_Missile_Clear(void);
void Game_Bullet_Clear(void);

void Game_Plane_Move(int dir);
void Game_Missile_Move(void);
void Game_Bullet_Move(void);

int Check_Collision(GameObject* object1, GameObject* object2);

void Draw_LCD(void);

void Draw_BorderLine(unsigned short color);

void Game_BGM_Sound(void);
void Shoot_Bullet_Sound(void);
void Game_Over_Sound(void);

void playSound(unsigned short frequency, unsigned short duration);

void Game_Change_State(char next_state);
void Game_SW1_Set_Next_State(void);
#endif
