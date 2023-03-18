// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <chrono>
#include <thread>

void Robot::RobotInit() {
  // Default to a length of 60, start empty output
  // Length is expensive to set, so only set it once, then just update data
  m_led.SetLength(kLength);
  m_led.SetData(m_ledBuffer);
  m_led.Start();
}

void Robot::RobotPeriodic() {
  // Fill the buffer with a rainbow
  //static int sleep = 220;
  chaser();
  std::this_thread::sleep_for(std::chrono::milliseconds(220));

  //sleep+= -20;

  // Set the LEDs
  m_led.SetData(m_ledBuffer);
}

void Robot::Rainbow() {
  // For every pixel
  Robot::allOff();
  for (int i = 0; i < kLength; i++) {
    // Calculate the hue - hue is easier for rainbows because the color
    // shape is a circle so only one value needs to precess
    const auto pixelHue = (firstPixelHue + (i * 180 / kLength)) % 180;
    // Set the value
    m_ledBuffer[i].SetHSV(pixelHue, 255, 128);
  }
  // Increase by to make the rainbow "move"
  firstPixelHue += 3;
  // Check bounds
  firstPixelHue %= 180;
}

void Robot::chaser(){
  static int i = 0;
  static int size = 24;

  Robot::allOff();
  m_ledBuffer[i].SetRGB(255,0,0);
  for(int j = 0; j < (size - 1); j++){
    m_ledBuffer[(i + (j * 2) % kLength)].SetRGB(255,0,0);
  }
  i++;
  if(i >= kLength){
    i=0;
  }
}

void Robot::knightRider(){
  static int i = 0;

  Robot::allOn(0,0,0);
}

void Robot::allOn(int R, int G, int B){
  for (int i = 0; i < kLength; i++){
    m_ledBuffer[i].SetRGB(R,G,B);
  }
}

void Robot::strobe(){
  static bool isOn = false;

  if (isOn)
    Robot::allOff();
  else
    Robot::allOn(255,0,0);
  isOn = !isOn;
}

void Robot::allOff(){
  Robot::allOn(0,0,0);
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
