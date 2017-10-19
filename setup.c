/** @file setup.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Controls the flow of the game, with a continous loop 
 *  restarting until the game is decided to be quit.
*/
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "game.h" 
#include "stdbool.h"

#define PACER_RATE 1000
#define LOOP_RATE 1000
#define MESSAGE_RATE 20
#define COUNT_ITERATIONS 5800 // Counter to know when to stop messages.

static int amount = 0;
static int player_number = 0;


/** Initilizes tinygl, sets the message to scroll and
 *  rotates the text. */
static void tinygl_startup(void)
{
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);

    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
}


/** Retransmits the given character after being called
 *  'COUNT_ITERATIONS' times.
 *  @param character the character to potentially be retransmitted.
 */
static void retransmit(char character) {
    amount++;
    if (amount > COUNT_ITERATIONS) {
        amount = 0;
        ir_uart_putc(character);
    }
}


/** Sets the text to "waiting on player 2".
*/
static void tinygl_wait_text(void)
{
    tinygl_text("  WAITING ON PLAYER2");
}


/** Displays a generic count down message from 3.
*/
static void led_countdown(void)
{
    tinygl_clear();
    tinygl_text("  321 GO");

    int counter = 0; 
    while(counter < COUNT_ITERATIONS) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
}


/** Waits for both players to push the navswitch, then exits the loop.
 *  The message displayed is "Click to ready up".
 */
static void ready_up(void)
{
    tinygl_clear();
    tinygl_text("  CLICK TO READY UP!");
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

            ir_uart_putc('z');  // An uncommon letter used to avoid
            tinygl_wait_text(); // interference when this was tested.
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
    while(counter < COUNT_ITERATIONS*2) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
}


/** Sends the characters answer over ir_uart, which signals
 *  whether or not they want to restart the game.
 *  @param player_answer char of the players answer; 'y' = yes,
 *  'n' = no
 */
static void send_restarting_choice(char player_answer)
{
    if (player_answer == 'y') {
        ir_uart_putc('y');
    } else {
        ir_uart_putc('n');
    }
}


/** Checks if player 1 wants to restart game. They decide this by 
 *  scrolling on the navswitch to either 'yes' or 'no and then pushing. 
 *  This is displayed on both players screens.
 */
static bool restart(void)
{
    tinygl_clear();
    tinygl_text(" PLAY AGAIN?");
    
    char player_answer = '?';
    bool answered = false;

    while (!answered) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();

        // Player 1 controls the restart.
        if (player_number == 1) {
            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                tinygl_text(" YES");
                player_answer = 'y';
                ir_uart_putc('y');
            }
            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                tinygl_text(" NO");
                player_answer = 'n';
                ir_uart_putc('n');
            }
            // Sending the final decision.
            if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                send_restarting_choice(player_answer);
                ir_uart_putc('b');
                answered = true; 
            }
        // Player 2 watches a mirror of player 1's screen.
        } else { 
            if (ir_uart_read_ready_p()) {
                char choice = ir_uart_getc();
                if (choice == 'y') {
                    player_answer = 'y';
                    tinygl_text(" YES");
                } else if (choice == 'n') {
                    player_answer = 'n';
                    tinygl_text(" NO");
                    
                // Receiving the final decision
                } else if (choice == 'b') {
                    answered = true;
                }
            }
        }
    }
    return player_answer == 'y';
}


/** Displays a gameover message, then clears the tinygl matrix.
 */
static void terminate(void)
{
    tinygl_clear();
    tinygl_update();
    tinygl_text( " GAMEOVER");
    int counter = 0;
    while (counter < COUNT_ITERATIONS) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
    tinygl_clear();
    tinygl_update();
}
 

/** The programs main loop. This is where the game is initilized,
 *  and controlled for displaying various messages. It also includes
 *  the games restart/termination dictated by the user.
 */
int main (void)
{
    system_init ();
    tinygl_startup();
    pacer_init (PACER_RATE);
    navswitch_init();
    ir_uart_init();

    bool keep_playing = true;
    int winner;
    while (keep_playing) {
        ready_up();
        led_countdown();
        winner = begin_game(player_number);
        end_game(winner);
        keep_playing = restart();
    }
    terminate();
    return 0;
}
