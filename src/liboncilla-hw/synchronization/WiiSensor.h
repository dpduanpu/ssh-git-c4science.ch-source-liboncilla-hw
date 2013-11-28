#ifndef WII_H_
#define WII_H_


#include <biorob-rbio/i2c/I2CDevice.h>
#include <biorob-rbio/i2c/Imu.h>
#include <inttypes.h>

class WiiSensor : public I2CDevice , public Sensor {
 public :

 	WiiSensor();
	WiiSensor(I2CDevice::Address address);
	virtual ~WiiSensor();

	virtual void ReadValue();
    static I2CDevice::Address getAddress(int toggle)
    const DataVector & Data() const;
 private :
 	typedef std::vector<uint16_t> DataVector;
 	const static I2CDevice::Address default_address = 0x50;
}

#endif //WII_H_

