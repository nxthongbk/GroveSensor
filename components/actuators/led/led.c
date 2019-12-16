//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the mangOH Yellow Single Led interface.
 *
 * Provides the Led API services and plugs into the Legato Data Hub.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * For more detail refer to: http://wiki.seeedstudio.com/Grove-LED_Socket_Kit/
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "json.h"

typedef enum IOT0_GPIO_PIN {
	D2 = 2,
	D3,
	D4,
	D5
} IOT0_GPIO_PIN_t;

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

void led_Display(uint8_t port,
                 const char *LE_NONNULL state)
{
    switch (port)
    {
    case D2:
        if (strcmp(state, "on") == 0){
            D2_Activate();}
        else if (strcmp(state, "off") == 0){
            D2_Deactivate();}
        else{
            LE_ERROR("INPUT state is wrong");}
        break;
    case D3:
        if (strcmp(state, "on") == 0){
            D3_Activate();}
        else if (strcmp(state, "off") == 0){
            D3_Deactivate();}
        else{
            LE_ERROR("INPUT state is wrong");}
        break;
    case D4:
        if (strcmp(state, "on") == 0){
            D4_Activate();}
        else if (strcmp(state, "off")==0){
            D4_Deactivate();}
        else{
            LE_ERROR("INPUT state is wrong");}
        break;
    case D5:
        if (strcmp(state, "on") == 0){
            D5_Activate();}
        else if (strcmp(state, "off") == 0){
            D5_Deactivate();}
        else{
            LE_ERROR("INPUT state is wrong");}
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------------------------
/**
 * Handler function for updates to the enable setpoint from the Data Hub.
 */
//--------------------------------------------------------------------------------------------------
static void ledHandler(double timestamp,
                       const char *LE_NONNULL value,
                       void *contextPtr)
{
    char buffer[IO_MAX_STRING_VALUE_LEN];
    int port;

    le_result_t le_res;
    json_DataType_t json_data_type;

    // JSON message looks like this:
    // "
    // {
    //  \"port\": 5,
    //  \"state\": \"on/off\",
    // }
    // "
    if (!json_IsValid(value))
    {
        LE_ERROR("INVALID JSON string");
        return;
    }

    // port
    memset(buffer, 0, IO_MAX_STRING_VALUE_LEN);
    le_res = json_Extract(buffer,
                          IO_MAX_STRING_VALUE_LEN,
                          value,
                          "port",
                          &json_data_type);
    json_extract_dump(le_res);
    if (json_data_type != JSON_TYPE_NUMBER)
    {
        LE_ERROR("WRONG data type for port");
        return;
    }
    port = (int)json_ConvertToNumber(buffer);

    // state
    memset(buffer, 0, IO_MAX_STRING_VALUE_LEN);
    le_res = json_Extract(buffer,
                          IO_MAX_STRING_VALUE_LEN,
                          value,
                          "state",
                          &json_data_type);
    json_extract_dump(le_res);
    if (json_data_type != JSON_TYPE_STRING)
    {
        LE_ERROR("WRONG data type for state");
        return;
    }

    led_Display(port, buffer);
}

COMPONENT_INIT
{
    LE_FATAL_IF(D2_SetPushPullOutput(D2_ACTIVE_HIGH, true) != LE_OK,
                "Couldn't configure D2 as a push pull output");
    LE_FATAL_IF(D3_SetPushPullOutput(D3_ACTIVE_HIGH, true) != LE_OK,
                "Couldn't configure D3 as a push pull output");
    LE_FATAL_IF(D4_SetPushPullOutput(D4_ACTIVE_HIGH, true) != LE_OK,
                "Couldn't configure D24 as a push pull output");
    LE_FATAL_IF(D5_SetPushPullOutput(D5_ACTIVE_HIGH, true) != LE_OK,
                "Couldn't configure D5 as a push pull output");

    LE_FATAL_IF(io_CreateOutput("led",
                                IO_DATA_TYPE_JSON,
                                "string") != LE_OK,
                "Couldn't create led io output");

    io_AddJsonPushHandler("led",
                          ledHandler,
                          NULL);
}