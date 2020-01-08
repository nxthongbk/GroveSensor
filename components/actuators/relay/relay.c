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

#define RELAY_CONTROL_ADDRESS 0x10
#define RELAY_I2C_ADDR 0x11

static const char *Outlet[] = {"outlet1", "outlet2", "outlet3", "outlet4"};

static uint8_t RelayState = 0;
static const char *relay_i2c_bus = "/dev/i2c-5";

static void json_extract_dump(le_result_t res)
{
    switch (res)
    {
    case LE_OK:
        LE_INFO("json_Extract: successful");
        break;
    case LE_FORMAT_ERROR:
        LE_ERROR("json_Extract: there's something wrong with the input JSON string.");
        break;
    case LE_BAD_PARAMETER:
        LE_ERROR("json_Extract: there's something wrong with the extraction specification");
        break;
    case LE_NOT_FOUND:
        LE_ERROR("json_Extract: the thing we are trying to extract doesn't exist in the JSON input");
        break;
    case LE_OVERFLOW:
        LE_ERROR("json_Extract: the provided result buffer isn't big enough");
        break;
    default:
        LE_ERROR("json_Extract: %s", LE_RESULT_TXT(res));
        break;
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
    uint8_t relayNewState = 0x00;
    uint8_t relayMask = 0x00;

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

    for (int i = 0; i < NUM_ARRAY_MEMBERS(Outlet); i++)
    {
        le_res = json_Extract(buffer,
                              IO_MAX_STRING_VALUE_LEN,
                              value,
                              Outlet[i],
                              &json_data_type);
        json_extract_dump(le_res);
        if (le_res != LE_OK)
        {
            // No value specified for this output, leave as-is
            continue;
        }

        if (json_data_type != JSON_TYPE_STRING)
        {
            LE_ERROR("WRONG data type for outlet%d", i);
            return;
        }

        if (strcmp(buffer, "on") == 0)
        {
            relayNewState |= 1 << i;
            relayMask |= (1 << i);
        }
        else if (strcmp(buffer, "off") == 0)
        {
            // bit is already 0
            relayMask |= (1 << i);
        }
        else
        {
            LE_ERROR("Unsupported value (%s) for %s", buffer, Outlet[i]);
            return;
        }
    }

    uint8_t toWrite = ((RelayState & (~relayMask)) | relayNewState);
    uint8_t controlRelay[] = {RELAY_CONTROL_ADDRESS, toWrite};
    int res = i2cSendBytes(relay_i2c_bus, RELAY_I2C_ADDR, controlRelay, sizeof(controlRelay));
    if (res != 0)
    {
        LE_ERROR("Failed to set relay state");
    }
    else
    {
        RelayState = toWrite;
    }
}

COMPONENT_INIT
{
    /*
     * TODO: Is it possible to read the state of the relays from hardware? If so, it would be good
     * to initialize RelayState here based on the hardware state.
     */
    io_CreateOutput("relay",
                    IO_DATA_TYPE_JSON,
                    "string");

    io_AddJsonPushHandler("relay",
                          relayHandler,
                          NULL);
}
