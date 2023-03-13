#include "subsystems/Led.h"

Led::Led(){};

void Led::SetColor(std::vector<int> color){
    red.SetVoltage(5 - (5 * color[0] / 255));
    green.SetVoltage(5 - (5 * color[1] / 255));
    blue.SetVoltage(5 - (5 * color[2] / 255));

};

void Led::SetColor(std::string color){
    SetColor(LedConst::COLORS[color]);

};