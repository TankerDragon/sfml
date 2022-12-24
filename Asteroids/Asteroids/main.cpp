//#include "Game.hpp"

//int main(int, char const**) {
    //srand(time(NULL));
    //Game game;
    //game.run();
    //return EXIT_SUCCESS;
//}

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>

/// // this function calculates distatce between a line and a point
/// float pointToLine2 (sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2) {
///     // take as first point of line as center of space (line1)
///     float x = point.x - line1.x, y = point.y - line1.y, tanLine = (line2.y - line1.y) / (line2.x - line1.x);
///     float tanXY = y / x;
///     float distance = (tanXY - tanLine) / (1 + tanXY * tanLine) * sqrt(x * x + y * y);
///     return distance;
/// }

float pointToLine (sf::Vector2f point, sf::Vector2f line1, sf::Vector2f line2) {
    float h = line2.y - line1.y, l = line2.x - line1.x;
    return (l * (point.y - line1.y) - h * (point.x - line1.x)) / sqrt(l * l + h * h);
}

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

bool isCollide(CollidableConvex convex1, CollidableConvex convex2) {

    for (auto it1 = convex1.points.begin() + 1; it1 != convex1.points.end(); it1++) {
        std::cout << std::endl;
        for (auto it2 = convex2.points.begin(); it2 != convex2.points.end(); it2++) {
            std::cout << pointToLine(convex2.getPosition() + *it2, convex1.getPosition() + *(it1 - 1), convex1.getPosition() + *it1) << std::endl;
        }
    }
    return false;
}

int main() {


    //////////////////// 

    const float speed = 5.f;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(30);

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
    convex.setOutlineThickness(3.f);

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
    convex2.setOutlineThickness(3.f);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            convex.setPosition(convex.getPosition().x, convex.getPosition().y - speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            convex.setPosition(convex.getPosition().x, convex.getPosition().y + speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            convex.setPosition(convex.getPosition().x - speed, convex.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            convex.setPosition(convex.getPosition().x + speed, convex.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            convex.setRotation(convex.getRotation() - speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            convex.setRotation(convex.getRotation() + speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            system("cls");

            isCollide(convex, convex2);
        }

        // draw
        window.clear();
        window.draw(convex);
        window.draw(convex2);
        window.display();
    }
    return EXIT_SUCCESS;
}