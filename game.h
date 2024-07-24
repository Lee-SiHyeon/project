#ifndef GAME_H_
#define GAME_H_

#define Lcd_W 				320
#define Lcd_H 				240

#define PLANE_W 			28
#define PLANE_H 			24
#define PLANE_SIZE			208

#define MISSILE_W			8
#define MISSILE_H			8
#define MISSILE_SIZE		20

#define BULLET_W			8
#define BULLET_H			8
#define BULLET_SIZE	    	20

#define DIGIT_WIDTH         10
#define DIGIT_HEIGHT        14

#define MAX_BULLET          10

typedef struct {
    int x, y;
    int prev_x, prev_y;
    int width, height;
    const int **image;
    int imageSize;
    char is_used;
} GameObject;

typedef enum {
    GAME_START = 0,
    GAME_PLAYING = 1,
    GAME_OVER = 2
} GameState;

typedef enum {
    KEY_PLANE_FORWARD = 1,
    KEY_PLANE_BACK = 2,
    KEY_PLANE_LEFT = 3,
    KEY_PLANE_RIGHT = 4,
    KEY_PLANE_MISSILE = 5,
    KEY_GAME_RESET = 6
} KeyMap;

void _Delay(int ms);

void Draw_Image(GameObject *obj);
void Clear_Image(GameObject *obj);

void Game_Init(void);

void Game_Missile_Generation(void);
void Game_Bullet_Generation(void);

void Game_Plane_Move(int dir);
void Game_Missile_Move(void);
void Game_Bullet_Move(void);

int Check_Collision(GameObject* object1, GameObject* object2);

void Draw_LCD(void);

void Play_BGM(void);

#endif
