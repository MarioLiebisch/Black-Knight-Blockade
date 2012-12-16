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

#include "game.h"
#include "entity.h"
#include "player.h"
#include "peasant.h"
#include "poof.h"
#include <algorithm>
#include <cstdio>

#if _MSC_VER
#define snprintf _snprintf
#endif

namespace {
	const char map[20][10] = {
		{6, 7, 6, 7, 1, 3, 6, 7, 6, 7},
		{7, 6, 7, 0, 1, 3, 0, 6, 7, 6},
		{6, 7, 0, 0, 1, 3, 0, 0, 6, 7},
		{7, 0, 0, 0, 1, 3, 0, 0, 0, 6},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{0, 0, 0, 0, 8, 8, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{7, 0, 0, 0, 1, 3, 0, 0, 0, 6},
		{6, 7, 0, 0, 1, 3, 0, 0, 6, 7},
		{5, 4, 5, 0, 0, 0, 0, 4, 5, 4},
		{4, 5, 0, 0, 0, 0, 0, 0, 4, 5},
		{5, 0, 0, 0, 0, 0, 0, 0, 0, 4},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{5, 0, 0, 0, 0, 0, 0, 0, 0, 4},
		{4, 5, 0, 0, 0, 0, 0, 0, 4, 5},
		{5, 4, 5, 4, 5, 4, 5, 4, 5, 4}
	};

	const bool smap[10][10] = {
		{1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	bool delcheck(Entity *e) {
		if (e->deletion()) {
			delete e;
			return true;
		}
		return false;
	}

	void delentity(Entity *e) {
		delete e;
	}

	void deltexture(std::pair<const std::string, sf::Texture*> &e) {
		delete e.second;
	}

	void delbuffer(std::pair<const std::string, sf::SoundBuffer*> &e) {
		delete e.second;
	}

	void delsound(std::pair<const std::string, sf::Sound*> &e) {
		delete e.second;
	}

	bool cmpentity (Entity const *const &ef, const Entity *const &es) {
		return ef->gety() < es->gety();
	}
}

Game::Game(void) : _window(), _running(true), _fps(0), _ups(0), _caption("LD25: Black Knight Blockade"), _tick(0), _killcount(0), _passcount(0), _rage(0), _doreset(false) {
	
}

Game::~Game(void) {
  
}

int Game::run(void) {
	const int __logicSteps = 100;
	const sf::Time __logicStep = sf::seconds(1) / (float)__logicSteps;
	_window.create(sf::VideoMode(320, 320), _caption, sf::Style::Titlebar | sf::Style::Close);
	_window.setVerticalSyncEnabled(true);
	_window.setView(sf::View(sf::FloatRect(0, 0, 160, 160)));
	
	sf::Event __e;
	sf::Clock __updateClock;
	sf::Clock __statClock;
	sf::Time __passedUpdateTime;
	sf::Time __passedStatTime;
	int __numFrames = 0;
	int __numUpdates = 0;
#ifndef NDEBUG
	char __caption[256];
#endif
	bool __keydown = false;

	sf::Texture &__maptex = gettexture("data/map.png");

	for (int i = 0; i < KEY_COUNT; _keytapped[i] = _keydown[i] = false, i++);
	for (int i = 0; i < 16; i++) {
		_mapsprites[i].setTexture(__maptex);
		_mapsprites[i].setTextureRect(sf::IntRect((i % 4) * 16, (i / 4) * 16, 16, 16));
	}
	
	_fontsprite.setTexture(gettexture("data/font.png"));
	_fontsprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
	_fontsprite.setOrigin(0, 0);

	_reset();

	while(_window.isOpen()) {
		while (_window.pollEvent(__e) && _running) {
			__keydown = false;
			switch (__e.type) {
			case sf::Event::Closed:
				_window.close();
				_running = false;
				break;
			case sf::Event::KeyPressed:
				__keydown = true;
			case sf::Event::KeyReleased:
				switch (__e.key.code) {
				case sf::Keyboard::Escape:
					_window.close();
					_running = false;
					break;
				case sf::Keyboard::Left:
				case sf::Keyboard::A:
					_keyevent(KEY_LEFT, __keydown);
					break;
				case sf::Keyboard::Right:
				case sf::Keyboard::D:
					_keyevent(KEY_RIGHT, __keydown);
					break;
				case sf::Keyboard::Up:
				case sf::Keyboard::W:
					_keyevent(KEY_UP, __keydown);
					break;
				case sf::Keyboard::Down:
				case sf::Keyboard::S:
					_keyevent(KEY_DOWN, __keydown);
					break;
				case sf::Keyboard::Space:
				case sf::Keyboard::Return:
					_keyevent(KEY_ATTACK, __keydown);
					break;
				}
				break;
			}
		}

		if (_doreset)
			_reset();

		__passedUpdateTime += __updateClock.restart();
		for (; _running && __passedUpdateTime > __logicStep; __passedUpdateTime -= __logicStep) {
			if (__numUpdates < __logicSteps) {
				_update();
				_entities.erase(std::remove_if(_entities.begin(), _entities.end(), &::delcheck), _entities.end());
				for (int i = 0; i < KEY_COUNT; _keytapped[i] = false, i++);
				++__numUpdates;
			}
			else {
				__passedUpdateTime = sf::Time::Zero;
				break;
			}
		}
		++__numFrames;

		std::sort(_entities.begin(), _entities.end(), &::cmpentity);

		if (!_running) {
			_window.close();
			break;
		}
		_draw();

		__passedStatTime += __statClock.restart();
		if ((int)__passedStatTime.asSeconds() > 0) {
			__passedStatTime = sf::Time::Zero;
			_fps = __numFrames;
			_ups = __numUpdates;
			__numFrames = 0;
			__numUpdates = 0;
#ifndef NDEBUG
			snprintf(__caption, 255, "%s [fps: %d, ups: %d]", _caption, _fps, _ups);
			_window.setTitle(__caption);
#endif
		}
		sf::sleep(sf::microseconds(1));
	}

	std::for_each(_entities.begin(), _entities.end(), ::delentity);
	std::for_each(_sounds.begin(), _sounds.end(), ::delsound);
	std::for_each(_buffers.begin(), _buffers.end(), ::delbuffer);
	std::for_each(_textures.begin(), _textures.end(), ::deltexture);

	return 0;
}

void Game::_update(void) {
	if (_tick++ % 1500 == 0)
		addpeasant();
	for (int i = 0, j = _entities.size(); i < j; _entities[i]->update(), i++);
}

void Game::_draw(void) {
	static const sf::Color __back(87, 127, 63);
	_window.clear(__back);
	for (int y = 0; y < 10; y++) {
		for (int x = 0, i = 0; x < 10; x++, i++) {
			//if (!::map[y][x])
			//	continue;
			sf::Sprite s = _mapsprites[::map[y][x]];
			s.setPosition(x * 16.f, y * 16.f);
			_window.draw(s);
		}
	}

	for (int i = 0, j = _entities.size(); i < j; _entities[i]->draw(), i++);

	for (int y = 0; y < 10; y++)
		for (int x = 0, i = 0; x < 10; x++, i++) {
			if (!::map[y + 10][x])
				continue;
			sf::Sprite s = _mapsprites[::map[y + 10][x]];
			s.setPosition(x * 16.f, y * 16.f);
			_window.draw(s);
		}
	
	drawtext(_scoretext, 25, 5);
	drawtext("R", 5, 5);
	drawtext("(", 5, 10);
	drawtext(_rage > 9 ? "X" : "_", 5, 20);
	drawtext(_rage > 8 ? "X" : "_", 5, 30);
	drawtext(_rage > 7 ? "X" : "_", 5, 40);
	drawtext(_rage > 6 ? "X" : "_", 5, 50);
	drawtext(_rage > 5 ? "X" : "_", 5, 60);
	drawtext(_rage > 4 ? "X" : "_", 5, 70);
	drawtext(_rage > 3 ? "X" : "_", 5, 80);
	drawtext(_rage > 2 ? "X" : "_", 5, 90);
	drawtext(_rage > 1 ? "X" : "_", 5, 100);
	drawtext(_rage > 0 ? "X" : "_", 5, 110);
	drawtext(")", 5, 120);
	
	_window.display();
}

void Game::_keyevent(const GAME_KEY gkey, const bool pressed) {
	if (!_keydown[gkey] && pressed)
		_keytapped[gkey] = true;
	_keydown[gkey] = pressed;
}

bool Game::solid(char x, char y) {
	if (x < 0 || y < 0 || x > 9 || y > 9)
		return false;
	return ::smap[y][x];
}

sf::Texture &Game::gettexture(const char *file) {
	std::map<const std::string, sf::Texture*>::iterator a = _textures.find(file);
	if (a != _textures.end())
		return *a->second;
	sf::Texture *__n = new sf::Texture();
	if (!__n->loadFromFile(file))
		throw "Failed to load texture.";
	_textures[file] = __n;
	return *__n;
}

sf::SoundBuffer &Game::getbuffer(const char *file) {
	std::map<const std::string, sf::SoundBuffer*>::iterator a = _buffers.find(file);
	if (a != _buffers.end())
		return *a->second;
	sf::SoundBuffer *__n = new sf::SoundBuffer();
	if (!__n->loadFromFile(file))
		throw "Failed to load sound.";
	_buffers[file] = __n;
	return *__n;
}

sf::Sound &Game::getsound(const char *file) {
	std::map<const std::string, sf::Sound*>::iterator a = _sounds.find(file);
	if (a != _sounds.end())
		return *a->second;
	sf::Sound *__n = new sf::Sound();
	__n->setBuffer(getbuffer(file));
	_sounds[file] = __n;
	return *__n;
}

void Game::addkill(void) {
	_killcount++;
	_rage = std::max(_rage - 1, 0);
	_updatescore();
}

void Game::addpass(void) {
	_passcount++;
	if (_rage < 11) { // collect up to 10 rage
		_rage = _rage + 1;
		((Player*)_player)->setangry();
	}
	else if (_rage == 11) { // 11 rage -> game over
		_player->querydeletion(); // i.e. kill the player
		_rage++;
	}
	_updatescore();
}

void Game::drawtext(const char *text, int x, int y) {
	for (;;) {
		switch (*text) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': // digits
			_fontsprite.setTextureRect(sf::IntRect((*text - '0') * 10, 0, 10, 10));
			break;
		case 'S': // skull
			_fontsprite.setTextureRect(sf::IntRect(10, 10, 10, 10));
			break;
		case 'x': // times
			_fontsprite.setTextureRect(sf::IntRect(0, 10, 10, 10));
			break;
		case 'R': // rage icon
			_fontsprite.setTextureRect(sf::IntRect(20, 10, 10, 10));
			break;
		case '(': // rage meter top
			_fontsprite.setTextureRect(sf::IntRect(30, 10, 10, 10));
			break;
		case '_': // rage meter empty
			_fontsprite.setTextureRect(sf::IntRect(40, 10, 10, 10));
			break;
		case 'X': // rage meter filled
			_fontsprite.setTextureRect(sf::IntRect(50, 10, 10, 10));
			break;
		case ')': // rage meter bottom
			_fontsprite.setTextureRect(sf::IntRect(60, 10, 10, 10));
			break;
		default:
			return;
		}
		_fontsprite.setPosition(x, y);
		_window.draw(_fontsprite);
		x += 9;
		text++;
	}
}

void Game::addpeasant(void) {
	const static int starts[][3] = {
		{-7 - rand() % 33, 72, TARGET_EAST},
		{-7 - rand() % 33, 80, TARGET_EAST},
		{-7 - rand() % 33, 88, TARGET_EAST},
		{-7 - rand() % 33, 96, TARGET_EAST},
		{-7 - rand() % 33, 104, TARGET_EAST},
		{167 + rand() % 33, 72, TARGET_WEST},
		{167 + rand() % 33, 80, TARGET_WEST},
		{167 + rand() % 33, 88, TARGET_WEST},
		{167 + rand() % 33, 96, TARGET_WEST},
		{167 + rand() % 33, 104, TARGET_WEST}
	};

	int i = rand() % 10;

	Peasant *__p = new Peasant(this, starts[i][0], starts[i][1]);
	__p->settarget((TARGET)starts[i][2]);
	_entities.push_back(__p);
}

void Game::addpoof(int x, int y) {
	Poof *__p = new Poof(this, x, y);
	_entities.push_back(__p);
}

void Game::_updatescore(void) {
	snprintf(_scoretext, 32, "Sx%d", _killcount);
}

void Game::_reset(void) {
	_doreset = false;
	std::for_each(_entities.begin(), _entities.end(), ::delentity);
	_entities.clear();
	_tick = _killcount = _passcount = _rage = 0;
	_updatescore();
	_entities.push_back(_player = new Player(this, 80, 88));
}

void Game::reset(void) {
	_doreset = true;
}