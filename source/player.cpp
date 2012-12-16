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

#include "player.h"

Player::Player(Game *parent, int x, int y) : Entity(parent), _dx(0), _dy(0), _movtick(false), _moving(false), _reverse(false), _step(0), _astep(-1), _alive(true), _exstep(0) {
	_x = x;
	_y = y;
	sf::Texture &__tex = _parent->gettexture("data/player.png");
	for (int i = 0; i < 4; i++) {
		_sprites[i].setTexture(__tex);
		_sprites[i].setTextureRect(sf::IntRect(i * 16, 0, 16, 16));
		_sprites[i].setOrigin(8, 12);
	}
	_sword.setTexture(__tex);
	_sword.setTextureRect(sf::IntRect(0, 16, 16, 16));
	_sword.setOrigin(2, 12);
	_exmark.setTexture(__tex);
	_exmark.setTextureRect(sf::IntRect(16, 16, 16, 16));
	_exmark.setOrigin(8, 14);
}

Player::~Player(void) {
  
}

void Player::update() {
	if (!_alive) {
		if (_step < 60) {
			switch (_step++) {
			case 10:
				_parent->addpoof(_x - 4, _y - 4);
				break;
			case 20:
				_parent->addpoof(_x + 4, _y + 4);
				break;
			case 30:
				_parent->addpoof(_x + 4, _y - 4);
				break;
			case 40:
				_parent->addpoof(_x - 4, _y + 4);
				break;
			case 50:
				_parent->addpoof(_x, _y);
				break;
			}
		}
		else {
			if (_parent->keytapped(KEY_ATTACK))
				_parent->reset();
		}
		return;
	}
	_step = (_step + 1) % 100;
	if (_movtick = !_movtick) {
		_moving = false;
		if (_parent->keydown(KEY_LEFT)) {
			_moving = true;
			_x--;
			_reverse = true;
			while (_x < 4 || _parent->solid(_x / 16, _y / 16))
				_x++;
		}
		else if (_parent->keydown(KEY_RIGHT)) {
			_moving = true;
			_x++;
			_reverse = false;
			while (_x > 156 || _parent->solid(_x / 16, _y / 16))
				_x--;
		}
		if (_parent->keydown(KEY_UP)) {
			_moving = true;
			_y--;
			while (_y < 4 || _parent->solid(_x / 16, _y / 16))
				_y++;
		}
		else if (_parent->keydown(KEY_DOWN)) {
			_moving = true;
			_y++;
			while (_y > 156 || _parent->solid(_x / 16, _y / 16))
				_y--;
		}
		if (_astep > -1 && _astep < 8)
			_astep++;
		else
			_astep = -1;
	}
	if (_astep == - 1 && _parent->keytapped(KEY_ATTACK)) {
		_astep = 0;
		_parent->getsound("data/stroke.wav").play();
	}
}

void Player::draw() {
	sf::RenderTarget &rt = _parent->gettarget();
	if (!_alive) {
		if (_step < 60) {
			_sprites[0].setColor(_step % 2 ? sf::Color::White : sf::Color::Red);
			_sprites[0].setScale(_reverse ? -1 : 1, 1);
			_sprites[0].setPosition(_x, _y);
			rt.draw(_sprites[0]);
		}
		return;
	}
	int __i = 0;
	if (_moving)
		__i = (_step / 8) % 4;
	if (_reverse) {
		_sprites[__i].setScale(-1, 1);
		_sword.setScale(-1, 1);
		_exmark.setScale(-1, 1);
	}
	else {
		_sprites[__i].setScale(1, 1);
		_sword.setScale(1, 1);
		_exmark.setScale(1, 1);
	}
	_sprites[__i].setPosition(_x, _y);
	_sword.setPosition(_reverse ? _x - _astep : _x + _astep, _y);
	if (_astep != -1)
		rt.draw(_sword);
	rt.draw(_sprites[__i]);
	if (_exstep) {
		if ((_exstep / 2) % 2) {
			_exmark.setPosition(_x, _y);
			rt.draw(_exmark);
		}
		_exstep--;
	}
}

void Player::querydeletion(void) {
	_alive = false;
	_step = 0;
}