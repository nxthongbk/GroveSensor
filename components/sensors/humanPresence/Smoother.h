#ifndef SMOOTHER_H
#define SMOOTHER_H

class Smoother
{
public:
    Smoother(float average_weight);
    void addDataPoint(float data);
    float getSmoothValue();
    float getDerivative();

private:
    float m_last_marked_value;
    float m_average_weight;
    float m_average;
};

#endif // SMOOTHER_H
