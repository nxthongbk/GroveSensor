//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the mangOH Red Air sensor interface.
 *
 * Provides the Air API services and plugs into the Legato Data Hub.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * The standard particulate matter mass concentration value refers to the mass concentration value
 * obtained by density conversion of industrial metal particles as equivalent particles, 
 * and is suitable for use in industrial production workshops and the like. 
 * The concentration of particulate matter in the atmospheric environment is converted by 
 * the density of the main pollutants in the air as equivalent particles, 
 * and is suitable for ordinary indoor and outdoor atmospheric environments. 
 * So you can see that there are two sets of data above.
 * For more detail refer to: http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include "i2c-utils.h"

#define AIR_I2C_ADDR        0x40
char air_sensor_i2c_bus[256] = "/dev/i2c-5";

uint8_t buf[29]={0,};
size_t buf_len = sizeof(buf);

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 1 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------

le_result_t air_ReadIndustrialPM1_0(uint16_t *value)
{
    *value = 0;
    int res = 0;

    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);

    if (res != 0)
        return LE_FAULT;
    if (buf_len != 29)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++)
    {
        if (i == 2) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 2.5 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------

le_result_t air_ReadIndustrialPM2_5(uint16_t *value)
{
    *value = 0;
    int res = 0;
    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);
    if (res != 0)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++)
    {
        if (i == 3) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 10 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------


le_result_t air_ReadIndustrialPM10(uint16_t *value)
{
    *value = 0;
    int res = 0;
    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);
    if (res != 0)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++)
    {
        if (i == 4) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 1 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------


le_result_t air_ReadEnvironmentPM1_0(uint16_t *value)
{
    *value = 0;
    int res = 0;
    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);
    if (res != 0)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++)
    {
        if (i == 5) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}
//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 2.5 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------

le_result_t air_ReadEnvironmentPM2_5(uint16_t *value)
{
    *value = 0;
    int res = 0;
    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);
    if (res != 0)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++) {
        if (i == 6) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * @return LE_OK if successful.
 * PM1.0 are particles less than 10 µm in diameter.
 * 
 */
//--------------------------------------------------------------------------------------------------

le_result_t air_ReadEnvironmentPM10(uint16_t *value)
{
    *value = 0;
    int res = 0;
    res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, buf_len);
    if (res != 0)
        return LE_FAULT;
    if (NULL == buf)
        return LE_FAULT;
    for (int i = 1; i < 8; i++) {
        if (i == 7) {
            *value = (uint16_t)buf[i*2]<<8|(buf[i*2+1]);
        }
    }
    sleep(5);
    return LE_OK;
}

static void SampleAirIndustrialPM1_0
(
    psensor_Ref_t ref,
    void *context
)
{
    uint16_t sample;

    le_result_t result = air_ReadIndustrialPM1_0(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

//Need to add for another type of Air Quality Sensor


COMPONENT_INIT
{
    psensor_Create("airIndustrialPM1_0", DHUBIO_DATA_TYPE_NUMERIC, "ug/m3", SampleAirIndustrialPM1_0, NULL);
}


