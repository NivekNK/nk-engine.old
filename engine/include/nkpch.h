#pragma once

// File manipulation
#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <filesystem>

// Wrtie to the console
#include <iostream>

// String manipulation
#include <string>
#include <format>

// Types
#include <cstdint>

// Threading
#include <mutex>

// Windows specific
#if defined(NK_PLATFORM_WINDOWS)
	#include <windows.h>
#endif

// Memory manipulation
#include <memory>

// Global engine files
#include "core/defines.h"
#include "system/logging.h"
#include "core/assertion.h"
