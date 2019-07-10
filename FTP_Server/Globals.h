#pragma once

#include <iostream>
#include <filesystem>
#include <array>
#include <CustomLibrary/ThirdParty/asio.hpp>
#include <CustomLibrary/Error.h>
#include <CustomLibrary/utility.h>

using asio::ip::tcp;
using namespace std::string_literals;

extern bool g_verbose;