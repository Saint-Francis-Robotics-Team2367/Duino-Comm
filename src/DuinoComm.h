/*
 * DuinoComm.h
 *
 *  Created on: Feb 10, 2016
 *      Author: Harvey Dent
 */

#ifndef SRC_DUINOCOMM_H_
#define SRC_DUINOCOMM_H_

#define MAX_READ 8

#include "WPILib.h"

class DuinoComm {

public:
	enum AutoMode
	{
		LowGoal,
		Moat,
		Ramparts,
		RockWall,
		RoughTerrain,
		Nothing,
		Error
	};
	DuinoComm(int id,DriverStation *ds);
	virtual ~DuinoComm();
	void write(char* msg);
	std::string read();
	void updateStatus();
	int getAutoMode();
private:
	I2C *i2cComm;
	uint8_t id;
	DriverStation *ds;

};

#endif /* SRC_DUINOCOMM_H_ */
