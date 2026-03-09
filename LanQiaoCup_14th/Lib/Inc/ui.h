#ifndef __ui_h__
#define __ui_h__

#include "main.h" 
#include <stdio.h>
#include "pwm_out.h" 
#include "pulse.h" 
#include "lcd.h"
#include "analog_in.h"

typedef enum {
    DATA,
    PARA,
    RECD,
}Page;

typedef struct {
    Page current_page;
    uint8_t current_line;
    
    double v;
    float mh;
    float ml;
    
    uint8_t R;
    uint8_t K;
}Ui;

extern Ui ui;
extern uint8_t lock_pulse;

void ui_init();
void ui_switch();

void ui_cal();
void ui_updata();

void ui_show_data();
void ui_show_para();
void ui_show_recd();
#endif
