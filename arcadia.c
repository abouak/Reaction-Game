
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "project.h"
#include "arcadia.h"
#include <stdio.h>

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint8_t sequence_counter;
static uint8_t sequence_fader;
const uint16_t RG_glowtable [9][4] = {
//Red Green Blue TimeInMS
{255, 0, 0, 500},
{0, 255, 0, 500},
{0, 0, 255, 500},
{0, 0, 0, 100},
{255, 255, 255, 100},
{0, 0, 0, 100},
{255, 255, 255, 100},
{0, 0, 0, 100},
{255, 255, 255, 100}
};

const uint16_t RG_glowtable_fade [9][4] = {
//Red Green Blue TimeInMS
{10, 0, 0, 10}, //1000 original 
{0, 10, 0, 10},
{0, 0, 10, 10},
{0, 0, 0, 5},
{10, 10, 10, 5},
{0, 0, 0, 5},
{10, 10, 10, 5},
{0, 0, 0, 5},
{10, 10, 10, 5}
};

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static uint16_t fader_r = 0;
static uint16_t fader_g = 0;
static uint16_t fader_b = 0;

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * the function will set the value of the value_to_level to be equal to 
 * the value_to_compare param and return its value.
 * @param uint16_t value_to_level - value to increase or decrease
 * @param uint16_t value_to_compare - value to compare to
 * @return value_to_level
 */
uint16_t level_value (uint16_t value_to_level, uint16_t value_to_compare){
    if (value_to_level < value_to_compare) {value_to_level++;}
    if (value_to_level > value_to_compare) {value_to_level--;}
    if (value_to_level == value_to_compare) {return value_to_level;}
    return value_to_level;
}


/**
 * Function that enables the fade the glow of each LED of the RGB LED, 
 * everytime the ticker paramis equal to the time specified 
 * in the RG_glowtable. the fade value will try to match the value on 
 * the table by either increasing or decreasing its value once the 
 * values of the 3 LEDs matches the table entries it will move on 
 * to the next row.
 * @param uint16_t ticker
 * @return None
 */
void ARCADIA_Glow_faded(uint16_t ticker){
    uint16_t show_ticker = ticker;
    
    if (ticker != 0 && (ticker % RG_glowtable[sequence_counter][3] == 0) )
    {
        printf("Vlaue = %d/n",show_ticker);
        fader_r = level_value(fader_r, RG_glowtable_fade[sequence_counter][0]);
        fader_g = level_value(fader_g, RG_glowtable_fade[sequence_counter][1]);
        fader_b = level_value(fader_b, RG_glowtable_fade[sequence_counter][2]);
        RGB_PWM_red_WriteCompare(fader_r*1000);
        RGB_PWM_green_WriteCompare(fader_g*1000);
        RGB_PWM_blue_WriteCompare(fader_b*1000);
        if (fader_r == RG_glowtable_fade[sequence_counter][0] &&
            fader_g == RG_glowtable_fade[sequence_counter][1] &&
            fader_b == RG_glowtable_fade[sequence_counter][2]){
                sequence_counter++;
                    if (sequence_counter == 8){ 
                        sequence_counter =0; 
                    }
            }
    }
}


/**
 * Function that enables the fade the glow of each LED of the RGB LED, 
 * everytime the ticker paramis equal to the time specified 
 * in the RG_glowtable. once the values of the 3 LEDs matches the 
 * to the next row table entries it will move on .
 * @param uint16_t ticker
 * @return None
 */
void ARCADIA_Glow(uint16_t ticker){
    
    if (ticker != 0 && (ticker % RG_glowtable[sequence_counter][3] == 0) )
    {
        RGB_PWM_red_WriteCompare(RG_glowtable[sequence_counter][0]);
        RGB_PWM_green_WriteCompare(RG_glowtable[sequence_counter][1]);
        RGB_PWM_blue_WriteCompare(RG_glowtable[sequence_counter][2]);
        sequence_counter++;
    }
    if (sequence_counter == 8){ sequence_counter =0; }
}