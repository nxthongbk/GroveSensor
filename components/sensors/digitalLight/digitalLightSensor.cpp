#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include "TSL2561.h"
#include "i2cUtils.h"
#include "digitalLightSensor.h"


TSL2561_CalculateLux tsl2561;

le_result_t digitalLight_Read(uint16_t *value)
{
    *value = tsl2561.readVisibleLux();
    return LE_OK;
}

static void SampleDigitalLight(
    psensor_Ref_t ref,
    void *context
)
{
    uint16_t sample;

    le_result_t result = digitalLight_Read(&sample);

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
    LE_INFO("TSL2561 sensor start!!!");
    tsl2561.init();

    psensor_Create("digital_light", DHUBIO_DATA_TYPE_NUMERIC, "", SampleDigitalLight, NULL); 
}