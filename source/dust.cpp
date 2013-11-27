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

#include "dust.h"

Dust::Dust(Game *parent, int x, int y) : Entity(parent), _step(0), _astep(0) {
	_x = x;
	_y = y;
	_sprite.setTexture(_parent->gettexture("dust.png"));
	_sprite.setOrigin(4, 5);
	_sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
	_sprite.setPosition(_x, _y);
	//_parent->getsound("poof.wav").play();
}

Dust::~Dust(void) {

}

void Dust::update() {
	if (_step++ % 10 == 0) {
		_sprite.setTextureRect(sf::IntRect((_astep % 2) * 8, (_astep / 2) * 8, 8, 8));
		if (_astep++ == 4)
			_delete = true;
	}
}

void Dust::draw() {
	_sprite.setOrigin(4, 5 + _step / 10);
	_parent->gettarget().draw(_sprite);
}
