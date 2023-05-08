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
	// TODO: add this later
	/*if (ledUpdateSpeedCounter > LEDConst::UPDATE_SPEED) {
		if (ledChooser.GetSelected() == 0)
			KnightRider();
		else if (ledChooser.GetSelected() == 1)
			Weirdness();
	}*/

		led.SetData(ledBuffer);
		ledUpdateSpeedCounter = 0;
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
