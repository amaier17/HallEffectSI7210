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
            SI7210_B_02_IV = 0x31,
            SI7210_B_03_IV,
            SI7210_B_04_IV,
            SI7210_B_05_IV = 0x33,
        };
        HallEffectSI7210(Devs dev)
            : _enabled(false)
            , _dev(dev)
        {}
        bool measure(int &val);
        bool isEnabled() { return _enabled; }
    
    private:
        bool init();
        const int _CHIPID = 0x14;
        bool _enabled;
        int _dev;
        bool writeRegister(int reg_addr, int value);
        bool writeRegister(int reg_addr);
        bool readRegister(int reg_addr, int &value);
        void delay_us(long us);
};

#endif // __HALLEFFECT__
