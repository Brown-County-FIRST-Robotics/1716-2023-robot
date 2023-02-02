# WPILib Reference Document

## Table of Contents:

1. [Component Reference](#component-reference)
2. [Naming Conventions](#naming-conventions)
3. [3rd Party Vendor Libraries](#3rd-party-vendor-libraries)
4. [Dashboard Values](#dashboard-values)
	1. [Smartdashboard](#smartdashboard)
5. [NetworkTables](#networktables)
6. [Solenoids](#solenoids)

## Component Reference:

|Class Name|Include Path|Notes|
|--|--|--|
|`WPI_TalonFX`|`<ctre/Phoenix.h>`|-|
|`WPI_TalonSRX`|`<ctre/Phoenix.h>`|-|
|`frc::DoubleSolenoid`|`<frc/DoubleSolenoid.h>`|Positions are accessible at `frc::DoubleSolenoid::Value`; more info available [here](#solenoids)|
|`rev::CANSparkMax`|`<rev/CANSparkMax.h>`|-|
|`frc::PneumaticHub`|`<frc/PneumaticHub.h>`|-|

## Naming Conventions:

CC is camelCase, PC is PaskalCase  
> ***Note:** If a single instance of a class is being used, it should be named the same as its class, but in camelCase, assuming this causes no conflicts.*

|Structure|Convention|
|-|-|
|Class|PC|
|Method/Function|PC|
|Variable|CC|
|Parameter|CC|

## 3rd Party Vendor Libraries

*3rd Party Vendor Libraries* are libraries written and distributed by component manufacturers for use in interfacing with their components. To install REVLib or the CTRE Phoenix library:
1. Install their respective software. 
	- ForREVLib, download the [Java/C++ API](https://docs.revrobotics.com/sparkmax/software-resourc​es/spark-max-api-information#c++-and-java) (big orange button with "Download latest Java API" on it) and unzip into the C:\Users\Public\wpilib\2023 directory on Windows or ~/wpilib/2023 directory on Linux. 
	- For the CTRE Phoenix library, download and install the [Phoenix Framework](https://store.ctr-electronics.com/software/). 
2. Next, in VSCode, press the wpilib icon in the top right in your project and go to `WPILib: Manage Vendor Libraries` then `Install new libraries (offline)` and select `REVLib`, `Phoenix (v5)`, or both.

**Do not** install the `Phoenix (Pro)` library, as it is locked behind a paywall and will prevent your code from running. To uninstall a library, go to `WPILib: Manage Vendor Libraries` again and select `Manage current libraries`, then select any libraries you wish to remove and press enter.

## Dashboard Values:

One very useful feature of WPILib is the ability to post values to a dashboard. This can be used to show values for debugging, display useful information while driving, etc. Of the several options for dashboards included with WPILib (including making your own), we use shuffleboard because it looks nice, is simple to use, and is feature rich. There are three methods for putting values on shuffleboard: 
- Putting a value on networktables, which you can then manually move it onto smartdashboard and save the position
- Using the API for Smartdashboard, an older dashboard
	- Shuffleboard can read values from Smartdashboard's API and will automatically display them on its `SmartDashboard` tab
- Using shuffleboard's own API, which supports specifying which tab to place the value on, along with other features

As the Smartdashboard API is the simplest option, it is used most as of now (though since shuffleboard's API supports more features, it may be switched to in the future).

### Smartdashboard:

You can put Boolean, Numeric, or String values on the dashboard very simply by including `<frc/smartdashboard/SmartDashboard.h>` and calling `frc::SmartDashboard::PutBoolean`/`PutNumber`/`PutString` with the parameters `("Displayed Name", [value])`. The value should appear on shuffleboard on the `SmartDashboard` tab. ([docs](https://docs.wpilib.org/en/stable/docs/software/dashboards/smartdashboard/displaying-expressions.html))

## NetworkTables:

Another feature of WPILib is NetworkTables, which can be used to communicate values between the driverstation computer, the RoboRIO, and any coprocessors such as Raspberry Pi's the robot my have on it. All NetworkTable values are copied to all devices connected to networktables, and values are organized similarly to a filesystem, where the folders are "subtables" and the files are "topics", which is the NetworkTables term for values. You can reference a value by accessing its table similarly to opening a folder, then declaring a `subscriber` to read values or a `publisher` to write values.
1. In your header, include `<networktables/NetworkTable.h>`, `<networktables/NetworkTableInstance.h>`, and the topic for the data type you are working with, for example a float: `<networktables/FloatTopic.h>`
2. Declare a private `NetworkTablesInstance`, a `shared_ptr<nt::NetworkTable>`, and a subscriber or publisher for the data type:
	```C++
	nt::NetworkTableInstance networkTableInst; //the default networktables network
	std::shared_ptr<nt::NetworkTable> table; //the table to be used

	nt::FloatSubscriber subscriber; //able to read values

	nt::FloatPublisher publisher; //able to write values
	```
3. In the source file constructor, set the values to read from the topic you want:
	```C++
	networkTableInst = nt::NetworkTableInstance::GetDefault(); //the network the RoboRIO and DriverStation are connected to
	table = networkTableInst.GetTable("YourTable"); //if the table does not exist, it will be created

	subscriber = table->GetFloatTopic("YourTopic").Subscribe(0.0); //will create the topic if it doesn't exist, 
		//Subscribe() parameter is the default value

	publisher = table->GetFloatTopic("YourTopic").Publish(); //will create the topic if it doesn't exist, 
	```
4. Read or write values:
	```C++
	subscriber.Get(); //returns the float value topic is currently set to
	publisher.Set(124.5); //takes a float to set the topic value to
	```

## Solenoids

Solenoids are the controllers and valves for air-powered devices, generally pistons. We use double solenoids, which have three settings: Forward, Reverse, and Off. The solenoid should never be in Forward or Reverse for longer than a fraction of a second (about 3 ms), just long enough to move the piston. It must then be set to Off again in order to avoid damaging the piston. There is a sample subsystem and command available for them [here](Command_Based#generalized-solenoid-subsystem-and-command). ([docs](https://docs.wpilib.org/en/stable/docs/software/hardware-apis/pneumatics/pneumatics.html))
