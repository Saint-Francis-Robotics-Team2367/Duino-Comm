#include "WPILib.h"
#include "DuinoComm.h"
#include <cstdio>

class Robot: public IterativeRobot
{
private:

	DriverStation *ds;
	bool once = true;
	double lastWriteTime,lastSendTime;
	DuinoComm *dc;
	void RobotInit()
	{
		ds = &DriverStation::GetInstance();
		dc = new DuinoComm(8,ds);

	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

	}
	void TestInit()
	{
		once = true;
		lastWriteTime = Timer::GetFPGATimestamp();
		lastSendTime = Timer::GetFPGATimestamp()+0.5;
	}

	void TestPeriodic()
	{
		if(Timer::GetFPGATimestamp() - lastWriteTime > 1)
		{
			lastWriteTime = Timer::GetFPGATimestamp();
			dc->updateStatus();
		}
		if(Timer::GetFPGATimestamp() - lastSendTime > 0.5)
		{
			lastSendTime = Timer::GetFPGATimestamp();
			ds->ReportError(std::to_string(dc->getAutoMode()));
		}

	}
};

START_ROBOT_CLASS(Robot)
