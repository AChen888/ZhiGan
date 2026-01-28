#ifndef __UI_FONT64_H
#define __UI_FONT64_H 	   

/*
* ⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇
*/

/*
**********************************************************************
*                        UcGUI Font
*                   Chinese GB2312 library
*           (c) Copyright 19xx-20xx, company name
*
*
* 注：本字体文件由 ucGUI字体生成器v9.0  生成，作者:馋嘴猫(qq:602426967)
* 字体:宋体,字形:常规,大小:45,宽:64,高:64,水平偏移:0,垂直偏移:0
*
**********************************************************************
*/

#include "..\ui\GUI.H"


///* char: ⑴   code:0xA2C5 */
//static GUI_CONST_STORAGE unsigned char acA2C5[512] = {
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,____XXXX,XXXXXXX_,________,________,________,
//  ________,________,________,_XXXXXXX,XXXXXXXX,XX______,________,________,
//  ________,________,______XX,XXXXXXXX,XXXXXXXX,XXXXX___,________,________,
//  ________,________,____XXXX,XXXXXXXX,XXXXXXXX,XXXXXX__,________,________,
//  ________,________,___XXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,________,________,
//  ________,________,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XX______,________,
//  ________,________,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX_____,________,
//  ________,_______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX_____,________,
//  ________,_____XXX,XXXXXXXX,XX______,_XXXXXXX,XXXXXXXX,XXXXX___,________,
//  ________,_____XXX,XXXXXXXX,X_______,XXXXXXXX,XXXXXXXX,XXXXXX__,________,
//  ________,____XXXX,XXXXXXXX,_______X,XXXXXXXX,XXXXXXXX,XXXXXX__,________,
//  ________,____XXXX,XXXXXX__,_______X,XXXXXXXX,XXXXXXXX,XXXXXXX_,________,
//  ________,___XXXXX,XXXXXX__,________,_______X,XXXXXXXX,XXXXXXXX,________,
//  ________,__XXXXXX,XXXXX___,________,________,__XXXXXX,XXXXXXXX,X_______,
//  ________,__XXXXXX,XXXXX___,________,________,___XXXXX,XXXXXXXX,X_______,
//  ________,_XXXXXXX,XXXX____,______XX,XXXX____,_____XXX,XXXXXXXX,X_______,
//  ________,_XXXXXXX,XXXX____,_____XXX,XXXXXXXX,X____XXX,XXXXXXXX,XX______,
//  ________,XXXXXXXX,XXXX____,_____XXX,XXXXXXXX,XXX___XX,XXXXXXXX,XX______,
//  _______X,XXXXXXXX,XXXXX___,_____XXX,XXXXXXXX,XXXXXXX_,XXXXXXXX,XXX_____,
//  _______X,XXXXXXXX,XXXXXX__,______XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXX__,______XX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXX_,_______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,X_______,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XX______,_XXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,X_______,_XXXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX_____,__XXXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXX____,___XXXXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXX___,_____XXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXX___,_____XXX,XXXXXXXX,XXXX____,
//  _______X,XXXXXXXX,XXX_XXXX,XXXXXXXX,XXXXX___,______X_,XXXXXXXX,XXX_____,
//  ________,XXXXXXXX,XXXX____,XXXXXXXX,XXXXXX__,_______X,XXXXXXXX,XX______,
//  ________,_XXXXXXX,XXXXX___,__XXXXXX,XXXXXX__,______XX,XXXXXXXX,XX______,
//  ________,_XXXXXXX,XXXXXX__,_______X,XXXX____,______XX,XXXXXXXX,X_______,
//  ________,__XXXXXX,XXXXXXXX,________,________,______XX,XXXXXXXX,X_______,
//  ________,__XXXXXX,XXXXXXXX,X_______,________,_____XXX,XXXXXXXX,X_______,
//  ________,___XXXXX,XXXXXXXX,XXX_____,________,_____XXX,XXXXXXXX,________,
//  ________,___XXXXX,XXXXXXXX,XXXXXX__,________,_____XXX,XXXXXXXX,________,
//  ________,____XXXX,XXXXXXXX,XXXXXXXX,XXXX____,___XXXXX,XXXXXX__,________,
//  ________,_____XXX,XXXXXXXX,XXXXXXXX,XXX_____,__XXXXXX,XXXXXX__,________,
//  ________,_____XXX,XXXXXXXX,XXXXXXXX,X_______,_XXXXXXX,XXXXX___,________,
//  ________,_______X,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX_____,________,
//  ________,________,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXX_____,________,
//  ________,________,XXXXXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,XX______,________,
//  ________,________,___XXXXX,XXXXXXXX,XXXXXXXX,XXXXXXXX,________,________,
//  ________,________,____XXXX,XXXXXXXX,XXXXXXXX,XXXXXX__,________,________,
//  ________,________,______XX,XXXXXXXX,XXXXXXXX,XXXXX___,________,________,
//  ________,________,________,_XXXXXXX,XXXXXXXX,XX______,________,________,
//  ________,________,________,____XXXX,XXXXXXX_,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//  ________,________,________,________,________,________,________,________,
//};



//static GUI_CONST_STORAGE GUI_CHARINFO Cinfo[1] = {
//  { 57, 57, 8, (unsigned char*)&acA2C5 }, /*0: ⑴*/
//};

static GUI_CONST_STORAGE GUI_FONT_PROP Prop1 = {
  0xA2C5, /*start :⑴*/
  0xA2C5, /*end   :⒇,  len=20*/
//  &Cinfo[0],
  (void*)0
};


GUI_CONST_STORAGE GUI_FONT GUI_FontHZ64x64 = {
  GUI_FONTTYPE_PROP_SJIS,
  64, /* height of font  */
  64, /* space of font y */
  1,
  1,
  (GUI_FONT_PROP*)&Prop1
};


#endif


