#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "dynam.h"

class bin {
public:
    bin() {}

    bin(sf::Color outlineColor, sf::Color bgColor, float binWidth, float binHeight, float xPosition, float yPosition, float OutlineThickness=2.0f) {

        binShape.setFillColor(bgColor);
        binShape.setOutlineColor(outlineColor);

        binShape.setPosition(xPosition, yPosition);
        binShape.setOutlineThickness(OutlineThickness);
        binShape.setSize(sf::Vector2f(binWidth, binHeight));
    }
    
    void drawTo(sf::RenderWindow& window) {
        window.draw(binShape);

    }
    sf::FloatRect getGlobalBounds() const {
        return binShape.getGlobalBounds();
    }

    bool collidesWith(const dynamic& circle) const {
        return binShape.getGlobalBounds().intersects(dynamics[0].getCircle().getGlobalBounds());
    }
private:
    sf::RectangleShape binShape;
};


std::vector<bin> initializeCatchmentBins() {
    int nBins = 28;

    std::vector<bin> catchmentBins(nBins);

    float binWidth = 40.0f;
    float binHeight = 60.0f;

    for (int i = 0; i < nBins; ++i) {

        float xPosition = 45 + i * binWidth;
        float yPosition = heightWindow-binHeight - 2.0f;
        
        bin nBin(sf::Color::White, sf::Color::Black, binWidth, binHeight, xPosition, yPosition, 2.0f);

        catchmentBins[i] = nBin;


    }

    return catchmentBins;
}
