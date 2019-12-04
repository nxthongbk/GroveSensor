#include "PresenceDetector.h"

static uint32_t millis(){
    uint32_t millisecond = 0;
    timeval tv;
    gettimeofday(&tv,NULL);
    millisecond = tv.tv_sec *1000000 + tv.tv_usec/1000;
    return millisecond;
}

PresenceDetector::PresenceDetector(
    AK9753 &sensor,
    float threshold_presence,
    float threshold_movement,
    int detect_interval) : m_sensor(&sensor),
                           m_smoothers{0.05, 0.05, 0.05, 0.05, 0.05, 0.05},
                           m_interval(detect_interval),
                           m_last_time(millis()),
                           m_presences{},
                           m_movement(MOVEMENT_NONE),
                           m_threshold_presence(threshold_presence),
                           m_threshold_movement(threshold_movement)
{
}

PresenceDetector::~PresenceDetector()
{
}


void PresenceDetector::loop()
{
    float ir1, ir2, ir3, ir4, diff13, diff24;
    uint32_t now = millis();

    if (!m_sensor->dataReady())
        return;

    ir1 = m_sensor->getIR1();
    ir2 = m_sensor->getIR2();
    ir3 = m_sensor->getIR3();
    ir4 = m_sensor->getIR4();
    diff13 = ir1 - ir3;
    diff24 = ir2 - ir4;
    m_sensor->startNextSample();

    m_smoothers[0].addDataPoint(ir1);
    m_smoothers[1].addDataPoint(ir2);
    m_smoothers[2].addDataPoint(ir3);
    m_smoothers[3].addDataPoint(ir4);
    m_smoothers[4].addDataPoint(diff13);
    m_smoothers[5].addDataPoint(diff24);

    if (now - m_last_time > (uint32_t)m_interval)
    {
        float d;
        for (int i = 0; i < 4; i++)
        {
            d = m_ders[i] = m_smoothers[i].getDerivative();

            if (d > m_threshold_presence)
            {
                m_presences[i] = true;
            }
            else if (d < (-m_threshold_presence))
            {
                m_presences[i] = false;
            }
        }

        d = m_der13 = m_smoothers[4].getDerivative();

        if (d > m_threshold_movement)
        {
            m_movement &= 0b11111100;
            m_movement |= MOVEMENT_FROM_3_TO_1;
        }
        else if (d < (-m_threshold_movement))
        {
            m_movement &= 0b11111100;
            m_movement |= MOVEMENT_FROM_1_TO_3;
        }

        d = m_der24 = m_smoothers[5].getDerivative();
        if (d > m_threshold_movement)
        {
            m_movement &= 0b11110011;
            m_movement |= MOVEMENT_FROM_4_TO_2;
        }
        else if (d < (-m_threshold_movement))
        {
            m_movement &= 0b11110011;
            m_movement |= MOVEMENT_FROM_2_TO_4;
        }

        m_last_time = now;
    }
}

bool PresenceDetector::presentField1()
{
    bool r = m_presences[0];
    m_presences[0] = false;
    return r;
}

bool PresenceDetector::presentField2()
{
    bool r = m_presences[1];
    m_presences[1] = false;
    return r;
}

bool PresenceDetector::presentField3()
{
    bool r = m_presences[2];
    m_presences[2] = false;
    return r;
}

bool PresenceDetector::presentField4()
{
    bool r = m_presences[3];
    m_presences[3] = false;
    return r;
}
bool PresenceDetector::presentFullField(bool clear)
{
    if (clear)
    {
        return presentField1() || presentField2() || presentField3() || presentField4();
    }
    else
    {
        return m_presences[0] || m_presences[1] || m_presences[2] || m_presences[3];
    }
}

float PresenceDetector::getDerivativeOfIR1()
{
    return m_ders[0];
}

float PresenceDetector::getDerivativeOfIR2()
{
    return m_ders[1];
}

float PresenceDetector::getDerivativeOfIR3()
{
    return m_ders[2];
}

float PresenceDetector::getDerivativeOfIR4()
{
    return m_ders[3];
}

uint8_t PresenceDetector::getMovement()
{
    uint8_t r = m_movement;
    m_movement = MOVEMENT_NONE;
    return r;
}

float PresenceDetector::getDerivativeOfDiff13()
{
    return m_der13;
}

float PresenceDetector::getDerivativeOfDiff24()
{
    return m_der24;
}
