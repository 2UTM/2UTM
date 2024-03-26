/*************************************************************************
* Rutoken                                                                *
* Copyright (c) 2003-2021, Aktiv-Soft JSC. All rights reserved.          *
* Подробная информация:  http://www.rutoken.ru                           *
*************************************************************************/

#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

#include <pki-core-cpp.h>

#define DO_STRING_JOIN(arg1, arg2) arg1 ## arg2
#define STRING_JOIN(arg1, arg2) DO_STRING_JOIN(arg1, arg2)

#define SCOPE_EXIT(...) \
	ScopeExit STRING_JOIN(scopeExit, __LINE__) = [__VA_ARGS__]()

class ScopeExit {
public:
	template<typename F>
	ScopeExit(F f)
		: mAction(f)
	{}

	~ScopeExit() {
		try {
			mAction();
		} catch (const std::exception& e) {
			try {
				std::cerr << e.what() << std::endl;
			} catch (...) {
			}
		}
	}

private:
	std::function<void()> mAction;
};

template<typename T, size_t n>
size_t arraysize(T (&)[n]) { return n; }

static std::string toHex(const std::vector<uint8_t>& data) {
	std::ostringstream ss;
	for (auto it = data.begin(); it != data.end(); ++it) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(*it);
	}
	return ss.str();
}

static std::vector<uint8_t> readFile(const std::string& path) {
	std::ifstream file(path, std::ifstream::binary);
	if (!file) {
		throw std::runtime_error("Couldn't open file: " + path);
	}
	SCOPE_EXIT(&file) {
		file.close();
	};

	file.seekg(0, std::ios::end);
	std::vector<uint8_t> data(static_cast<size_t>(file.tellg()));

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(data.data()), data.size());
	if (!file) {
		throw std::runtime_error("Couldn't read file: " + path);
	}

	return data;
}

static void writeFile(const std::string& path, const void* data, size_t size) {
	std::ofstream file(path, std::ofstream::binary | std::ofstream::trunc);
	if (!file) {
		throw std::runtime_error("Couldn't open file: " + path);
	}
	SCOPE_EXIT(&file) {
		file.close();
	};

	file.write(reinterpret_cast<const char*>(data), size);
}
