#include <lvgl.h>
#include <music.h>
#include <httpApi.h>
#include <TFT_eSPI.h>
#ifndef STASSID
#define STASSID "CMCC-Xfak"
#define STAPSK  "fsycpt2q"
#define BLUETOOTH "TWS_p10 xsp"
#endif

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_obj_t *btns[10];
static int mIndex = 0;
static int pIndex = 0;
bool isAct = true;
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

/* Reading input device (simulated encoder here) */
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    static int32_t last_diff = 0;
    int32_t diff = 0; /* Dummy - no movement */
    int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

    data->enc_diff = diff - last_diff;;
    data->state = btn_state;

    last_diff = diff;
    
    return false;
}
// int32_t get_sound_data(Channels* data, int32_t len) {
// //   return out == nullptr ? 0 : out->read(data, len);
//     return 1;
// }
void timeout(int mm,void callback()){
    hw_timer_t *timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, callback, true);  // 绑定中断函数
    timerAlarmWrite(timer, mm * 1000, false); // 设置报警保护函数
    timerAlarmEnable(timer);                          // 使能报警器   
}

void flush_screen(){
    lv_task_handler();
}
void init_lvgl(){
    lv_init();
    #if USE_LV_LOG != 0
        lv_log_register_print_cb(my_print); /* register print function for debugging */
    #endif

    tft.begin(); /* TFT init */
    tft.setRotation(1); /* Landscape orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_drv_register(&indev_drv);



    lv_obj_t *list1 = lv_list_create(lv_scr_act(),NULL);
    lv_obj_set_size(list1,240,240);
    lv_obj_align(list1, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    for (size_t i = 0; i < 10; i++)
    {
        btns[i] = lv_list_add_btn(list1,LV_SYMBOL_AUDIO,lists[i].name.c_str());
    }
    lv_obj_t *keyboard = lv_keyboard_create(lv_scr_act(),NULL);
    lv_keyboard_set_mode(keyboard,LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_set_size(keyboard, 128,70);
    lv_obj_set_hidden(keyboard,true);
    lv_obj_align(keyboard, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}

void IRAM_ATTR admAct() { // 中断函数
    isAct=true;
}
void checkIndex(bool d){
    if(!d){
        if(mIndex<=0){
            mIndex = 10-1;
        }else {
            while(!btns[mIndex--]){
                if(mIndex<=0){
                    mIndex=10-1;
                }
            }
        }
        return;
    }
    if(d){
        if(mIndex>=10-1){
            mIndex=0;
        } else {
            while(!btns[mIndex++]){
                if(mIndex>=10-1){
                    mIndex=0;
                }
            }
        }
        return;
    }
}
void handleIndex(){
    static int lastms = 0;
    if(digitalRead(25)==HIGH&&isAct==true){
        if(millis()-lastms<30) {return;}
         Serial.println("25 pin");
        if(digitalRead(25)!=HIGH){return;}
        isAct = false;
        lv_obj_set_style_local_bg_color(btns[mIndex],NULL,NULL,LV_COLOR_WHITE);
        checkIndex(false);
        lv_obj_set_style_local_bg_color(btns[mIndex],NULL,NULL,LV_COLOR_GRAY);
        lv_list_focus(btns[mIndex], LV_ANIM_ON);
        timeout(200,&admAct);
    }
    if(digitalRead(27)==HIGH&&isAct==true){
        if(millis()-lastms<30) {return;}
         Serial.println("27 pin");
        if(digitalRead(27)!=HIGH){return;}
        isAct = false;
        lv_obj_set_style_local_bg_color(btns[pIndex],NULL,NULL,LV_COLOR_WHITE);
        pIndex = mIndex;
        lv_obj_set_style_local_bg_color(btns[pIndex],NULL,NULL,LV_COLOR_BLACK);
        play(getUrl(lists[pIndex].id));
        timeout(200,&admAct);
    };
    if(digitalRead(14)==HIGH&&isAct==true){
        if(millis()-lastms<30) {return;}
         Serial.println("14pin");
        if(digitalRead(14)!=HIGH){return;}
        isAct = false;
        lv_obj_set_style_local_bg_color(btns[mIndex],NULL,NULL,LV_COLOR_WHITE);
        checkIndex(true);
        lv_obj_set_style_local_bg_color(btns[mIndex],NULL,NULL,LV_COLOR_GRAY);
        lv_list_focus(btns[mIndex], LV_ANIM_ON);
        timeout(200,&admAct);
    }    
}