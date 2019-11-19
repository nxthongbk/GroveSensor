#ifndef AIR_SENSOR_H
#define AIR_SENSOR_H

LE_SHARED le_result_t mangOH_ReadIndustrialPM1_0(double *reading);
LE_SHARED le_result_t mangOH_ReadIndustrialPM2_5(double *reading);
LE_SHARED le_result_t mangOH_ReadIndustrialPM10(double *reading);

#endif // AIR_SENSOR_H
