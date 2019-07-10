#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <CustomLibrary/ThirdParty/asio.hpp>
#include <CustomLibrary/Error.h>

using asio::ip::tcp;
using namespace std::string_literals;

extern bool g_verbose;