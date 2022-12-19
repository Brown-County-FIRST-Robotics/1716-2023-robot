#include "RobotContainer.h"
#include "Constants.h"

#include <frc2/command/button/JoystickButton.h>
#include <frc/XboxController.h> //used for enumerators
#include <units/time.h>
#include <frc2/command/ParallelRaceGroup.h>

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings to commands here
	(JoystickButton(&controller, XboxController::Button::kA)
		&& JoystickButton(&controller, XboxController::Button::kB))
		.WhileActiveOnce(SpinAll{&motors}); //if both are held, spin both
	
	(JoystickButton(&controller, XboxController::Button::kA)
		&& (!JoystickButton(&controller, XboxController::Button::kB)))
		.WhileActiveOnce(SpinTfx{&motors}); //if a is held, but not b, spin Tfx

	(JoystickButton(&controller, XboxController::Button::kB)
		&& (!JoystickButton(&controller, XboxController::Button::kA)))
		.WhileActiveOnce(SpinTsrx{&motors}); //if b is held, but not a, spin Tsrx

	JoystickButton(&controller, XboxController::Button::kY).WhenActive(ToggleSolenoid{&solenoidSubsystem}.WithTimeout(SOLENOIDSETLENGTH));
}