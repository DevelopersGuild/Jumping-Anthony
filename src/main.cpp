#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "OpeningScreen.h"
#include "Overlap.h"
#include "main.h"
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow&, sf::Sprite&, vector<sf::Sprite>, vector<sf::Sprite>);

int main()
{
	// initalizing window
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "Jumping Anthony");
	window.setFramerateLimit(20);
	
	// initalizing a texture object
	sf::Texture mainCharacterTexture;
	// applying an image to the texture object (it can be found in your assets folder)
	mainCharacterTexture.loadFromFile(resourcePath() + "assets\\pikachu.png");

	// applying texture object to sprite
	sf::Sprite mainCharacterSprite;
	mainCharacterSprite.setScale(0.1, 0.1);
	mainCharacterSprite.setTexture(mainCharacterTexture);
	mainCharacterSprite.setPosition(100, 600);

	sf::Texture blockTexture;
	blockTexture.loadFromFile(resourcePath() + "assets\\block.png");
	vector<sf::Sprite> platform;

	for (int i = 0; i < 4; i++) //platform
	{
		platform.push_back(sf::Sprite(blockTexture));
		platform[i].setPosition(sf::Vector2f(i * 120, 700));
		//platform[i].setScale(sf::Vector2f(0.1, 0.1));
	}
	int numBlock = 0;
	vector<sf::Sprite> block;

	while (numBlock < 100)
	{
		block.push_back(sf::Sprite(blockTexture));
		block[numBlock].setOrigin(sf::Vector2f(60, 20));
		block[numBlock].setPosition(sf::Vector2f((rand() % 280 + 1), 600 - (150 * numBlock)));
		//block[numBlock].setScale(sf::Vector2f(0.1, 0.1));
		numBlock++;
	}

	gameState = OPENING;

	while (window.isOpen())
	{
		switch (gameState)
		{
		case OPENING:
			OpeningSceneMode(window);
			break;
		case PLAY:
			PlayMode(window, mainCharacterSprite, platform, block);
			break;
		}
		
	
	}//end of while(window.isopen)

	return 0;
}

//OPENING SCENE EVENT HANDLE
void OpeningSceneMode(sf::RenderWindow &window)
{
	OpeningScreen(window);
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			window.clear();
			gameState = PLAY;
		}
		else if (event.type == sf::Event::Closed)
			window.close();
	}
}

//PLAY MODE EVENT HANDLE
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite, vector<sf::Sprite> platform, vector<sf::Sprite> block)
{
	sf::Event event;
	const float GRAVITY = 1;
	sf::Vector2f velocity(sf::Vector2f(0, 0));
	float moveSpeed = 5.0f, jumpSpeed = 15.0f;
	srand(time(NULL));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		for (int k = 0; k < 4; k++)
		{
			platform[k].move(sf::Vector2f(0, 20));
		}
		for (int k = 0; k < 100; k++)
		{
			block[k].move(sf::Vector2f(0, 20));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //stops when the key is not pressed
			velocity.x = -moveSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			velocity.x = moveSpeed;
		else
			velocity.x = 0;

		//when shape touches the block the shape should jump
		for (int x = 0; x < 4; x++)
		{
				if (overlap(mainCharacterSprite, platform[x]))
				{
					velocity.y = -jumpSpeed;
				}
				mainCharacterSprite.move(velocity.x, velocity.y);
				if (mainCharacterSprite.getPosition().y < platform[x].getPosition().y) // groundposition
				{
					velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
				}
				else if (velocity.y > 0 && overlap(mainCharacterSprite, platform[x])) //if overlap with platform setposition
				{
					mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, platform[x].getPosition().y);
				}
		}
		for (int elem = 0; elem < 100; elem++)
		{
			if(mainCharacterSprite.getPosition().y < block[elem].getPosition().y) // groundposition
			{
				velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
			}
			else if (velocity.y > 0 && overlap(mainCharacterSprite, block[elem])) //if overlap with one of the blocks setposition
			{
				mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, block[elem].getPosition().y);
			}
			if (velocity.y > 0 && overlap(mainCharacterSprite, block[elem])) //when the velocity is greater than 0 and character overlap with one of the block, jump
			{
				velocity.y = -jumpSpeed;
			}
		}

		window.clear();
		window.draw(mainCharacterSprite);
		for (int k = 0; k < 4; k++)
		{
			window.draw(platform[k]);
		}
		for (int k = 0; k < 100; k++)
		{
			window.draw(block[k]);
		}

		window.display();
	}
	
}