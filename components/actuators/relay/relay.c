//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the mangOH Yellow 4-Channel SPDT Relay interface.
 *
 * Provides the Relay API services and plugs into the Legato Data Hub.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * The Grove - 4-Channel SPDT Relay has four single pole - double throw (SPDT) switches. 
 * It only requires low-voltage and low current signals to control those switches
 * For more detail refer to: http://wiki.seeedstudio.com/Grove-4-Channel_SPDT_Relay/
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "json.h"
#include "i2cUtils.h"

const char *relay_i2c_bus = "/dev/i2c-5";
#define RELAY_CONTROL_ADDRESS 0x10
#define RELAY_I2C_ADDR 0x11

static const struct RelayControl
{
    const char *outlet;
    int offset;
} Relay[] = {
    {
        "outlet1",
        1,
    },
    {
        "outlet2",
        2,
    },
    {
        "outlet3",
        4,
    },
    {
        "outlet4",
        8,
    }};

static void json_extract_dump(le_result_t res)
{
    if (res == LE_OK) {
        LE_INFO("json_Extract: successful");
    }
    if (res == LE_FORMAT_ERROR) {
        LE_ERROR("json_Extract: there's something wrong with the input JSON string.");
    }
    if (res == LE_BAD_PARAMETER) {
        LE_ERROR("json_Extract: there's something wrong with the extraction specification");
    }
    if (res == LE_NOT_FOUND) {
        LE_ERROR("json_Extract: the thing we are trying to extract doesn't exist in the JSON input");
    }
    if (res == LE_OVERFLOW) {
        LE_ERROR("json_Extract: the provided result buffer isn't big enough");
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Handler function for updates to the enable setpoint from the Data Hub.
 */
//--------------------------------------------------------------------------------------------------
static void relayHandler(double timestamp,
                       const char *LE_NONNULL value,
                       void *contextPtr)
{
    char buffer[IO_MAX_STRING_VALUE_LEN];
    uint8_t relay_state = 0x00;

    le_result_t le_res;
    json_DataType_t json_data_type;

    // JSON message looks like this:
    // "
    // {
    //  \"outlet1\": \"on\",
    //  \"outlet2\": \"on\",
    //  \"outlet3\": \"off\",
    //  \"outlet4\": \"on\"
    // }
    // "
    if (!json_IsValid(value))
    {
        LE_ERROR("INVALID JSON string");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        memset(buffer, 0, IO_MAX_STRING_VALUE_LEN);
        le_res = json_Extract(buffer,
                          IO_MAX_STRING_VALUE_LEN,
                          value,
                          Relay[i].outlet,
                          &json_data_type);
        json_extract_dump(le_res);
        if (json_data_type != JSON_TYPE_STRING)
        {
            LE_ERROR("WRONG data type for outlet%d", i);
            return;
        }
        if (strcmp(buffer, "on") == 0)
        {
            relay_state |= Relay[i].offset;
        }
    }

    uint8_t control_relay[2]= {RELAY_CONTROL_ADDRESS, relay_state};
    i2cSendBytes(relay_i2c_bus, RELAY_I2C_ADDR, control_relay, sizeof(control_relay));
}

COMPONENT_INIT
{
    io_CreateOutput("relay",
                    IO_DATA_TYPE_JSON,
                    "string");

    io_AddJsonPushHandler("relay",
                          relayHandler,
                          NULL);
}
