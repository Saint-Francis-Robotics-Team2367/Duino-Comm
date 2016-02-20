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
int DuinoComm::getAutoMode() {
	std::string str = this->read();
	if((str[0] - '0')>=0 && (str[0] - '0')<=9)
		return str[0] - '0';
	else
		return -1;
}
