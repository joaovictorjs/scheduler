#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <chrono>
#include <string>
#include <climits>
#include <functional>
#include <thread>

namespace Scheduler {
	class RangeError : public std::runtime_error{
		public:
			using std::runtime_error::runtime_error;
	};
	
	class FormatError : public std::runtime_error{
		public:
			using std::runtime_error::runtime_error;
	};
	
	class OrderError : public std::runtime_error{
		public:
			using std::runtime_error::runtime_error;
	};
	
	class Task {
		private:
			std::tm m_datetime_start, m_datetime_end;
			std::string m_datetime_format {"%Y-%m-%d %H:%M:%S"};
			int m_interval;
			
		protected:
			void create_datetime(
				std::tm& a_receiver,
				const std::string& a_datetime
			);
			
			void throw_if_invalid(
				const std::string& a_field_name, 
				int a_value_actual, 
				int a_value_max,
				int a_value_min
			);
			
			void validate_datetime(const std::tm& a_datetime);
			
			long int get_schedules(
				const std::time_t& a_datetime_start, 
				const std::time_t& a_datetime_end
			) const;
			
			std::time_t now() const;
			
		public:
			Task& set_datetime_start(const std::string& a_datetime);
			Task& set_datetime_end(const std::string& a_datetime);
			Task& set_datetime_format(const std::string& a_format);
			Task& set_interval(const int a_interval);
			Task& run(const std::function<void(void)>& a_callback);
	};
}