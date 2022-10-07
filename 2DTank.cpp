#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <iostream>
using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;


class Entity {
    public:
    Sprite sTurrent, sBody;
    // Vector2f position, velocity;
    float x = 100, y = 100, r, tr, omega, tOmega, speed, a;
    Entity() {}

};

class Tank: public Entity {
    public:
    Tank() {}
    Tank(Texture &t) {
        
        sTurrent.setTexture(t); 
        sBody.setTexture(t); 
        // s.setTextureRect(IntRect(40,0,40,40));
        sTurrent.setTextureRect(IntRect(32, 335, 404, 106));
        sBody.setTextureRect(IntRect(172, 88, 342, 197));
        sTurrent.setOrigin(307, 53);
        sBody.setOrigin(171, 98);
        a = 100;
        omega = 40;
        tOmega = 30;
        speed = 0.0;
    }
    void update(float dt) {
        // input
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) r += omega * dt;
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) r -= omega * dt;
        if (Keyboard::isKeyPressed(Keyboard::E)) tr += tOmega * dt;
        if (Keyboard::isKeyPressed(Keyboard::Q)) tr -= tOmega * dt;
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
            // vx += cos(r * DEGTORAD) * a * dt;
            // vy += sin(r * DEGTORAD) * a * dt;
            speed += a * dt;

        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            // vx -= cos(r * DEGTORAD) * a * dt;
            // vy -= sin(r * DEGTORAD) * a * dt;
            speed -= a * dt;
        }
        // limit in map
        // if (x < 0) x = W;
        // if (x > W) x = 0;
        // if (y < 0) y = H;
        // if (y > H) y = 0;

        // update
        x += cos(r * DEGTORAD) * speed * dt;
        y += sin(r * DEGTORAD) * speed * dt;
        
        sBody.setRotation(r + 180);
        sTurrent.setRotation(r + tr + 180);
        // std::cout << x << " " << y << std::endl;
        sBody.setPosition(x, y);
        sTurrent.setPosition(x, y);
    }

};


int main()
{
    // srand(time(0));
    Clock clock;
    float dt;
    sf::View view;
    float vw = W, vh = H;
    view.setCenter(sf::Vector2f(100.f, 100.f));
    view.setSize(sf::Vector2f(vw, vh));

    RenderWindow window(VideoMode(W, H), "Asteroids!"); //  sf::Style::Fullscreen
    window.setFramerateLimit(60);

    Texture t;
    t.loadFromFile("images/tigr.png");
    t.setSmooth(true);

    Tank player(t);

    CircleShape circle;
    circle.setRadius(50);
    circle.setPosition(300, 300);

    /////main loop/////
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();


            if (event.type == sf::Event::MouseWheelMoved) {
                std::cout << vw << vh << std::endl;
                // view.setSize(vw -= event.mouseWheel.delta * 100.0, vh -= event.mouseWheel.delta * 100.0 * (H/W));
                view.setSize(vw *= 1.0 - event.mouseWheel.delta * 0.1, vh *= 1.0 - event.mouseWheel.delta * 0.1);

            }
        }

        

        //////update//////
        dt = clock.restart().asSeconds();
        // std::cout << dt << std::endl;
        player.update(dt);
        view.setCenter(player.x, player.y);

        //////draw//////
        window.clear();
        window.setView(view);
        window.draw(circle);
        window.draw(player.sBody);
        window.draw(player.sTurrent);
        window.display();
    }

    return 0;
}
