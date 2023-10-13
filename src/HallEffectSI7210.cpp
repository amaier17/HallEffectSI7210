#include "HallEffectSI7210.h"

void HallEffectSI7210::init(HallEffectSI7210::Devs dev) {
    _dev = dev;
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Wire.begin();
    delay_us(100000);
    waitForInit();
    
    writeRegister(0xC4, 0); // Clear stop values
    writeRegister(0xC5, 1); // Enable auto increment
}

void HallEffectSI7210::waitForInit() {
    while (readRegister(0xC0) != _CHIPID)
        delay_us(100000);
}


void HallEffectSI7210::writeRegister(int reg_addr, int value) {
    int rc;
    do {
        delay_us(11);
        Wire.beginTransmission(_dev);
        Wire.write(reg_addr);
        Wire.write(value);
        rc = Wire.endTransmission();
    } while(rc);
}

void HallEffectSI7210::writeRegister(int reg_addr) {
    int rc;
    do {
        delay_us(11);
        Wire.beginTransmission(_dev);
        Wire.write(reg_addr);
        rc = Wire.endTransmission();
    } while(rc);
}

void HallEffectSI7210::delay_us(long us) {
    long target = micros() + us;
    while (target - (long)micros() >= 0) {
        yield();
        Particle.process();
    }
}

int HallEffectSI7210::readRegister(int reg_addr) {
    do {
        writeRegister(reg_addr);
        Wire.requestFrom(_dev, 1);
    } while(!Wire.available());

    return Wire.read();
}

int HallEffectSI7210::measure() {
    writeRegister(0xc4, 0x4);
    int msb = readRegister(0xc1);
    while (!(msb & 0x80)) {
        delay_us(100000);
        msb = readRegister(0xc1);
    }
    return (((msb & 0x7f) << 8) | readRegister(0xc2)) - 16384;
}
