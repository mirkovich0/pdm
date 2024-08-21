/*
 * API_sensor.h
 *
 *  Created on: Aug 21, 2024
 *      Author: Mirko Serra
 */

#ifndef API_INC_API_SENSOR_H_
#define API_INC_API_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>

void sensor_Init();
float sensor_ReadTemp();
float sensor_ReadHumidity();
void sensor_Process();

#endif /* API_INC_API_SENSOR_H_ */
