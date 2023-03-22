# NetworkTables

## `1716Drive`
| Name             | Type    | Value                                                         |
|------------------|---------|---------------------------------------------------------------|
| x                | double  | The speed of the robot in the x direction                     |
| y                | double  | The speed of the robot in the y direction                     |
| rotation         | double  | The speed of the robot in the rotation direction              |
| isTank           | boolean | Whether the robot is in tank or mecanum mode                  |
| startAutoBalance | boolean | Tells the robot to start balancing                            |
| setArmFloor      | boolean | Tells the robot to drop the piece on the lowest level         |
| setArmPortal     | boolean | Tells the robot to drop the piece on the lowest level         |
| pickupObject     | boolean | Tells the robot to turn on the intake                         |
| setArmLow        | boolean | Tells the robot to set the arm on the lowest level            |
| setArmMedium     | boolean | Tells the robot to set the arm to the medium level            |
| setArmHigh       | boolean | Tells the robot to set the arm to the highest level           |
| setArmLowNode    | boolean | Tells the robot to set the arm to the lowest level cone node  |
| setArmHighNode   | boolean | Tells the robot to set the arm to the highest level cone node |
| retractArm       | boolean | Tells the robot to retract the arm                            |
| dropObject       | boolean | Tells the robot to drop piece it is holding                   |

### Notes on how some functions should work

- For some commands (ie: dropLow), The Pi will set the value to true, but once you finish it, set it to false
- For startAutoBalance, The Pi will set the value to `False` to signal that the auto balance should stop

## `1716Encoder`
| Name              | Type    | Value                                    |
|-------------------|---------|------------------------------------------|
| frontLeftEncoder  | double  | The position according to the encoder    |
| backLeftEncoder   | double  | The position according to the encoder    |
| frontRightEncoder | double  | The position according to the encoder    |
| backRightEncoder  | double  | The position according to the encoder    |
| encoderReset      | boolean | Reset the position of the encoders to 0  |

see [this](https://codedocs.revrobotics.com/cpp/classrev_1_1_c_a_n_spark_max.html#aa1a4166c3a802a2379500ba5b979a64f) and [this](https://codedocs.revrobotics.com/cpp/classrev_1_1_spark_max_relative_encoder.html)


## `1716Pigeon`
| Name   | Type    | Value                                       |
|--------|---------|---------------------------------------------|
| xAccel | double  | The accelerometer output in the x direction |
| yAccel | double  | The accelerometer output in the Y direction |
| zAccel | double  | The accelerometer output in the z direction |
| yaw    | double  | The gyroscope output in the yaw direction   |


## `1716GameInfo`
| Name            | Type     | Value                                          |
|-----------------|----------|------------------------------------------------|
| isAutonomous    | boolean  | If the robot is currently in autonomous mode   |
| isTeleop        | boolean  | If the robot is currently in teleop mode       |
| isRedAlliance   | boolean  | If the robot is on the red alliance            |
| matchTime       | double   | The time remaining in the match                |

To get these values, see [the wpilib documentation](https://github.wpilib.org/allwpilib/docs/release/cpp/classfrc_1_1_driver_station.html). 

## `1716DashboardInput`
| Name      | Type          | Value                                                                                                           |
|-----------|---------------|-----------------------------------------------------------------------------------------------------------------|
| pickUpPos | integer       | The location to pick up a gameobject from, ranging from 1-3, left to right                                      |
| placePos  | integer array | The location to place a gameobject at, index 0 is row, 1 is column, top left to bottom right, 3 rows, 9 columns |
