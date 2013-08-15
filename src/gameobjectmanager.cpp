#include "gameobjectmanager.hpp"
#include <math.h>

objectlist::objectlist(int n)
{
    size = n;
    list = new gameObject* [n];
    isActive = new bool [n];
    for (int i = 0; i < n; i++) {
        isActive[i] = 1; //All objects are drawn by default
    }
}

objectlist::~objectlist()
{
    for (int i = 0; i < size; i++) {
        delete list[i];
    }
    delete[] list;
    delete[] isActive;
}


void objectlist::checkCollision(int i, int j)
{
    if (isActive[i] && isActive[j]) {
        collision(list[i], list[j]);
    }
}

void objectlist::checkAllCollisions()
{
    for (int i = 0; i < size - 2; i++) {
        for (int j = i+1; j < size - 1; j++) {
            if(isActive[i] && isActive[j]) {
                checkCollision(i, j);
            }
        }
    }
}

void objectlist::updateall()
{
    if (game::_gamestate != game::_new) {
        list[0]->update();
    }
    for (int i = 1; i < size; i++) {
        if(isActive[i]) {
            list[i]->update();
        }
    }
}

void objectlist::setActive(int i)
{
    isActive[i] = 1;
}

void objectlist::setInActive(int i)
{
    isActive[i] = 0;
}

bool objectlist::getState(int i) const
{
    return isActive[i];
}

void objectlist::arrangeCoins()
{
    float r = list[1]->circle.getRadius();
    list[1]->circle.setPosition(game::windowsize/2.0,
				game::windowsize/2.0);

    list[2]->circle.setPosition(game::windowsize/2.0,
				game::windowsize/2.0 - 2*r);
    list[11]->circle.setPosition(game::windowsize/2.0 + sqrt(3)*r,
				 game::windowsize/2.0 - r);
    list[3]->circle.setPosition(game::windowsize/2.0 +sqrt(3)*r,
				game::windowsize/2.0 + r);
    list[12]->circle.setPosition(game::windowsize/2.0,
				 game::windowsize/2.0 + 2*r);
    list[4]->circle.setPosition(game::windowsize/2.0 - sqrt(3)*r,
				game::windowsize/2.0 + r);
    list[13]->circle.setPosition(game::windowsize/2.0 - sqrt(3)*r,
				 game::windowsize/2.0 - r);

    list[5]->circle.setPosition(game::windowsize/2.0,
				game::windowsize/2.0 - 4*r);
    list[14]->circle.setPosition(game::windowsize/2.0 + sqrt(3)*r,
				 game::windowsize/2.0 - 3*r);
    list[6]->circle.setPosition(game::windowsize/2.0 + 2*sqrt(3)*r,
				game::windowsize/2.0 - 2*r);
    list[15]->circle.setPosition(game::windowsize/2.0 + 2*sqrt(3)*r,
				 game::windowsize/2.0);
    list[7]->circle.setPosition(game::windowsize/2.0 + 2*sqrt(3)*r,
				game::windowsize/2.0 + 2*r);

    list[16]->circle.setPosition(game::windowsize/2.0 + sqrt(3)*r,
				 game::windowsize/2.0 + 3*r);
    list[8]->circle.setPosition(game::windowsize/2.0, game::windowsize/2.0 + 4*r);
    list[17]->circle.setPosition(game::windowsize/2.0 - sqrt(3)*r,
				 game::windowsize/2.0 + 3*r);
    list[9]->circle.setPosition(game::windowsize/2.0 - 2*sqrt(3)*r,
				game::windowsize/2.0 + 2*r);
    list[18]->circle.setPosition(game::windowsize/2.0 - 2*sqrt(3)*r,
				 game::windowsize/2.0);
    list[10]->circle.setPosition(game::windowsize/2.0 - 2*sqrt(3)*r,
				 game::windowsize/2.0 - 2*r);
    list[19]->circle.setPosition(game::windowsize/2.0 - sqrt(3)*r,
				 game::windowsize/2.0 - 3*r);
}

void objectlist::rotateCoins(sf::Event &event)
{
    for (int i = 1; i < 20; i++) {
        sf::Vector2f position = list[i]->circle.getPosition();
        sf::Vector2f relpos = sf::Vector2f(position.x - game::windowsize/2.0, position.y - game::windowsize/2.0);
        float radiusvec = sqrt(relpos.x*relpos.x + relpos.y*relpos.y);
        float o = atan2(relpos.y, relpos.x);
        if (sf::Joystick::isConnected(game::player)) {
            if (fabs(sf::Joystick::getAxisPosition(game::player, sf::Joystick::X)) > 0.5f) {
                list[i]->circle.setPosition(radiusvec*cos(o - 0.01f) + game::windowsize/2.0,
					    radiusvec*sin(o - 0.01f) + game::windowsize/2.0);
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            list[i]->circle.setPosition(radiusvec*cos(o - 0.01f) + game::windowsize/2.0,
					radiusvec*sin(o - 0.01f) + game::windowsize/2.0);
        }
    }
}


bool objectlist::hasAllStopped()
{
    for (int i = 0; i < 20; i++) {
        if (!((fabs(list[i]->vel_x) < 5.f && fabs(list[i]->vel_y) < 5.f) || list[i]->isInPocket())) {
            return 0;
        }
    }
    return 1;
}

int objectlist::isboardover()
{
    bool w = 1, b = 1;
    for (int i = 2; i < 11; i++) {
	if (isActive[i] == 1) {
	    w = 0;
	    break;
	}
    }
    for (int i = 11; i < 20; i++) {
	if (isActive[i] == 1) {
	    b = 0;
	    break;
	}
    }
    if (!isActive[1] && w) return 1;
    else if (!isActive[1] && b) return 2;
    return 0;
}
