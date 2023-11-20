#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

std::vector<sf::CircleShape> staticCircles;

sf::CircleShape makeCircle(float rad, int posx, int posy, bool isStatic) {
    sf::CircleShape circle;
    sf::Vector2f pos = sf::Vector2f(posx, posy);
    circle.setPosition(pos);
    circle.setRadius(rad);
    circle.setFillColor(sf::Color (59, 58, 57));
    circle.setPointCount(600);
    return circle;
}

void initializeStatics() {
    int layerWidth = 55; 
    int layers = 20;
    float circleRadius = 5.0f;
    float tool = 1.f;
    float xOffset = circleRadius * 2.0f * tool;
    
    for (int col = 0; col < layerWidth; ++col) {
        for (int row = 0; row < layers; ++row) {
            float xPosition = 50 + col * (circleRadius * 4) + (row % 2 == 1 ? xOffset : 0);
            float yPosition = 150 + 1.1 * row * (circleRadius * 4);
            staticCircles.push_back(makeCircle(circleRadius, xPosition, yPosition, true));
        }
    }
}