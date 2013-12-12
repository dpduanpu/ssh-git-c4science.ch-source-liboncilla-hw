/*
 * \file MainSection.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "MainSection.h"

namespace liboncilla {
namespace hw {

MainSection::MainSection(const options::Section::Ptr & base) 
	: options::ConfigSection(base)
	, d_hMainPriority(Section())
	, d_hSBCPPriority(Section())
	, d_hRBIOPriority(Section())
	, d_hTimestep(Section()) {

}

MainSection::~MainSection() {
}

} /* namespace hw */
} /* namespace liboncilla */
