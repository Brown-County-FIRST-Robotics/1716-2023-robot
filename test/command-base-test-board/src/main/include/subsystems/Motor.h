#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/AnalogOutput.h>
#include "rev/CANSparkMax.h"

class Motor : public frc2::SubsystemBase {
 public:
	//Method declarations go here
	Motor(); //Constructor

	//void Periodic() override; //there are built in methods such as this, which is called once per frame

	void Set(double speed);

	private:
		//subsystem components such as motors
		rev::CANSparkMax sparkMax;
	};
