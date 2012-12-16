===============================================================================

  Black Knight Blockade

===============================================================================

  A simple game entry for Ludum Dare 25: You Are The Villain

-------------------------------------------------------------------------------

CONTENTS:
1. About The Game
2. Controls
3. Building The Game (from sources)

-------------------------------------------------------------------------------

1. About The Game
-----------------
You are the feared Black Knight. You are in charge of a small passage in the
dark wood and you decide who's allowed to pass: no one!

Unfortunately there are quite a few pesky peasants out trying to cross the
bridge to do some of their daily bidding, something you shall not endorse.

Keep the peasants from crossing, but watch your rage, as you might get angry
enough causing your head to explode in anger. You don't want your head explode,
do you?

Every killed peasant will lower your anger, while every escaped peasant will
raise your rage. If your rage exceeds your rage meter, you're going to explode
and the game is lost. Use your attack key to restart the game.

2. Controls
-----------
The game cam be played with two sets of controls:

+--------------+---------------+---------------+
| Action       | Alternative 1 | Alternative 2 |
+--------------+---------------+---------------+
| Walk left    | Cursor left   | A             |
| Walk right   | Cursor right  | D             |
| Walk up      | Cursor up     | W             |
| Walk down    | Cursor down   | S             |
| Sword thrust | Space         | Return        |
+--------------+---------------+---------------+

3. Building The Game
--------------------
To create build files (e.g. makefiles or project files) for your system you'll
need CMake (http://www.cmake.org/).
Furthermore, you'll need a copy of SFML 2 (http://www.sfml-dev.org/) as well as
further dependiencies, such as OpenGL or OpenAL audio headers (depending on
your system).
