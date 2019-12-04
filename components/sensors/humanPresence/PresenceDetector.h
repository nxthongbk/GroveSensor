#ifndef PRESENCE_DETECTOR_H
#define PRESENCE_DETECTOR_H

#include "AK9753.h"

// Movement
#define MOVEMENT_NONE        0b0000
#define MOVEMENT_FROM_1_TO_3 0b0001
#define MOVEMENT_FROM_3_TO_1 0b0010
#define MOVEMENT_FROM_2_TO_4 0b0100
#define MOVEMENT_FROM_4_TO_2 0b1000

#define NUM_SMOOTHER    6

class PresenceDetector
{
public:
    /**
     * @param sensor - the ref of AK9753 class instance
     * @param threshold_presence - compares with the derivative of the readings of a specific IR
     *                             sensor (1/2/3/4)
     * @param threshold_movement - compares with the derivative of the difference value between IR
     *                             sensor 1-3 or 2-4
     * @param detect_interval - the interval of the presence detection, unit: millisecond
     */
    PresenceDetector(
        AK9753 &sensor,
        float threshold_presence = 10,
        float threshold_movement = 10,
        int detect_interval = 30);
    ~PresenceDetector();

    /**
     * This is the driven loop of the detector, should call this as fast as possible
     */
    void loop();

    /**
     * if an IR object is in the view of a specific sensor, clear after read
     * @return - true: an IR object is in view (entrance event)
     */
    bool presentField1();
    bool presentField2();
    bool presentField3();
    bool presentField4();

    /**
     * @param clear - clear after read or not
     * @return - true: if an IR object enters any view section of this sensor
     */
    bool presentFullField(bool clear = true);

    float getDerivativeOfIR1();
    float getDerivativeOfIR2();
    float getDerivativeOfIR3();
    float getDerivativeOfIR4();

    /**
     * Read the movement flags, clear after read
     * @return - one/OR of the MOVEMENT_FROM_X_TO_X macro
     */
    uint8_t getMovement();

    float getDerivativeOfDiff13();
    float getDerivativeOfDiff24();

private:
    AK9753 *m_sensor;
    Smoother m_smoothers[NUM_SMOOTHER];
    int m_interval;
    uint32_t m_last_time;

    bool m_presences[4];
    uint8_t m_movement;

    float m_threshold_presence, m_threshold_movement;
    float m_ders[4];
    float m_der13, m_der24;
};

#endif // PRESENCE_DETECTOR_H
