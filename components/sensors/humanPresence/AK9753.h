#ifndef __GROVE_HUMAN_PRESENCE_SENSOR_H__
#define __GROVE_HUMAN_PRESENCE_SENSOR_H__

#include "legato.h"
#include "interfaces.h"
#include "i2cUtils.h"
#include "Smoother.h"

#define AK975X_DEFAULT_ADDRESS 0x64


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

#endif
