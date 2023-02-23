# Standard CAN IDs for robot components

Each subsystem on the robot gets 50 IDs, with the first 20 set aside for output (split in half between motors and non-motors) and the 20 after that set aside for input. The last 10 are miscellaneous. There are breaks in the table between motors and other outputs, as well as between each main section described above.

## Drivetrain

|Component|ID|
|---|---|
|Front Left Drive|0|
|Front Right Drive|1|
|Back Left Drive|2|
|Back Right Drive|3|
|||
|Pnuematics Control Module|10|
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

Solenoids have their own separate ID system off of the PCM, ranging from 1-16 on REV. Double solenoids have two IDs, one for forward and one for reverse.

|Solenoid|ID(s)|
|---|---|
|Front Left|0, 1|
|Front Right|2, 3|
|Back Left|4, 5|
|Back Right|6, 7|