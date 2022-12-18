 
#ifndef ARCADIA_H
#define ARCADIA_H

/*****************************************************************************/
/* Global pre-processor symbols/macros and type declarations                 */
/*****************************************************************************/

#include "project.h"


/*****************************************************************************/
/* API functions                                                             */
/*****************************************************************************/

/**
 * Function that enables the fade the glow of each LED of the RGB LED, 
 * everytime the ticker paramis equal to the time specified 
 * in the RG_glowtable. once the values of the 3 LEDs matches the 
 * to the next row table entries it will move on .
 * @param uint16_t ticker
 * @return None
 */
void ARCADIA_Glow(uint16_t ticker);

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
void ARCADIA_Glow_faded(uint16_t ticker);

/**
 * the function will set the value of the value_to_level to be equal to 
 * the value_to_compare param and return its value.
 * @param uint16_t value_to_level - value to increase or decrease
 * @param uint16_t value_to_compare - value to compare to
 * @return value_to_level
 */
uint16_t level_value (uint16_t value_to_level, uint16_t value_to_compare);


#endif /* FILE_H */
