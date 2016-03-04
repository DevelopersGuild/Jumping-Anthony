#ifndef MOVINGBACKGROUND_H
#define MOVINGBACKGROUND_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.h"
class MovingBackground
{
private:
	sf::Texture beginningBackgroundTexture;
	sf::Texture movingBackgroundTexture;
	 
	sf::Sprite beginningBackground;
	sf::Sprite movingBackground1;
	sf::Sprite movingBackground2;
public:
	int skipNumber = 0; //this number will allow the slowing of the scrolling speed
	//getter
	sf::Sprite getBackground(int i)
	{
		if (i == 1)
			return movingBackground1;
		else if (i == 2)
			return movingBackground2;
		else
			return beginningBackground;
	}
	//automatically constructs everything needed for the movingBackground object
	MovingBackground()
	{
		if (!movingBackgroundTexture.loadFromFile(resourcePath() + "assets\\loopingBackground.png")
			|| !movingBackgroundTexture.loadFromFile(resourcePath() + "assets\\loopingBackground.png")
			|| !beginningBackgroundTexture.loadFromFile(resourcePath() + "assets\\background.png"))
		{
			std::cout << "Error: File not found" << std::endl;
		}
		//sets the background pic
		beginningBackground.setTexture(beginningBackgroundTexture);
		movingBackground1.setTexture(movingBackgroundTexture);
		movingBackground2.setTexture(movingBackgroundTexture);
		//sets the starting positions
		movingBackground1.setPosition(0, -800);
		movingBackground2.setPosition(0, -1600);
		
	}
	void moving();
	void drawBackground1(sf::RenderWindow &window)
	{	
		window.draw(movingBackground1);
	}
	void drawBackground2(sf::RenderWindow &window)
	{
		window.draw(movingBackground2);
	}
};

void MovingBackground::moving()
{
	//slows down the scrolling!
	if (skipNumber % 2 == 0)
	{
		beginningBackground.move(0, 1);
		movingBackground1.move(0, 1);
		movingBackground2.move(0, 1);
	}
	skipNumber++;
	//checking if the background is in the bounds
	if (movingBackground1.getPosition().y >= 800)
		movingBackground1.setPosition(0, -800);
	//same as above for the second
	if (movingBackground2.getPosition().y >= 800)
		movingBackground2.setPosition(0, -800);	
	//DRAW
	
}
#endif