# WPILib Reference Document

## Table of Contents:

1. [Component Reference](#component-reference)
2. [Naming Conventions](#naming-conventions)
3. [3rd Party Vendor Libraries](#3rd-party-vendor-libraries)
4. [Smartdashboard/Shuffleboard Values](#smartdashboardshuffleboard-values)
	1. [Smartdashboard](#smartdashboard)
5. [Solenoids](#solenoids)

## Component Reference:

|Class Name|Include Path|Notes|
|--|--|--|
|`WPI_TalonFX`|`<ctre/Phoenix.h>`|-|
|`WPI_TalonSRX`|`<ctre/Phoenix.h>`|-|
|`frc::DoubleSolenoid`|`<frc/DoubleSolenoid.h>`|Positions are accessible at `frc::DoubleSolenoid::Value`|
|`rev::CANSparkMax`|`<rev/CANSparkMax.h>`|-|

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
2. Next, in VSCode, press the wpilib icon in the top right in your project and go to `WPILib: Manage Vendor Libraries` then `Install new libraries (offline)` and select `REVLib` ***(check needed)***, `Phoenix (v5)`, or both.

**Do not** install the `Phoenix (Pro)` library, as it is locked behind a paywall and will prevent your code from running. To uninstall a library, go to `WPILib: Manage Vendor Libraries` again and select `manage current libraries` ***(check needed)***, then select any libraries you wish to remove and press enter.

## Smartdashboard/Shuffleboard Values:

One very useful feature of WPILib is the ability to post values to a smartdashboard ***(Check Name in DS)***. This can be used to show values for debugging, display useful information while driving, etc. Of the several options for smartdashboards included with WPILib (including making your own), we use shuffleboard because it looks nice, is simple to use, and is feature rich. There are three methods for putting values on shuffleboard: by putting a value on networktables (you can then manually move it onto smartdashboard), through WPILib's smartdashboard's API, which is meant for the older "smartdashboard", and shuffleboard can read and display on its `Smartdashboard` tab, or through shuffleboard's own API, which supports specifying which tab to place the value on, along with other features. The smartdashboard API is simpler, and should be used as of now (though since shuffleboard's API supports more features, it may be switched to in the future).

### Smartdashboard:

You can put Boolean, Numeric, or String values on the smartdashboard very simply by including `<frc/smartdashboard/SmartDashboard.h>` and calling `frc::SmartDashboard::PutBoolean`/`PutNumber`/`PutString` with the parameters `("Displayed Name", [value])`. The value should appear on shuffleboard on the `SmartDashboard` tab. ([docs](https://docs.wpilib.org/en/stable/docs/software/dashboards/smartdashboard/displaying-expressions.html))

## Solenoids

Solenoids are the controllers for air-powered devices, generally pistons. They require an air compressor, which is automatically activated if a solenoid is declared ***(Does this still work with REV?)*** and does not require any code interaction. We use double solenoids, which have three settings: Forward, Reverse, and Off. The solenoid should never be in Forward or Reverse for longer than a fraction of a second (about 3 ms), just long enough to move the piston. It must then be set to Off again in order to avoid damaging the piston. ([docs](https://docs.wpilib.org/en/stable/docs/software/hardware-apis/pneumatics/pneumatics.html))
