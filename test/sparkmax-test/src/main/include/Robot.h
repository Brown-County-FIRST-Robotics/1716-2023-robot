// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>
#include <chrono>
#include "RobotContainer.h"
#include "rev/CANSparkMax.h"
#include <frc/XboxController.h>

#define MOTOR_ID 31
#define MAX_POWER 0.7

class Robot : public frc::TimedRobot {
  std::chrono::time_point<std::chrono::system_clock> m_motorStart;
  rev::CANSparkMax m_motor{ MOTOR_ID, rev::CANSparkMax::MotorType::kBrushless };
  frc::XboxController m_xbox{0};
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void DisabledInit() override;
  void DisabledPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;

 private:
  // Have it null by default so that if testing teleop it
  // doesn't have undefined behavior and potentially crash.
  frc2::Command* m_autonomousCommand = nullptr;

  RobotContainer m_container;
};
