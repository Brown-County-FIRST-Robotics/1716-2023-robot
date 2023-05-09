#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/AddressableLED.h>

#include "Constants.h"

enum LedStatus {
	NONE = 0,
	OSCILATE = 1,
	KNIGHT_RIDER = 2,
	SINGLE_COLOR = 3,
	WEIRDNESS = 4,
};


class LED : public frc2::SubsystemBase {
public:
	LED();
	void SetAllLeds(int r, int g, int b);
	void SetAllLeds(std::vector<int> color);
	void SetLed(int id, int r, int g, int b);

	void SetLedStatus(int status);
	int GetLedStatus();

	void SetSingleColor(std::vector<int> color);
	std::vector<int> GetSingleColor();
	
	void SetOscilateColor(std::vector<std::vector<int>> colors);
	std::vector<std::vector<int>> GetOscilateColor();

	void Periodic() override;
private:
	frc::AddressableLED led{LEDConst::ID};
	std::array<frc::AddressableLED::LEDData, LEDConst::LENGTH> ledBuffer;
	
	int ledStatus=LedStatus::NONE;
	std::vector<int> singleColor;
	std::vector<std::vector<int>> oscilateColor;

	int oscilateInd = 0;
	int oscilateTimer = 0;

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