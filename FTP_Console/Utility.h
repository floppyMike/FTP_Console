#pragma once

#include "Globals.h"
#include "Connection.h"

void verboseLog(const std::string_view&);

void handleList(Connection&);
void handleFile(Connection&, char*);