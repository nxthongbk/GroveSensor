#ifndef HUMANPRESENCE_SENSOR_H
#define HUMANPRESENCE_SENSOR_H

LE_SHARED le_result_t humanPresence_ReadIR1(uint16_t *reading);
LE_SHARED le_result_t humanPresence_ReadIR2(uint16_t *reading);
LE_SHARED le_result_t humanPresence_ReadIR3(uint16_t *reading);
LE_SHARED le_result_t humanPresence_ReadIR4(uint16_t *reading);

#endif // HUMANPRESENCE_SENSOR_H
