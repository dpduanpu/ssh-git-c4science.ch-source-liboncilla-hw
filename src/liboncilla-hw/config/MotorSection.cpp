#include "MotorSection.h"



namespace liboncilla {
namespace hw {

BrushlessMotorSection::BrushlessMotorSection(options::Section::Ptr s)
  : options::ConfigSection(s)
  , d_hPGain(Section())
  , d_hIGain(Section())
  , d_hDGain(Section())
  , d_hStiffness(Section())
  , d_hDamping(Section())
  , d_hPreCompression(Section())
  , d_hMaxSpeed(Section())
  , d_hMaxAcceleration(Section())
  , d_hMaxTorque(Section()){
}

BrushlessMotorSection::~BrushlessMotorSection(){}



BrushlessParameters::BrushlessParameters(options::Section::Ptr s)
  : options::DynamicConfigSection<BrushlessMotorSection>(s){
  Section().AddSubSection("hip","Default parameter for hip motors");
  Section().AddSubSection("knee","Default parameters for knee motors");
}

BrushlessParameters::~BrushlessParameters(){}



MotorDriverSection::MotorDriverSection(options::Section::Ptr s)
  : options::ConfigSection(s)
  , d_hBoardID(Section())
  , d_hM1Params(Section())
  , d_hM2Params(Section()){

}

MotorDriverSection::~MotorDriverSection(){}



MotorDriverBoards::MotorDriverBoards(options::Section::Ptr s)
  : options::ConfigSection(s)
  , d_shLeftFore(Section())
  , d_shRightFore(Section())
  , d_shLeftHind(Section())
  , d_shRightHind(Section()){

	LeftFore(). BoardIDOption()->SetValue(1);
	RightFore().BoardIDOption()->SetValue(2);
	LeftHind(). BoardIDOption()->SetValue(3);
	RightHind().BoardIDOption()->SetValue(4);

}

MotorDriverBoards::~MotorDriverBoards(){}



} // namespace hw
} // namespace liboncilla

