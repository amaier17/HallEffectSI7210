#include "HallEffectSI7210.h"

HallEffectSI7210 hallEffect;

void setup() {
    hallEffect.init(HallEffectSI7210::Devs::SI7210_B_04_IV);
}

void loop() {
    int value = hallEffect.measure();
    delay(1000);
}
