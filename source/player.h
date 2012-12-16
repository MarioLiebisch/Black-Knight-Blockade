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
#include "entity.h"

class Player : public Entity {
private:
	int _dx;
	int _dy;
	sf::Sprite _sprites[4];
	sf::Sprite _sword;
	sf::Sprite _exmark;
	bool _movtick;
	bool _reverse;
	bool _moving;
	char _step;
	char _astep;
	bool _alive;
	char _exstep;
public:
	Player(Game *parent, int x = 80, int y = 88);
	virtual ~Player(void);
	virtual void draw(void);
	virtual void update(void);
	virtual bool reversed(void) { return _reverse; }
	virtual bool attacking(void) { return _astep == 8; }
	virtual void querydeletion(void);
	virtual bool alive(void) { return _alive; }
	virtual void setangry(void) { _exstep = 20; }
};
