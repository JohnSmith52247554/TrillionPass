/**
 * @file pch.hpp
 * @author username (username52247554@gmail.com)
 * @brief pre-compiled header
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <vector>
#include <array>
#include <map>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#include <conio.h>
#endif

#include <nlohmann/json.hpp>
#include <sodium.h>