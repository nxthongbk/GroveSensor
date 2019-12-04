#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include "humanPresenceSensor.h"
#include "i2cUtils.h"
#include "AK9753.h"

// need to adjust these sensitivities lower if you want to detect more far
// but will introduce error detection
#define sensitivity_presence    (float)6.0
#define sensitivity_movement    (float)10.0
#define detect_interval         (float)30 //milliseconds

/* Declare classe to read data */
AK9753 movementSensor;
PresenceDetector detector(movementSensor,
                          sensitivity_presence,
                          sensitivity_movement,
                          detect_interval);

static le_result_t setupSensor(void)
{
    // Turn on sensor
    return movementSensor.initialize() ? LE_OK : LE_FAULT;
}

le_result_t humanPresence_ReadPresentField1
(
    double *value
)
{
    *value = detector.presentField1();
    return LE_OK;
}

static void SamplePresentField1
(
    psensor_Ref_t ref,
    void *context
)
{
    detector.loop();

    double sample;

    le_result_t result = humanPresence_ReadPresentField1(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

le_result_t humanPresence_ReadPresentField2
(
    double *value
)
{
    *value = detector.presentField2();
    return LE_OK;
}

static void SamplePresentField2
(
    psensor_Ref_t ref,
    void *context
)
{
    detector.loop();

    double sample;

    le_result_t result = humanPresence_ReadPresentField2(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

le_result_t humanPresence_ReadPresentField3
(
    double *value
)
{
    *value = detector.presentField3();
    return LE_OK;
}

static void SamplePresentField3
(
    psensor_Ref_t ref,
    void *context
)
{
    detector.loop();

    double sample;

    le_result_t result = humanPresence_ReadPresentField3(&sample);

    if (result == LE_OK)
    {
        psensor_PushNumeric(ref, 0 /* now */, sample);
    }
    else
    {
        LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
    }
}

le_result_t humanPresence_ReadPresentField4
(
    double *value
)
{
    *value = detector.presentField4();
    return LE_OK;
}

static void SamplePresentField4
(
    psensor_Ref_t ref,
    void *context
)
{
    detector.loop();

    double sample;

    le_result_t result = humanPresence_ReadPresentField4(&sample);

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
    LE_FATAL_IF(setupSensor() != LE_OK, "Device not found. Check wiring.");
    psensor_Create("human_present_field1", DHUBIO_DATA_TYPE_NUMERIC, "", SamplePresentField1, NULL);
    psensor_Create("human_present_field2", DHUBIO_DATA_TYPE_NUMERIC, "", SamplePresentField2, NULL);
    psensor_Create("human_present_field3", DHUBIO_DATA_TYPE_NUMERIC, "", SamplePresentField3, NULL);
    psensor_Create("human_present_field4", DHUBIO_DATA_TYPE_NUMERIC, "", SamplePresentField4, NULL);
}
