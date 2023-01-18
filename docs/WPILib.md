# WPILib Reference Document

## Table of Contents:

1. [Component Reference](#component-reference)
2. [Naming Conventions](#naming-conventions)
3. [3rd Party Vendor Libraries](#3rd-party-vendor-libraries)
4. [Smartdashboard/Shuffleboard Values](#smartdashboardshuffleboard-values)
	1. [Smartdashboard](#smartdashboard)
5. [Solenoids](#solenoids)

## Component Reference:

|Class Name|Include|Notes|
|--|--|--|
|`WPI_TalonFX`|`<ctre/Phoenix.h>`|-|
|`WPI_TalonSRX`|`<ctre/Phoenix.h>`|-|
|`frc::DoubleSolenoid`|`<frc/DoubleSolenoid.h>`|Positions can be accessed at `frc::DoubleSolenoid::Value::kForward`/`kReverse`/`kOff`. It should only be left on for a fraction of a second.(make more succinct)|
|`rev::CANSparkMax`|`<rev/CANSparkMax.h>`|-|

## Naming Conventions:

CC is camelCase, PC is PaskalCase  
> ***Note:*** *If a single instance of a class is being used, it should be named the same as its class, but in camelCase, assuming this causes no conflicts.*

|Structure|Convention|Suffix|
|-|-|-|
|Class|PC||
|Method/Function|PC||
|General Variable|CC||
|Parameter|CC|If a conflict is caused with a normal variable, add an `_a` suffix|

## 3rd Party Vendor Libraries

*3rd Party Vendor Libraries* are libraries created by component manufacturers. They are required for the use of some components (for example, the ctre library is required for use of Falcon500 TalonFX's). To install one, [copy the link to the `.json` file](https://docs.wpilib.org/en/stable/docs/software/vscode-overview/3rd-party-libraries.html#libraries), then, in VSCode, press the wpilib icon in the top right and go to `WPILib: Manage Vendor Libraries`>`Install new libraries (online)` and paste the link. Generally you will only need either the ctre or rev library.

## Smartdashboard/Shuffleboard Values:

One very useful feature is that you can post values to a driver station dashboard. This can be used to show values for debugging, or just display useful information while driving. We use shuffleboard for this, as it looks nice, is simple to use, and is feature rich. There are two methods for putting values on shuffleboard: through smartdashboard's API, which shuffleboard can read, or through shuffleboard's own API. Smartdashboard is simpler, and should be used as of now (though shuffleboard's API supports more features such as tabs, and so may be switched to in the future).

### Smartdashboard:

You can put Boolean, Numeric, or String values on smartdashboard very simply by including `<frc/smartdashboard/SmartDashboard.h>` and calling `frc::SmartDashboard::PutBoolean`/`PutNumber`/`PutString("Displayed Name", [value])`. The value should appear on shuffleboard on the `SmartDashbaord` tab. ([docs](https://docs.wpilib.org/en/stable/docs/software/dashboards/smartdashboard/displaying-expressions.html))

## Solenoids

Solenoids are the controllers for air-powered devices, generally pistons. They require an air compressor, which is automatically activated if a solenoid is declared. We use double solenoids, which have three settings: Forward, Reverse, and Off. The solenoid should never be in Forward or Reverse for longer than a fraction of a second (about 3 ms), just long enough to move the piston. It must then be set to Off again in order to avoid damaging the piston.
