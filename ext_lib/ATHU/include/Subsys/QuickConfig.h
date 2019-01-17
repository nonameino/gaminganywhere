//==========================================
//  Create by hungnh on 2018/03/18
//==========================================

#pragma once

#include "Native_stdafx.h"

namespace Core {
    namespace Subsys {
        class QuickConfig {
            private:
				static const int MAX_BLOCK = 2;
				static const int MAX_OPTIONS = 6;
				static char *_dir;
                static const char *_quickFile;
                static int _blockActive[2][6];
            public:
                static void initialize(const char *dir);
                static bool isActive(int block);
				static void updateLogger(bool &bActive, bool &bFileLog, int &iLevel);
		};
    }
}