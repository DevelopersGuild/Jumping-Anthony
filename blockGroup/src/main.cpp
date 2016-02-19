#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "main.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "SFML Works!");
	sf::RectangleShape rectangle;
	int run = 0;
	vector<sf::RectangleShape> jumpOn;
	srand(time(NULL));

    window.setFramerateLimit (60);
	
		rectangle.setFillColor(sf::Color::Green);
		rectangle.setSize(sf::Vector2f(400, 50));
		rectangle.setPosition(sf::Vector2f(0, 700));
		
		//for (int i = 1; i < 4; i++)
		//{
		//	jumpOn.push_back(sf::RectangleShape(sf::Vector2f(100, 50)));
		//	jumpOn[i - 1].setFillColor(sf::Color::Cyan);
		//	jumpOn[i - 1].setOrigin(sf::Vector2f(50, 25));
		//	jumpOn[i - 1].setPosition((rand() % 280 + 1), rectangle.getPosition().y - (200 * i));
		//} 
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
            
            //shape.move(0.5, 0.5);
            
            

		}
		if (run == 0)
		{
			for (int i = 1; i < 4; i++)
			{
				jumpOn.push_back(sf::RectangleShape(sf::Vector2f(100, 50)));
				jumpOn[i - 1].setFillColor(sf::Color::Cyan);
				jumpOn[i - 1].setOrigin(sf::Vector2f(50, 25));
				jumpOn[i - 1].setPosition((rand() % 280 + 1), rectangle.getPosition().y - (200 * i));
			}
			run++;
		}
		
        //rectangle.move(0.5, 0.5);

		window.clear();
		
		window.draw(rectangle);
		for (int k = 0; k < 3; k++)
		{
			window.draw(jumpOn[k]);
		}
		
		window.display();
	}

	return 0;
}
