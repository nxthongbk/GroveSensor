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

  bool initialize() ;

  uint8_t getCompanyCode() ;
  uint8_t getDeviceID() ;
  bool    dataReady() ; /* returns ST1[0], read ST2 to clear */
  bool    dataOverRun() ; /* return ST1[1], read ST2, etc, to clear */
  uint8_t getINTST() ; /** return REG_INTST */
  uint8_t getST1() ;
  int16_t getRawIR1() ;
  float   getIR1() ;
  int16_t getRawIR2() ;
  float   getIR2() ;
  int16_t getRawIR3() ;
  float   getIR3() ;
  int16_t getRawIR4() ;
  float   getIR4() ;
  int16_t getRawTMP() ;
  float   getTMP() ;
  float   getTMP_F() ;
  uint8_t getST2() ;
  int16_t getETH13H() ;
  int16_t getETH13L() ;
  int16_t getETH24H() ;
  int16_t getETH24L() ;
  uint8_t getEHYS13() ;
  uint8_t getEHYS24() ;
  uint8_t getEINTEN() ;
  uint8_t getECNTL1() ;
  uint8_t getCNTL2() ;

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

  void    softReset() ;

  /**
   * This is an alias of getST2(), just for friendly name
   */
  void    startNextSample() ;


private:
  uint8_t m_addr;
  int readRegs(int addr, uint8_t *data, int len);
  int writeRegs(uint8_t *data, int len);
};

#endif
