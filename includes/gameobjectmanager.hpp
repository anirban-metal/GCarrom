#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "ingameobjects.hpp"
#include "game.hpp"

class objectlist
{
private:
    int size;
    bool *isActive;
public:
    gameObject **list;
    objectlist(int n);
    ~objectlist();
    void setActive(int i);
    void setInActive(int i);
    bool getState(int i) const;
    void checkCollision(int i, int j);
    void checkAllCollisions();
    void updateall();
    void arrangeCoins();
    void rotateCoins(sf::Event &event);
    bool hasAllStopped();
    int isboardover(); //return 0 for flase, 1 for white, 1 for black
};

#endif
