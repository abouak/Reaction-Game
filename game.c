

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "project.h"
#include "game.h"
#include "seven.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint8_t button_press = 0;
GAME_EVENT_t button = EV_NO_EVENT;
static uint16_t counter_ticker_previous = 0;
static uint16_t counter_ticker_current = 0;
GAME_STATE_t state = GAME_ISDLE;
static uint8_t display_value;
static uint8_t game_rounds = 0;
static uint8_t game_score = 0;
static uint8_t random_display_value = 0;
static uint16_t random_display_time = 0;
uint8_t game_time_display [3];
static uint16_t average_reaction_time [10] = {0};
uint8_t total_average_time [3];


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * Initializes the game by displaying a message
 * @param None
 * @return None
 */
void GAME_init () {
    UART_PutString("Reaction Game Started!!\n");
    UART_PutString("Please press a Button\n");
}
/**
 * Sets the Button flag after pressing a button 
 * @param GAME_EVENT_t button_press - Event received by the press of a button
 * @return None
 */
void GAME_SetButtonEvent(GAME_EVENT_t button_press){
    button = button_press;
}
/**
 * Get function that returns the flag of the pressed button
 * @param None
 * @return returns the pressed button either button_1 or button_2
 */
GAME_EVENT_t GAME_GetButtonEvent(){
    return button;
}
/**
 * returns a random number in a specified range in seconds
 * @param uint16_t value - range of numbers for the timer, E.x., 3 is from 1 to 3
 * @return returns a random number in the specified range in seconds
 */
uint16_t GAME_RandomTimer(uint16_t value){
    random_display_time = rand() % value + 1;
    random_display_time = random_display_time * 1000;
    return random_display_time;
}
/**
 * <Description>
 * @param uint16_t value - range of numbers for the timer, E.x., 3 is from 1 to 3
 * @return returns a random number in the specified range
 */
uint8_t GAME_RandomDisplayNumber(uint8_t value){
    random_display_value = rand() % value + 1;
    return random_display_value;
}
/**
 * Sets the current value of the counter_tick of the tsk_counter_tick task 
 * to a flag to mark the initial time of the reaction
 * @param None
 * @return None
 */
void GAME_SetTime(){
    counter_ticker_previous = counter_ticker;
}
/**
 * returns the time elapsed between the set of the timer and the current timer
 * @param None
 * @return returns the difference between the current time and the Setted time
 */
uint16_t GAME_GetTime(){
    counter_ticker_current = (counter_ticker - counter_ticker_previous);
    return counter_ticker_current;
}
/**
 * Returns the average of each won round in the game
 * @param uint16_t * score_list - array of the list measured time for each
 * round of the game
 * @return returns the average value of the time measured for each correct
 * press of a button
 */
uint16_t GAME_GetAverageTime(uint16_t * score_list){

    uint16_t average = 0; 
    for (int i=0; i<10 ; i++){
        average = average + score_list[i];
    }
    return average/MAX_GAME_ROUNDS;
}
/**
 * State machine implementation for the reaction game
 * @param GAME_EVENT_t state - Event that reacts depending on the state of the Game
 * @return None
 */
void GAME_start(GAME_EVENT_t event){

    uint8_t game_rounds_display = game_rounds + 48;
    uint8_t round_greeting []= "Reaction Game Round: ";
    strncat(round_greeting,&game_rounds_display,1);
    switch (state){
        
    case GAME_ISDLE:
        if (event == EV_BUTTON_1 || event == EV_BUTTON_2 ){
        UART_PutString(round_greeting);
        UART_PutString("\n");
        UART_PutString("STATE: GAME_ISIDLE Waiting for EV_Button to Start\n");
            state = GAME_START;
        }
        break;
        
    case GAME_START:
        if (event == EV_DISPLAY_SEVEN){
            UART_PutString("STATE: GAME_START Waiting for EV_seven to Display\n");
            display_value = GAME_RandomDisplayNumber(2);
            SEVEN_Set_All(display_value);
            GAME_SetTime();
            state = GAME_IS_WAITING_TO_REACT;
        }
    break;
        
    case GAME_IS_WAITING_TO_REACT:
        UART_PutString("STATE: GAME_IS_WAITING_TO_REACT Please Press a Button \n");
        if ( (event == display_value) || (event == display_value) ){
            UART_PutString("Good Button Press, Time: ");
            itoa(GAME_GetTime(),game_time_display,10);
            UART_PutString(game_time_display);
            UART_PutString("ms\n");
            average_reaction_time [game_rounds] = GAME_GetTime() ;
            game_score++;
        } 
        if( event != display_value && event != EV_TIMEOUT) {
            UART_PutString("Wrong Button\n"); 
        }
        
        if (event == EV_TIMEOUT){
            UART_PutString("Timeout\n"); 
        }
        state = GAME_ISDLE;
        SEVEN_Set_All(3);
        UART_PutString("End of Round\n");
        game_rounds++;
        if (game_rounds == MAX_GAME_ROUNDS)
        {
            state = GAME_IS_FINISHED;
        }
        break;
    case GAME_IS_FINISHED:
        UART_PutString("Game is Finished\n");
        UART_PutString("Score is: ");
        UART_PutChar(game_rounds+48);
        UART_PutString("\n");
        UART_PutString("Average Time is: ");
        itoa(GAME_GetAverageTime(average_reaction_time),total_average_time,10);
        UART_PutString(total_average_time);
        UART_PutString("ms");
        UART_PutString("\n");
        state = GAME_ISDLE;
        break;
    }
    GAME_SetButtonEvent(EV_NO_EVENT); 
    }
