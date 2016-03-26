#include <SFML/System.hpp>
#include "game.hpp"

//Create a separate global thread for our game, so that the gtk frontend and the game
//run in different threads

class launcher
{
    public:
        static void startgame()
        {
            int winsize_int = 700;
            game::setwindowsize(winsize_int);
            game::init();
        }

};

int main(int argc, char **argv) {
    launcher::startgame();
    return 0;
}

