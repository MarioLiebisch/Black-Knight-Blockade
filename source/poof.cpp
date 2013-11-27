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

#include "poof.h"

Poof::Poof(Game *parent, int x, int y) : Entity(parent), _step(0), _astep(0) {
	_x = x;
	_y = y;
	_sprite.setTexture(_parent->gettexture("poof.png"));
	_sprite.setOrigin(8, 12);
	_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	_sprite.setPosition(_x, _y);
	_parent->getsound("poof.wav").play();
}

Poof::~Poof(void) {
  
}

void Poof::update() {
	if (_step++ % 10 == 0) {
		_sprite.setTextureRect(sf::IntRect((_astep % 2) * 16, (_astep / 2) * 16, 16, 16));
		if (++_astep == 5)
			_delete = true;
	}
}

void Poof::draw() {
	_parent->gettarget().draw(_sprite);
}
