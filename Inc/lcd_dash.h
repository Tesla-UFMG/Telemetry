/*
 * lcd_dash.h
 *
 *  Created on: 30 de sept de 2019
 *      Author: Formula Tesla UFMG
 */

#ifndef LCD_DASH_H_
#define LCD_DASH_H_

#include <Interface_Master.h>

/* Nextion PAGE Enum */
typedef enum {
    PAGE0,
    PAGE1,
    PAGE2,
    PAGE3,
    PAGE4,
    PAGE5
}NextionPage_e;

/* Nextion Advise Enum */
typedef enum {
    NO_ADVICE,
    GLV,
    BSE
}NextionAdvice_e;

/* Nextion Page 0 data */
typedef struct {
    uint16_t actual;
    uint16_t previous;
}NextionData_t;


void uart3MessageReceived(void);
void nextionLoop(void);
void nextionTestLoop(void);


#endif /* LCD_DASH_H_ */