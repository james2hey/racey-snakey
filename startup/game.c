#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 1000
#define LOOP_RATE 1000
#define MESSAGE_RATE 16

static int playerReady = 0; 
static int opponentReady = 0;


static void tinygl_startup(void)
{
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text("  READY UP!");
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}

static void led_countdown(void)
{
    tinygl_text("  321  GO");
    while(1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
    }
}



int main (void)
{
    system_init ();
    tinygl_startup();
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();
    while(playerReady == 0 || opponentReady == 0)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            playerReady = 1;
            ir_uart_putc(playerReady);
        }
        //if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            //const char[] message = "TEST";
            //tinygl_draw_messgae(message
            
        if (ir_uart_read_ready_p()) {
            opponentReady = '1'; 
        }
    }
    led_countdown();
    //init_game
    return 0;
}






