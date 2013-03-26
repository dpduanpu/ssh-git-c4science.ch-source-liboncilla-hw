#pragma once


#include <tr1/memory>

//This file is only here because robo-xeno is not well packaged yet

//redefines the robo xeno io interface

namespace rx{

	class Input {
	public :
		typedef std::tr1::shared_ptr<rx::Input> Ptr;
		virtual ~Input();
		
		virtual void SendToProcess(double value) = 0;
	};
	
	class Output{
	public :
		typedef std::tr1::shared_ptr<rx::Output> Ptr;
		virtual ~Output();

		virtual double ReadFromProcess() = 0;
		
	};

}
