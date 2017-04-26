﻿/*
Copyright 2017 Borja Gomez - kub0x@elhacker.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Singleton.h"
#include <fstream>
#include <string>
#include <mutex>

class FileHandler final : public Singleton<FileHandler> {

private:

	friend class Singleton<FileHandler>;

	std::mutex mwrite;

	std::ofstream dumpfile;

	void WriteStateFile(const std::string& filename, const std::string& params);

	FileHandler() : Singleton<FileHandler>() { }

public:

	long double GetFileSize(const std::string& filename);
	void OpenDumpFile(const std::string &filename);
	std::string ReadStateFile(const std::string& filename);
	void LogFile(std::string& buffer, const std::string& filename, const std::string& params, int j);

};