#include "Game\Game.h"

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>

//http://stackoverflow.com/questions/6469849/one-or-more-multiply-defined-symbols-found
//https://www.youtube.com/watch?v=p33yOjgPSb4

int main()
{
	Game game;
	while (game.isRunning())
	{
		game.update();
		game.draw();
		game.lateUpdate();
	}

	return 0;
}

