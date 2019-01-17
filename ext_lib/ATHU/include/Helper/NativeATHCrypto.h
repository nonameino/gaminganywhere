#pragma once

#include <string>
#include "Native_stdafx.h"

namespace Core {
	namespace Helper {
		class NativeATHCrypto {
		private:
			static int _curEncodePosition;
			static int _baseStringLength;

			static char *TAG;
		private:
			static int getBaseEncodeLength(const char* text);
		public:
			static inline int getCurrentEncodePosition() { return _curEncodePosition; }
			static inline int getBaseStringLength() { return _baseStringLength; }

			static std::string ATHEncrypt(std::string key, std::string value);
			static std::string ATHDecrypt(std::string key, std::string value);
		};
	}
}