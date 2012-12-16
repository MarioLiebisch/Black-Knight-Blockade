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
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <map>

enum GAME_KEY {
	KEY_LEFT = 0,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_ATTACK,
	KEY_COUNT
};

class Entity;

class Game {
private:
	sf::RenderWindow _window;
	bool _running;
	int _fps;
	int _ups;
	const char *_caption;
	bool _keytapped[KEY_COUNT];
	bool _keydown[KEY_COUNT];
	std::vector<Entity*> _entities;
	sf::Sprite _mapsprites[16];
	sf::Sprite _fontsprite;
	std::map<const std::string, sf::Texture*> _textures;
	std::map<const std::string, sf::SoundBuffer*> _buffers;
	std::map<const std::string, sf::Sound*> _sounds;
	Entity *_player;
	unsigned int _tick;
	int _killcount;
	int _passcount;
	int _rage;
	char _scoretext[32];
	bool _doreset;
public:
	Game(void);
	~Game(void);
	int run(void);
	bool keytapped(GAME_KEY key) { return _keytapped[key]; }
	bool keydown(GAME_KEY key) { return _keydown[key]; }
	bool solid(char x, char y);
	sf::Texture &gettexture(const char *file);
	sf::SoundBuffer &getbuffer(const char *file);
	sf::Sound &getsound(const char *file);
	sf::RenderTarget &gettarget(void) {return _window; }
	Entity *getplayer(void) { return _player; }
	void addpeasant(void);
	void addpoof(int x, int y);
	void drawtext(const char *text, int x, int y);
	void addkill(void);
	void addpass(void);
	void reset(void);
private:
	void _draw(void);
	void _update(void);
	void _keyevent(const GAME_KEY gkey, const bool pressed);
	void _keysreset(void);
	void _updatescore(void);
	void _reset(void);
};
