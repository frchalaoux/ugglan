#include <catch.h>

#include <thread>
#include <i2c_conn_stub.h>
#include <afro_esc.h>

static const double RPM_TOL = 1.0;
static const double FLOAT_TOL = 1e-1;
static const uint16_t ONE_S_IN_MS = 6000;

/*
Expected read buffer:
    Byte 0-1 : Rev counter set to 4321 [rpm] (assuming 1 sec of sleep)
    Byte 2-3 : Voltage set to 13.7 [V]
    Byte 4-5 : Temperature set to 24.7 [C]
    Byte 6-7 : Current set to 7.3 [A]
    Byte 8   : Alive byte set to true
*/
uint8_t READ_BUF[AFRO_READ_BUF_SIZE] =
{
    // 4321 * 7 * (6000 / 60000) ~ 3025 (0b00001011 | 0b11010001)
    0b00001011, // MSB
    0b11010001, // LSB

    // 13.7 / 32.25 * 65535 ~ 27840 = (0b01101100 | 0b11000000)
    0b01101100, // MSB
    0b11000000, // LSB

    // 65535 / (3300 / (exp((1 / (24.7 + 273.15) - 1/(25 + 273.15)) * 3900) * 1e4) + 1) ~
    // 49435 = (0b11000001 | 0b00011011)
    0b11000001, // MSB
    0b00011011, // LSB

    // 7.3 * 65535 / 73.53 + 32767 ~ 39273 = (0b10011001 | 0b01101001)
    0b10011001, // MSB
    0b01101001, // LSB

    AFRO_IF_ALIVE_BYTE
};

static I2cWriteMap WRITE_MAP = {AFRO_WRITE_THROTTLE_H};
static I2cReadBlockMap READ_MAP = { {AFRO_READ_REV_H, READ_BUF} };

TEST_CASE("afro esc")
{
    I2cConn i2c_conn;
    i2c_conn.set_write_map(WRITE_MAP);
    i2c_conn.set_read_block_map(READ_MAP);

    AfroEsc esc(&i2c_conn);

    SECTION("initialized")
    {
        REQUIRE(esc.get_status() == AFRO_STATUS_OK);
    }

    SECTION("read")
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ONE_S_IN_MS));
        esc.read();

        REQUIRE(esc.get_is_alive() == true);

        REQUIRE(fabs(esc.get_rpm() - 4321.0) <= RPM_TOL);
        REQUIRE(fabs(esc.get_voltage() - 13.7) <= FLOAT_TOL);
        REQUIRE(fabs(esc.get_current() - 7.3) <= FLOAT_TOL);
        REQUIRE(fabs(esc.get_temperature() - 24.7) <= FLOAT_TOL);

        REQUIRE(esc.get_status() == AFRO_STATUS_OK);
    }

    SECTION("write")
    {
        esc.write(1U);
        REQUIRE(esc.get_status() == AFRO_STATUS_OK);
    }
}
