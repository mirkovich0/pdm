/*
 * error.c
 *
 *  Created on: Aug 20, 2024
 *      Author: Mirko Serra
 */

#include "error.h"

void Error_Handler(void);

volatile void* error_value;
volatile int error_value_as_int;
volatile const char* reason_error;

void Handle_Error(const char* errormessage, void* value)
{
    error_value = value;
    error_value_as_int = (int) value;
    reason_error = errormessage;
    Error_Handler();
}
