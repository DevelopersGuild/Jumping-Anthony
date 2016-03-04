#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "main.h"
#include <iostream>

void EndScreen(sf::RenderWindow &window)
{
	sf::Font font;
	sf::Text text1;
	sf::Text text2;
	sf::Text text3;
	sf::Texture backgroundTexture;
	sf::Sprite background;
	if (!backgroundTexture.loadFromFile(resourcePath() + "assets/background.png"))
	{
		std::cout << "Error: File not found" << std::endl;
	}

	if (!font.loadFromFile(resourcePath() + "assets/snap.ttf"))
	{
		std::cout << "Error: File not found" << std::endl;
	}

	//Sets the sprite as the background pic
	background.setTexture(backgroundTexture);

	//This is where the title appears
	text1.setFont(font);
	text1.setString("Jumping\nAnthony");
	text1.setCharacterSize(75);
	text1.setColor(sf::Color::Magenta);
	text1.setStyle(sf::Text::Bold); // | sf::Text::Underlined
	text1.setPosition(30, 200);

	//Quick Instruction to change to next state
	text2.setFont(font);
	text2.setString("GAME OVER");
	text2.setCharacterSize(24);
	text2.setColor(sf::Color::Black);
	text2.setPosition(100, 450);	//WILL DEFINITELY HAVE TO ADJUST POSITIONING

	text3.setFont(font);
	text3.setString("Click to Close");
	text3.setCharacterSize(24);
	text3.setColor(sf::Color::Black);
	text3.setPosition(100, 700);
	window.clear();
	//Draws background first and then the texts
	window.draw(background);
	window.draw(text1);
	window.draw(text2);
	window.draw(text3);
	window.display();

}
#endif