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

#include "peasant.h"
#include "player.h"

Peasant::Peasant(Game *parent, int x, int y) : Entity(parent), _reverse(false), _step(0), _target(TARGET_ANY), _speed(1 + rand() % 3), _health(_speed), _hstep(0), _justhit(false) {
	_x = x;
	_y = y;
	sf::Texture &tex = _parent->gettexture("peasant.png");
	sf::Color c(rand() % 2 ? 0x7f : 0xff, rand() % 2 ? 0x7f : 0xff, rand() % 2 ? 0x7f : 0xff);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			_sprites[i + j * 4].setTexture(tex);
			_sprites[i + j * 4].setTextureRect(sf::IntRect(i * 16, j * 16, 16, 16));
			_sprites[i + j * 4].setOrigin(8, 14);
		}
		_sprites[i].setColor(c);
	}
}

Peasant::~Peasant(void) {
  
}

void Peasant::update() {
	if (!((Player*)_parent->getplayer())->alive())
		return; // stop moving if player is dead
	_step = (_step + 1) % 100;
	if (_hstep)
		_hstep--;

	const int px = _parent->getplayer()->getx();
	const int py = _parent->getplayer()->gety();
	const int pr = ((Player*)_parent->getplayer())->reversed();
	const int pa = ((Player*)_parent->getplayer())->attacking();

	const bool left = (_target == TARGET_WEST);
	const bool right = (_target == TARGET_EAST);
	const bool up = _y > 104;
	const bool down = _y < 70;
	const bool water = _parent->water(_x / 16, _y / 16);

	if ((!water && _step % _speed == 0) || (_step % (2 * _speed) == 0)) {
		if (left) {
			_x--;
			_reverse = true;
		}
		else if (right) {
			_x++;
			_reverse = false;
		}
		if (up) {
			_y--;
		}
		else if (down) {
			_y++;
		}
		if (!water && _speed == 1 && _step % 5 == 0)
			_parent->adddust(_x, _y - 1);
	}

	if (_target == TARGET_WEST && _x == -8) {
		_delete = true;
		_parent->addpeasant();
		_parent->addpass();
	}
	else if (_target == TARGET_EAST && _x == 168) {
		_delete = true;
		_parent->addpeasant();
		_parent->addpass();
	}

	// should better be done outside, but saves lots of collision detection
	if (!_justhit && pa && std::abs(px - _x) < 20 && std::abs(py - _y) < 8 && pr == px > _x) {
		if (_x < px)
			_x -= 8;
		else
			_x += 8;
		_y -= (py - _y) / 2.f;

		_parent->getsound("hurt.wav").play();

		if (--_health == 0) {
			_delete = true;
			_parent->addpeasant();
			_parent->addpoof(_x, _y);
			_parent->addkill();
		}
		else {
			_justhit = true;
			_hstep = 20;
		}
	}
	else
		_justhit = false;

	// collision stuff
	if (left) {
		while (_parent->solid(_x / 16, _y / 16))
			_x++;
	}
	else if (right) {
		while (_parent->solid(_x / 16, _y / 16))
			_x--;
	}
	if (up) {
		while (_parent->solid(_x / 16, _y / 16))
			_y++;
	}
	else if (down) {
		while (_parent->solid(_x / 16, _y / 16))
			_y--;
	}
}

void Peasant::draw() {
	int i = 0, j = 4;
	j = 4 + (i = (_step / 8) % 4);
	if (_hstep)
		_sprites[j].setColor((_speed * _step / 2) % 2 ? sf::Color::White : sf::Color::Red);
	else
		_sprites[j].setColor(sf::Color::White);
	if (_reverse) {
		_sprites[i].setScale(-1, 1);
		_sprites[j].setScale(-1, 1);
	}
	else {
		_sprites[i].setScale(1, 1);
		_sprites[j].setScale(1, 1);
	}
	_sprites[i].setPosition(_x, _y);
	_sprites[j].setPosition(_x, _y);
	_parent->gettarget().draw(_sprites[i]);
	_parent->gettarget().draw(_sprites[j]);
}

void Peasant::settarget(TARGET target) {
	_target = target;
}
