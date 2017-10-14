#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "timer.h"


#define PACER_RATE 1000
#define LOOP_RATE 1000
#define MESSAGE_RATE 16
#define COUNTDOWN_TASK_RATE 10
#define COUNT_ITERATIONS 6000

static int playerReady = 0; 
static int opponentReady = 0;

/** Initilizes tinygl and sets the message to scroll and
 *  ask for the player to ready up. */
static void tinygl_startup(void)
{
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text("  READY UP!");
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}

/** Sets the text to "wait" */
static void tingl_ready_text(void)
{
    tinygl_text(" WAIT");
}

/** Displays a message counting down from 3    */
static void led_countdown(void)
{
    tinygl_clear();
    tinygl_text("  321 GO");

    int counter = 0;  //Counter to know when to stop the message. 
    while(counter < COUNT_ITERATIONS) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
}

/** Waits for both players to push the navswitch. */
static void ready_up(void)
{
        while(playerReady == 0 || opponentReady == 0)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            playerReady = 1;
            ir_uart_putc(playerReady);
            tingl_ready_text();
            break;
        }
            
        if (ir_uart_read_ready_p()) {
            opponentReady = '1'; 
        }
    }
}

int main (void)
{
    system_init ();
    tinygl_startup();
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();
    
    ready_up();

    led_countdown();
    //init_game
    return 0;
}






