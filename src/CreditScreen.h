#ifndef CREDITSCREEN_H
#define CREDITSCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.h"
#include "main.h"
/****************************************
this function will draw the credit screen with developers
and return to the game if user press R
*****************************************/
void drawCredit(sf::RenderWindow &window, typeName &gameState) {
	sf::Font font;
	if (!font.loadFromFile(resourcePath() + "assets/snap.ttf"))
		std::cout << "Error: File not found" << std::endl;

	sf::Text title;
	sf::Text names;
	sf::Text prompt;

	sf::Texture backgroundTexture;
	sf::Sprite background;
	if (!backgroundTexture.loadFromFile(resourcePath() + "assets/background.png"))
		std::cout << "Error: File not found" << std::endl;

	//Sets the sprite as the background pic
	background.setTexture(backgroundTexture);

	//This is where the title appears
	title.setFont(font);
	title.setString("Developers: ");
	title.setCharacterSize(60);
	title.setColor(sf::Color::Magenta);
	title.setStyle(sf::Text::Bold);
	title.setPosition(30, 200);

	//names of developers
	names.setFont(font);
	names.setString("Anh Pham\nYe-Eun Myung\nPhillip Nguyen\nand more");
	names.setCharacterSize(24);
	names.setColor(sf::Color::Black);
	names.setPosition(100, 300);

	//ask if user want to replay
	prompt.setFont(font);
	prompt.setString("Press R to replay\n\nClick to Close");
	prompt.setCharacterSize(24);
	prompt.setColor(sf::Color::Black);
	prompt.setPosition(100, 450);

	//Draws background first and then the texts
	window.clear();
	window.draw(background);
	window.draw(title);
	window.draw(names);
	window.draw(prompt);
	window.display();

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			gameState = PLAY;

		if (event.type == sf::Event::MouseButtonPressed)
		{
			window.clear();
			window.close();
		}
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

#endif