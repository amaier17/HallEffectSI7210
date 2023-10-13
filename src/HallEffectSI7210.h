#ifndef __HALLEFFECT__
#define __HALLEFFECT__

#include <Arduino.h>
#include <Wire.h>

/**
 * Interface with the Si7210 Hall Effect Sensor.
 * 
 * This was written using the https://www.silabs.com/documents/public/data-sheets/si7210-datasheet.pdf datasheet.
 * 
 */
class HallEffectSI7210 {
    public:
        enum Devs : int {
            Si7210_B_02_IV = 0x31,
            Si7210_B_03_IV,
            Si7210_B_04_IV,
            Si7210_B_05_IV = 0x33,
        };
        void init(Devs dev);
        int measure();
    
    private:
        const int _CHIPID = 0x14;
        int _dev;
        void waitForInit();
        void writeRegister(int reg_addr, int value);
        void writeRegister(int reg_addr);
        int readRegister(int reg_addr);
        void delay_us(long us);
};

#endif // __HALLEFFECT__