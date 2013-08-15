#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ingameobjects.hpp"

class objectlist;

class game
{
private:
    static sf::RenderWindow window;
    enum gameStates {_init, _new, _striker, _pointer, _hit, _motion,  _quit};
    static gameStates _gamestate;
    static sf::Texture brd;
    static sf::Texture cointex;
    static sf::Texture strikertex;
    static board B;
    static void loop();
    static void changeState(sf::Event &event);
    static void captureMouse();
    static void close();
    static int player_count;
    static int player;
    static int turn;
    static int turnincr;
    static int coinsinpocket;
    static int windowsize;
    static bool capmouse;
    static sf::SoundBuffer soundbuf;
    static sf::Sound sound;
    static sf::Time frametime;
    static sf::Color brdcolor;
    static sf::Color wcolor;
    static sf::Color bcolor;
    static sf::Color qcolor;
    static sf::Color scolor;
    static sf::Color pointercolorin;
    static sf::Color pointercolorout;
    static objectlist *objects;
    static void processPocket(int i);
    static void processFine();
    static void endturn();
public:
    static void init();
    static void playsound(float impact);
    static void setwindowsize(int winsize);
    static void setColor(int brdR,int brdG,int brdB,
			 int wR, int wG, int wB,
			 int bR, int bG, int bB,
			 int qR, int qG, int qB,
			 int sR, int sG, int sB,
			 int piR, int piG, int piB,
			 int poR, int poG, int poB);
    friend class board;
    friend class gameObject;
    friend class striker;
    friend class coin;
    friend class pointer;
    friend class objectlist;
    friend void collision(gameObject* A, gameObject* B);
    friend void collisionloop();
};

#endif
