/**
 * Black Knight Blockade, a simple game entry for Ludum Dare 25: You are the Villain
 * 
 * Copyright (C) 2012 Mario Liebisch <mario.liebisch@gmail.com>
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
#include "game.h"

int main(int argc, const char *argv[])
{
	srand(time(0));
	try {
		Game game;
		return game.run();
	}
	catch(const char *exception) {
		printf("An exception occured: %s\n", exception);
	}
}
