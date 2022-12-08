#include "RobotContainer.h"

#include <frc2/command/button/JoystickButton.h>
#include <frc/XboxController.h> //used for enumerators

RobotContainer::RobotContainer() {
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings to commands here
	(frc2::JoystickButton(p_controller, frc::XboxController::Button::kA)
		&& frc2::JoystickButton(p_controller, frc::XboxController::Button::kB))
		.WhileActiveOnce(SpinAll(p_motors)); //if both are held, spin both (have to use WhileActiveOnce() because of && (docs))
	frc2::JoystickButton(p_controller, frc::XboxController::Button::kA).WhenHeld(SpinTfx(p_motors));
	frc2::JoystickButton(p_controller, frc::XboxController::Button::kB).WhenHeld(SpinTsrx(p_motors));
}