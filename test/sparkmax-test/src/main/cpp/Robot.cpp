// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>
#include <frc/XboxController.h>
#include <frc/drive/DifferentialDrive.h>
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
}

/**
 * This function is called every robot packet, n>o matter the mode. Use
 * this for items like diagnostics that you want to run during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

/**
 * This function is called once each time the robot enters Disabled mode. You
 * can use it to reset any subsystem information you want to clear when the
 * robot is disabled.
 */
void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

/**
 * This autonomous runs the autonomous command selected by your {@link
 * RobotContainer} class.
 */
void Robot::AutonomousInit() {
  m_autonomousCommand = m_container.GetAutonomousCommand();

  if (m_autonomousCommand != nullptr) {
    m_autonomousCommand->Schedule();
  }

  m_motorStart = std::chrono::system_clock::now();
  m_motor.Set(0.0);
}

void Robot::AutonomousPeriodic() {
  std::chrono::time_point now = std::chrono::system_clock::now();
  std::chrono::duration<double> timeSinceStart = now - m_motorStart;
  double power = timeSinceStart.count() * 0.2;  
  m_motor.Set((power) > MAX_POWER ? MAX_POWER : (power)); 
  std::cout << m_motor.Get() << '\n';
  frc::SmartDashboard::PutNumber("spark max power", m_motor.Get());
}

void Robot::TeleopInit() {
  m_motor.Set(0.0);
}

/**
 * This function is called periodically during operator control.
 */
void Robot::TeleopPeriodic() { 
  // This makes sure that the autonomous stops running when
  // teleop starts running. If you want the autonomous to
  // continue until interrupted by another command, remove
  // this line or comment it out.
  if (m_autonomousCommand != nullptr) {
    m_autonomousCommand->Cancel();
    m_autonomousCommand = nullptr;
  }

  static bool pressed = false;

  //If A is pressed, start spinning the motor
  if(m_xbox.GetAButton())
  {
    if(!pressed)
    {
      m_motorStart = std::chrono::system_clock::now();
      pressed = true;
    }
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> timeSinceStart = now - m_motorStart; 
    double power = timeSinceStart.count() * 0.05;
    if(power > MAX_POWER)
      power = MAX_POWER; 
    m_motor.Set(power);
    frc::SmartDashboard::PutNumber("spark max power", m_motor.Get());
    std::cout << m_motor.Get() << '\n';
  }
  else
  {
    static std::chrono::time_point<std::chrono::system_clock> 
      start = std::chrono::system_clock::now(), 
      end = std::chrono::system_clock::now();

    if(pressed)
      start = std::chrono::system_clock::now();

    end = std::chrono::system_clock::now();
  
    double currentPower = m_motor.Get();
    //Slowly turn off the robot
    if(currentPower > 0.0)
    {
      std::chrono::duration<double> timePassed = end - start;
      currentPower -= 0.5 * timePassed.count();
    }
    else if(currentPower < 0.0)
      currentPower = 0.0;
    m_motor.Set(currentPower);

    pressed = false;
    frc::SmartDashboard::PutNumber("spark max power", m_motor.Get());
    std::cout << m_motor.Get() << '\n';

    start = std::chrono::system_clock::now(); 
  }
}

/**
 * This function is called periodically during test mode.
 */
void Robot::TestPeriodic() {
  
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
