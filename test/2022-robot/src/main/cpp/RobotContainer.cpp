#include <frc/XboxController.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/ParallelRaceGroup.h>

#include "RobotContainer.h"
#include "commands/ToggleClimberSolenoid.h"
#include "commands/Shoot.h"
#include "Constants.h"

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
	frc2::JoystickButton(&controller, frc::XboxController::Button::kB).WhenActive(ToggleClimberSolenoid{&arm}.WithTimeout(SolenoidConstants::SOLENOIDSETLENGTH));

	frc2::JoystickButton(&controller, frc::XboxController::Axis::kRightTrigger)
		.WhenHeld(Shoot{&arm, [this]{return controller.GetRightTriggerAxis();}}); //lambda expression allows for the command to get the trigger
}