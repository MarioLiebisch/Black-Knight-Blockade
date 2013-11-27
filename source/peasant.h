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

#pragma once
#include "entity.h"

enum TARGET {
	TARGET_ANY = 0,
	TARGET_WEST,
	TARGET_EAST
};

class Peasant : public Entity {
private:
	sf::Sprite _sprites[8];
	bool _movtick;
	bool _reverse;
	char _step;
	unsigned char _hstep;
	unsigned char _speed;
	TARGET _target;
	unsigned char _health;
	bool _justhit;
public:
	Peasant(Game *parent, int x = 80, int y = 88);
	virtual ~Peasant(void);
	virtual void draw(void);
	virtual void update(void);
	virtual void settarget(TARGET target);
};
