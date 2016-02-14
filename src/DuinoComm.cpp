/*
 * DuinoComm.cpp
 *
 *  Created on: Feb 10, 2016
 *      Author: Harvey Dent
 */

#include <DuinoComm.h>

DuinoComm::DuinoComm(int id, DriverStation *ds) {
	this->id = id;
	this->i2cComm = new I2C(I2C::kMXP, id);
	this->ds = ds;
}

DuinoComm::~DuinoComm() {
	// TODO Auto-generated destructor stub
}

void DuinoComm::write(char* msg) {
	this->i2cComm->WriteBulk((unsigned char*) msg, strlen(msg));
}

std::string DuinoComm::read() {
	unsigned char msg[MAX_READ];
	memset(msg, 0, MAX_READ);
	this->i2cComm->Read(id, MAX_READ, msg);
	DriverStation::ReportWarning((char*) msg);
	return std::string((char*) msg);
}
void DuinoComm::updateStatus() {
	char msg[8];
	sprintf(msg, "B%.2f", ds->GetBatteryVoltage());
	this->write(msg);
	memset(msg, 0, 8);
	msg[0] = 'C';
	if (ds->IsDSAttached()) {
		strcat(msg, "DS");
		if (ds->IsFMSAttached())
			strcat(msg, "+FMS");
	} else
		strcat(msg, "D/C");
	this->write(msg);
}
DuinoComm::AutoMode DuinoComm::getAutoMode() {
	std::string str = this->read();
	switch (str[0]) {
	case '1':
		return DuinoComm::LowGoal;
	case '2':
		return DuinoComm::Moat;
	case '3':
		return DuinoComm::Ramparts;
	case '4':
		return DuinoComm::RockWall;
	case '5':
		return DuinoComm::RoughTerrain;
	case '6':
		return DuinoComm::Nothing;
	default:
		return DuinoComm::Error;
	}
}
