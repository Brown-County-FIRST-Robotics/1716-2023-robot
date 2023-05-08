#include "Robot.h"

#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
	//Pickup and placement position selector
	pickUpPos[0] = pickUpGrid
		.Add("Drop", false)
		.WithPosition(3, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[1] = pickUpGrid
		.Add("Left Slide", false)
		.WithPosition(4, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[2] = pickUpGrid
		.Add("Right Slide", false)
		.WithPosition(5, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	
	for (int r = 0; r < 2; r++) {
		for (int c = 0; c < 9; c++) {
			if (c % 3 != 1) {
				placePos[r][c] = placeGrid
					.Add("<" + std::to_string(c + (r * 9) + 1) + ">", false)
					.WithPosition(c, r)
					.WithWidget(frc::BuiltInWidgets::kToggleButton)
					.GetEntry();
			}
			else {
				placePos[r][c] = placeGrid
					.Add("[" + std::to_string(c + (r * 9) + 1) + "]", false)
					.WithPosition(c, r)
					.WithWidget(frc::BuiltInWidgets::kToggleButton)
					.GetEntry();
			}
		}
	}
	for (int c = 0; c < 9; c++) {
		placePos[2][c] = placeGrid
			.Add("(" + std::to_string(c + 19) + ")", false)
			.WithPosition(c, 2)
			.WithWidget(frc::BuiltInWidgets::kToggleButton)
			.GetEntry();
	}
	
	//LED lights:
	led.SetLength(LEDConst::LENGTH);
	led.SetData(ledBuffer);
	led.Start();
	SetAllLeds(0, 0, 0);
	ledChooser.SetDefaultOption("Knight Rider", 0);
	ledChooser.AddOption("Wierdness", 1);
	frc::SmartDashboard::PutData("LED Mode", &ledChooser);
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();

	//LEDs:
	ledUpdateSpeedCounter++;
	if (ledUpdateSpeedCounter > LEDConst::UPDATE_SPEED) {
		if (ledChooser.GetSelected() == 0)
			KnightRider();
		else if (ledChooser.GetSelected() == 1)
			Weirdness();

		led.SetData(ledBuffer);
		ledUpdateSpeedCounter = 0;
	}
}

void Robot::AutonomousInit() {
	//Autonomous
	autonomousCommand = robotContainer.GetAutonomousCommand();

	if (autonomousCommand != nullptr) {
		autonomousCommand->Schedule();
	}
	robotContainer.Init();
}

void Robot::TeleopInit() {
	//Autonomous
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	//Controller logging
	frc::Shuffleboard::StartRecording();
	robotContainer.Init();
}

void Robot::TeleopPeriodic() {
	//Controller logging
	robotContainer.UpdateControllerLogging();
}

void Robot::DisabledInit() {	
	//Controller Logging
	frc::Shuffleboard::StopRecording();
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif

void Robot::SetAllLeds(int r, int g, int b) {
	for (int i = 0; i < LEDConst::LENGTH; i++){
		ledBuffer[i].SetRGB(r, g, b);
	}
}

void Robot::SetLed(int id, int r, int g, int b) {
	ledBuffer[id].SetRGB(r, g, b);
}

void Robot::KnightRider() {
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

void Robot::Weirdness() {
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