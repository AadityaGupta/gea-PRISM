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
#include <string>

class Crypto final : public Singleton<Crypto> {

private:

	friend class Singleton<Crypto>;

	int *bfpos;

	Crypto() : Singleton<Crypto>() {}

	std::string ToHex(unsigned const char *str, int size);

	void ComputeMD5(char *buffer, const char *word, int _size);
	void ComputeSHA1(char *buffer, const char * word, int _size);
	void ComputeSHA256(char *buffer, const char * word, int _size);

public:

	void HashWord(char *buffer, const char *word, int nhash, int *_bfpos, int size);

};