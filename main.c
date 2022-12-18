/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "global.h"
#include "game.h"
#include "seven.h"
#include "arcadia.h" 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//ISR which will increment the systick counter every ms
ISR(systick_handler)
{
    CounterTick(syst_counter);
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    //Set systick period to 1 ms. Enable the INT and start it.
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
	EE_systick_enable_int();
   
    // Start Operating System
    for(;;)	    
    	StartOS(OSDEFAULTAPPMODE);
}

void unhandledException()
{
    //Ooops, something terrible happened....check the call stack to see how we got here...
    __asm("bkpt");
}


uint8_t game_counter = 0;
uint16_t rand_time = 0;
uint16_t rand_time_disp = 0;
GAME_EVENT_t event = EV_NO_EVENT;
uint16_t counter_ticker = 0;
uint16_t game_average_time = 0;

/********************************************************************************
 * Task Definitions
 ********************************************************************************/

TASK(tsk_init)
{
    //Init MCAL Drivers
    UART_Start();
    SEVEN_Init();
    GAME_init();
    RGB_PWM_red_Start();
    RGB_PWM_green_Start();
    RGB_PWM_blue_Start();
    srand(time(0)); 
    
    //Reconfigure ISRs with OS parameters.
    //This line MUST be called after the hardware driver initialisation!
    EE_system_init();
	
    //Start SysTick
	//Must be done here, because otherwise the isr vector is not overwritten yet
    EE_systick_start();  
	
    SetRelAlarm(alrm_counter_ticker,1,1);
    SetRelAlarm(alrm_arcadian,1,1);
    ActivateTask(tsk_counter_ticker); 
    ActivateTask(tsk_arcadian_lights);
    ActivateTask(tsk_game);
    ActivateTask(tsk_background); 
    TerminateTask();
}
 
TASK(tsk_counter_ticker)
{
    counter_ticker++; 
    TerminateTask();
}


TASK(tsk_arcadian_lights)
{
    ARCADIA_Glow_faded(counter_ticker);
    TerminateTask();
}

TASK(tsk_game)
{
    EventMaskType ev = 0;
    while(1)
    {
        //Wait, read and clear the event
        WaitEvent(ev_button_1 | ev_button_2 |ev_button_3 |ev_seven_seg | ev_timeout);
        GetEvent(tsk_game,&ev);
        ClearEvent(ev);
 
        
    if ( ((ev & ev_button_1) || (ev & ev_button_2)) ){
        event = GAME_GetButtonEvent();
        SetRelAlarm(alrm_display,GAME_RandomTimer(5),0);
    }   

    if (ev & ev_seven_seg) {
        event = EV_DISPLAY_SEVEN;
        SetRelAlarm(alrm_timeout,GAME_RandomTimer(5),0);
    }
    if (ev & ev_timeout) {
        event = EV_TIMEOUT;
    }
        if (ev & ev_button_3) {
        //kill the game
        event = EV_NO_EVENT;
        state = GAME_IS_FINISHED;
    }
    GAME_start(event);
    }
    TerminateTask();
}

TASK(tsk_background)
{
    while(1)
    {
        //do something with low prioroty
        __asm("nop");
    }
}


/********************************************************************************
 * ISR Definitions
 ********************************************************************************/

ISR2(isr_button_press)
{
    if ( Button_1_Read() == 1 ) {
        GAME_SetButtonEvent(EV_BUTTON_1);        
        SetEvent(tsk_game,ev_button_1);
    }
    if ( Button_2_Read() == 1 ) {
        GAME_SetButtonEvent(EV_BUTTON_2);
        SetEvent(tsk_game,ev_button_2);
    }
    if ( Button_3_Read() == 1 ) {
        SetEvent(tsk_game,ev_button_3);
    }
}
/* [] END OF FILE */
