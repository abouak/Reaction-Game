# Reaction Game on FREE RTOS 
Embedded development: The control and logic of the Game is driven using a Free Real Time Operating System.

The Reaction Game is deployed using the CY8C5888LTQ-LP097 microcontroller from Cypress, the control of the 
peripherals is executed using the APIs provided by the manufacturer, the IDE used for this development is 
the PSOC Creator, also from Cypress and finnaly the operating system used is the RTOS ERIKA Enterprise, 
the whole game is based on the C programming language. 

### Game Description: 

The game consists of 10 rounds, once the microcontroller is started a greetings message shall be received
via UART, once a button is pressed an ISR will trigger the game, on the start of the round another messa-
ge will be transmited, after a random time a random number from 1 to 2 will appear on the Seven Segment 
LED, if the button is correctly pressen a message shall be sent by UART as well as reaction time in ms,
if the wrong button is pressed or if the Timeout ISR is triggered the round will end, the Timeout signal
is triggered in a random time as an ISR. Once the 10 rounds are finish the score and average time will be
printed via UART. 

The game is based on a State Machine which is triggered by the events from the RTOS, after each button
pressed an event will be dispatched to the state machine. 

### Arcandian Lights Description:

As the game is beign played there will be another taks on the RTOS which will display a fading light using
and RGB LED, the LED will display the colors Red, Blue and Green in a fading fashion going from Red to 
Green to Blue and then flashing all together. 

The color pattern and the time it takes to change the LED will be set using a const array table, which will 
be read by the API and which will be triggered using an event triggered task. 


