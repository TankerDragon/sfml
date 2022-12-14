/// #include "Game.hpp"
/// 
/// int main(int, char const**) {
///     srand(time(NULL));
///     Game game;
///     game.run();
///     return EXIT_SUCCESS;
/// }

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include "Constants.hpp"

/// // this function calculates distatce between a line and a point
/// float pointToLine2 (sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2) {
///     // take as first point of line as center of space (line1)
///     float x = point.x - line1.x, y = point.y - line1.y, tanLine = (line2.y - line1.y) / (line2.x - line1.x);
///     float tanXY = y / x;
///     float distance = (tanXY - tanLine) / (1 + tanXY * tanLine) * sqrt(x * x + y * y);
///     return distance;
/// }

sf::Vector2f getGlobalCoors(sf::Vector2f localCoors, sf::Vector2f position, float rotation) {
    float sinA = sin(rotation * DEG2RAD), cosA = cos(rotation * DEG2RAD);
    sf::Vector2f l(localCoors.x * cosA - localCoors.y * sinA, localCoors.y * cosA + localCoors.x * sinA);
    return position + l;
}

float pointToLine (sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2) {
    float h = line2.y - line1.y, s = line2.x - line1.x;
    return (s * (point.y - line1.y) - h * (point.x - line1.x)) / sqrt(s * s + h * h); // s * s + h * h = l * l
}

bool isPointToLinePositive(sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2) {
    float h = line2.y - line1.y, s = line2.x - line1.x;
    return ((s * (point.y - line1.y) - h * (point.x - line1.x)) / sqrt(s * s + h * h)) >= 0;
}

/// // if we iclude alpha0:
/// float pointToLine(sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2, float alpha0) {
///     float h = line2.y - line1.y, s = line2.x - line1.x, l = sqrt(s * s + h * h);
///     float sinA = h / l, cosA = s / l, sinA0 = sin(alpha0 * DEG2RAD), cosA0 = cos(alpha0 * DEG2RAD);
///     return (cosA * cosA0 - sinA * sinA0) * (point.y - line1.y) - (sinA * cosA0 + sinA0 * cosA) * (point.x - line1.x);
/// }


class CollidableConvex: public sf::ConvexShape {
private:
public:
    std::vector<sf::Vector2f> points;
    void setColliderPoint(int count, const sf::Vector2f &point) {
        this->setPoint(count, point);
        points.push_back(point);
    }

    void printPoints() {
        std::vector<sf::Vector2f>::iterator start_points = points.begin();
        while (start_points != points.end()) {
            std::cout << "x= " << start_points->x << ", y= " << start_points->y << std::endl;
            start_points++;
        }
    }
};



bool isCollide(CollidableConvex& convex1, CollidableConvex& convex2) {
    bool last = false;
    auto endline = convex1.points.end();
    for (auto it1 = convex1.points.begin(); it1 != convex1.points.end(); it1++) {
        for (auto it2 = convex2.points.begin(); it2 != convex2.points.end(); it2++) {
            last = false;
            endline = (it1 == convex1.points.end() - 1) ? convex1.points.begin() : endline = it1 + 1;
            std::cout << convex1.getRotation() << std::endl;
            if (isPointToLinePositive(
                getGlobalCoors(*it2, convex2.getPosition(), convex2.getRotation()),
                getGlobalCoors(*it1, convex1.getPosition(), convex1.getRotation()),
                getGlobalCoors(*(endline), convex1.getPosition(), convex1.getRotation())
                // convex2.getPosition() + *it2,
                // convex1.getPosition() + *it1,
                // convex1.getPosition() + *(endline)
            )) {
                last = true;    
                break;
            }
        }
        if (!last) return false;
    }
    return true;
}

int main() {

    //////////////////// 

    const float speed = 200.f;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(60);

    CollidableConvex convex;
    convex.setPointCount(5);
    convex.setColliderPoint(0, sf::Vector2f(0.f, 0.f));
    convex.setColliderPoint(1, sf::Vector2f(150.f, 10.f));
    convex.setColliderPoint(2, sf::Vector2f(120.f, 90.f));
    convex.setColliderPoint(3, sf::Vector2f(30.f, 100.f));
    convex.setColliderPoint(4, sf::Vector2f(0.f, 50.f));
    convex.setPosition(sf::Vector2f(50.f, 50.f));
    //convex.setRotation(45.f);
    convex.setFillColor(sf::Color::Transparent);
    convex.setOutlineThickness(1.f);

    ////////////////////////

    CollidableConvex convex2;
    convex2.setPointCount(5);
    convex2.setColliderPoint(0, sf::Vector2f(0.f, 0.f));
    convex2.setColliderPoint(1, sf::Vector2f(150.f, 10.f));
    convex2.setColliderPoint(2, sf::Vector2f(120.f, 90.f));
    convex2.setColliderPoint(3, sf::Vector2f(30.f, 100.f));
    convex2.setColliderPoint(4, sf::Vector2f(0.f, 50.f));
    convex2.setPosition(sf::Vector2f(350.f, 250.f));
    //convex2.setRotation(15.f);
    convex2.setFillColor(sf::Color::Transparent);
    convex2.setOutlineThickness(1.f);

    sf::Clock clock;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // update
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            convex.setPosition(convex.getPosition().x, convex.getPosition().y - speed * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            convex.setPosition(convex.getPosition().x, convex.getPosition().y + speed * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            convex.setPosition(convex.getPosition().x - speed * dt, convex.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            convex.setPosition(convex.getPosition().x + speed * dt, convex.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            convex.setRotation(convex.getRotation() - speed * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            convex.setRotation(convex.getRotation() + speed * dt);
        }
        if (!isCollide(convex, convex2) || !isCollide(convex2, convex)) {
            convex.setFillColor(sf::Color::Transparent);
        }
        else {
            convex.setFillColor(sf::Color::Red);
        }

        // draw
        window.clear();
        window.draw(convex);
        window.draw(convex2);
        window.display();
    }
    return EXIT_SUCCESS;
}