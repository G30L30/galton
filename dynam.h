#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "statics.h"
#include "bins.h"
#include <string>
#include <fstream>

int widthWindow = 1200;
int heightWindow = 800;

float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

sf::Vector2f normalize(const sf::Vector2f& vector) {
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length != 0) {
        return vector / length;
    }
    return vector;
}

bool touching(const sf::CircleShape& c1, const sf::CircleShape& c2) {
    bool collision = false;
    float distance = std::sqrt((c2.getPosition().x - c1.getPosition().x) * (c2.getPosition().x - c1.getPosition().x) +
        (c2.getPosition().y - c1.getPosition().y) * (c2.getPosition().y - c1.getPosition().y));

    if (distance <= c1.getRadius() + c2.getRadius())
        collision = true;

    return collision;
}
class dynamic {
    public:
        dynamic() {}
        dynamic(sf::Color outline, sf::Color fill, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f force, float m, float rad, bool randStart) {
            position = pos;
            velocity = vel;
            forceContainer = force;
            mass = m;
            randomStart = randStart;
            radius = rad;
            circleShape.setOutlineColor(outline);
            circleShape.setPosition(position);
            circleShape.setRadius(radius);
            circleShape.setPointCount(600);
            circleShape.setFillColor(fill);


        }
        void setPos(sf::Vector2f pos) {
            circleShape.setPosition(pos);
            position = pos;
        }
        sf::Vector2f getPos() {
            return position;
        }
        bool getRandomStart() {
            return randomStart;
        }
        void setRandomStart(bool b) {
            randomStart = b;

        }
        void setForceContainer(sf::Vector2f force) {
            forceContainer = force;
        }
        
        void drawTo(sf::RenderWindow& window) {
            window.draw(circleShape);

        }
        float getMass() {
            return mass;
        }
        sf::Vector2f getVelocity() {
            return velocity;
        }
        void setVelocity(sf::Vector2f vel) {
            velocity = vel;
        }
        sf::CircleShape getCircle() {
            return circleShape;
        }
        float getRadius() {
            return radius;
        }

    private:
        sf::CircleShape circleShape;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f forceContainer;
        float radius;
        bool randomStart;
        float mass;

    };

std::vector<sf::Vector2f> rands(float rad, int N, float minX, float maxX, float minY, float maxY) {
	std::vector<int> randomsX;
	std::vector<int> randomsY;
	std::vector<sf::Vector2f> randoms;
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distributionX(minX + 2 * rad, maxX - 2 * rad);
	std::uniform_int_distribution<> distributionY(minY + 2 * rad, maxY - 2 * rad);

	for (int i = 0; i < N; ++i) {

		bool tooClose;

		do {
			int randomNumberX = distributionX(gen);
			int randomNumberY = distributionY(gen);
			tooClose = false;

			for (int f = 0; f < randomsX.size(); ++f) {
				if (sqrt(pow(randomNumberX - randomsX[f], 2) + pow(randomNumberY - randomsY[f], 2)) < 2.0f * rad) {
					tooClose = true;
					break;
				}
			}
			if (!tooClose) {
				randomsX.push_back(randomNumberX);
				randomsY.push_back(randomNumberY);
				randoms.push_back(sf::Vector2f(randomNumberX, randomNumberY));
			}
		} while (tooClose);

	}
	return randoms;
}


std::vector<dynamic> initializeDynamics(int nDyns, int radiusDynamic, float mass) {

    std::vector<dynamic> dynamics(nDyns);


    for (int i = 0; i < nDyns; ++i) {

        dynamic dyn(sf::Color::Blue, sf::Color (161, 51, 66), { 0, 0 }, { 0, 0 }, { 0, 0 }, mass, radiusDynamic, false); // change whether its random or if its coming from a 'gap' (specified by a range)
        dynamics[i] = dyn;

        if (dynamics[0].getRandomStart() == true) {
            std::vector<sf::Vector2f> randList = rands(radiusDynamic, nDyns, 50, widthWindow-50, 0, 50);
            for (int i = 0; i < nDyns; ++i) {
                dynamics[i].setPos(randList[i]);
                }
        }
        else {
            for (int i = 0; i < nDyns; ++i) {
                float startX = static_cast<float>(widthWindow)/(2+0.001*i); // opening range

                dynamics[i].setPos({startX,0});
            }
        }
    }
    return dynamics;
}

std::vector<dynamic> dynamics = initializeDynamics(100, 5, 3);

std::vector<dynamic> updateDynamics() {
    float dt = 0.1; // example time step

    for (int i = 0; i < dynamics.size(); ++i) {
        sf::Vector2f forceContainer = { 0, 0 };
        float mass = dynamics[i].getMass();
        sf::Vector2f gravity = { 0, 9.8 };
        forceContainer += gravity * mass;

        dynamics[i].setForceContainer(forceContainer);
        
        sf::Vector2f velocity = dynamics[i].getVelocity();
        sf::Vector2f position = dynamics[i].getPos();

        velocity += (forceContainer * dt) / mass; // Euler integration
        position.x += velocity.x * dt; // Euler integration
        position.y += velocity.y * dt; // Euler integration

        dynamics[i].setVelocity(velocity);
        dynamics[i].setPos(position);

        bool interparticleCollisions = false;

        if (interparticleCollisions == true) {
            for (size_t j = i + 1; j < dynamics.size(); ++j) { // reduce pairwise collisions by using spatial partitioning - only check for collisions per cell breach
                if (touching(dynamics[i].getCircle(), dynamics[j].getCircle())) {
                    sf::Vector2f delta = dynamics[j].getPos() - dynamics[i].getPos();
                    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                    sf::Vector2f collisionNormal = normalize(delta);

                    // now rel vel
                    float xrel = dynamics[j].getVelocity().x - dynamics[i].getVelocity().x;
                    float yrel = dynamics[j].getVelocity().y - dynamics[i].getVelocity().y;
                    sf::Vector2f relvel = sf::Vector2f(xrel, yrel);

                    // dot product of relvel onto normal
                    float impulse = (2.0f * relvel.x * collisionNormal.x + collisionNormal.y * relvel.y) / (1.0f + 1.0f);

                    dynamics[i].setVelocity(dynamics[i].getVelocity() + (impulse)*collisionNormal);
                    dynamics[j].setVelocity(dynamics[j].getVelocity() - (impulse)*collisionNormal);

                    float radius = dynamics[i].getRadius();

                    float overlap = 2 * radius - distance;


                    sf::Vector2f correction = (overlap / 2.0f) * collisionNormal;

                    float totalMass = mass + mass;
                    float massRatioA = mass / totalMass;
                    float massRatioB = mass / totalMass;

                    const float epsilon = 0.05f;
                    dynamics[i].setPos(dynamics[i].getPos() - (massRatioA * correction) + epsilon * collisionNormal);
                    dynamics[j].setPos(dynamics[j].getPos() + (massRatioA * correction) + epsilon * collisionNormal);
                }
            }
        }

        for (size_t j = 0; j < staticCircles.size(); ++j) {
            if (touching(dynamics[i].getCircle(), staticCircles[j])) {

                sf::Vector2f delta = staticCircles[j].getPosition() - dynamics[i].getPos();
                float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                sf::Vector2f collisionNormal = normalize(delta);

                float xrel = -dynamics[i].getVelocity().x; 
                float yrel = -dynamics[i].getVelocity().y;
                sf::Vector2f relvel = sf::Vector2f(xrel, yrel);

                float impulse = (2.0f * relvel.x * collisionNormal.x + collisionNormal.y * relvel.y) / (1.0f + 1.0f);

                dynamics[i].setVelocity(dynamics[i].getVelocity() + (impulse)*collisionNormal);

                float radius = dynamics[i].getRadius();

                float overlap = 2 * radius - distance;
                sf::Vector2f correction = (overlap / 2.0f) * collisionNormal ;

                dynamics[i].setPos(dynamics[i].getPos() - correction);
            }
        }
        float radius = dynamics[i].getRadius();

        if (dynamics[i].getPos().y > heightWindow - 2 * radius) {
            dynamics[i].setPos({ dynamics[i].getPos().x, heightWindow - 2 * radius });
            dynamics[i].setVelocity({ 0, 0 });

            std::ofstream fw("galtonResults.txt", std::ofstream::trunc);

            if (fw.is_open()) {

                for (int j = 0; j < dynamics.size(); j++) {
                    std::string xString = std::to_string(dynamics[j].getPos().x);
                    std::string yString = std::to_string(dynamics[j].getPos().y);
                    fw << xString << "," << yString << "\n";
                }
                fw.close();  
            }
        }
    }

    return dynamics;

}