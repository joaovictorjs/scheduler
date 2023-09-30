#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "scheduler.hpp"

class TaskTestHelper : public Scheduler::Task {
	public:
		void create_datetime(
			std::tm& a_receiver,
			const std::string& a_datetime
		){
			Scheduler::Task::create_datetime(a_receiver, a_datetime);
		}
		
		void validate_datetime(const std::tm& a_datetime){
			Scheduler::Task::validate_datetime(a_datetime);
		}
		
		TaskTestHelper& set_datetime_start(const std::string& a_datetime){
			Scheduler::Task::set_datetime_start(a_datetime);
			return *this;
		}
		
		TaskTestHelper& set_datetime_end(const std::string& a_datetime){
			Scheduler::Task::set_datetime_end(a_datetime);
			return *this;
		}
		
		TaskTestHelper& set_datetime_format(const std::string& a_format){
			Scheduler::Task::set_datetime_format(a_format);
			return *this;
		}
		
		TaskTestHelper& set_interval(const int a_interval){
			Scheduler::Task::set_interval(a_interval);
			return *this;
		}
		
		long int get_schedules(
			const std::time_t& a_datetime_start, 
			const std::time_t& a_datetime_end
		){
			return Scheduler::Task::get_schedules(a_datetime_start, a_datetime_end);
		}
};

TEST(TaskTestHelper, create_datetime){
	TaskTestHelper task;
	
	std::tm datetime {};
	
	// using default format %Y-%m-%d %H:%M:%S
	ASSERT_THROW(task.create_datetime(datetime, "2023-01-32 00:00:00"), Scheduler::FormatError);
	ASSERT_NO_THROW(task.create_datetime(datetime, "2023-01-02 03:04:05"));
	
	ASSERT_EQ(datetime.tm_year, 123);
	ASSERT_EQ(datetime.tm_mon, 0);
	ASSERT_EQ(datetime.tm_mday, 2);
	ASSERT_EQ(datetime.tm_hour, 3);
	ASSERT_EQ(datetime.tm_min, 4);
	ASSERT_EQ(datetime.tm_sec, 5);
	ASSERT_EQ(datetime.tm_isdst, -1);
}

TEST(TaskTestHelper, validate_datetime){
	TaskTestHelper task;
	
	std::tm datetime {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 1,
		.tm_mon = 0,
		.tm_year = 123, // 2023
	};
	
	ASSERT_NO_THROW(task.validate_datetime(datetime));
	
	// ferbuary with 29 days in non leap year
	datetime.tm_mon = 1;
	datetime.tm_mday = 29;
	ASSERT_THROW(task.validate_datetime(datetime), Scheduler::RangeError);
	
	// ferbuary with 29 days in leap year
	datetime.tm_year = 2024;
	ASSERT_NO_THROW(task.validate_datetime(datetime));
	
	// ferbuary with 28 days
	datetime.tm_year = 2023;
	datetime.tm_mday = 28;
	ASSERT_NO_THROW(task.validate_datetime(datetime));
}

TEST(TaskTestHelper, set_datetime_start){
	TaskTestHelper task;
	
	ASSERT_NO_THROW(task.set_datetime_start("2023-01-02 03:04:05"));
	// month with 32 days
	ASSERT_THROW(task.set_datetime_start("2023-01-32 03:04:05"), Scheduler::FormatError);
	// empty
	ASSERT_THROW(task.set_datetime_start(""), Scheduler::FormatError);
}

TEST(TaskTestHelper, set_datetime_end){
	TaskTestHelper task;
	
	ASSERT_NO_THROW(task.set_datetime_end("2023-01-02 03:04:05"));
	// month with 32 days
	ASSERT_THROW(task.set_datetime_end("2023-01-32 03:04:05"), Scheduler::FormatError);
	// empty value
	ASSERT_THROW(task.set_datetime_end(""), Scheduler::FormatError);
}

TEST(TaskTestHelper, set_datetime_format){
	TaskTestHelper task;
	
	// emoty value
	ASSERT_THROW(task.set_datetime_format(""), Scheduler::FormatError);
	// BR format
	ASSERT_NO_THROW(task.set_datetime_format("%d/%m/%Y %H:%M:%S"));
}

TEST(TaskTestHelper, set_interval){
	TaskTestHelper task;
	
	ASSERT_THROW(task.set_interval(0), Scheduler::RangeError);
	ASSERT_NO_THROW(task.set_interval(1));
}

TEST(TaskTestHelper, get_schedules){
	TaskTestHelper task;
	
	task.set_interval(1);
	
	ASSERT_THROW(task.get_schedules(1, 0), Scheduler::OrderError);
	
	ASSERT_EQ(task.get_schedules(0, 0), 1);
	ASSERT_EQ(task.get_schedules(0, 2), 2);
	
	task.set_interval(3);
	ASSERT_EQ(task.get_schedules(0, 10), 3);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
