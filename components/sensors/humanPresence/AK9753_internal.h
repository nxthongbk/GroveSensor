#ifndef AK9753_INTERNAL_H
#define AK9753_INTERNAL_H

// Register addresses
#define REG_WIA1       0x00
#define REG_WIA2       0x01
#define REG_INFO1      0x02
#define REG_INFO2      0x03
#define REG_INTST      0x04
#define REG_ST1        0x05
#define REG_IR1L       0x06
#define REG_IR1H       0x07
#define REG_IR2L       0x08
#define REG_IR2H       0x09
#define REG_IR3L       0x0A
#define REG_IR3H       0x0B
#define REG_IR4L       0x0C
#define REG_IR4H       0x0D
#define REG_TMPL       0x0E
#define REG_TMPH       0x0F
#define REG_ST2        0x10
#define REG_ETH13H_LSB 0x11
#define REG_ETH13H_MSB 0x12
#define REG_ETH13L_LSB 0x13
#define REG_ETH13L_MSB 0x14
#define REG_ETH24H_LSB 0x15
#define REG_ETH24H_MSB 0x16
#define REG_ETH24L_LSB 0x17
#define REG_ETH24L_MSB 0x18
#define REG_EHYS13     0x19
#define REG_EHYS24     0x1A
#define REG_EINTEN     0x1B
#define REG_ECNTL1     0x1C
#define REG_CNTL2      0x1D

// EEPROM
#define REG_EKEY          0x50
#define EEPROM_ETH13H_LSB 0x51
#define EEPROM_ETH13H_MSB 0x52
#define EEPROM_ETH13L_LSB 0x53
#define EEPROM_ETH13L_MSB 0x54
#define EEPROM_ETH24H_LSB 0x55
#define EEPROM_ETH24H_MSB 0x56
#define EEPROM_ETH24L_LSB 0x57
#define EEPROM_ETH24L_MSB 0x58
#define EEPROM_EHYS13     0x59
#define EEPROM_EHYS24     0x5A
#define EEPROM_EINTEN     0x5B
#define EEPROM_ECNTL1     0x5C

// Valid sensor modes - Register ECNTL1
#define AK975X_MODE_STANDBY 0b000
#define AK975X_MODE_EEPROM_ACCESS 0b001
#define AK975X_MODE_SINGLE_SHOT 0b010
#define AK975X_MODE_0 0b100
#define AK975X_MODE_1 0b101
#define AK975X_MODE_2 0b110
#define AK975X_MODE_3 0b111

// Valid digital filter cutoff frequencies
#define AK975X_FREQ_0_3HZ 0b000
#define AK975X_FREQ_0_6HZ 0b001
#define AK975X_FREQ_1_1HZ 0b010
#define AK975X_FREQ_2_2HZ 0b011
#define AK975X_FREQ_4_4HZ 0b100
#define AK975X_FREQ_8_8HZ 0b101

// Movement
#define MOVEMENT_NONE        0b0000
#define MOVEMENT_FROM_1_TO_3 0b0001
#define MOVEMENT_FROM_3_TO_1 0b0010
#define MOVEMENT_FROM_2_TO_4 0b0100
#define MOVEMENT_FROM_4_TO_2 0b1000

#endif // AK9753_INTERNAL_H
