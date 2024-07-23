// /*
//  * lcd_test.c
//  *
//  *  Created on: 2024. 7. 16.
//  *      Author: yeeun99.kim
//  */

// #include "device_driver.h"
// #include "lcd.h"
// #include "lcd_test.h"
// #include "queue.h"

// #define Lcd_W 				320
// #define Lcd_H 				240

// #define PLANE_W 			30
// #define PLANE_H 			30
// #define PLANE_SIZE			208

// #define MISSILE_W			8
// #define MISSILE_H			8
// #define MISSILE_SIZE		20

// #define DIGIT_WIDTH 10
// #define DIGIT_HEIGHT 14

// #define MAX_BULLET 10

// GameObject plane;
// GameObject bullet[MAX_BULLET];
// GameObject missile[10];

// int score;
// int game_level;
// int game_state_flag;
// int plane_move_flag;

// int level_delay[3] = {500, 300, 120};
// int level_px_move[3] = {1, 3, 5};
// int level_score[3] = {10, 30, 50};
// int level_max_missiles[3] = {5, 8, 10};

// extern volatile int key_value;

// const int Missile[MISSILE_SIZE][3] = {
// 	{1, 3, 0xFBE4}, {1, 4, 0xFBE4}, {2, 2, 0xFBE4}, {2, 3, 0xFBE4}, {2, 4, 0xFBE4}, {2, 5, 0xFBE4}, {3, 1, 0xFBE4}, 
// 	{3, 2, 0xFBE4}, {3, 5, 0xFBE4}, {3, 6, 0xFBE4}, {4, 1, 0xFBE4}, {4, 2, 0xFBE4}, {4, 5, 0xFBE4}, {4, 6, 0xFBE4}, 
// 	{5, 2, 0xFBE4}, {5, 3, 0xFBE4}, {5, 4, 0xFBE4}, {5, 5, 0xFBE4}, {6, 3, 0xFBE4}, {6, 4, 0xFBE4} 
// };

// const int Plane[PLANE_SIZE][3] = {
//   	{4, 9, 0xB73D}, {4, 10, 0x5E3C}, {4, 11, 0x5E3D}, {4, 12, 0xB75E}, {5, 9, 0xD79E}, {5, 10, 0xD5C}, {5, 11, 0x55E}, {5, 12, 0x259C}, {5, 13, 0xE7BF}, 
// 	{6, 10, 0x769D}, {6, 11, 0x57D}, {6, 12, 0x55D}, {6, 13, 0x5E1D}, {7, 10, 0xDFBF}, {7, 11, 0x159D}, {7, 12, 0x5BE}, {7, 13, 0x55D}, {7, 14, 0x96FE}, 
// 	{8, 11, 0x767D}, {8, 12, 0x55E}, {8, 13, 0x59D}, {8, 14, 0x157D}, {8, 15, 0xCF9F}, {9, 11, 0xDFBF}, {9, 12, 0x159D}, {9, 13, 0x59E}, {9, 14, 0x57D}, 
// 	{9, 15, 0x45FD}, {10, 2, 0xC73D}, {10, 3, 0x4DDB}, {10, 4, 0x8EDD}, {10, 12, 0x769E}, {10, 13, 0x53D}, {10, 14, 0x5BE}, {10, 15, 0x55D}, {10, 16, 0x7E9E}, 
// 	{11, 2, 0xD7BF}, {11, 3, 0xD5D}, {11, 4, 0x53D}, {11, 5, 0x96DD}, {11, 12, 0xD7BF}, {11, 13, 0x157C}, {11, 14, 0x57E}, {11, 15, 0x59E}, {11, 16, 0x57D}, 
// 	{11, 17, 0xBF5F}, {12, 3, 0x667D}, {12, 4, 0x55E}, {12, 5, 0xD7D}, {12, 6, 0xA73E}, {12, 13, 0x7EDE}, {12, 14, 0x55D}, {12, 15, 0xD9D}, {12, 16, 0x59D}, 
// 	{12, 17, 0x2DBD}, {12, 18, 0xEFDF}, {13, 3, 0xBF5E}, {13, 4, 0xD5D}, {13, 5, 0x59E}, {13, 6, 0x157D}, {13, 7, 0x6E5D}, {13, 8, 0x767D}, {13, 9, 0x765C}, 
// 	{13, 10, 0x6E7C}, {13, 11, 0x6E7C}, {13, 12, 0x7E7C}, {13, 13, 0x563D}, {13, 14, 0x57E}, {13, 15, 0x59E}, {13, 16, 0x57F}, {13, 17, 0x55D}, {13, 18, 0x45FD}, 
// 	{13, 19, 0x767C}, {13, 20, 0x765D}, {13, 21, 0x765D}, {13, 22, 0x765D}, {13, 23, 0x765D}, {13, 24, 0x767D}, {13, 25, 0x8EBD}, {13, 26, 0xD79E}, {14, 4, 0x3DFD}, 
// 	{14, 5, 0x57D}, {14, 6, 0x59E}, {14, 7, 0x55E}, {14, 8, 0x55D}, {14, 9, 0x55D}, {14, 10, 0x55D}, {14, 11, 0x55E}, {14, 12, 0x55D}, {14, 13, 0x57E}, {14, 14, 0x57D}, 
// 	{14, 15, 0x59D}, {14, 16, 0x59D}, {14, 17, 0x59E}, {14, 18, 0x57D}, {14, 19, 0x55E}, {14, 20, 0x55D}, {14, 21, 0x55D}, {14, 22, 0x55D}, {14, 23, 0x55D}, {14, 24, 0x55D}, 
// 	{14, 25, 0x57D}, {14, 26, 0x155C}, {14, 27, 0xAF1E}, {15, 4, 0x3DFD}, {15, 5, 0x57D}, {15, 6, 0x59E}, {15, 7, 0x55E}, {15, 8, 0x55D}, {15, 9, 0x55D}, {15, 10, 0x55D}, 
// 	{15, 11, 0x55E}, {15, 12, 0x55D}, {15, 13, 0x57E}, {15, 14, 0x57D}, {15, 15, 0x59D}, {15, 16, 0x59D}, {15, 17, 0x59E}, {15, 18, 0x57D}, {15, 19, 0x55E}, {15, 20, 0x55D}, 
// 	{15, 21, 0x55D}, {15, 22, 0x55D}, {15, 23, 0x55D}, {15, 24, 0x55D}, {15, 25, 0x57D}, {15, 26, 0xD3C}, {15, 27, 0xAF1E}, {16, 3, 0xBF5F}, {16, 4, 0xD5D}, {16, 5, 0x59E}, 
// 	{16, 6, 0x157D}, {16, 7, 0x6E5D}, {16, 8, 0x767D}, {16, 9, 0x765C}, {16, 10, 0x6E7C}, {16, 11, 0x6E7C}, {16, 12, 0x7E7C}, {16, 13, 0x563D}, {16, 14, 0x57E}, {16, 15, 0x59E}, 
// 	{16, 16, 0x57F}, {16, 17, 0x55D}, {16, 18, 0x45FD}, {16, 19, 0x767C}, {16, 20, 0x765D}, {16, 21, 0x765D}, {16, 22, 0x765D}, {16, 23, 0x765D}, {16, 24, 0x767D}, {16, 25, 0x8EBD}, 
// 	{16, 26, 0xD79E}, {17, 3, 0x667D}, {17, 4, 0x55E}, {17, 5, 0xD5D}, {17, 6, 0xA73E}, {17, 13, 0x7EDE}, {17, 14, 0x55D}, {17, 15, 0xD9D}, {17, 16, 0x59D}, {17, 17, 0x2DBC}, 
// 	{17, 18, 0xEFDF}, {18, 2, 0xD7BF}, {18, 3, 0xD5D}, {18, 4, 0x53D}, {18, 5, 0x96DD}, {18, 12, 0xD7BF}, {18, 13, 0x157C}, {18, 14, 0x57E}, {18, 15, 0x59E}, {18, 16, 0x57D}, 
// 	{18, 17, 0xBF5F}, {19, 2, 0xC73D}, {19, 3, 0x4DDB}, {19, 4, 0x8EDD}, {19, 12, 0x769E}, {19, 13, 0x53D}, {19, 14, 0x5BE}, {19, 15, 0x55D}, {19, 16, 0x7E9E}, {20, 11, 0xDFBF}, 
// 	{20, 12, 0x159D}, {20, 13, 0x59E}, {20, 14, 0x57D}, {20, 15, 0x45FD}, {21, 11, 0x767D}, {21, 12, 0x55E}, {21, 13, 0x59D}, {21, 14, 0x157D}, {21, 15, 0xCF9F}, {22, 10, 0xDFBF}, 
// 	{22, 11, 0x159D}, {22, 12, 0x5BE}, {22, 13, 0x55D}, {22, 14, 0x96FE}, {23, 10, 0x769D}, {23, 11, 0x57D}, {23, 12, 0x55D}, {23, 13, 0x5E1D}, {24, 9, 0xD79E}, {24, 10, 0xD5C}, 
// 	{24, 11, 0x55E}, {24, 12, 0x259C}, {24, 13, 0xE7BF}, {25, 9, 0xB73D}, {25, 10, 0x5E3C}, {25, 11, 0x5E3D}, {25, 12, 0xB75E}
// };

// const int numberImages[10][10 * 14] = {
//     // 0
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 1
//     {
//         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff
//     },
//     // 2
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 3
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 4
//     {
//         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff,
//         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 5
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 6
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 7
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 8
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     },
//     // 9
//     {
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
//         0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
//         0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
//     }
// };

// static void _Delay(int ms)
// {
// 	volatile int i;
// 	for(i=0;i<(0x1000*ms);i++);
// }
// void Draw_Image(GameObject *obj, int size) {
// 	Lcd_Set_Windows(obj->x, obj->y, obj->x + obj->width - 1, obj->y + obj->height - 1);

// 	int i=0, j=0;

// 	for (;i<size;i++) {
// 		Lcd_Put_Pixel(obj->image[i][0], obj->image[i][1], obj->image[i][2]);
// 	}
// }

// void Clear_Image(int x, int y, int w, int h, unsigned short Color) {
// 	Lcd_Set_Windows(x, y, x + w - 1, y + h - 1);

// 	int i=0, j=0;

// 	for (;i < w; i++) {
// 		j = 0;
// 		for (;j<h;j++){
// 			Lcd_Put_Pixel(x+i, y+j, Color);
// 		}
// 	}
// }

// void Game_Init(void)
// {
// 	Lcd_Clr_Screen();

//     plane.x = 20;
//     plane.y = Lcd_H/2 - PLANE_H/2;
//     plane.prev_x = 0;
//     plane.prev_y = 0;
// 	plane.width = PLANE_W;
// 	plane.height = PLANE_H;
// 	plane.image = Plane;
//     plane.is_used = 1;

// 	score = 0;
// 	game_level = 0;
// 	game_state_flag = GAME_PLAYING;
//     plane_move_flag = 1;

//     Game_Missile_Generation();
// }


// void Game_Missile_Generation(void)
// {
//     int i;
//     for(i = 0; i < level_max_missiles[game_level]; i++)
//     {
//         if(missile[i].is_used == 0)
//         {
//             missile[i].x = 320;
//             missile[i].y = rand() % (Lcd_H - 10);
//             missile[i].width = MISSILE_W;
//             missile[i].height = MISSILE_H;
//             missile[i].image = Missile;
//             missile[i].is_used = 1;
//         }
//     }
// }

// void Generate_Bullet(void)
// {
//     volatile int i;
//     for(i = 0; i < 10; i++)
//     {
//         if(bullet[i].is_used == 0)
//         {
//             bullet[i].x = plane.x;
//             bullet[i].y = plane.y;
//             bullet[i].width = MISSILE_W;
//             bullet[i].height = MISSILE_H;
//             bullet[i].image = Missile;
//             bullet[i].is_used = 1;
//             break;
//         }
//     }
// }

// void Game_Plane_Move(int dir)
// {
//     plane_move_flag = 1;

// 	int dx = 10;
// 	int dy = 10;

// 	switch(dir){ //1-left, 2-right, 3-down, 4-up
// 		case 1:
// 			if(plane.y >= 15) plane.y -= dy;
// 			break;
// 		case 2:
// 			if(plane.y <= 200) plane.y += dy;
// 			break;
// 		case 3:
// 			if(plane.x >= 15) plane.x -= dx;
// 			break;
// 		case 4:
// 			if(plane.x <= 300) plane.x += dx;
// 			break;
// 	}
// }

// void Game_Missile_Move(void)
// {
//     int i;
//     int dx = -level_px_move[game_level];
//     for(i = 0; i < level_max_missiles[game_level]; i++)
//     {
//         if (missile[i].is_used == 0)
//             continue;
//         missile[i].prev_x = missile[i].x;
//         missile[i].prev_y = missile[i].y;

//         missile[i].x += dx;

//         if(missile[i].x <= 0)
//         {
//             missile[i].x = 0;
//             missile[i].y = 0;
//             missile[i].prev_x = 0;
//             missile[i].prev_y = 0;
//             missile[i].is_used = 0;
//         }
//     }
// }

// void Game_Bullet_Move(void)
// {
//     int i;
//     int dx = level_px_move[game_level];
//     for(i = 0; i < MAX_BULLET; i++)
//     {
//         if(bullet[i].is_used == 0)
//             continue;

//         bullet[i].prev_x = bullet[i].x;
//         bullet[i].prev_y = bullet[i].y;
        
//         bullet[i].x += dx;

//         if(bullet[i].x >= Lcd_W - bullet[i].width)
//         {
//             bullet[i].x = 0;
//             bullet[i].y = 0;
//             bullet[i].prev_x = 0;
//             bullet[i].prev_y = 0;
//             bullet[i].is_used = 0;
//         }
//     }
// }

// int Check_Collision(GameObject* object1, GameObject* object2) {
//     int left1 = object1->x;
//     int right1 = object1->x + object1->width;
//     int top1 = object1->y;
//     int bottom1 = object1->y + object1->height;
    
//     int left2 = object2->x;
//     int right2 = object2->x + object2->width;
//     int top2 = object2->y;
//     int bottom2 = object2->y + object2->height;
    
//     if (right1 >= left2 && left1 <= right2 && bottom1 >= top2 && top1 <= bottom2) {
//         return 1;
//     }
//     return 0;
// }

// void Draw_LCD(void) // 미사일, 총알, 비행기 그림(draw queue)
// {
// 	int index, count, i, j;

// 	if(game_state_flag==GAME_START){
// 		Lcd_Draw_Back_Color(BLUE);
// 		_Delay(300);
// 		Lcd_Clr_Screen();
// 		_Delay(300);
// 	}

// 	else if(game_state_flag==GAME_PLAYING){
//         Game_Missile_Move();
//         // 미사일과 비행기 충돌 여부
//         for(i = 0; i < level_max_missiles[game_level]; i++)
//         {
//             if (missile[i].is_used)
//             {
//                 if(Check_Collision(&plane, &missile[i]))
//                 {
//                     ;// TODO  :게임 종료
//                 	Uart_Printf("plane and missile Collision \n");
//                 }
                    
//             }
//         }

//         Game_Bullet_Move();
//         // 미사일과 총알 충돌 여부
//         for(i = 0; i < level_max_missiles[game_level]; i++)
//         {
//             if(missile[i].is_used == 0)
//                 continue;
//             for(j = 0; j < MAX_BULLET; j++)
//             {
//                 if(bullet[j].is_used == 0)
//                     continue;
//                 if (Check_Collision(&missile[i], &bullet[j]))
//                 {
//                     missile[i].x = 0;
//                     missile[i].y = 0;
//                     missile[i].prev_x = 0;
//                     missile[i].prev_y = 0;
//                     missile[i].is_used = 0;
                    
//                     bullet[i].x = 0;
//                     bullet[i].y = 0;
//                     bullet[i].prev_x = 0;
//                     bullet[i].prev_y = 0;
//                     bullet[i].is_used = 0;
//                 }
//             }
//         }
         
//         // 비행기 draw
//         if(plane_move_flag) {
//             Clear_Image(plane.prev_x, plane.prev_y, PLANE_W, PLANE_H, BLACK);
//             Draw_Image(&plane, PLANE_SIZE);
// 			plane_move_flag = 0;
// 		}

//         // missile draw
//         for(i = 0; i < level_max_missiles[game_level]; i++)
//         {
//             if(missile[i].is_used == 1)
//             {
//                 Clear_Image(missile[i].prev_x, missile[i].prev_y, MISSILE_W, MISSILE_H, BLACK);
//                 Draw_Image(&missile[i], MISSILE_SIZE);
//             }
//         }

//        // bullet draw
//         for(i = 0; i < 10; i++)
//         {
//             if(bullet[i].is_used == 1)
//             {
//                 Clear_Image(bullet[i].prev_x, bullet[i].prev_y, MISSILE_W, MISSILE_H, BLACK);
//                 Draw_Image(&bullet[i], MISSILE_SIZE);
//             }
//         }
// 	}

// 	else if(game_state_flag==GAME_OVER){
// 		Lcd_Draw_Back_Color(RED);
// 		_Delay(300);
// 		Lcd_Clr_Screen();
// 		_Delay(300);
// 	}
// }

// void Play_BGM(void)
// {
// 	int index, count, i;
// 	if(game_state_flag==GAME_START){
// 		Lcd_Draw_Back_Color(BLUE);
// 		_Delay(300);
// 		Lcd_Clr_Screen();
// 		_Delay(300);
// 	}
// 	else if(game_state_flag==GAME_PLAYING){
// 		// DrawPlane();
//         ;
// 	}
// 	else if(game_state_flag==GAME_OVER){
// 		Lcd_Draw_Back_Color(RED);
// 		_Delay(300);
// 		Lcd_Clr_Screen();
// 		_Delay(300);
// 	}
// }