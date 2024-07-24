#include "device_driver.h"
#include "lcd.h"
#include "game.h"

const unsigned char Plane[168][2] = {
   {3 , 9}, {3 , 10}, {3 , 11}, {4 , 9}, {4 , 10}, {4 , 11}, {4 , 12}, {5 , 9},
   {5 , 10}, {5 , 11}, {5 , 12}, {5 , 13}, {6 , 10}, {6 , 11}, {6 , 12}, {6 , 13},
   {6 , 14}, {7 , 10}, {7 , 11}, {7 , 12}, {7 , 13}, {7 , 14}, {8 , 2}, {8 , 3},
   {8 , 4}, {8 , 11}, {8 , 12}, {8 , 13}, {8 , 14}, {8 , 15}, {9 , 2}, {9 , 3},
   {9 , 4}, {9 , 5}, {9 , 12}, {9 , 13}, {9 , 14}, {9 , 15}, {9 , 16}, {10 , 3},
   {10 , 4}, {10 , 5}, {10 , 6}, {10 , 7}, {10 , 8}, {10 , 9}, {10 , 10}, {10 , 11},
   {10 , 12}, {10 , 13}, {10 , 14}, {10 , 15}, {10 , 16}, {10 , 17}, {10 , 18}, {10 , 19},
   {10 , 20}, {10 , 21}, {10 , 22}, {10 , 23}, {10 , 24}, {11 , 3}, {11 , 4}, {11 , 5},
   {11 , 6}, {11 , 7}, {11 , 8}, {11 , 9}, {11 , 10}, {11 , 11}, {11 , 12}, {11 , 13},
   {11 , 14}, {11 , 15}, {11 , 16}, {11 , 17}, {11 , 18}, {11 , 19}, {11 , 20}, {11 , 21},
   {11 , 22}, {11 , 23}, {11 , 24}, {11 , 25}, {12 , 3}, {12 , 4}, {12 , 5}, {12 , 6},
   {12 , 7}, {12 , 8}, {12 , 9}, {12 , 10}, {12 , 11}, {12 , 12}, {12 , 13}, {12 , 14},
   {12 , 15}, {12 , 16}, {12 , 17}, {12 , 18}, {12 , 19}, {12 , 20}, {12 , 21}, {12 , 22},
   {12 , 23}, {12 , 24}, {12 , 25}, {13 , 3}, {13 , 4}, {13 , 5}, {13 , 6}, {13 , 7},
   {13 , 8}, {13 , 9}, {13 , 10}, {13 , 11}, {13 , 12}, {13 , 13}, {13 , 14}, {13 , 15},
   {13 , 16}, {13 , 17}, {13 , 18}, {13 , 19}, {13 , 20}, {13 , 21}, {13 , 22}, {13 , 23},
   {13 , 24}, {14 , 2}, {14 , 3}, {14 , 4}, {14 , 5}, {14 , 12}, {14 , 13}, {14 , 14},
   {14 , 15}, {14 , 16}, {15 , 2}, {15 , 3}, {15 , 4}, {15 , 11}, {15 , 12}, {15 , 13},
   {15 , 14}, {15 , 15}, {16 , 10}, {16 , 11}, {16 , 12}, {16 , 13}, {16 , 14}, {17 , 10},
   {17 , 11}, {17 , 12}, {17 , 13}, {17 , 14}, {18 , 9}, {18 , 10}, {18 , 11}, {18 , 12},
   {18 , 13}, {19 , 9}, {19 , 10}, {19 , 11}, {19 , 12}, {20 , 9}, {20 , 10}, {20 , 11}
};


const int Missile[MISSILE_W*MISSILE_H] = {
    0x0000, 0x0000, 0xFBE4, 0xFBE4, 0x0000, 0x0000, 
    0x0000, 0xFBE4, 0xFBE4, 0xFBE4, 0xFBE4, 0x0000, 
    0xFBE4, 0xFBE4, 0x0000, 0x0000, 0xFBE4, 0xFBE4, 
    0xFBE4, 0xFBE4, 0x0000, 0x0000, 0xFBE4, 0xFBE4, 
    0x0000, 0xFBE4, 0xFBE4, 0xFBE4, 0xFBE4, 0x0000, 
    0x0000, 0x0000, 0xFBE4, 0xFBE4, 0x0000, 0x0000, 
};

const int Bullet[BULLET_W*BULLET_H] = {
    0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 
    0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 
    0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000 
};

GameObject plane;
GameObject bullet[MAX_BULLET];
GameObject missile[MAX_MISSILE];

int score;
int game_state_flag;
int plane_move_flag;

void _Delay(int ms)
{
	volatile int i;
	for(i=0;i<(0x1000*ms);i++);
}

void Draw_Plane() {
    int x, y;

    int i=0;

    for(;i<168;i++){
        x = plane.x+Plane[i][1];
        y = plane.y+Plane[i][0];

        Lcd_Set_Windows(x, y, x, y);
        Lcd_Write_Data_16Bit(PLANE_COLOR);
    }
}

void Draw_Image(GameObject *obj) {
    Lcd_Set_Windows(obj->x, obj->y, obj->x + obj->width - 1, obj->y + obj->height - 1);

    int i=0, j=0;
 
    for (;i<obj->height;i++) {
        j = 0;
        for (;j<obj->width;j++){
            Lcd_Put_Pixel(obj->x+j, obj->y+i, obj->image[obj->width*i+j]);
        }
    }
}
 
void Clear_Image(GameObject *obj) {
    Lcd_Draw_Box(obj->prev_x, obj->prev_y, obj->width, obj->height, BLACK);
}

void Game_Init(void)
{
    plane.x = 20;
    plane.y = Lcd_H/2 - PLANE_H/2;
    plane.prev_x = 0;
    plane.prev_y = 0;
	plane.width = PLANE_W;
	plane.height = PLANE_H;
	plane.is_used = 1;

	score = 0;

    if (game_state_flag != GAME_PLAYING)
        game_state_flag = GAME_START;
    
    plane_move_flag = 1;

    Game_Missile_Clear();
    Game_Bullet_Clear();

	Lcd_Clr_Screen();
}

void Game_Missile_Clear(void)
{
    volatile int i;
    for(i = 0; i < MAX_MISSILE; i++)
    {
        missile[i].width = MISSILE_W;
        missile[i].height = MISSILE_H;
        missile[i].image = Missile;
        missile[i].is_used = 0;
    }
}
void Game_Missile_Generation(void)
{
    int i;
    for(i = 0; i < MAX_MISSILE; i++)
    {
        if(missile[i].is_used == 0)
        {
            missile[i].x = rand() % 40 + 280;
            missile[i].y = rand() % (Lcd_H - 18) + 9; //PLANE_H/2 - MISSILE_H/2 = 9
            missile[i].prev_x = 0;
            missile[i].prev_y = 0;
            missile[i].is_used = 1;
        }
    }
}

void Game_Bullet_Clear(void)
{
    volatile int i;
    for(i = 0; i < MAX_BULLET; i++)
    {
        bullet[i].width = BULLET_W;
        bullet[i].height = BULLET_H;
        bullet[i].image = Bullet;
        bullet[i].is_used = 0;
    }
}
void Game_Bullet_Generation(void)
{
    volatile int i;
    for(i = 0; i < MAX_BULLET; i++)
    {
        if(bullet[i].is_used == 0)
        {
            bullet[i].x = plane.x + PLANE_W;
            bullet[i].y = plane.y + PLANE_H/2 - BULLET_H/2;
            bullet[i].prev_x = 0;
            bullet[i].prev_y = 0;
            bullet[i].is_used = 1;
            break;
        }
    }
}

void Game_Plane_Move(int dir)
{
    // Uart_Printf("Dir %d", dir);
    int dx = 10;
	int dy = 10;

    plane_move_flag = 1;

    plane.prev_x = plane.x;
    plane.prev_y = plane.y;

	switch(dir){ //1-left, 2-right, 3-down, 4-up
		case 1:
			if(plane.y >= 15) plane.y -= dy;
			break;
		case 2:
			if(plane.y <= 200) plane.y += dy;
			break;
		case 3:
			if(plane.x >= 15) plane.x -= dx;
			break;
		case 4:
			if(plane.x <= 300) plane.x += dx;
			break;
	}
}

void Game_Missile_Move(void)
{
    int i;
    int dx = -MISSILE_SPEED;
    for(i = 0; i < MAX_MISSILE; i++)
    {
        if (missile[i].is_used == 0)
            continue;
        missile[i].prev_x = missile[i].x;
        missile[i].prev_y = missile[i].y;

        missile[i].x += dx;

        if(missile[i].x <= 0)
        {
            Clear_Image(&missile[i]);
            missile[i].x = 0;
            missile[i].y = 0;
            missile[i].is_used = 0;
        }
    }
}

void Game_Bullet_Move(void)
{
    int i;
    int dx = BULLET_SPEED;
    for(i = 0; i < MAX_BULLET; i++)
    {
        if(bullet[i].is_used == 0)
            continue;

        bullet[i].prev_x = bullet[i].x;
        bullet[i].prev_y = bullet[i].y;
        
        bullet[i].x += dx;

        if(bullet[i].x >= Lcd_W - bullet[i].width)
        {
            Clear_Image(&bullet[i]);
            bullet[i].x = 0;
            bullet[i].y = 0;
            bullet[i].is_used = 0;
        }
    }
}

int Check_Collision(GameObject* object1, GameObject* object2) {
    int left1 = object1->x;
    int right1 = object1->x + object1->width;
    int top1 = object1->y;
    int bottom1 = object1->y + object1->height;
    
    int left2 = object2->x;
    int right2 = object2->x + object2->width;
    int top2 = object2->y;
    int bottom2 = object2->y + object2->height;
    
    if (right1 >= left2 && left1 <= right2 && bottom1 >= top2 && top1 <= bottom2) {
        return 1;
    }
    return 0;
}

void Draw_LCD(void)
{
    int index, count;
    volatile int i, j;

	if(game_state_flag == GAME_START){
		Lcd_Draw_Back_Color(BLUE);
		_Delay(300);
		Lcd_Clr_Screen();
		_Delay(300);
	}

	else if(game_state_flag==GAME_PLAYING){
        Game_Missile_Move();
        Game_Bullet_Move();

        // 미사일과 비행기 충돌 여부
        for(i = 0; i < MAX_MISSILE; i++)
        {
            if (missile[i].is_used == 1)
            {
                if(Check_Collision(&plane, &missile[i]))
                {
                    game_state_flag = GAME_OVER;
                    return;
                }
                    
            }
        }

        // 미사일과 총알 충돌 여부
        for(i = 0; i < MAX_MISSILE; i++)
        {
            if(missile[i].is_used == 0)
                continue;
            for(j = 0; j < MAX_BULLET; j++)
            {
                if(bullet[j].is_used == 0)
                    continue;
                if (Check_Collision(&missile[i], &bullet[j]))
                {
                    missile[i].prev_x = missile[i].x;
                    missile[i].prev_y = missile[i].y;
                    Clear_Image(&missile[i]);
                    bullet[i].prev_x = bullet[i].x;
                    bullet[i].prev_y = bullet[i].y;
                    Clear_Image(&bullet[j]);

                    missile[i].x = 0;
                    missile[i].y = 0;
                    missile[i].is_used = 0;

                    bullet[j].x = 0;
                    bullet[j].y = 0;
                    bullet[j].is_used = 0;
                }
            }
        }

        // 비행기 draw
        if(plane_move_flag) {
            Clear_Image(&plane);
            Draw_Plane();
			plane_move_flag = 0;
		}

        // missile draw
        for(i = 0; i < MAX_MISSILE; i++)
        {
            if(missile[i].is_used == 1)
            {
                Clear_Image(&missile[i]);
                Draw_Image(&missile[i]);
            }
        }

       // bullet draw
        for(i = 0; i < MAX_BULLET; i++)
        {
            if(bullet[i].is_used == 1)
            {
                Clear_Image(&bullet[i]);
                Draw_Image(&bullet[i]);
            }
        }
	}

	else if(game_state_flag==GAME_OVER){
		Lcd_Draw_Back_Color(RED);
		_Delay(300);
		Lcd_Clr_Screen();
		_Delay(300);
	}
}