/** @file startup.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Waits for both users to ready up, then begins the game.
*/
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "timer.h"
#include "game.h" 
#include "stdbool.h"


#define PACER_RATE 1000
#define LOOP_RATE 1000
#define MESSAGE_RATE 16
#define COUNT_ITERATIONS 6000
#define SENDING_NUMBER 191

static int amount = 0;
static int player_number = 0;

/** Initilizes tinygl and sets the message to scroll and
 *  ask for the player to ready up. */
static void tinygl_startup(void)
{
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}

/** Retransmits the given character after 6000 calls.
 * @param character the character to potentially be retransmitted.
 */
static void retransmit(char character) {
    amount++;
    if (amount > COUNT_ITERATIONS) {
        amount = 0;
        ir_uart_putc(character);
    }
}

/** Sets the text to "wait" */
static void tinygl_ready_text(void)
{
    tinygl_text(" WAIT");
}


/** Displays a generic count down message from 3    */
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


/** Waits for both players to push the navswitch, then exits the loop.
 */
static void ready_up(void)
{
    tinygl_clear();
    tinygl_text("  READY UP!");
    int playerReady = 0; 
    int opponentReady = 0;
    bool pushed = false;

    while(playerReady == 0 || opponentReady == 0)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (opponentReady == 1) {
                player_number = 2;
            } else {
                player_number = 1;
            }
            pushed = true;
            playerReady = 1;
            ir_uart_putc('z');
            tinygl_ready_text();
            //break;
        }
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 'z') {
                opponentReady = 1; 
            }
        }
        if (pushed) {
            retransmit('z');
        }
    }
}


/** Shows the players if they won, lost or drew the game.
 *  @param won values stand for 0 = lost, 1 = winner, 2 = draw. */
static void end_game(int won)
{
    tinygl_clear();
    
    if (won == 0) {
        tinygl_text(" LOSER");
    } else if (won == 1) {
        tinygl_text(" WINNER");
    } else {
        tinygl_text(" DRAW");
    }
    
    int counter = 0;
    while(counter < COUNT_ITERATIONS) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
}


static void send_restarting_choice(bool player_restarting)
{
    if (player_restarting) {
        ir_uart_putc('y');
    } else {
        ir_uart_putc('n');
    }
}


static char receive_restarting_choice(void)
{
    if (ir_uart_getc() == 'y') { // Yes
        return 'y';
    } else if (ir_uart_getc() == 'n') { // No
        return 'n';
    } else {
        return 'u'; // Undecided
    }
}

/** Checks if both users want to restart the game by them pushing
 *  the navswitch on either "yes" or "no".
 *  @return true if both players want to restart, false otherwise.
 */
static bool restart(void)
{
    bool player_restarting = true;
    char opponent_answer = '?';
    bool answered = true;
    tinygl_clear();
    tinygl_text(" PLAY AGAIN?");
    
    while(1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            tinygl_text(" YES");
            player_restarting = true;
            answered = true;
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            tinygl_text(" NO");
            player_restarting = false;
            answered = true;
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH) && answered) {
            send_restarting_choice(player_restarting);
        }
        if (ir_uart_read_ready_p()) {
            opponent_answer = receive_restarting_choice();
        }
        if (opponent_answer != '?' && answered) {
            break;
        }
        
        ///dasfdasfdsafhdaskjf;dlasfkjd;asflkjdas;f
    }
    return player_restarting && opponent_answer == 'y'; // True if both players want to restart.
}


int main (void)
{
    system_init ();
    tinygl_startup();
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();
    
    bool keep_playing = true;
    while (keep_playing) {
        ready_up();
        //led_countdown();
        begin_game(player_number);
        //end_game(1);
        //keep_playing = restart();
    }
    tinygl_clear();
    tinygl_update();
    

    return 0;
}






