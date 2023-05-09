#include <iostream>
#include "subsystems/LED.h"

LED::LED() {
	led.SetLength(LEDConst::LENGTH);
	led.SetData(ledBuffer);
	led.Start();
	SetAllLeds(0, 0, 0);
}

void LED::SetAllLeds(int r, int g, int b) {
	for (auto i: ledBuffer){
		i.SetRGB(r, g, b);
	}
}

void LED::SetAllLeds(std::vector<int> color) {
	SetAllLeds(color[0], color[1], color[2]);
}

void LED::SetLed(int id, int r, int g, int b) {
	ledBuffer[id].SetRGB(r, g, b);
}

void LED::Periodic() {
	ledUpdateSpeedCounter++;
	if (ledUpdateSpeedCounter > LEDConst::UPDATE_SPEED) {
		if (ledStatus == LedStatus::KNIGHT_RIDER)
			KnightRider();
		else if (ledStatus == LedStatus::WEIRDNESS)
			Weirdness();
		ledUpdateSpeedCounter = 0;
	}
	if (ledStatus == LedStatus::OSCILATE){
		if(oscilateTimer==0)
			SetAllLeds(oscilateColor[oscilateInd]);
		oscilateTimer++;
		if(oscilateTimer==LEDConst::OSCILATION_SPEED){
			oscilateTimer=0;
			oscilateInd++;
		}
		if(oscilateInd==oscilateColor.size())
			oscilateInd=0;
	}
	if (ledStatus == LedStatus::SINGLE_COLOR)
		SetAllLeds(singleColor);
	if (ledStatus == LedStatus::NONE)
		SetAllLeds(0,0,0);
	led.SetData(ledBuffer);
}


void LED::KnightRider() {
	SetAllLeds(0, 0, 0);

	if (knightRiderLedGoingOut) {
		knightRiderIndex++;
		if (knightRiderIndex >= LEDConst::LENGTH - 1) {
			knightRiderLedGoingOut = false;
		}
		
		SetLed(knightRiderIndex, 255, 0, 0);
		for (int i = 0; i < LEDConst::NUM_OF_NIGHT_RIDER_TRAILING_LIGHTS; i++) {
			if (knightRiderIndex >= i + 1)
				SetLed(knightRiderIndex - i, 255 / pow(2, i + 1), 0, 0);
		}
	}
	else {
		knightRiderIndex--;
		if (knightRiderIndex <= 0) {
			knightRiderLedGoingOut = true;
		}

		SetLed(knightRiderIndex, 255, 0, 0);
		for (int i = 0; i < LEDConst::NUM_OF_NIGHT_RIDER_TRAILING_LIGHTS; i++) {
			if (knightRiderIndex <= LEDConst::LENGTH - (i + 1))
				SetLed(knightRiderIndex + i, 255 / pow(2, i + 1), 0, 0);
		}
	}
}

void LED::Weirdness() {
	SetAllLeds(0, 0, 0);

	if (rUp)
		r += 1;
	else
		r -= 1;
	if (gUp)
		g += 2;
	else
		g -= 2;
	if (bUp)
		b += 3;
	else
		b -= 3;

	if (r >= 256 || r <= 0) {
		rUp = !rUp;
	}
	if (g >= 256 || g <= 0) {
		gUp = !gUp;
	}
	if (b >= 256 || b <= 0) {
		bUp = !bUp;
	}

	for (int i = 0; i < LEDConst::LENGTH / 2; i += 2) {
		SetLed(i, r, g, b);
		led.SetData(ledBuffer);
	}
}


void LED::SetLedStatus(int status) { ledStatus=status; }
int LED::GetLedStatus() { return ledStatus; }
void LED::SetSingleColor(std::vector<int> color) { singleColor=color; }
std::vector<int> LED::GetSingleColor() { return singleColor; }
void LED::SetOscilateColor(std::vector<std::vector<int>> colors) { oscilateColor=colors; }
std::vector<std::vector<int>> LED::GetOscilateColor() { return oscilateColor; }
void LED::SetDrivetrainMode(frc::DoubleSolenoid::Value mode) {
	std::vector<int> currentColor = {0,255,0};
	std::vector<int> nextColor = {255,0,0};
	if(mode==DrivetrainConst::TANK_MODE){
		currentColor = {255,0,0};
		nextColor = {0,255,0};
	}
	std::erase_if(oscilateColor, [currentColor] (std::vector<int> i) {return i==currentColor;});
	oscilateColor.push_back(nextColor);
}

void LED::AddOscilateColor(std::vector<int> color){
	oscilateColor.push_back(color);
}

void LED::RemoveOscilateColor(std::vector<int> color) {
	std::erase_if(oscilateColor, [color] (std::vector<int> i) {return i==color;});
}
