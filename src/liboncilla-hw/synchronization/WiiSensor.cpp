#include "WiiSensor.h"

#include <limits>
#include <iostream>
#include <inttypes.h>

WiiSensor::WiiSensor()
	: I2CDevice(WiiSensor::default_address){
	d_data.assign(12,0); //initialize data to zero
}

WiiSensor::WiiSensor(I2CDevice::Address address)
	: I2CDevice(address){
	d_data.assign(12,0); //initialize data to zero
}

void WiiSensor::ReadValue(){
	std::vector<uint8_t> data(18);
	//data.reserve(18);
	data = Read(18); //read result of last sent command

	std::vector<uint8_t> command(21);
	//data.reserve(21);
	command.push_back(0x01);//forward to wii
	command.push_back(0x81);
	command.push_back(0x37);
	for(int i=0; i<18; i++){
		command.push_back(0xFF);
	}
	Write(command);
	for(int i=0; i<4; i++){
		//unpack the data, which is spread out over 3 bytes in the response
		uint16_t s = data[4+3*i]
		d_data[0+4*i] = ((s & 0x30)<<4) | data[2+3*i]
		d_data[1+4*i] = ((s & 0xC0)<<4) | data[3+3*i]
		d_data[2+4*i] = (s & 0x0F)
	}
}

I2CDevice::Address WiiSensor::getAddress(int toggle){
	return (I2CDevice::Address) (WiiSensor::default_address + (toggle << 1))
}

const WiiSensor::DataVector & Sensor::Data() const {
	return d_data;
}

