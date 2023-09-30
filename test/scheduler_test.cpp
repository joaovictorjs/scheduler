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
	
	// ferbuary with 29 days in leap year
	datetime.tm_year = 2023;
	datetime.tm_mday = 28;
	ASSERT_NO_THROW(task.validate_datetime(datetime));
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}