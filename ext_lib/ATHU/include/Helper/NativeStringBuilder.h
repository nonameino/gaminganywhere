#pragma once

#include <string>
#include <deque>
#include "Native_stdafx.h"

namespace Core {
	namespace Helper {
		class NativeStringBuilder {
		private:
			std::string _data;
			std::string _scratch;
			std::string::size_type SCRATCH_MAX_SIZE; //Max char for each append
		public:
			NativeStringBuilder();

			NativeStringBuilder& append(const std::string &src);
			NativeStringBuilder& append(const char src);

			std::string toString();
		};
	}
}