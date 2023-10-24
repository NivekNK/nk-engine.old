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

// Interfaces
#include <concepts>
#include <type_traits>
#include <optional>

// Memory manipulation
#include <memory>
#include <functional>
#include <initializer_list>
#include <iterator>

// Global engine files
#include "core/defines.h"
#include "system/logging.h"
#include "core/assertion.h"
