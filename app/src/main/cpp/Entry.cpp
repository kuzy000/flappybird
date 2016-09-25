#include "Entry.h"

#include "Engine/Engine.h"
#include "Game/Game.h"

#include <memory>

namespace Flappy {

int entry(Platform::Context& context) {
	Engine engine {context};
	
	std::unique_ptr<Game> game;
	
	engine.on_start.watch([&engine, &game] {
		game = std::unique_ptr<Game> {new Game(engine)};
		game->new_game();
	});

	engine.run();

	return 0;
}

}
