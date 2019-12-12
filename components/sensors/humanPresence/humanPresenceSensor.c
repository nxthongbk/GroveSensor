//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the mangOH Yellow Human Presence sensor interface.
 *
 * Provides the Human Presence API services and plugs into the Legato Data Hub.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * Human Presence sensor is composed of four quantum IR sensors provides 16-bits data outputs
 * For more detail refer to: http://wiki.seeedstudio.com/Grove-Human_Presence_Sensor-AK9753/
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include "humanPresenceSensor.h"
#include "i2cUtils.h"

#define HUMAN_PRESENCE_I2C_ADDR 0x64
static const char *human_presence_sensor_i2c_bus = "/dev/i2c-5";

#define REG_WIA2 0x01
#define REG_ST1 0x05
#define REG_IR1L 0x06
#define REG_IR2L 0x08
#define REG_IR3L 0x0A
#define REG_IR4L 0x0C
#define REG_ST2 0x10

uint8_t REG_CNTL2[2] = {0x1D, 0xFF};
uint8_t ECNTL1[2] = {0x1C, 0x2C};
uint8_t REG_EINTEN[2] = {0x1B, 0x1F};

enum HumanPresenceMeasurementType
{
    MEAS_T_HUMAN_PRESENCE_IR1,
    MEAS_T_HUMAN_PRESENCE_IR2,
    MEAS_T_HUMAN_PRESENCE_IR3,
    MEAS_T_HUMAN_PRESENCE_IR4,
};

static const struct HumanPresenceMeasurement
{
    const char *name;
    const char *dhubResource;
    uint8_t irRegister;
} Measurements[] = {
    [MEAS_T_HUMAN_PRESENCE_IR1] = {
        "HumanPresenceIR1",
        "humanPresenceIR1",
        REG_IR1L,
    },
    [MEAS_T_HUMAN_PRESENCE_IR2] = {
        "HumanPresenceIR2",
        "humanPresenceIR2",
        REG_IR2L,
    },
    [MEAS_T_HUMAN_PRESENCE_IR3] = {
        "HumanPresenceIR3",
        "humanPresenceIR3",
        REG_IR3L,
    },
    [MEAS_T_HUMAN_PRESENCE_IR4] = {
        "HumanPresenceIR4",
        "humanPresenceIR4",
        REG_IR4L,
    },
};


//--------------------------------------------------------------------------------------------------
/**
 * Retrieve a IR sample from the sensor.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------

le_result_t humanPresence_ReadGeneric(
    const struct HumanPresenceMeasurement *measurement, ///< specifier of measurement to read
    uint16_t *value //
)
{
    int res;
    uint8_t buf[2];

    //Check data ready
    res = i2cReceiveBytes_v2(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, REG_ST1, buf, 1);
    
    if (res != LE_OK)
    {
        return LE_FAULT;
    }

    if ((buf[0] & 0x01) != 0x01)
    {
        LE_INFO("No data ready to get");
        return LE_FAULT;
    }

    res = i2cReceiveBytes_v2(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, measurement->irRegister, buf, sizeof(buf));
    if (res != LE_OK)
    {
        return LE_FAULT;
    }
    else
    {
        *value = (buf[1] << 8) | buf[0];
    }

    //Get Next Sample
    res = i2cReceiveBytes_v2(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, REG_ST2, buf, 1);
    if (res != 0)
    {
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Read human presence IR1 measurement 
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t humanPresence_ReadIR1
(
    uint16_t *value  
)
{
    return humanPresence_ReadGeneric(&Measurements[MEAS_T_HUMAN_PRESENCE_IR1], value);
}

//--------------------------------------------------------------------------------------------------
/**
 * Read human presence IR2 measurement 
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t humanPresence_ReadIR2
(
    uint16_t *value  
)
{
    return humanPresence_ReadGeneric(&Measurements[MEAS_T_HUMAN_PRESENCE_IR2], value);
}

//--------------------------------------------------------------------------------------------------
/**
 * Read human presence IR3 measurement 
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t humanPresence_ReadIR3
(
    uint16_t *value  
)
{
    return humanPresence_ReadGeneric(&Measurements[MEAS_T_HUMAN_PRESENCE_IR3], value);
}

//--------------------------------------------------------------------------------------------------
/**
 * Read human presence IR4 measurement 
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t humanPresence_ReadIR4
(
    uint16_t *value  
)
{
    return humanPresence_ReadGeneric(&Measurements[MEAS_T_HUMAN_PRESENCE_IR4], value);
}

static void SampleHumanPresenceGeneric(
    psensor_Ref_t ref,
    void *context)
{
    uint16_t sample;
    const struct HumanPresenceMeasurement *measurement = context;

    le_result_t result = humanPresence_ReadGeneric(measurement, &sample);

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
    uint8_t id[1];
    //Initialize
    //softReset
    LE_FATAL_IF(
        i2cSendBytes(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, REG_CNTL2, sizeof(REG_CNTL2)) != 0,
        "Failed to reset sensor");

    //Check chip ID
    i2cReceiveBytes_v2(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, REG_WIA2, id, 1);
    if (id[0] != 0x13)
    {
        LE_ERROR("WRONG Chip ID: %x", id[0]);
    }

    //set mode and filter freq
    LE_FATAL_IF(
        i2cSendBytes(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, ECNTL1, sizeof(ECNTL1))!= 0,
        "Failed to set mode and filter freq");

    //enable interrupt
    LE_FATAL_IF(
        i2cSendBytes(human_presence_sensor_i2c_bus, HUMAN_PRESENCE_I2C_ADDR, REG_EINTEN, sizeof(REG_EINTEN))!= 0,
        "enable interrupt");

    for (int i = 0; i < NUM_ARRAY_MEMBERS(Measurements); i++)
    {
        psensor_Create(
            Measurements[i].dhubResource,
            DHUBIO_DATA_TYPE_NUMERIC,
            "",
            SampleHumanPresenceGeneric,
            (void *)&Measurements[i]);
    }
}
