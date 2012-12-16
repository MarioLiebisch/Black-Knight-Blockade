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

#pragma once
#include "game.h"

class Entity {
protected:
	Game *_parent;
	int _x;
	int _y;
	bool _delete;
public:
	Entity(Game *parent);
	virtual ~Entity(void);
	virtual void draw(void) = 0;
	virtual void update(void) = 0;
	int getx(void) const { return _x; }
	int gety(void) const { return _y; }
	virtual void querydeletion(void) { _delete = true; }
	bool deletion(void) { return _delete; }
};
