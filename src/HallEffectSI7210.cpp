#include "HallEffectSI7210.h"

bool HallEffectSI7210::init() {
    bool rc;
    int val;
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Wire.begin();
    delay_us(100000);

    rc = readRegister(0xC0, val);
    if (!rc || val != _CHIPID) {
        _enabled = false;
    } else {
        if (!writeRegister(0xC4, 0) || !writeRegister(0xC5, 1)) // Clear stop values and enable auto increment
            _enabled = false;
        else
            _enabled = true;
    }

    return _enabled;
}

bool HallEffectSI7210::writeRegister(int reg_addr, int value) {
    int numRetries = 3;
    int rc;
    do {
        delay_us(11);
        Wire.beginTransmission(WireTransmission(_dev).timeout(100ms));
        Wire.write(reg_addr);
        Wire.write(value);
        rc = Wire.endTransmission();
    } while(rc && numRetries--);

    return rc == 0;
}

bool HallEffectSI7210::writeRegister(int reg_addr) {
    int numRetries = 3;
    int rc;
    do {
        delay_us(11);
        Wire.beginTransmission(WireTransmission(_dev).timeout(100ms));
        Wire.write(reg_addr);
        rc = Wire.endTransmission();
    } while(rc && numRetries--);

    return rc == 0;
}

void HallEffectSI7210::delay_us(long us) {
    long target = micros() + us;
    while (target - (long)micros() >= 0) {
        yield();
        Particle.process();
    }
}

bool HallEffectSI7210::readRegister(int reg_addr, int &value) {
    long endTime = millis() + 500;
    do {
        if (!writeRegister(reg_addr))
            return false;
        Wire.requestFrom(WireTransmission(_dev).quantity(1).timeout(100ms));
    } while(!Wire.available() && (endTime - millis()) > 0);

    if (endTime - millis() <= 0)
        return false;

    value = Wire.read();
    return true;
}

bool HallEffectSI7210::measure(int &val) {
    int msb, lsb;
    if (!_enabled && !init())
        return false;

    if (!writeRegister(0xc4, 0x4))
        goto disable;

    if (!readRegister(0xc1, msb))
        goto disable;

    while (!(msb & 0x80)) {
        delay_us(100000);
        if (!readRegister(0xc1, msb))
            goto disable;
    }

    if (!readRegister(0xc2, lsb))
        goto disable;
    val = (((msb & 0x7f) << 8) | lsb) - 16384;
    return true;

disable:
    _enabled = false;
    Wire.end();
    return false;
}
