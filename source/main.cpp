/**
 * Black Knight Blockade, a simple game entry for Ludum Dare 25: You are the Villain
 * 
 * Copyright (c) 2013 Mario Liebisch <mario.liebisch@gmail.com>
 * 
 * This file is part of Black Knight Blockade.
 * 
 * Black Knight Blockade is free software: you can redistribute it and/or modify it under the terms
 * of the  * GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * 
 * Black Knight Blockade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Black Knight Blockade.
 * If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <cstdio>
#ifdef _MSC_VER
#include <direct.h>
#define chdir(a) _chdir(a)
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif
#elif ANDROID
#else
#include <dir.h>
#endif
#include "game.h"

int main(int argc, const char *argv[])
{
	srand((unsigned int)time(0));
#ifdef ANDROID
#else
	chdir("data");
#endif

	try {
		Game game;
		return game.run();
	}
	catch(const char *exception) {
		printf("An exception occured: %s\n", exception);
	}
}
