#pragma once

#include <string>
#include <ctime>
#include "Native_stdafx.h"

namespace Core {
	namespace Helper {
		class NativeDateTime {
		private:
			std::string dayNames[7];
			std::string monthNames[12];

			int _year;
			int _month;
			int _day;
			int _dayOfWeek;
			int _hour;
			int _minute;
			int _second;
		public:
			const inline int getYear() { return _year; }
			const inline int getMonth() { return _month; }
			const inline int getDay() { return _day; }
			const inline int getDayOfWeek() { return _dayOfWeek; }
			const inline int getHour() { return _hour; }
			const inline int getMinute() { return _minute; }
			const inline int getSecond() { return _second; }
		public:
			NativeDateTime();
			NativeDateTime(const std::string str/*, const std::string format*/);
			NativeDateTime(const int year, const int month, const int day,
							const int hour, const int minute, const int second);
			NativeDateTime(const std::tm time);

			const std::string toString();
			const std::time_t getTime();
		};

		/*typedef void(*GetDateInfoCB)(NativeDateTime&);
		GetDateInfoCB __get_date_info_cb;*/
	}
}