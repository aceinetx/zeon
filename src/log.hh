#pragma once
#include <format>
#include <iostream>

#define INFO(...) std::cout << "[zeon: info] " << std::format(__VA_ARGS__) << std::endl
#define WARN(...) std::cout << "[zeon: warn] " << std::format(__VA_ARGS__) << std::endl
#define ERROR(...) std::cout << "[zeon: error] " << std::format(__VA_ARGS__) << std::endl
