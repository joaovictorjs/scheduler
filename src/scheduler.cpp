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
	
	void Task::throw_if_invalid(
		const std::string& a_field_name, 
		int a_value_actual, 
		int a_value_min,
		int a_value_max
	){
		if(a_value_actual >= a_value_min && a_value_actual <= a_value_max) return;
		std::string msg {
			"range error, " + 
			a_field_name + " must be >= " + 
			std::to_string(a_value_min) + " and <= " + std::to_string(a_value_max) + 
			", but got " + std::to_string(a_value_actual)
		};
		throw Scheduler::RangeError(msg);
	}
	
	void Task::validate_datetime(const std::tm& a_datetime){
		int year {a_datetime.tm_year + 1900};
		bool is_leap {(year % 2 == 0 && year % 100 != 0) || (year % 400 == 0)};
		
		throw_if_invalid("month", a_datetime.tm_mon, 0, 11);
		
		int day {a_datetime.tm_mday};
		switch(a_datetime.tm_mon){
			case 1: throw_if_invalid("day", day, 1, is_leap ? 29 : 28); break;
			case 3: 
			case 5: 
			case 8:
			case 10: throw_if_invalid("day", day, 1, 30); break;
			default: throw_if_invalid("day", day, 1, 31);
		}
		
		throw_if_invalid("hour", a_datetime.tm_hour, 0, 23);
		throw_if_invalid("minute", a_datetime.tm_min, 0, 59);
		throw_if_invalid("second", a_datetime.tm_sec, 0, 59);
	}
	
	Task& Task::set_datetime_start(const std::string& a_datetime){
		if(a_datetime.empty()) {
			throw Scheduler::FormatError("format error, datetime start must no receive an empty value");
		}
		this->create_datetime(this->m_datetime_start, a_datetime);
		this->validate_datetime(this->m_datetime_start);
		return *this;
	}
	
	Task& Task::set_datetime_end(const std::string& a_datetime){
		if(a_datetime.empty()) {
			throw Scheduler::FormatError("format error, datetime end must no receive an empty value");
		}
		this->create_datetime(this->m_datetime_end, a_datetime);
		this->validate_datetime(this->m_datetime_end);
		return *this;
	}
	
	Task& Task::set_datetime_format(const std::string& a_format){
		if(a_format.empty()) {
			throw Scheduler::FormatError("format error, datetime format must no receive an empty value");
		}
		this->m_datetime_format = a_format;
		return *this;
	}
	
	Task& Task::set_interval(const int a_interval){
		if(a_interval <= 0){
			throw Scheduler::RangeError("range error, interval must be >= 0 and <= "+std::to_string(INT_MAX));
		}
		this->m_interval = a_interval;
		return *this;
	}
	
	long int Task::get_schedules(
		const std::time_t& a_datetime_start, 
		const std::time_t& a_datetime_end
	) const {
		if(a_datetime_start > a_datetime_end){
			throw Scheduler::OrderError("order error, datetime start must not be after datetime end");
		}
		
		long int remaining {a_datetime_end - a_datetime_start};
		
		if(this->m_interval >= remaining) return 1;
		
		return remaining / this->m_interval;
	};
	
	std::time_t Task::now() const {
		return std::time(nullptr);
	};
	
	Task& Task::run(const std::function<void(void)>& a_callback) {
		auto datetime_start {std::mktime(&this->m_datetime_start)};
		auto datetime_end {std::mktime(&this->m_datetime_end)};
		
		long int limit {this->get_schedules(datetime_start, datetime_end)};
		long int init {0};
		
		std::time_t datetime_actual {this->now()};

		if(datetime_start < datetime_actual){
			long int remaining {datetime_actual - datetime_start};
			init = remaining / this->m_interval;
		}
		
		for(long int it {init}; it < limit; it++){
			std::time_t now {this->now()};
			std::time_t schedule {datetime_start + (this->m_interval * it)};
			
			if(schedule < now) continue;
			
			if(schedule > now){
				std::this_thread::sleep_for(std::chrono::seconds(schedule - now));
			};
			
			a_callback();
		}
		
		return *this;
	}
}
