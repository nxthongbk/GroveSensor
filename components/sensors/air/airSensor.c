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
 * obtained by density conversion of industrial metal particles as equivalent particles, and is
 * suitable for use in industrial production workshops and the like.  The concentration of
 * particulate matter in the atmospheric environment is converted by the density of the main
 * pollutants in the air as equivalent particles, and is suitable for ordinary indoor and outdoor
 * atmospheric environments.  So you can see that there are two sets of data above.
 * For more detail refer to: http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include "airSensor.h"
#include "i2cUtils.h"

#define AIR_I2C_ADDR        0x40
static const char *air_sensor_i2c_bus = "/dev/i2c-5";
static uint8_t buf[29];


//--------------------------------------------------------------------------------------------------
/**
 * Read all of the registers from the device into the buf variable
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadDevice
(
    void
)
{
    LE_INFO("Start Reading Sensor");
    int res = i2cReceiveBytes(air_sensor_i2c_bus, AIR_I2C_ADDR, buf, sizeof(buf));
    if (res != NULL)
    {
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * PM1.0 are particles less than 1 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadIndustrialPM1_0
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    const int pmBufNum = 2;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * PM2.5 are particles less than 2.5 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadIndustrialPM2_5
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    int pmBufNum = 3;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Industrial Condition
 *
 * PM1.0 are particles less than 10 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadIndustrialPM10
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    int pmBufNum = 4;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * PM1.0 are particles less than 1 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadEnvironmentPM1_0
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    int pmBufNum = 5;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * PM2.5 are particles less than 2.5 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadEnvironmentPM2_5
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    int pmBufNum = 6;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read Air measurement in Environment Condition
 *
 * PM10 are particles less than 10 µm in diameter.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t air_ReadEnvironmentPM10
(
    uint16_t *value
)
{
    le_result_t readRes = air_ReadDevice();
    if (readRes != LE_OK)
    {
        return readRes;
    }

    int pmBufNum = 7;
    *value = (uint16_t)buf[pmBufNum * 2] << 8 | (buf[pmBufNum * 2 + 1]);

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

static void SampleAirIndustrialPM2_5
(
    psensor_Ref_t ref,
    void *context
)
{
    uint16_t sample;

    le_result_t result = air_ReadIndustrialPM2_5(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

static void SampleAirIndustrialPM10
(
    psensor_Ref_t ref,
    void *context
)
{
    uint16_t sample;

    le_result_t result = air_ReadIndustrialPM10(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

COMPONENT_INIT
{
    psensor_Create(
        "airIndustrialPM1_0",
        DHUBIO_DATA_TYPE_NUMERIC,
        "ug/m3",
        SampleAirIndustrialPM1_0,
        NULL);
    psensor_Create(
        "airIndustrialPM2_5",
        DHUBIO_DATA_TYPE_NUMERIC,
        "ug/m3",
        SampleAirIndustrialPM2_5,
        NULL);
    psensor_Create(
        "airIndustrialPM10",
        DHUBIO_DATA_TYPE_NUMERIC,
        "ug/m3",
        SampleAirIndustrialPM10,
        NULL);
}
