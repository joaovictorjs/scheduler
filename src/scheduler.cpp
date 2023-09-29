#include "scheduler.hpp"

namespace Scheduler {
	void Task::create_datetime(
		std::tm& a_receiver,
		const std::string& a_datetime
	){
		std::istringstream ss {a_datetime};
		ss.imbue(std::locale());
		ss >> std::get_time(&a_receiver, this->m_datetime_format.c_str());
		
		if(ss.fail()){
			a_receiver = {};
			ss.clear();
			throw Scheduler::FormatError("format error, expected format " + this->m_datetime_format);
		}
		
		a_receiver.tm_isdst = -1;
	}
}
