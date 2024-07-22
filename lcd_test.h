/*
 * lcd_test.h
 *
 *  Created on: 2024. 7. 16.
 *      Author: yeeun99.kim
 */

#ifndef LCD_TEST_H_
#define LCD_TEST_H_

typedef struct {
    int x, y;
    int width, height;
    const int * image;
} GameObject;

extern void Generate_Missile(GameObject *missile);
extern void Draw_Image(int x, int y, int w, int h, const int *image);
extern void Clear_Image(int x, int y, int w, int h, unsigned short Color);
extern void DrawPlane(void);
extern void ClearPlane(void);
extern void DrawMissile(GameObject* missile);
extern void ClearMissile(GameObject* missile);
extern void Game_Init(void);
extern void Game_Plane_Move(int dir);
extern void Game_Missile_Generation(void);
extern void Game_Missile_Move(void);
extern void Game_Over(void);
extern void Draw_LCD(void);
extern void PLay_BGM(void);
extern void intToStr(int score, char *str);
extern void drawDigit(int x, int y, int digit);
extern void drawScore(int x, int y, int score);


#endif /* LCD_TEST_H_ */
