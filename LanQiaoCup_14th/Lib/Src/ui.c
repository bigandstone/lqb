#include "ui.h"

Ui ui = {
    .current_page = DATA,
    .current_line = 0,
    .mh = 0,
    .ml = 0,
    .v = 0,
    .R = 1,
    .K = 1,
};

uint8_t lock_pulse = 0;

static char buff[30];

void ui_init(){
    LCD_Init();
    LCD_SetBackColor(Black);
    LCD_Clear(Black);
    LCD_SetTextColor(White);
    ui_show_data();
}

void ui_switch(){
    LCD_Clear(Black);
    if(ui.current_page == RECD) ui.current_page = DATA;
    else ui.current_page ++;
    ui.current_line = 0;
    //R and K set
    pulse.R = ui.R;
    pulse.K = ui.K;
}

void ui_cal(){//1ms
    pwm1.pulse = lock_pulse == 0? (10+(85-10)/(3722.73-1240.91)* pot.val):pwm1.pulse;
    if(pwm1.pulse > 85) pwm1.pulse = 85;
    ui.v = pwm1.freq * 2 * 3.14 * pulse.R / (100 * pulse.K);
    
    if(pwm1.mode == PWM_HIGH )       ui.mh = ui.mh < ui.v?ui.v:ui.mh;
    else if(pwm1.mode == PWM_LOW)  ui.ml = ui.ml < ui.v?ui.v:ui.ml;
}

void ui_updata(){
    static uint8_t time = 0;
    if(++time < 100) return;
    time = 0;
    
    switch (ui.current_page)
    {
    	case DATA:
            ui_show_data();
    		break;
    	case PARA:
            ui_show_para();
    		break;
    	case RECD:
            ui_show_recd();
    		break;
    }
}

void ui_show_data(){
    LCD_DisplayStringLine(Line1, (u8*)"        DATA");
    
    sprintf(buff, "     M = %c    ",pwm1.mode == PWM_LOW ? 'L':'H');
    LCD_DisplayStringLine(Line3, (u8*)buff);
    
    sprintf(buff, "     P = %d%%    ",pwm1.pulse);
    LCD_DisplayStringLine(Line4, (u8*)buff);
    
    sprintf(buff, "     V = %.1f    ",ui.v);
    LCD_DisplayStringLine(Line5, (u8*)buff);
    
}

void ui_show_para(){
    LCD_DisplayStringLine(Line1, (u8*)"        PARA");
    
    sprintf(buff, "     R = %d    ",ui.R);
    LCD_DisplayStringLine(Line3, (u8*)buff);
    
    sprintf(buff, "     K = %d    ",ui.K);
    LCD_DisplayStringLine(Line4, (u8*)buff);
}

void ui_show_recd(){
    LCD_DisplayStringLine(Line1, (u8*)"        RECD");
    
    sprintf(buff, "     N = %d    ",pwm1.mode_cnt);
    LCD_DisplayStringLine(Line3, (u8*)buff);
    
    sprintf(buff, "     MH = %.1f    ",ui.mh);
    LCD_DisplayStringLine(Line4, (u8*)buff);
    
    sprintf(buff, "     ML = %.1f    ",ui.ml);
    LCD_DisplayStringLine(Line5, (u8*)buff);
}
