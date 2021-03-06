/*
LPS25H pressure sensor, see data sheet
https://www.pololu.com/file/download/LPS25H.pdf?file_id=0J761
*/

#ifndef LPS25H_H
#define LPS25H_H

#include <cstdint>
#include <pololu_alt_imu.h>

inline const uint8_t LPS25H_I2C_ADDRESS = 0x5D;

// Register addresses
inline const uint8_t LPS25H_RES_CONF     = 0x10;
inline const uint8_t LPS25H_CTRL_REG1    = 0x20;
inline const uint8_t LPS25H_PRESS_OUT_XL = 0x28;
inline const uint8_t LPS25H_PRESS_OUT_L  = 0x29;
inline const uint8_t LPS25H_PRESS_OUT_H  = 0x2A;
inline const uint8_t LPS25H_TEMP_OUT_P_L = 0x2B;
inline const uint8_t LPS25H_TEMP_OUT_P_H = 0x2C;

// Buffer indices and size
inline const uint8_t LPS25H_BUF_PRES_SIZE    = 3;
inline const uint8_t LPS25H_BUF_PRESS_OUT_XL = 0x00;
inline const uint8_t LPS25H_BUF_PRESS_OUT_L  = 0x01;
inline const uint8_t LPS25H_BUF_PRESS_OUT_H  = 0x02;

inline const uint8_t LPS25H_BUF_TEMP_SIZE    = 2;
inline const uint8_t LPS25H_BUF_TEMP_OUT_P_L = 0x00;
inline const uint8_t LPS25H_BUF_TEMP_OUT_P_H = 0x01;

// Scaling, resolution and offset
inline const double LPS25H_PRES_SCALE = 100; // Convert from hPa to Pa
inline const double LPS25H_PRES_RESOLUTION = 4096;

inline const double LPS25H_TEMP_RESOLUTION = 480;
inline const double LPS25H_TEMP_OFFSET = 42.5; // [C]

// Config and read map
inline const ConfigMap LPS25H_CONFIG_MAP = {
    {LPS25H_CTRL_REG1, 0b11000000}, // Enable pressure and temperature for continues output at 25 Hz
    {LPS25H_RES_CONF, 0b00001111}  // Set highest resolution on pressure and temperature
    };

inline const ReadMap LPS25H_READ_MAP = {
    {LPS25H_PRESS_OUT_XL, LPS25H_BUF_PRES_SIZE},
    {LPS25H_TEMP_OUT_P_L, LPS25H_BUF_TEMP_SIZE},
    };

class Lps25h : public PololuAltImu
{
public:
    Lps25h(I2cConn& i2c_conn);

    double get_pressure();
    double get_temperature();
};

#endif /* LPS25H_H */
