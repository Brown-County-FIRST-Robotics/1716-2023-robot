#include <frc/AnalogOutput.h>
#include <frc2/command/SubsystemBase.h>

#include <string>
#include <vector>

#include "Constants.h"


class Led : public frc2::SubsystemBase {
public:
    Led();
    void SetColor(std::vector<int> color);
    void SetColor(std::string);

private:
    frc::AnalogOutput red{LedConst::RED_PIN};
    frc::AnalogOutput green{LedConst::GREEN_PIN};
    frc::AnalogOutput blue{LedConst::BLUE_PIN};

};