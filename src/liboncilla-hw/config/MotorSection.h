/**
 * \file Config.h
 *
 *  \date Dec 17,  2012
 *  \author Alexandre Tuleu
 */

#pragma once

#include <stdint.h>
#include <biorob-cpp/options/Config.h>

#include <libsbcp/devices/amarsi/MotorDriver.h>

/**
 *
 */
namespace liboncilla {
namespace hw {

class BrushlessParameterSection : public options::ConfigSection {
public:

	BrushlessParameterSection(options::Section::Ptr s);
	virtual ~BrushlessParameterSection();

	BCPP_ADD_OPTION(PGain, "p-gain", uint16_t, 400, "P-gain of the motor")
	BCPP_ADD_OPTION(IGain, "i-gain", uint16_t, 20 , "I-gain of the motor")
	BCPP_ADD_OPTION(DGain, "d-gain", uint16_t, 200, "D-gain of the motor")

	BCPP_ADD_OPTION(Stiffness, "stiffness", uint16_t, 0,
	                "virtual compliance stiffness")
	BCPP_ADD_OPTION(Damping, "damping", uint16_t, 0,
	                "virtual compliance damping")
	BCPP_ADD_OPTION(PreCompression, "precompression", uint16_t, 1100,
			"virtual compliance precompression")

	BCPP_ADD_OPTION(MaxSpeed, "max-speed", uint16_t, 32000, "Maximal speed")
	BCPP_ADD_OPTION(MaxAcceleration, "max-acceleration", uint16_t, 32000,
	                "Maximal speed")
	BCPP_ADD_OPTION(MaxTorque, "max-torque", uint16_t, 1100, "Maximal speed")

};

class BrushlessParameterGroup : public options::DynamicConfigSection< BrushlessParameterSection > {
public:
	BrushlessParameterGroup(options::Section::Ptr s);
	virtual ~BrushlessParameterGroup();
};

class MotorDriverSection : public options::ConfigSection {
public:
	MotorDriverSection(options::Section::Ptr s);
	virtual ~MotorDriverSection();

	BCPP_ADD_OPTION(BoardID, "id", int, -1, "Board ID to use")
	BCPP_ADD_OPTION(M1Params, "m1-parameters", std::string, "default",
	                "Template parameter to use with M1 motor")
	BCPP_ADD_OPTION(M2Params, "m2-parameters", std::string, "default",
	                "Template parameter to use with M2 motor")

};

class MotorDriverGroup : public options::ConfigSection {
public:
	MotorDriverGroup(options::Section::Ptr s);
	virtual ~MotorDriverGroup();

	BCPP_ADD_SUBSECTION(MotorDriverSection,
	                    LeftFore,
	                    "left-fore",
	                    "left fore motors of oncilla");
	BCPP_ADD_SUBSECTION(MotorDriverSection,
	                    RightFore,
	                    "right-fore",
	                    "right fore motors of oncilla");
	BCPP_ADD_SUBSECTION(MotorDriverSection,
	                    LeftHind,
	                    "left-hind",
	                    "left hind motors of oncilla");
	BCPP_ADD_SUBSECTION(MotorDriverSection,
	                    RightHind,
	                    "right-hind",
	                    "right hind motors of oncilla");

};

class MotorSection : public options::ConfigSection {
public:
	MotorSection(options::Section::Ptr s);
	virtual ~MotorSection();

	BCPP_ADD_OPTION(CommunicationErrorThreshold, "communication-error-threshold", int , 1,
	                "Number of SBCP communication error, that will threshold a failure of the robot, (i.e. unhandled exception)")


	BCPP_ADD_SUBSECTION(BrushlessParameterGroup,
	                    Params,
	                    "params",
	                    "Group of parameters that could be applied to a motor. A 'default' "
	                    "one is always existing,  but you can create any number of thoses. "
	                    "You could use them to apply it to a perticular motor in the "
	                    "'device' section");

	BCPP_ADD_SUBSECTION(MotorDriverGroup,
	                    Devices,
	                    "device",
	                    "Configuration for the motor device neede for oncilla");

};

} /* namespace hw */
} /* namespace liboncilla */

