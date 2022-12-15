#include "RobotContainer.h"

#include <frc2/command/button/JoystickButton.h>
#include <frc/XboxController.h> //used for enumerators

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings to commands here
	(JoystickButton(&controller, XboxController::Button::kA)
		&& JoystickButton(&controller, XboxController::Button::kB))
		.WhileActiveOnce(SpinAll(p_motors)); //if both are held, spin both
	
	(JoystickButton(&controller, XboxController::Button::kA)
		&& (!JoystickButton(&controller, XboxController::Button::kB)))
		.WhileActiveOnce(SpinTfx(p_motors)); //if a is held, but not b, spin Tfx

	(JoystickButton(&controller, XboxController::Button::kB)
		&& (!JoystickButton(&controller, XboxController::Button::kA)))
		.WhileActiveOnce(SpinTsrx(p_motors)); //if b is held, but not a, spin Tsrx
}