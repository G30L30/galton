#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "statics.h"
#include "bins.h"
#include "dynam.h"
#include "button.h"

sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "Galton Board Sim");
sf::Event event;


void update() {

    //Button btn1("Clear beads", { 300,50 }, 20, sf::Color{ 69, 69, 69, 255 }, sf::Color{ 0, 0, 0, 155 });
    //btn1.setPosition({ 500,500 });
    //sf::Font font;
    //btn1.setFont(font);

    //if (!font.loadFromFile("arial.ttf"))
    //{
    //    std::cout << "err loading file";
    //}

    //btn1.drawTo(window);

    for (int i = 0; i < staticCircles.size(); ++i) {
        window.draw(staticCircles[i]);
    }
    std::vector<bin> catchmentBins = initializeCatchmentBins();

    for (int i = 0; i < catchmentBins.size(); ++i) {
        catchmentBins[i].drawTo(window);
    }

    std::vector<dynamic> dynamics = updateDynamics();

    for (int i = 0; i < dynamics.size(); ++i) {
        dynamics[i].drawTo(window);
    }

}

int main() {
    std::vector<dynamic> dynamics = initializeDynamics(10, 5, 1);
    for (int i = 0; i < dynamics.size(); ++i) {
        dynamics[i].drawTo(window);
    }
    initializeStatics();
    initializeCatchmentBins();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::White);

        update();
        window.display();
    }

    return 0;
}
