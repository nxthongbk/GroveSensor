#ifndef __GROVE_HUMAN_PRESENCE_SENSOR_H__
#define __GROVE_HUMAN_PRESENCE_SENSOR_H__

#include "legato.h"
#include "interfaces.h"
#include "i2cUtils.h"

#define AK975X_DEFAULT_ADDRESS 0x64

class Smoother
{
public:
    Smoother(float average_weight)
    {
        m_average_weight = average_weight;
        m_last_marked_value = 0;
        m_average = 0;
    }

    void addDataPoint(float data)
    {
        m_average = m_average_weight * data + (1 - m_average_weight) * m_average;
    }

    float getSmoothValue()
    {
        return m_average;
    }

    float getDerivative()
    {
        float d = m_average - m_last_marked_value;
        m_last_marked_value = m_average;
        return d;
    }

private:
    float m_last_marked_value;
    float m_average_weight;
    float m_average;
};


class AK9753
{
public:
  AK9753(uint8_t i2c_addr = AK975X_DEFAULT_ADDRESS);

  bool initialize(void) ;

  uint8_t getCompanyCode(void) ;
  uint8_t getDeviceID(void) ;
  bool    dataReady(void) ; /* returns ST1[0], read ST2 to clear */
  bool    dataOverRun(void) ; /* return ST1[1], read ST2, etc, to clear */
  uint8_t getINTST(void) ; /** return REG_INTST */
  uint8_t getST1(void) ;
  int16_t getRawIR1(void) ;
  float   getIR1(void) ;
  int16_t getRawIR2(void) ;
  float   getIR2(void) ;
  int16_t getRawIR3(void) ;
  float   getIR3(void) ;
  int16_t getRawIR4(void) ;
  float   getIR4(void) ;
  int16_t getRawTMP(void) ;
  float   getTMP(void) ;
  float   getTMP_F(void) ;
  uint8_t getST2(void) ;
  int16_t getETH13H(void) ;
  int16_t getETH13L(void) ;
  int16_t getETH24H(void) ;
  int16_t getETH24L(void) ;
  uint8_t getEHYS13(void) ;
  uint8_t getEHYS24(void) ;
  uint8_t getEINTEN(void) ;
  uint8_t getECNTL1(void) ;
  uint8_t getCNTL2(void) ;

  int16_t ETHpAtoRaw(float pA) ;
  void    setETH13H(int16_t value) ;
  void    setETH13L(int16_t value) ;
  void    setETH24H(int16_t value) ;
  void    setETH24L(int16_t value) ;
  uint8_t EHYSpAtoRaw(float pA) ;
  void    setEHYS13(uint8_t value) ;
  void    setEHYS24(uint8_t value) ;
  void    setEINTEN(uint8_t value) ;
  void    setECNTL1(uint8_t value) ;

  void    softReset(void) ;

  /**
   * This is an alias of getST2(), just for friendly name
   */
  void    startNextSample(void) ;


private:
  uint8_t m_addr;
  int readRegs(int addr, uint8_t *data, int len);
  int writeRegs(uint8_t *data, int len);
};

class Smoother;

#define NUM_SMOOTHER    6

class PresenceDetector
{
public:
  /**
   * @param sensor - the ref of AK9753 class instance
   * @param threshold_presence - compares with the derivative of the readings of a specific IR sensor (1/2/3/4)
   * @param threshold_movement - compares with the derivative of the difference value between IR sensor 1-3 or 2-4
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

#endif
