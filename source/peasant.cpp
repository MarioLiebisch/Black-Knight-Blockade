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

#include "peasant.h"
#include "player.h"

Peasant::Peasant(Game *parent, int x, int y) : Entity(parent), _reverse(false), _step(0), _target(TARGET_ANY), _speed(1 + rand() % 3), _health(_speed), _hstep(0), _justhit(false) {
	_x = x;
	_y = y;
	sf::Texture &__tex = _parent->gettexture("data/peasant.png");
	sf::Color __c(rand() % 2 ? 0x7f : 0xff, rand() % 2 ? 0x7f : 0xff, rand() % 2 ? 0x7f : 0xff);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			_sprites[i + j * 4].setTexture(__tex);
			_sprites[i + j * 4].setTextureRect(sf::IntRect(i * 16, j * 16, 16, 16));
			_sprites[i + j * 4].setOrigin(8, 12);
		}
		_sprites[i].setColor(__c);
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

	const int __px = _parent->getplayer()->getx();
	const int __py = _parent->getplayer()->gety();
	const int __pr = ((Player*)_parent->getplayer())->reversed();
	const int __pa = ((Player*)_parent->getplayer())->attacking();

	const bool __left = (_target == TARGET_WEST);
	const bool __right = (_target == TARGET_EAST);
	const bool __up = _y > 104;
	const bool __down = _y < 70;

	if (_step % _speed == 0) {
		if (__left) {
			_x--;
			_reverse = true;
		}
		else if (__right) {
			_x++;
			_reverse = false;
		}
		if (__up) {
			_y--;
		}
		else if (__down) {
			_y++;
		}
	}

	if (_target == TARGET_WEST && _x == -8) {
		_delete = true;
		_parent->addpeasant();
		_parent->addpass();
		_parent->getsound("data/getaway.wav").play();
	}
	else if (_target == TARGET_EAST && _x == 168) {
		_delete = true;
		_parent->addpeasant();
		_parent->addpass();
		_parent->getsound("data/getaway.wav").play();
	}

	// should better be done outside, but saves lots of collision detection
	if (!_justhit && __pa && std::abs(__px - _x) < 20 && std::abs(__py - _y) < 8 && __pr == __px > _x) {
		if (_x < __px)
			_x -= 4;
		else
			_x += 4;

		_parent->getsound("data/hurt.wav").play();

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
	if (__left) {
		while (_parent->solid(_x / 16, _y / 16))
			_x++;
	}
	else if (__right) {
		while (_parent->solid(_x / 16, _y / 16))
			_x--;
	}
	if (__up) {
		while (_parent->solid(_x / 16, _y / 16))
			_y++;
	}
	else if (__down) {
		while (_parent->solid(_x / 16, _y / 16))
			_y--;
	}
}

void Peasant::draw() {
	int __i = 0, __j = 4;
	__j = 4 + (__i = (_step / 8) % 4);
	if (_hstep)
		_sprites[__j].setColor((_step / 2) % 2 ? sf::Color::White : sf::Color::Red);
	else
		_sprites[__j].setColor(sf::Color::White);
	if (_reverse) {
		_sprites[__i].setScale(-1, 1);
		_sprites[__j].setScale(-1, 1);
	}
	else {
		_sprites[__i].setScale(1, 1);
		_sprites[__j].setScale(1, 1);
	}
	_sprites[__i].setPosition(_x, _y);
	_sprites[__j].setPosition(_x, _y);
	_parent->gettarget().draw(_sprites[__i]);
	_parent->gettarget().draw(_sprites[__j]);
}

void Peasant::settarget(TARGET target) {
	_target = target;
}
