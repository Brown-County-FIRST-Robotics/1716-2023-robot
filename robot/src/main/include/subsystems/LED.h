#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/AddressableLED.h>

#include "Constants.h"

class LED : public frc2::SubsystemBase {
public:
	LED();
	void SetAllLeds(int r, int g, int b);
	void SetAllLeds(std::vector<int> color);
	void SetLed(int id, int r, int g, int b);


	void Periodic() override;
private:
	frc::AddressableLED led{LEDConst::ID};
	std::array<frc::AddressableLED::LEDData, LEDConst::LENGTH> ledBuffer;
	int ledUpdateSpeedCounter = 0;
	void KnightRider();
	bool knightRiderLedGoingOut = true;
	int knightRiderIndex = 0;

	void Weirdness();
	bool weirdnessValueIsIncreasing = false;
	int weirdnessBrightness;
	int r;
	bool rUp = true;
	int g;
	bool gUp = true;
	int b;
	bool bUp = true;
};