#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ingameobjects.hpp"
#include "game.hpp"
#include "gameobjectmanager.hpp"

void game::init()
{
    _gamestate = _init;

    player = 0;
    turn = 0;
    turnincr = 1;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    window.create(sf::VideoMode(windowsize, windowsize, 32), "gnarrom",
            sf::Style::Default);
    window.setVerticalSyncEnabled(1);
    //window.setFramerateLimit(60);
    window.setMouseCursorVisible(0);
    brd.loadFromFile("../resources/board.png");
    brd.setSmooth(1);
    B.setTexture();

    cointex.loadFromFile("../resources/coin.png");
    cointex.setSmooth(1);
    strikertex.loadFromFile("../resources/striker.png");
    strikertex.setSmooth(1);

    soundbuf.loadFromFile("../resources/sound.ogg");
    sound.setBuffer(soundbuf);
    sound.setLoop(0);

    _gamestate = _new;

    frametime = sf::seconds(0);

    loop();
}

void game::loop()
{
    //We do not intend the objectslist to have a lifetime beyond the scope of this function
    objects = new objectlist(21);

    objects->list[0] = new striker();
    objects->list[1] = new coin();

    for (int i = 2; i < 11; i++) {
        objects->list[i] = new coin('w');
    }
    for (int i = 11; i < 20; i++) {
        objects->list[i] = new coin('b');
    }

    objects->list[20] = new pointer();
    objects->arrangeCoins();

    sf::Thread endturnthread(&endturn);

    //--------------------------------------------------------------------------
    sf::Clock deltaTime;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            changeState(event);
            if (event.type == sf::Event::Closed ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (_gamestate == _new) {
                captureMouse();
                objects->rotateCoins(event);
            }

            if (_gamestate == _quit) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                capmouse = 0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                capmouse = 1;
            }
        }

        window.clear(brdcolor);
        B.draw();

        if (objects->isboardover() > 0) {
            _gamestate = _quit;
        }

        if (_gamestate == _hit) {
            objects->checkCollision(0, 20);
        }

        if (_gamestate == _motion) {
            objects->checkAllCollisions();
            endturnthread.launch();
        }

        for (int i = 0; i < 20; i++) {
            if (objects->list[i]->isInPocket()) {
                if (objects->getState(i) && i != 0) {
                    processPocket(i);
                }
                objects->setInActive(i);
            }
        }

        frametime = deltaTime.restart();
        objects->updateall();
        window.display();
    }

    delete objects;
}

void game::endturn()
{
    if (_gamestate == _motion) {
        if (objects->hasAllStopped()) {
            if (objects->list[0]->isInPocket()) {
                processFine();
            }

            sf::sleep(sf::milliseconds(100));

            coinsinpocket = 0;
            turn += turnincr;
            turnincr = 1;
            player = turn % player_count;
            objects->list[0]->reset();
        }
    }
}

void game::captureMouse()
{
    if (!capmouse) return;
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (mousePos.x > board::right || mousePos.x < board::left || mousePos.y > board::bottom || mousePos.y < board::top) {
        sf::Mouse::setPosition(sf::Vector2i(300, 300), window);
    }
}

void game::changeState(sf::Event &event)
{
    //Check if any joystick is connected for given player
    if (sf::Joystick::isConnected(player)) {

        if (_gamestate != _motion && (sf::Joystick::isButtonPressed(player, 0) || sf::Joystick::isButtonPressed(player, 1) ||
                    sf::Joystick::isButtonPressed(player, 2) || sf::Joystick::isButtonPressed(player, 3))) {
            if (_gamestate == _new) {
                _gamestate = _striker;
            }
            else {
                _gamestate = _pointer;
            }
        }
        if (_gamestate == _pointer && !(sf::Joystick::isButtonPressed(player, 0) || sf::Joystick::isButtonPressed(player, 1) ||
                    sf::Joystick::isButtonPressed(player, 2) || sf::Joystick::isButtonPressed(player, 3))) {
            _gamestate = _hit;
        }
    }
    else {
        if(_gamestate != _motion && event.type == sf::Event::MouseButtonPressed){
            if (_gamestate == _new) {
                _gamestate = _striker;
            }
            else {
                _gamestate = _pointer;
            }
        }
        else if(_gamestate == _pointer && event.type == sf::Event::MouseButtonReleased){
            _gamestate = _hit;
        }
    }
}

void game::playsound(float impact)
{
    if (!(sound.getStatus() == sf::Sound::Playing)) {
        if (impact < 2000.f) {
            sound.setVolume(impact/20.f);
        }
        else {
            sound.setVolume(100.f);
        }
        sound.play();
    }
}

void game::setwindowsize(int winsize)
{
    windowsize = winsize;
}

void game::close()
{
    _gamestate = _quit;
    window.close();
}

void game::processPocket(int n)
{
    if (player == 0 && n > 0 && n < 11) {
        turnincr = 0;
        coinsinpocket++;
    }
    else if (player == 1 && (n == 1 || (n > 10 && n < 20))) {
        turnincr = 0;
        coinsinpocket++;
    }
}

void game::processFine()
{
    do {
        if (player == 0) {
            for (int i = 2; i < 11; i++) {
                if (objects->getState(i) == 0) {
                    objects->list[i]->reset();
                    objects->setActive(i);
                    coinsinpocket -= 1;
                    break;
                }
            }
        }
        else if (player == 1) {
            for (int i = 11; i < 20; i++) {
                if (objects->getState(i) == 0) {
                    objects->list[i]->reset();
                    objects->setActive(i);
                    coinsinpocket -= 1;
                    break;
                }
            }
        }
    } while (coinsinpocket > 0);
}

void game::setColor(int brdR, int brdG, int brdB,
        int wR, int wG, int wB,
        int bR, int bG, int bB,
        int qR, int qG, int qB,
        int sR, int sG, int sB,
        int piR, int piG, int piB,
        int poR, int poG, int poB)
{
}

sf::RenderWindow game::window;
game::gameStates game::_gamestate;
sf::Texture game::brd;
sf::Texture game::cointex;
sf::Texture game::strikertex;
sf::Time game::frametime = sf::seconds(0);
objectlist *game::objects;
board game::B;
int game::player_count = 2;
int game::player = 0;
int game::turn = 0;
int game::turnincr = 1;
int game::windowsize = 700;
int game::coinsinpocket = 0; //stores the number of coins pocketed in present turn;
sf::Color game::brdcolor(221, 218, 216);
sf::Color game::wcolor = sf::Color::White;
sf::Color game::bcolor(50, 50, 50);
sf::Color game::qcolor = sf::Color::Red;
sf::Color game::scolor = sf::Color::Green;
sf::Color game::pointercolorin = sf::Color::Green;
sf::Color game::pointercolorout = sf::Color::Black;
bool game::capmouse = 1;
sf::SoundBuffer game::soundbuf;
sf::Sound game::sound;
