/*
 * Utilities.c
 *
 *  Created on: 12.04.2018
 *      Author: Dawid Adamik
 */

#include <Utilities.h>

void delay(int time)
{
    int i;
    for (i = 0; i < time * 4000; i++) {}
}
