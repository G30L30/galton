#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
class Button {
public:
	Button() {}
	Button(std::string t, sf::Vector2f size, int characterSize, sf::Color bgColor, sf::Color textColor) {

		text.setString(t);
		text.setFillColor(textColor);
		text.setCharacterSize(characterSize);

		button.setSize(size);
		button.setFillColor(bgColor);

	}

	void setFont(sf::Font& font) {
		text.setFont(font);

	}
	void setBGColor(sf::Color color) {
		button.setFillColor(color);

	}
	void setTextColor(sf::Color textColor) {
		text.setFillColor(textColor);

	}
	void setPosition(sf::Vector2f position) {
		button.setPosition(position);

		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(button.getPosition().x + 0.3f * button.getSize().x, button.getPosition().y + 0.25f * button.getSize().y); // CHANGES BASED ON TEXT LENGTH
	}

	void drawTo(sf::RenderWindow& window) {
		window.draw(button);
		window.draw(text);

	}

	bool isMouseOver(sf::RenderWindow& window) {
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;

		float btnXPosWidth = button.getPosition().x + button.getLocalBounds().width;
		float btnYPosHeight = button.getPosition().y + button.getLocalBounds().height;

		if (mouseX < btnXPosWidth && mouseX > btnPosX && mouseY < btnYPosHeight && mouseY>btnPosY) {
			return true;

		}
		return false;



	}


private:
	sf::RectangleShape button;
	sf::Text text;

};
 
