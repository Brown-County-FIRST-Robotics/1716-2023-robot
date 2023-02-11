# NetworkTables

## `1716Drive`
| Name              | Type    | Value                                                       |
|-------------------|---------|-------------------------------------------------------------|
| X                 | double  | The speed of the robot in the x direction                   |
| Y                 | double  | The speed of the robot in the y direction                   |
| Rotation          | double  | The speed of the robot in the rotation direction            |
| isTank            | boolean | Whether the robot is in tank or mecanum mode                |
| isMecanum         | boolean | Whether the robot is in mecanum or tank mode                |
| startAutoBalance  | boolean | Tells the robot to start balancing                          |
| pickupFloorPiece  | boolean | Tells the robot to pickup the piece on the floor            |
| pickupPortalPiece | boolean | Tells the robot to pickup the piece from the portal         |
| setLow            | boolean | Tells the robot to drop the piece on the lowest level       |
| setMedium         | boolean | Tells the robot to drop the piece on the medium level       |
| setHigh           | boolean | Tells the robot to drop the piece on the highest level      |
| setLowNode        | boolean | Tells the robot to drop the piece on the lowest level node  |
| setHighNode       | boolean | Tells the robot to drop the piece on the highest level node |
| dropObject        | boolean | Tells the robot to drop piece it is holding                 |

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
| Name             | Type    | Value                                       |
|------------------|---------|---------------------------------------------|
| XAccel           | double  | The accelerometer output in the x direction |
| YAccel           | double  | The accelerometer output in the Y direction |
| Gyro             | double  | The gyroscope output in the yaw direction   |


## `1716GameInfo`
| Name            | Type     | Value                                          |
|-----------------|----------|------------------------------------------------|
| isAutonomous    | boolean  | If the robot is currently in autonomous mode   |
| isTeleop        | boolean  | If the robot is currently in teleop mode       |
| isRedAlliance   | boolean  | If the robot is on the red alliance            |
| matchTime       | double   | The time remaining in the match                |

To get these values, see [the wpilib documentation](https://github.wpilib.org/allwpilib/docs/release/cpp/classfrc_1_1_driver_station.html). 
