#include "Robot.h"

#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
	//Pickup and placement position selector
	pickUpPos[0] = pickUpGrid
		.Add("Drop", false)
		.WithPosition(0, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[1] = pickUpGrid
		.Add("Left Slide", false)
		.WithPosition(1, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[2] = pickUpGrid
		.Add("Right Slide", false)
		.WithPosition(2, 0)
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
	
	//Update networktables info
	networkTableInst = nt::NetworkTableInstance::GetDefault();
	dashboardTable = networkTableInst.GetTable("1716DashboardInput");
	gameInfoTable = networkTableInst.GetTable("1716GameInfo");

	isAutonomous = gameInfoTable->GetBooleanTopic("isAutonomous").Publish();
	isTeleop = gameInfoTable->GetBooleanTopic("isTeleop").Publish();
	isRedAlliance = gameInfoTable->GetBooleanTopic("isRedAlliance").Publish();
	matchTime = gameInfoTable->GetDoubleTopic("matchTime").Publish();

	isRedAlliance.Set(frc::DriverStation::GetAlliance() == frc::DriverStation::Alliance::kRed);
	isAutonomous.Set(false);
	isTeleop.Set(false);
	matchTime.Set(0);

	//Pickup and place selector
	pickUpPublisher = dashboardTable->GetIntegerTopic("pickUpPos").Publish();
	placePublisher = dashboardTable->GetIntegerArrayTopic("placePos").Publish();

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

	//Update matchtime networktables variable
	matchTime.Set(frc::DriverStation::GetMatchTime());

	//Pickup and place position selectors:
	//Pick up
	for (int i = 0; i < 3; i++) {
		if (pickUpPos[i]->GetBoolean(false) && i != currentPickUp) {
			if (currentPickUp != -1) {
				pickUpPos[currentPickUp]->SetBoolean(false);
			}
			currentPickUp = i;
			pickUpPublisher.Set(i + 1);
		}
	}

	//Place
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 9; c++) {
			if (placePos[r][c]->GetBoolean(false) && (currentPlace[0] != r || currentPlace[1] != c)) {
				if (currentPlace[r] != -1) {
					placePos[currentPlace[0]][currentPlace[1]]->SetBoolean(false);
				}
				currentPlace[0] = r;
				currentPlace[1] = c;

				placeCoords.clear();
				placeCoords.push_back(r + 1);
				placeCoords.push_back(c + 1);
				placePublisher.Set(placeCoords); //this builds, DO NOT CHANGE
			}
		}
	}

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
	//Networktables variable update
	isAutonomous.Set(true);
}

void Robot::TeleopInit() {
	//Autonomous
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	//Networktables
	isAutonomous.Set(false);
	isTeleop.Set(true);

	//Controller logging
	frc::Shuffleboard::StartRecording();
	robotContainer.Init();
}

void Robot::TeleopPeriodic() {
	//Controller logging
	robotContainer.UpdateControllerLogging();
}

void Robot::DisabledInit() {
	//Networktables
	isTeleop.Set(false);
	
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