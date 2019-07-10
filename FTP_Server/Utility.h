#pragma once

#include "Globals.h"
#include "Connection.h"

void verboseLog(const std::string_view& str);

void handleList(Connection&, char*);
void handleFile(Connection&, const std::string& fileName);