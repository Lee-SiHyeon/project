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
extern void Game_Plane_Move(void);
extern void Game_Missile_Generation(void);
extern void Game_Missile_Move(void);
extern void Game_Over(void);

#endif /* LCD_TEST_H_ */
