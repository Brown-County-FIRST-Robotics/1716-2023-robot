#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/AnalogOutput.h>
#include <rev/CANSparkMax.h>

class Neo : public frc2::SubsystemBase {
 public:
	//Method declarations go here
	Neo(); //Constructor

	//void Periodic() override; //there are built in methods such as this, which is called once per frame

	
	void SetNeo(double speed);

	private:
		//subsystem components such as motors
		rev::CANSparkMax neo;
};
