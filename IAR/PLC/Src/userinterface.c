#include "userinterface.h"
#include "video.h"
#include "lcd.h"
#include "stmpe811.h"
#include "image888.h"
#include "rtc.h"

GUI_Object* Circle1 =0x00000000;
GUI_Object* Image1; 
GUI_Object* Text1;
GUI_Object* Text2; 
GUI_Object* Text3;
GUI_Object* Poly1;
u8 StrDate[11]="25.04.2016";
u8 StrTime[9]="20:00:00";
date_time_t DataTime;



void Load_GUI_1(void){
  static Point Poly1_points[4]={{250,250},{260,130},{270,250},{260,240}};

  GUI_Free();

//  GUI_SetObject(LINE_TYPE, 0xFFFF0000, 3, 4, 100,100,1200,1200);
 // GUI_SetObject(LINE_TYPE, 0x33FF0000, 3, 4, 100,100,500,800);
 // GUI_SetObject(CIRCLE_TYPE, 0xFFFFFF00, 3, 3, 300, 300, 10);
//  GUI_SetObject(CIRCLE_TYPE, 0xFFFF00FF, 2, 3, 200+1, 200, 10);
//  Circle1 = GUI_SetObject(FILLED_CIRCLE_TYPE, 0xFFFFFF99, 4, 3, 300-10, 300, 10-5);
 // GUI_SetObject(FILLED_CIRCLE_TYPE, 0xFF00FF00, 1, 3, 300-10, 300, 10);
//  GUI_SetObject(FILLED_RECT_TYPE, 0xFFCC0000, 1, 4, 100, 100, 200, 200);
 // GUI_SetObject(HORIZONTAL_LINE_TYPE,0xFF00FF00, 1, 3, 100, 200, 300);
 // GUI_SetObject(FILLED_TRIANGLE, 0xFF00AA00, 1, 6, 50, 100, 650, 20, 300, 150);
  Image1 = GUI_SetObject(IMAGE_FAST_FILL,0xFF00FF00, 1, 5, SDRAM_BANK_ADDR + IMAGE_1_OFFSET, 0, 0, 800, 480);  
//  Text1 = GUI_SetObject(TEXT_STRING ,0xFF0000FF, 2, 5, 100, 200, StrData, LEFT_MODE, 1);   // with 1 pix kerning
 
  GUI_SetObject(FILLED_RECT_TYPE, 0xFF000000, 2, 4, 20, 10, 110, 35);
  GUI_SetObject(FILLED_RECT_TYPE, 0xFF000000, 2, 4, 690, 10, 780, 35);
  LCD_SetBackColor(0x0000FFFF);
  Text2 = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 5, 40, 10, StrTime, LEFT_MODE, 1);   // with 1 pix kerning and center
//  LCD_SetBackColor(0x00FFFFFF);
  Text3 = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 5, 700, 10, StrDate, LEFT_MODE, 1);   // with 1 pix kerning
  
  Poly1 = GUI_SetObject(FILLED_POLY,0xFFFF0000, 1, 2, (u32)Poly1_points, 4);
 // Image2 = GUI_SetObject(IMAGE_FAST_FILL,0xFF00FF00, 2, 5, SDRAM_BANK_ADDR + IMAGE_2_OFFSET, 0, 0, 800, 480);
}

void Run_GUI_1(void){

  u8 buff[12];
  date_time_t dt;
  PCF8563_read_datetime(&dt);
 // buff[0] = dt. 
  TwoDigitsToChars(buff);
  PCF8563_read_datetime(&DataTime);
  GetDateToStr(StrDate, &dt);
  GetTimeToStr(StrTime, &dt);
 //  Image1->params[1] = 300-iLoad;
 //  Circle1->params[1] = 300;
 //  Circle1->params[2] = iLoad-5;

}

void Load_GUI_2(void){
GUI_Free();
//LCD_InitParams(0, 0, 0xFF00FF00, &GOST_B_22x24); // set the font, color of font and the color of line
GUI_SetObject(LINE_TYPE, 1, 1, 4, 100,100,300,180);
GUI_SetObject(LINE_TYPE, 1, 1, 4, 100,100,110,110);
}

void ChangeCircle1(u8 Consistance){

  switch(Consistance){
    case TOUCH_PRESSED:
     //  Circle1->color = 0xFFFFFF00;
    //   Circle1->z_index = 1;
    //   if (Touch_Data.yp > 150) Touch_Data.yp = 150;
     //  Circle1->params[1] = 300- Touch_Data.yp;
      Image1->params[0] =SDRAM_BANK_ADDR + IMAGE_2_OFFSET;
      break;
    case TOUCH_RELEASED: 
     //  Circle1->color = 0xFFFFFF99; 
     //  Circle1->z_index = 4;
      Image1->params[0] =SDRAM_BANK_ADDR + IMAGE_1_OFFSET;
      break;
  }

}
