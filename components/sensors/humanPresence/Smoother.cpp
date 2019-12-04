#include "Smoother.h"

Smoother::Smoother(float average_weight)
    : m_last_marked_value(0),
      m_average_weight(average_weight),
      m_average(0)
{
}

void Smoother::addDataPoint(float data)
{
    m_average = m_average_weight * data + (1 - m_average_weight) * m_average;
}

float Smoother::getSmoothValue()
{
    return m_average;
}

float Smoother::getDerivative()
{
    float d = m_average - m_last_marked_value;
    m_last_marked_value = m_average;
    return d;
}
