#include "MotorSection.h"

namespace liboncilla {
namespace hw {

BrushlessParameterSection::BrushlessParameterSection(options::Section::Ptr s) 
	: options::ConfigSection(s)
	, d_hPGain(Section())
	, d_hIGain(Section())
	, d_hDGain(Section())
	, d_hStiffness(Section())
	, d_hDamping(Section())
	, d_hPreCompression(Section())
	, d_hMaxSpeed(Section())
	, d_hMaxAcceleration(Section())
	, d_hMaxTorque(Section()) {
}

BrushlessParameterSection::~BrushlessParameterSection() {
}

BrushlessParameterGroup::BrushlessParameterGroup(options::Section::Ptr s) 
	: options::DynamicConfigSection<BrushlessParameterSection>(s) {
	Section().AddSubSection("default", "Default parameters for knee motors");
}

BrushlessParameterGroup::~BrushlessParameterGroup() {
}

MotorDriverSection::MotorDriverSection(options::Section::Ptr s) 
	: options::ConfigSection(s)
	, d_hBoardID(Section())
	, d_hM1Params(Section())
	, d_hM2Params(Section()) {
}

MotorDriverSection::~MotorDriverSection() {
}

MotorDriverGroup::MotorDriverGroup(options::Section::Ptr s) 
	: options::ConfigSection(s)
	, d_shLeftFore(Section())
	, d_shRightFore(Section())
	, d_shLeftHind(Section())
	, d_shRightHind(Section()) {
	// sets default if od the boards
	LeftFore().BoardIDOption()->SetValue(1);
	RightFore().BoardIDOption()->SetValue(2);
	LeftHind().BoardIDOption()->SetValue(3);
	RightHind().BoardIDOption()->SetValue(4);
}

MotorDriverGroup::~MotorDriverGroup() {
}

MotorSection::MotorSection(options::Section::Ptr s) 
	: options::ConfigSection(s)
	, d_shParams(Section())
	, d_shDevices(Section()) {
}

MotorSection::~MotorSection() {
}

} // namespace hw
} // namespace liboncilla

