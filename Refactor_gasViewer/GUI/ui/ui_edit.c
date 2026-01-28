/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_edit.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static void ui_format_data(ui_edit_t *self) {
  char *p = 0;

  // ui_format_string(self->string, sizeof(self->string), &self->udata,
  // self->format, 0);

  switch (self->format) // 被编辑控件格式
  {
  case edf_int_5:
    if (p == 0)
      p = "%05d";
    snprintf(self->string, sizeof(self->string), p, self->idata);
    break;
  case edf_sint_5:
    if (p == 0) {
      if (self->idata >= 0)
        p = "+%05d";
      else
        p = "%05d";
    }
    snprintf(self->string, sizeof(self->string), p, self->idata);
    break;

  case edf_uint_1:
    if (p == 0)
      p = "%01u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_uint_2:
    if (p == 0)
      p = "%02u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_uint_3:
    if (p == 0)
      p = "%03u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_uint_4:
    if (p == 0)
      p = "%04u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_uint_5:
    if (p == 0)
      p = "%05u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_uint_9:
    if (p == 0)
      p = "%09u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_ulong_10:
    if (p == 0)
      p = "%010u";
    snprintf(self->string, sizeof(self->string), p, self->udata);
    break;
  case edf_sfloat_14:
    if (p == 0) {
      if (self->fdata >= 0)
        p = "+%06.4f";
      else
        p = "%06.4f";
    }
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_sfloat_23:
    if (p == 0) {
      if (self->fdata >= 0)
        p = "+%06.3f";
      else
        p = "%07.3f";
    }
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_sfloat_32:
    if (p == 0) {
      if (self->fdata >= 0)
        p = "%+06.2f";
      else
        p = "%07.2f";
    }
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_sfloat_41:

    if (p == 0) {
      if (self->fdata >= 0)
        p = "+%06.1f";
      else
        p = "%07.1f";
    }
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_sfloat_50:
    if (p == 0) {
      if (self->fdata >= 0)
        p = "+%05.0f";
      else
        p = "%06.0f";
    }
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_float_14:
    if (p == 0)
      p = "%06.4f";
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_float_23:
    if (p == 0)
      p = "%06.3f";
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_float_32:
    if (p == 0)
      p = "%06.2f";
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_float_41:

    if (p == 0)
      p = "%06.1f";
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  case edf_float_50:
  default:
    if (p == 0)
      p = "%05.0f";
    snprintf(self->string, sizeof(self->string), p, self->fdata);
    break;
  }
}

static void ui_convert_data(ui_edit_t *self) {
  switch (self->format) // 被编辑控件格式
  {
  case edf_int_5:
    self->idata = atoi(self->string);
    break;
  case edf_uint_1:
  case edf_uint_2:
  case edf_uint_3:
  case edf_uint_4:
  case edf_uint_5:
    self->udata = atoi(self->string);
    break;
  case edf_uint_9:
    self->udata = atoi(self->string);
    break;
  case edf_ulong_10:
    self->udata = atoll(self->string);
    break;
  case edf_float_14:
  case edf_float_23:
  case edf_float_32:
  case edf_float_41:
  case edf_float_50:
  default:
    self->fdata = atof(self->string);
    break;
  }
}

static void ui_edit_draw_callback(void *parg) {
  ui_edit_t *win = (ui_edit_t *)parg;

  ui_show_text(&win->pos, &win->text, 1);

  //	MainWin_UpdateStatusBar(" + ", "->", "确认");
}

static void ui_edit_timer_callback(void *parg) {
  ui_edit_t *win = (ui_edit_t *)parg;
  char string[4];

  win->tick.newtick = HAL_GetTick();

  if (win->tick.newtick - win->tick.lasttick > win->tick.period) {
    win->tick.lasttick = win->tick.newtick;

    snprintf(string, sizeof(string), "%c", win->text.text[win->focused]);
    if (win->twinkle)
      ui_show_lable(win->pos.x + win->focused * win->text.height / 2,
                    win->pos.y, win->text.height / 2, win->pos.height, string,
                    win->text.height, win->text.color.bcolor,
                    win->text.color.fcolor, win->text.align);
    else
      ui_show_lable(win->pos.x + win->focused * win->text.height / 2,
                    win->pos.y, win->text.height / 2, win->pos.height, string,
                    win->text.height, win->text.color.fcolor,
                    win->text.color.bcolor, win->text.align);

    if (win->twinkle)
      win->twinkle = 0;
    else
      win->twinkle++;
  }
}

static void ui_edit_callback(void *parg) {
  ui_edit_t *self = (ui_edit_t *)parg;
  u16 key = 0;

  if (self->event.event & ewm_init) {
    self->event.event &= ~ewm_init;
    self->event.event |= ewm_paint;
  } else if (self->event.event & ewm_paint) {
    self->event.paint(self);
    self->event.event &= ~ewm_paint;
  } else if (self->event.event & ewm_update) {
    // 结果模式，赋值，返回上一窗口
    if (self->confirm.result == ecr_OK) {
      self->udata = self->udata;
      ui_widgets_set_top_win(self->prev);
      self->prev->event.event |= ewm_update;
    }
    self->event.event &= ~ewm_update;
  } else if (self->event.event & ewm_key) {

    /*
    硬键盘：加一，右移，确认
    遥控器：加减，左右，数字，确认，退出
    */

    key = ui_GetInputKey(1);
    switch (key) {
    case ewk_resc:
      // 回到上一个窗口
      ui_widgets_set_top_win(self->prev);
      self->prev->event.event |= ewm_update;
      break;
    case ewk_ok:
    case ewk_rok:
      ui_convert_data(self);
      // 回到上一个窗口
      ui_widgets_set_top_win(self->prev);
      self->prev->event.event |= ewm_update;
      break;

    case ewk_left:
      if (self->focused)
        self->focused--;
      else
        self->focused = self->length - 1;
      if (self->string[self->focused] < '0' ||
          self->string[self->focused] > '9') {
        if (self->focused)
          self->focused--;
      }
      break;

    case ewk_down:
    case ewk_right:
      self->focused++;
      if (self->string[self->focused] < '0' ||
          self->string[self->focused] > '9')
        self->focused++;
      if (self->focused >= self->length)
        self->focused = 0;
      break;

    case ewk_esc: // 加 +
    case ewk_rup: // 加 +
      if (self->string[self->focused] >= '0' &&
          self->string[self->focused] <= '9') {
        if (self->string[self->focused] >= '0' &&
            self->string[self->focused] < '9')
          self->string[self->focused]++;
        else
          self->string[self->focused] = '0';
      } else if (self->string[self->focused] == '+')
        self->string[self->focused] = '-';
      else if (self->string[self->focused] == '-')
        self->string[self->focused] = '+';
      break;

    case ewk_rdown: // 减 -
      if (self->string[self->focused] >= '0' &&
          self->string[self->focused] <= '9') {
        if (self->string[self->focused] > '0' &&
            self->string[self->focused] <= '9')
          self->string[self->focused]--;
        else
          self->string[self->focused] = '9';
      } else if (self->string[self->focused] == '+')
        self->string[self->focused] = '-';
      else if (self->string[self->focused] == '-')
        self->string[self->focused] = '+';
      break;

      /*		case ewk_mute:
                              if(self->mode == eem_edit)
                              {
                                      if(self->focused)
                                              self->focused--;
                                      else
                                              self->focused = self->length - 1;
                                      if(self->string[self->focused] < '0' ||
         self->string[self->focused] > '9')
                                      {
                                              if(self->focused)
                                                      self->focused--;
                                      }
                              }
                              break;
      */
    case ewk_func0:
    case ewk_func1:
    case ewk_func2:
    case ewk_func3:
    case ewk_func4:
    case ewk_func5:
    case ewk_func6:
    case ewk_func7:
    case ewk_func8:
    case ewk_func9:
      if (self->mode == eem_edit) {
        if (self->string[self->focused] >= '0' &&
            self->string[self->focused] <= '9') {
          self->string[self->focused] = '0' + (key - ewk_func0);

          self->focused++;
          if (self->string[self->focused] < '0' ||
              self->string[self->focused] > '9')
            self->focused++;
          if (self->focused >= self->length) {
            // 输入完最后一位后自动确认
            ui_convert_data(self);
            ui_widgets_set_top_win(self->prev);
            self->prev->event.event |= ewm_update;
            return;
          }
        }
      }
      break;
    }
    self->event.event |= ewm_paint;
    self->event.event &= ~ewm_key;
  } else if (self->event.event & ewm_delete) {
    self->event.event &= ~ewm_delete;
  } else if (self->event.event & ewm_time) {
    ui_edit_timer_callback(self);
    // win->event.event &= ~ewm_time;		//周期运行
  }
}

/*

*/
WM_HWIN ui_edit_create(ui_widgets_t *parent, ui_edit_t *self, union_Data data,
                       u16 format, char *title) {
  if (parent == 0 || self == 0)
    return 0;

  self->pos.x = (320 - 240) / 2;
  self->pos.y = (240 - 120) / 2;
  self->pos.width = 240;
  self->pos.height = 120;

  self->text.text = self->string;
  self->text.height = efsize_32;
  self->text.align = (eat_hleft | eat_vcenter);
  self->text.color.fcolor = rgb565_edit_front;
  self->text.color.bcolor = rgb565_edit_background;
  self->event.cb = ui_edit_callback;
  self->event.paint = ui_edit_draw_callback;
  self->focused = 0;
  self->string[0] = 0;

  self->format = format;
  self->udata = data.udata;
  ui_format_data(self);

  self->length = strlen(self->string);
  self->mode = eem_edit;
  self->twinkle = 0;
  self->event.event |= ewm_time;
  self->event.event |= ewm_init;
  self->prev = parent;

  self->tick.step = 0;
  self->tick.period = 499;
  self->tick.lasttick = HAL_GetTick();

  ui_widgets_set_top_win(
      (ui_widgets_t *)self); // 很重要，一定要有这一句，第一个运行的窗口

  return self;
}

WM_HWIN ui_edit_create_ex(ui_widgets_t *parent, ui_edit_t *self,
                          union_Data data, u16 format, u16 x, u16 y, u16 w,
                          u16 h, u16 fsize) {
  if (parent == 0 || self == 0)
    return 0;

  ui_edit_create(parent, self, data, format, 0);

  self->pos.x = x;
  self->pos.y = y;
  self->pos.width = w;
  self->pos.height = h;
  self->text.height = fsize;

  return self;
}

/**********************
 **********************/
