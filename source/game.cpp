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

#include "game.h"
#include "entity.h"
#include "player.h"
#include "peasant.h"
#include "poof.h"
#include "dust.h"
#include <algorithm>
#include <cstdio>

#if _MSC_VER
#define snprintf _snprintf
#endif

namespace {
	const char map[20][10] = {
		{6, 7, 6, 7, 1, 3, 6, 7, 6, 7},
		{7, 6, 7, 6, 9,10, 7, 6, 7, 6},
		{6, 7, 0, 0, 1, 3, 0, 0, 6, 7},
		{7, 0, 0, 0, 1, 3, 0, 0, 0, 6},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{0, 0, 0, 0, 8, 8, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
		{7, 0, 0, 0, 1, 3, 0, 0, 0, 6},
		{6, 7, 0, 0, 1, 3, 0, 0, 6, 7},
		/*---------------------------*/
		{5, 4, 5, 4, 5, 4, 5, 4, 5, 4},
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
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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

Game::Game(void) : _window(), _running(true), _fps(0), _ups(0), _caption("Black Knight Blockade"), _tick(0), _rageticks(0), _scoreticks(0), _killcount(0), _passcount(0), _rage(0), _doreset(false), _titlewait(true) {
	
	_window.create(sf::VideoMode(640, 640), _caption, sf::Style::Titlebar | sf::Style::Close);
	_window.setVerticalSyncEnabled(true);
	_window.setView(sf::View(sf::FloatRect(0, 0, 160, 160)));
	_window.setIcon(16, 16, (sf::Uint8*)
		"\0\0\0\0\0\0\0\0\272\0\0\377\315\0\0\377\0\0\0\377\272\0\0\377\315\0\0\377"
		"\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0"
		"\0\0\0\0\0\0\0\0\0\0\0\0\0\214\0\0\377\315\0\0\377&&&\377\214\0\0\377\315"
		"\0\0\377III\377III\377III\377rrr\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0"
		"\0\0\0\0\315\0\0\377\315\0\0\377\272\0\0\377\315\0\0\377III\377\214\0\0\377"
		"\272\0\0\377\315\0\0\377\315\0\0\377III\377III\377rrr\377\0\0\0\377\0\0\0"
		"\377\0\0\0\0\0\0\0\0\214\0\0\377\214\0\0\377\214\0\0\377III\377rrr\377rr"
		"r\377\214\0\0\377\214\0\0\377\214\0\0\377rrr\377rrr\377III\377rrr\377\0\0"
		"\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377&&&\377III\377III\377III"
		"\377III\377&&&\377&&&\377III\377&&&\377III\377\0\0\0\377\0\0\0\0\0\0\0\0"
		"\315\0\0\377\315\0\0\377\315\0\0\377&&&\377III\377III\377\315\0\0\377\315"
		"\0\0\377\315\0\0\377\0\0\0\377III\377\0\0\0\377rrr\377\0\0\0\377\0\0\0\0"
		"\0\0\0\0\214\0\0\377\214\0\0\377\272\0\0\377\315\0\0\377III\377\214\0\0\377"
		"\272\0\0\377\214\0\0\377\315\0\0\377III\377III\377III\377rrr\377\0\0\0\377"
		"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\214\0\0\377\315\0\0\377&&&\377\214\0\0\377"
		"\315\0\0\377III\377III\377III\377III\377rrr\377\0\0\0\377\0\0\0\0\0\0\0\0"
		"\0\0\0\0\0\0\0\0\0\0\0\0\214\0\0\377\315\0\0\377\0\0\0\377\214\0\0\377\315"
		"\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0"
		"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377&&&\377&&&\377III\377"
		"\0\0\0\377\0\0\0\377III\377rrr\377rrr\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0"
		"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377&&&\377III\377\0\0\0\377bL\14\377mR\1"
		"\377\0\0\0\377III\377rrr\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
		"\0\0\0\0\0\0\377&&&\377III\377III\377\0\0\0\377MB!\377bL\14\377\0\0\0\377"
		"III\377III\377rrr\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377"
		"&&&\377III\377III\377III\377\0\0\0\377\0\0\0\377III\377III\377III\377rrr"
		"\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377"
		"\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0"
		"\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377MB!\377"
		"MB!\377WG\27\377\0\0\0\377MB!\377WG\27\377bL\14\377mR\1\377\0\0\0\377\0\0"
		"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377"
		"\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0"
		"\0\0\0\0\0\0\0\0\0\0");

	// preload textures and sounds
	_window.clear();
	_window.display();

	gettexture("font.png");
	gettexture("title.png");
	gettexture("map.png");
	gettexture("peasant.png");
	gettexture("player.png");
	gettexture("poof.png");
	getsound("gameover.wav");
	getsound("getaway.wav");
	getsound("hurt.wav");
	getsound("poof.wav");
	getsound("stroke.wav");
}

Game::~Game(void) {
  
}

int Game::run(void) {
	const int logicSteps = 100;
	const sf::Time logicStep = sf::seconds(1) / static_cast<float>(logicSteps);
	
	sf::Event e;
	sf::Clock updateClock;
	sf::Clock statClock;
	sf::Time passedUpdateTime;
	sf::Time passedStatTime;
	int numFrames = 0;
	int numUpdates = 0;
#ifndef NDEBUG
	char caption[256];
#endif
	bool keydown = false;

	sf::Texture &maptex = gettexture("map.png");

	for (int i = 0; i < KEY_COUNT; _keytapped[i] = _keydown[i] = false, i++);
	for (int i = 0; i < 16; i++) {
		_mapsprites[i].setTexture(maptex);
		_mapsprites[i].setTextureRect(sf::IntRect((i % 4) * 16, (i / 4) * 16, 16, 16));
	}
	
	_fontsprite.setTexture(gettexture("font.png"));
	_fontsprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
	_fontsprite.setOrigin(0, 0);

	_title.setTexture(gettexture("title.png"));

	_reset();

	while(_window.isOpen()) {
		while (_window.pollEvent(e) && _running) {
			keydown = false;
			switch (e.type) {
			case sf::Event::Closed:
				_window.close();
				_running = false;
				break;
			case sf::Event::KeyPressed:
				keydown = true;
			case sf::Event::KeyReleased:
				switch (e.key.code) {
				case sf::Keyboard::Escape:
					_window.close();
					_running = false;
					break;
				case sf::Keyboard::Left:
				case sf::Keyboard::A:
					_keyevent(KEY_LEFT, keydown);
					break;
				case sf::Keyboard::Right:
				case sf::Keyboard::D:
					_keyevent(KEY_RIGHT, keydown);
					break;
				case sf::Keyboard::Up:
				case sf::Keyboard::W:
					_keyevent(KEY_UP, keydown);
					break;
				case sf::Keyboard::Down:
				case sf::Keyboard::S:
					_keyevent(KEY_DOWN, keydown);
					break;
				case sf::Keyboard::Space:
				case sf::Keyboard::Return:
					_keyevent(KEY_ATTACK, keydown);
					break;
				}
				break;
			}
		}

		if (_doreset)
			_reset();

		passedUpdateTime += updateClock.restart();
		for (; _running && passedUpdateTime > logicStep; passedUpdateTime += updateClock.restart() - logicStep) {
			if (numUpdates < logicSteps) {
				_update();
				_entities.erase(std::remove_if(_entities.begin(), _entities.end(), &::delcheck), _entities.end());
				for (int i = 0; i < KEY_COUNT; _keytapped[i] = false, i++);
				++numUpdates;
				sf::sleep(sf::microseconds(1));
			}
			else {
				passedUpdateTime = sf::Time::Zero;
				break;
			}
		}
		++numFrames;

		std::sort(_entities.begin(), _entities.end(), &::cmpentity);

		if (!_running) {
			_window.close();
			break;
		}
		_draw();

		passedStatTime += statClock.restart();
		if (static_cast<int>(passedStatTime.asSeconds()) > 0) {
			passedStatTime = sf::Time::Zero;
			_fps = numFrames;
			_ups = numUpdates;
			numFrames = 0;
			numUpdates = 0;
#ifndef NDEBUG
			//snprintf(caption, 255, "%s [fps: %d, ups: %d]", _caption, _fps, _ups);
			snprintf(caption, 255, "BKB [fps: %d, ups: %d]", _fps, _ups);
			_window.setTitle(caption);
#endif
		}
	}

	std::for_each(_entities.begin(), _entities.end(), ::delentity);
	std::for_each(_sounds.begin(), _sounds.end(), ::delsound);
	std::for_each(_buffers.begin(), _buffers.end(), ::delbuffer);
	std::for_each(_textures.begin(), _textures.end(), ::deltexture);

	return 0;
}

void Game::_update(void) {
	if (_titlewait) {
		if (keydown(KEY_ATTACK)) {
			_titlewait = false;
			_killcount = 0;
			for (int i = 0; i < KEY_COUNT; _keytapped[i] = _keydown[i] = false, i++);
		}
		return;
	}

	if (_tick++ % 1500 == 0)
		addpeasant();
	if (_rageticks)
		_rageticks--;
	if (_scoreticks)
		_scoreticks--;
	for (int i = 0, j = _entities.size(); i < j; _entities[i]->update(), i++);
}

void Game::_draw(void) {
	static const sf::Color back(87, 127, 63);
	_window.clear(back);
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

	const sf::Color &c = (_rageticks && (_rageticks / 4) % 2) ? sf::Color::Red : sf::Color::White;
	const sf::Color &c1 = (_scoreticks && (_scoreticks / 4) % 2) ? sf::Color::Green : sf::Color::White;

	drawtext(_scoretext, 25, 5, c1);
	if (_titlewait) {
		_window.draw(_title);
	}
	else {
		drawtext("R", 5, 5);
		drawtext("(", 5, 10, c);
		drawtext(_rage > 9 ? "X" : "_", 5, 20, c);
		drawtext(_rage > 8 ? "X" : "_", 5, 30, c);
		drawtext(_rage > 7 ? "X" : "_", 5, 40, c);
		drawtext(_rage > 6 ? "X" : "_", 5, 50, c);
		drawtext(_rage > 5 ? "X" : "_", 5, 60, c);
		drawtext(_rage > 4 ? "X" : "_", 5, 70, c);
		drawtext(_rage > 3 ? "X" : "_", 5, 80, c);
		drawtext(_rage > 2 ? "X" : "_", 5, 90, c);
		drawtext(_rage > 1 ? "X" : "_", 5, 100, c);
		drawtext(_rage > 0 ? "X" : "_", 5, 110, c);
		drawtext(")", 5, 120, c);
	}
	
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

bool Game::water(char x, char y) {
	return y != 5 && (x == 4 || x == 5);
}

sf::Texture &Game::gettexture(const char *file) {
	std::map<const std::string, sf::Texture*>::iterator a = _textures.find(file);
	if (a != _textures.end())
		return *a->second;
	sf::Texture *n = new sf::Texture();
	if (!n->loadFromFile(file))
		throw "Failed to load texture.";
	_textures[file] = n;
	return *n;
}

sf::SoundBuffer &Game::getbuffer(const char *file) {
	std::map<const std::string, sf::SoundBuffer*>::iterator a = _buffers.find(file);
	if (a != _buffers.end())
		return *a->second;
	sf::SoundBuffer *n = new sf::SoundBuffer();
	if (!n->loadFromFile(file))
		throw "Failed to load sound.";
	_buffers[file] = n;
	return *n;
}

sf::Sound &Game::getsound(const char *file) {
	std::map<const std::string, sf::Sound*>::iterator a = _sounds.find(file);
	if (a != _sounds.end())
		return *a->second;
	sf::Sound *n = new sf::Sound();
	n->setBuffer(getbuffer(file));
	_sounds[file] = n;
	return *n;
}

void Game::addkill(void) {
	_killcount++;
	_rage = std::max(_rage - 1, 0);
	_scoreticks = 40;
	_updatescore();
}

void Game::addpass(void) {
	_passcount++;
	if (_rage < 10) { // collect up to 10 rage
		_rage++;
		((Player*)_player)->setangry();
		getsound("getaway.wav").play();
		_rageticks = 40;
	}
	else if (_rage == 10) { // 11 rage -> game over
		_player->querydeletion(); // i.e. kill the player
		getsound("gameover.wav").play();
		for (int i = 0; i < KEY_COUNT; _keytapped[i] = _keydown[i] = false, i++);
	}
	_updatescore();
}

void Game::drawtext(const char *text, int x, int y, const sf::Color &c) {
	_fontsprite.setColor(c);
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
		_fontsprite.setPosition(static_cast<float>(x), static_cast<float>(y));
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

	Peasant *p = new Peasant(this, starts[i][0], starts[i][1]);
	p->settarget((TARGET)starts[i][2]);
	_entities.push_back(p);
}

void Game::addpoof(int x, int y) {
	Poof *p = new Poof(this, x, y);
	_entities.push_back(p);
}

void Game::adddust(int x, int y) {
	Dust *p = new Dust(this, x, y);
	_entities.push_back(p);
}

void Game::_updatescore(void) {
	snprintf(_scoretext, 32, "Sx%d", _killcount);
}

void Game::_reset(void) {
	_doreset = false;
	std::for_each(_entities.begin(), _entities.end(), ::delentity);
	_entities.clear();
	_tick = _passcount = _rage = 0;
	_updatescore();
	_entities.push_back(_player = new Player(this, 80, 88));
	_titlewait = true;
}

void Game::reset(void) {
	_doreset = true;
}