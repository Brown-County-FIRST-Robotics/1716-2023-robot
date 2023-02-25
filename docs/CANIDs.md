# Standard CAN IDs for robot components

Each subsystem on the robot gets 50 IDs, with the first 20 set aside for output (split in half between motors and non-motors) and the 20 after that set aside for input. The last 10 are miscellaneous. There are breaks in the table between motors and other outputs, as well as between each main section described above.

## Drivetrain

|Component|ID|
|---|---|
|Front Left Motor|0|
|Front Right Motor|1|
|Back Left Motor|2|
|Back Right Motor|3|
|||
|Pneumatic Hub|10|
|||
|Pigeon|20|

## Arm

|Component|ID|
|---|---|
|Shoulder Motor|50|
|Elbow Motor|51|
|||
|Shoulder Limit Switch|70|
|Elbow Limit Switch|71|

## Solenoids

Solenoids have their own separate ID system off of the Pneumatic Hub, ranging from 1-16. Double solenoids have two IDs, one for forward and one for reverse.

|Solenoid|ID(s)|
|---|---|
|Drivetrain|0, 1|
|Upper Arm Brake|2, 3|
|Upper Arm Direction|4, 5|
|Forearm Brake|6, 7|
|Forearm Direction|8, 9|
|Claw|10, 11|