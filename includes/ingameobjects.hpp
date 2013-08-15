#ifndef IN_GAME_OBJECTS_H
#define IN_GAME_OBJECTS_H

#include <SFML/Graphics.hpp>

class board
{
private:
    sf::Sprite brd;
public:
    static int top;
    static int bottom;
    static int left;
    static int right;
    static int playzoneout;
    static int playzonein;
    static int pocket_offset;
    static float frictioncoefficient;
    static float edgerestitution;
    void setTexture();
    void draw();
};

class gameObject
{
protected:
    sf::CircleShape circle;
    sf::Sprite coinsprite;
    float massfactor;
    float vel_x;
    float vel_y;
    float restitution;
    float frictionfactor;
public:
    virtual ~gameObject() {};
    virtual void update() = 0;
    virtual void reset() {};
    void boundtoboard();
    void move();
    friend void collision(gameObject *A, gameObject *B);
    friend void swapvelocities(gameObject *A, gameObject *B);
    static float vel_init;
    bool isInPocket();
    friend class objectlist;
    friend class striker;
    friend class coin;
};

class pointer : public gameObject
{
private:
    sf::Vector2f relMousePos;
public:
    pointer();
    void update();
    void input();
    void boundaroundstriker();
    void hitStriker(float dist);
};

class striker : public gameObject
{
private:
    sf::Vector2f relMousePos;
public:
    static sf::Vector2f position;
    striker();
    void update();
    void reset();
private:
    void input();
    void boundtoplay();
};

class coin : public gameObject
{
private:
    char type; //w-white, b-black, other is red
public:
    coin(char t = 'q');
    void update();
    void reset();
};

#endif
