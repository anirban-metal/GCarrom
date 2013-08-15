#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "gameobjectmanager.hpp"
#include "game.hpp"
#include "ingameobjects.hpp"

striker::striker()
{
    circle.setRadius(0.0267f*game::windowsize);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(game::windowsize/2.0f, game::windowsize/2.0f);
    circle.setFillColor(game::scolor);
    circle.setOutlineThickness(-3.f);
    circle.setOutlineColor(game::scolor);
    circle.setTexture(&game::strikertex);
    sf::Mouse::setPosition(sf::Vector2i(300, 495), game::window);
    relMousePos = sf::Vector2f(300, 495);
    vel_x = vel_init;
    vel_y = vel_init;
    massfactor = 2.8f;
    restitution = 0.9f;
    frictionfactor = 1.f;
    position = circle.getPosition();
}

void striker::update()
{
    position = circle.getPosition();
    if (game::_gamestate == game::_striker) {
        input();
        boundtoplay();
    }
    else if (game::_gamestate == game::_motion) {
        move();
    }
    boundtoboard();
    game::window.draw(circle);
}

void striker::input()
{
    if (sf::Joystick::isConnected(game::player)) {
        circle.move(0.03f*sf::Joystick::getAxisPosition(game::player, sf::Joystick::X),
                0.03f*sf::Joystick::getAxisPosition(game::player, sf::Joystick::Y));
    }
    else {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game::window);
        sf::Vector2f circleMove(mousePos.x - relMousePos.x, mousePos.y - relMousePos.y);
        circle.setPosition(circle.getPosition().x + circleMove.x, circle.getPosition().y + circleMove.y);
        game::captureMouse();
        relMousePos = sf::Vector2f(sf::Mouse::getPosition(game::window).x, sf::Mouse::getPosition(game::window).y);
    }
}

void striker::boundtoplay(){

    for (int i = 1; i < 20; i++) {

        sf::Vector2f posA = circle.getPosition();
        sf::Vector2f posB = game::objects->list[i]->circle.getPosition();

        float dist = sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));

        if (dist < circle.getRadius() + game::objects->list[i]->circle.getRadius() + 0.001f) {
            if (posA.x < posB.x) {
                circle.setPosition(posB.x + circle.getRadius() +
                        game::objects->list[i]->circle.getRadius() + 0.0015f,
                        posA.y);
            }
            else {
                circle.setPosition(posB.x - circle.getRadius() -
                        game::objects->list[i]->circle.getRadius() - 0.0015f,
                        posA.y);
            }
        }

        posA = circle.getPosition();
        dist = sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));

        if (dist < circle.getRadius() + game::objects->list[i]->circle.getRadius() + 0.001f) {
            circle.setPosition(game::windowsize/2.0f, posA.y);
        }
    }

    sf::Vector2f circlePos = circle.getPosition();
    float x = circlePos.x;
    float y = circlePos.y;
    if (game::player == 0) {
        if (y < board::bottom - board::playzonein ) {
            circle.setPosition(sf::Vector2f(x, board::bottom - board::playzonein));
        }
        if (y > board::bottom - board::playzoneout) {
            circle.setPosition(sf::Vector2f(x, board::bottom - board::playzoneout));
        }

        circlePos = circle.getPosition();
        x = circlePos.x;
        y = circlePos.y;

        if(x + y < game::windowsize + circle.getRadius() + 0.011f*game::windowsize) {
            circle.setPosition(game::windowsize + circle.getRadius() + 0.011f*game::windowsize - y, y);
        }

        else if(y - x < circle.getRadius() + 0.011f*game::windowsize) {
            circle.setPosition(y - circle.getRadius() - 0.011f*game::windowsize, y);
        }
    }

    else if (game::player == 1) {
        if (y > board::top + board::playzonein ) {
            circle.setPosition(sf::Vector2f(x, board::top + board::playzonein));
        }
        if (y < board::top + board::playzoneout) {
            circle.setPosition(sf::Vector2f(x, board::top + board::playzoneout));
        }

        circlePos = circle.getPosition();
        x = circlePos.x;
        y = circlePos.y;

        if(x + y > game::windowsize - circle.getRadius() - 0.011f*game::windowsize) {
            circle.setPosition(game::windowsize - circle.getRadius() - 0.011f*game::windowsize - y, y);
        }

        else if(y - x + circle.getRadius() + 0.011f*game::windowsize > 0) {
            circle.setPosition(y + circle.getRadius() + 0.011f*game::windowsize, y);
        }
    }

    for (int i = 1; i < 20; i++) {

        sf::Vector2f posA = circle.getPosition();
        sf::Vector2f posB = game::objects->list[i]->circle.getPosition();

        float dist = sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));

        if (dist < circle.getRadius() + game::objects->list[i]->circle.getRadius() + 0.001f) {
            if (posA.x < game::windowsize/2.0) {
                circle.setPosition(posB.x + circle.getRadius() +
                        game::objects->list[i]->circle.getRadius() + 0.0015f , posA.y);
            }
            else {
                circle.setPosition(posB.x - circle.getRadius() -
                        game::objects->list[i]->circle.getRadius() - 0.0015f, posA.y);
            }
        }
    }

}

void striker::reset()
{
    vel_x = vel_init;
    vel_y = vel_init;
    circle.setPosition(game::windowsize/2.0, game::windowsize/2.0);
    game::_gamestate = game::_striker;
    game::objects->setActive(0);
}

coin::coin(char t)
{
    circle.setRadius(0.019f*game::windowsize);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(-0.005*game::windowsize);
    circle.setPosition(300, 300);
    circle.setTexture(&game::cointex);
    type = t;
    switch(type){
        case 'w':
            circle.setFillColor(game::wcolor);
            circle.setOutlineColor(game::wcolor);
            break;
        case 'b':
            circle.setFillColor(game::bcolor);
            circle.setOutlineColor(game::bcolor);
            break;
        default:
            circle.setFillColor(game::qcolor);
            circle.setOutlineColor(game::qcolor);
    }
    vel_x = vel_init;
    vel_y = vel_init;
    massfactor = 1.f;
    restitution = 0.95f;
    frictionfactor = 3.f;
}

void coin::update()
{
    if (game::_gamestate == game::_motion) {
        move();
    }
    boundtoboard();
    game::window.draw(circle);
}

void coin::reset()
{
    vel_x = vel_init;
    vel_y = vel_init;
    circle.setPosition(game::windowsize/2.0, game::windowsize/2.0);

    for (int i = 1; i < 20; i++) {
        if (this != game::objects->list[i]) {
            sf::Vector2f posA = circle.getPosition();
            sf::Vector2f posB = game::objects->list[i]->circle.getPosition();

            float dist = sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));

            if (dist < circle.getRadius() + game::objects->list[i]->circle.getRadius() + 0.001f) {
                if (posA.x > posB.x) {
                    circle.setPosition(posB.x + circle.getRadius() +
                            game::objects->list[i]->circle.getRadius() + 0.0015f , posA.y);
                }
                else {
                    circle.setPosition(posB.x - circle.getRadius() -
                            game::objects->list[i]->circle.getRadius() - 0.0015f, posA.y);
                }
            }
        }
    }
}

void board::setTexture()
{
    this->brd.setTexture(game::brd);
    brd.setScale(game::windowsize/2268.f, game::windowsize/2268.f);
    top = floor(0.033f*game::windowsize);
    bottom = game::windowsize - top;
    left = top;
    right = bottom;
    board::playzonein = floor(0.143*game::windowsize);
    board::playzoneout = floor(0.135*game::windowsize);
    board::pocket_offset = playzoneout;
}

void board::draw()
{
    game::window.draw(brd);
}


pointer::pointer()
{
    circle.setRadius(3.f);
    circle.setOrigin(3.f, 3.f);
    circle.setOutlineThickness(-1.f);
    circle.setFillColor(game::pointercolorin);
    circle.setOutlineColor(game::pointercolorout);
    relMousePos = sf::Vector2f(sf::Mouse::getPosition(game::window).x, sf::Mouse::getPosition(game::window).y);
    vel_x = 0.f;
    vel_y = 0.f;
    massfactor = 2.5f;
    restitution = 1.f;
    frictionfactor = 0.f;
}

void pointer::update()
{
    static float dist = 0.f;
    if(game::_gamestate == game::_striker){
        circle.setPosition(striker::position);
        relMousePos = sf::Vector2f(sf::Mouse::getPosition(game::window).x, sf::Mouse::getPosition(game::window).y);
        return;
    }

    else if (game::_gamestate == game::_pointer) {
        input();
        boundaroundstriker();
        dist = sqrt(pow(circle.getPosition().x - striker::position.x,2) +
                pow(circle.getPosition().y - striker::position.y,2));
    }
    else if (game::_gamestate == game::_hit) {
        hitStriker(dist);
    }
    game::window.draw(circle);
}

void pointer::input()
{
    if (sf::Joystick::isConnected(game::player)) {
        circle.move(0.02f*sf::Joystick::getAxisPosition(game::player, sf::Joystick::X),
                0.02f*sf::Joystick::getAxisPosition(game::player, sf::Joystick::Y));
    }
    else {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game::window);
        sf::Vector2f circleMove(mousePos.x - relMousePos.x, mousePos.y - relMousePos.y);
        circle.setPosition(circle.getPosition().x + circleMove.x, circle.getPosition().y + circleMove.y);
        game::captureMouse();
        relMousePos = sf::Vector2f(sf::Mouse::getPosition(game::window).x, sf::Mouse::getPosition(game::window).y);
    }
}


void pointer::boundaroundstriker()
{
    float x = circle.getPosition().x - striker::position.x;
    float y = circle.getPosition().y - striker::position.y;
    float o = atan2f(y, x);
    float r = sqrt(x*x + y*y);

    if (r > 0.135f*game::windowsize) {
        r = 0.135*game::windowsize;
    }
    else if (r < 0.03*game::windowsize){
        r = 0.03*game::windowsize;
    }

    x = r*cos(o);
    y = r*sin(o);

    circle.setPosition(striker::position.x + x, striker::position.y + y);
}

void pointer::hitStriker(float dist)
{
    float x = circle.getPosition().x - striker::position.x;
    float y = circle.getPosition().y - striker::position.y;
    float o = atan2(y, x);
    vel_x = -15.5f*dist/(0.03*sqrt(game::windowsize))*cos(o);
    vel_y = -15.5f*dist/(0.03*sqrt(game::windowsize))*sin(o);
    circle.move(vel_x*game::frametime.asSeconds(), vel_y*game::frametime.asSeconds());
}


void gameObject::boundtoboard()
{
    float impact = sqrt(vel_x*vel_x + vel_y*vel_y);
    sf::Vector2f circlePos = circle.getPosition();
    if (circlePos.x - circle.getRadius() < board::left ) {
        circle.setPosition(sf::Vector2f(board::left + circle.getRadius(), circlePos.y));
        vel_x *= -board::edgerestitution;
	game::playsound(impact);
    }
    if (circlePos.x + circle.getRadius() > board::right ) {
        circle.setPosition(sf::Vector2f(board::right - circle.getRadius(), circlePos.y));
        vel_x *= -board::edgerestitution;
	game::playsound(impact);
    }
    if (circlePos.y - circle.getRadius() < board::top ) {
        circle.setPosition(sf::Vector2f(circlePos.x, board::top + circle.getRadius()));
        vel_y *= -board::edgerestitution;
	game::playsound(impact);
    }
    if (circlePos.y + circle.getRadius() > board::bottom ) {
        circle.setPosition(sf::Vector2f(circlePos.x, board::bottom - circle.getRadius()));
        vel_y *= -board::edgerestitution;
	game::playsound(impact);
    }
}

void gameObject::move()
{

    if (fabs(vel_x) < 10.f)
        vel_x = vel_init;
    if (fabs(vel_y) < 10.f)
        vel_y = vel_init;

    circle.move(vel_x*game::frametime.asSeconds(), 0);
    circle.move(0, vel_y*game::frametime.asSeconds());


    if (fabs(vel_x) > vel_init)
        vel_x -= board::frictioncoefficient*massfactor*frictionfactor*game::frametime.asSeconds()*(vel_x)/(sqrt(vel_x*vel_x + vel_y*vel_y));
    if (fabs(vel_y) > vel_init)
        vel_y -= board::frictioncoefficient*massfactor*frictionfactor*game::frametime.asSeconds()*(vel_y)/(sqrt(vel_x*vel_x + vel_y*vel_y));

}

bool gameObject::isInPocket()
{
    sf::Vector2f circlePos = circle.getPosition();
    if (circlePos.x + circlePos.y - board::pocket_offset < 0) {
        return 1;
    };
    if (circlePos.x + circlePos.y - 2*game::windowsize + board::pocket_offset > 0) {
        return 1;
    }
    if (circlePos.y - circlePos.x + game::windowsize - board::pocket_offset < 0) {
        return 1;
    }
    if (circlePos.y - circlePos.x - game::windowsize +  board::pocket_offset > 0) {
        return 1;
    }
    return 0;
}

void collision(gameObject *A, gameObject *B)
{
    sf::Vector2f posA = A->circle.getPosition();
    sf::Vector2f posB = B->circle.getPosition();
    float o = atan2(posB.y - posA.y, posB.x - posA.x);
    float ov = atan2(B->vel_y - A->vel_y, B->vel_x - A->vel_x);
    float sinx = sin(o);
    float cosx = cos(o);
    float sinvx = sin(ov);
    float cosvx = cos(ov);
    float dist = sqrt(pow(posA.x - posB.x, 2) + pow(posA.y - posB.y, 2));
    float orv = cosvx*cosx + sinvx*sinx;
    if (dist < A->circle.getRadius() + B->circle.getRadius() + 0.1f) {
        if (orv < 0) {
            swapvelocities(A, B);
        }
        //dirty hack to ensure intersection of sprites does not occur
        float D =  A->circle.getRadius() + B->circle.getRadius() + 0.5 - dist;
        float xmove = D*fabs(cos(o))/(A->circle.getRadius() + B->circle.getRadius());
        float ymove = D*fabs(sin(o))/(A->circle.getRadius() + B->circle.getRadius());
        if (posA.x < posB.x) {
            A->circle.move(-xmove*A->circle.getRadius(), 0);
            B->circle.move(xmove*B->circle.getRadius(), 0);
        }
        else {
            A->circle.move(xmove*A->circle.getRadius(), 0);
            B->circle.move(-xmove*B->circle.getRadius(), 0);
        }
        if (posA.y < posB.y) {
            A->circle.move(0, -ymove*A->circle.getRadius());
            B->circle.move(0, ymove*B->circle.getRadius());
        }
        else {
            A->circle.move(0, ymove*A->circle.getRadius());
            B->circle.move(0, -ymove*B->circle.getRadius());
        }
        game::_gamestate = game::_motion;
    }
}

void swapvelocities(gameObject *A, gameObject *B)
{
    sf::Vector2f posA = A->circle.getPosition();
    sf::Vector2f posB = B->circle.getPosition();
    float impact = sqrt(pow(A->vel_x - B->vel_x, 2) +
			pow(A->vel_y - B->vel_y, 2)); 
    //direction of the vector joining the centres of A and B
    float o = atan2(posB.y - posA.y, posB.x - posA.x);
    //velocities in a rotated coordinate system with the x-axis in the direction of 'o'
    float va, vaT, vb, vbT;

    va = A->vel_y*sin(o) + A->vel_x*cos(o);
    vaT = A->vel_y*cos(o) - A->vel_x*sin(o);


    vb = B->vel_y*sin(o) + B->vel_x*cos(o);
    vbT = B->vel_y*cos(o) - B->vel_x*sin(o);

    float vcm = (A->massfactor*va + B->massfactor*vb)/(A->massfactor +
						       B->massfactor);
    float vcma = va - vcm;
    float vcmb = vb - vcm;

    va = vcm - vcma*A->restitution;
    vb = vcm - vcmb*B->restitution;


    A->vel_x = va*cos(o) - vaT*sin(o);
    A->vel_y = vaT*cos(o) + va*sin(o);

    B->vel_x = vb*cos(o) - vbT*sin(o);
    B->vel_y = vbT*cos(o) + vb*sin(o);

    if (!(A->frictionfactor < 0.001f || B->frictionfactor < 0.001f)) {
	game::playsound(impact);
    }
}

int board::top;
int board::bottom;
int board::left;
int board::right;
int board::playzonein;
int board::playzoneout;
int board::pocket_offset;
sf::Vector2f striker::position;
float board::frictioncoefficient = 160.f;
float board::edgerestitution = 0.9f;
float gameObject::vel_init(0.00001f);
