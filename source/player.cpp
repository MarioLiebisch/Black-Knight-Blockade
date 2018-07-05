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

#include "player.h"

Player::Player(Game *parent, int x, int y) : Entity(parent), _dx(0), _dy(0), _moving(false), _reverse(false), _step(0), _astep(-1), _alive(true), _exstep(0) {
	_x = x;
	_y = y;
	sf::Texture &tex = _parent->gettexture("player.png");
	for (int i = 0; i < 4; i++) {
		_sprites[i].setTexture(tex);
		_sprites[i].setTextureRect(sf::IntRect(i * 16, 0, 16, 16));
		_sprites[i].setOrigin(8, 14);
	}
	_sword.setTexture(tex);
	_sword.setTextureRect(sf::IntRect(0, 16, 16, 16));
	_sword.setOrigin(2, 14);
	_exmark.setTexture(tex);
	_exmark.setTextureRect(sf::IntRect(16, 16, 16, 16));
	_exmark.setOrigin(8, 16);
}

Player::~Player(void) {
  
}

void Player::update() {
	if (!_alive) {
		if (_step < 120) {
			switch (_step++) {
			case 20:
				_parent->addpoof(_x - 4, _y - 4);
				break;
			case 40:
				_parent->addpoof(_x + 4, _y + 4);
				break;
			case 60:
				_parent->addpoof(_x + 4, _y - 4);
				break;
			case 80:
				_parent->addpoof(_x - 4, _y + 4);
				break;
			case 100:
				_parent->addpoof(_x, _y);
				break;
			}
		}
		else {
			_parent->reset();
		}
		return;
	}
	_step = (_step + 1) % 100;
	
	if ((!_parent->water(_x / 16, (_y + 2) / 16) && _step % 2 == 0) || (_step % 3 == 0)) {
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
	}
	if (_astep > -1 && _astep < 20)
		_astep++;
	else
		_astep = -1;
	if (_astep == - 1 && _parent->keydown(KEY_ATTACK)) {
		_astep = 0;
		_parent->getsound("stroke.wav").play();
	}
}

void Player::draw() {
	sf::RenderTarget &rt = _parent->gettarget();
	if (!_alive) {
		if (_step < 120) {
			_sprites[0].setColor(_step % 4 ? sf::Color::White : sf::Color::Red);
			_sprites[0].setScale(_reverse ? -1 : 1.f, 1.f);
			_sprites[0].setPosition(static_cast<float>(_x), static_cast<float>(_y));
			rt.draw(_sprites[0]);
		}
		return;
	}
	int i = 0;
	if (_moving)
		i = (_step / 8) % 4;
	if (_reverse) {
		_sprites[i].setScale(-1, 1);
		_sword.setScale(-1, 1);
		_exmark.setScale(-1, 1);
	}
	else {
		_sprites[i].setScale(1, 1);
		_sword.setScale(1, 1);
		_exmark.setScale(1, 1);
	}
	_sprites[i].setPosition(static_cast<float>(_x), static_cast<float>(_y));
	if (_astep > -1 && _astep < 18) {
		_sword.setPosition(static_cast<float>(_reverse ? _x - _astep / 2: _x + _astep / 2), static_cast<float>(_y));
		rt.draw(_sword);
	}
	rt.draw(_sprites[i]);
	if (_exstep) {
		if ((_exstep / 2) % 2) {
			_exmark.setPosition(static_cast<float>(_x), static_cast<float>(_y));
			rt.draw(_exmark);
		}
		_exstep--;
	}
}

void Player::querydeletion(void) {
	_alive = false;
	_step = 0;
}