 
#ifndef GAME_H
#define GAME_H

/*****************************************************************************/
/* Global pre-processor symbols/macros and type declarations                 */
/*****************************************************************************/
#include "global.h"
    
 
//####################### Defines/Macros
/* Macros defined for the Reaction Game  */
#define MAX_GAME_ROUNDS                    10 /*< Max number of rounds in the Reaction Game */


//####################### Enumerations
/* Enumarations for the Game states and Events that react depending on the State */
 typedef enum {
  GAME_ISDLE,  
  GAME_START,  
  GAME_IS_WAITING_TO_REACT, 
  GAME_IS_FINISHED,
} GAME_STATE_t;

 typedef enum {
  EV_NO_EVENT,
  EV_BUTTON_1,  
  EV_BUTTON_2,  
  EV_DISPLAY_SEVEN, 
  EV_TIMEOUT
} GAME_EVENT_t;

/*****************************************************************************/
/* Extern global variables                                                   */
/*****************************************************************************/

/**
 * Global variables, the state of the games that needs to be checked by the 
 * game application and taks and the counter that is trigger by the OS and 
 * it functions as a timer for the game and the Arcadian lights 
 */
extern GAME_STATE_t state;
extern GAME_EVENT_t button;
extern uint16_t counter_ticker;

/*****************************************************************************/
/* API functions                                                             */
/*****************************************************************************/

/**
 * Initializes the game by displaying a message
 * @param None
 * @return None
 */
void            GAME_init();
/**
 * Sets the Button flag after pressing a button 
 * @param GAME_EVENT_t button_press - Event received by the press of a button
 * @return None
 */
void            GAME_SetButtonEvent(GAME_EVENT_t button_press);
/**
 * Get function that returns the flag of the pressed button
 * @param None
 * @return returns the pressed button either button_1 or button_2
 */
GAME_EVENT_t    GAME_GetButtonEvent();

/**
 * State machine implementation for the reaction game
 * @param GAME_EVENT_t state - Event that reacts depending on the state of the Game
 * @return None
 */
void            GAME_start(GAME_EVENT_t state);
/**
 * returns a random number in a specified range in seconds
 * @param uint16_t value - range of numbers for the timer, E.x., 3 is from 1 to 3
 * @return returns a random number in the specified range in seconds
 */
uint16_t        GAME_RandomTimer(uint16_t value);
/**
 * <Description>
 * @param uint16_t value - range of numbers for the timer, E.x., 3 is from 1 to 3
 * @return returns a random number in the specified range
 */
uint8_t         GAME_RandomDisplayNumber(uint8_t value);
/**
 * Sets the current value of the counter_tick of the tsk_counter_tick task 
 * to a flag to mark the initial time of the reaction
 * @param None
 * @return None
 */
void            GAME_SetTime();
/**
 * returns the time elapsed between the set of the timer and the current timer
 * @param None
 * @return returns the difference between the current time and the Setted time
 */
uint16_t        GAME_GetTime();
/**
 * Returns the average of each won round in the game
 * @param uint16_t * score_list - array of the list measured time for each
 * round of the game
 * @return returns the average value of the time measured for each correct
 * press of a button
 */
uint16_t        GAME_GetAverageTime(uint16_t * score_list);


#endif /* FILE_H */
