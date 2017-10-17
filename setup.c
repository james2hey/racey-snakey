/** @file setup.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Controls the flow of the game, with a continous loop 
 *  restarting the game until a player decides to quit.
*/
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "timer.h"
#include "game.h" 
#include "stdbool.h"


#define PACER_RATE 1000
#define LOOP_RATE 1000
#define MESSAGE_RATE 20
#define COUNT_ITERATIONS 5800
#define SENDING_NUMBER 191

static int amount = 0;
static int player_number = 0;

/** Initilizes tinygl and sets the message to scroll and
 *  ask for the player to ready up. */
static void tinygl_startup(void)
{
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
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
static void tinygl_wait_text(void)
{
    tinygl_text("  WAITING ON PLAYER2");
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
            
            ir_uart_putc('z');
            tinygl_wait_text();
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
    while(counter < COUNT_ITERATIONS*2) {
        pacer_wait ();
        tinygl_update ();
        counter++;
    }
}


static void send_restarting_choice(char player_answer)
{
    if (player_answer == 'y') {
        ir_uart_putc('y');
    } else {
        ir_uart_putc('n');
    }
}


static char receive_restarting_choice(void)
{
    char choice = ir_uart_getc();
    if (choice == 'y') { // Yes
        return 'y';
    } else if (choice == 'n') { // No
        return 'n';
    } else {
        return '?'; // Undecided
    }
}

/** Checks if both users want to restart the game by them pushing
 *  the navswitch on either "yes" or "no".
 *  @return true if both players want to restart, false otherwise.
 */
static bool restart(void)
{
    
    tinygl_clear();
    tinygl_text(" PLAY AGAIN?");
    
    bool player_answer = '?';
    char opponent_answer = '?';
    bool answered = false;
    
    while(player_answer == '?' || opponent_answer == '?') {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_NORTH) && !answered) {
            tinygl_text(" YES");
            player_answer = 'y';
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && !answered) {
            tinygl_text(" NO");
            player_answer = 'n';
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH) && player_answer != '?') {
            send_restarting_choice(player_answer);
            answered = true;
            tinygl_wait_text();
        }
        if (ir_uart_read_ready_p() && ir_uart_write_finished_p()) {
            opponent_answer = receive_restarting_choice();
        }
    }
    return player_answer && opponent_answer == 'y'; // True if both players want to restart.
}

/*
static void send_difficulty(int difficulty, bool pushed)
{
    if (difficulty == 10) {
        ir_uart_putc('a');
    } else if (difficulty == 20) {
        ir_uart_putc('b');
    } else if (difficulty == 30) {
        ir_uart_putc('c');
    }
    if (pushed) {
        ir_uart_putc('q'); // Terminating character.
    }
}


static int choose_difficulty(void)
{
    tinygl_clear();
    tinygl_text("  CHOOSE DIFFICULTY");
    
    int difficulty = 0;
    char text[3];
    bool pushed = false;
    
    while (1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_NORTH) && difficulty < 30) {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            difficulty += 10;
            itoa(difficulty, text, 10);
            tinygl_text(text);
        }
        
        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && difficulty > 10) {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            difficulty -= 10;
            itoa(difficulty, text, 10);
            tinygl_text(text);
        }
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH) && difficulty != 0) {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            pushed = true;
            break;
        }
        send_difficulty(difficulty, pushed);
    }
    return difficulty;
}

static int mirror_difficulty(void)
{
    tinygl_clear();
    tinygl_text("  CHOOSE DIFFICULTY");
    
    int difficulty = 0;
    char text[3];
    
    while (1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 'a') {
                tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
                difficulty = 10;
                itoa(difficulty, text, 10);
                tinygl_text(text);
                
            } else if (ir_uart_getc() == 'b') {
                tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
                difficulty = 20;
                itoa(difficulty, text, 10);
                tinygl_text(text);
                
            } else if (ir_uart_getc() == 'c') {
                tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
                difficulty = 30;
                itoa(difficulty, text, 10);
                tinygl_text(text);
            }
            if (ir_uart_getc() == 'q') {
                tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
                break;
            }
        }
    }
    return difficulty;
}*/

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
    
    //int difficulty;
    
    bool keep_playing = true;
    while (keep_playing) {
        ready_up();
        /*if (player_number == 1) {
            difficulty = choose_difficulty();
        } else {
            difficulty = mirror_difficulty();
        }
        */
        //led_countdown();
        //begin_game(player_number);
        //end_game(0);
        keep_playing = restart();
    }
    
    tinygl_clear();
    tinygl_update();

    return 0;
}
