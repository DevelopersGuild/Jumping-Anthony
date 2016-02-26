#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "ResourcePath.h"
#include "OpeningScreen.h"
#include "Overlap.h"
#include "main.h"
using namespace std;

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow&, sf::Sprite&, vector<sf::Sprite>, vector<sf::Sprite>); // during game

int main()
{
	// initalizing window
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "Jumping Anthony");

	window.setFramerateLimit(30);

	// initalizing a texture object
	sf::Texture mainCharacterTexture;
	mainCharacterTexture.loadFromFile(resourcePath() + "assets\\pikachu.png");

	//initializing main character sprite
	sf::Sprite mainCharacterSprite;
	mainCharacterSprite.setScale(0.05, 0.05);
	mainCharacterSprite.setTexture(mainCharacterTexture);
	mainCharacterSprite.setPosition(100, 600);

	//initializing block sprite

	sf::Texture blockTexture;
	blockTexture.loadFromFile(resourcePath() + "assets\\block.png");
	vector<sf::Sprite> platform;

	for (int i = 0; i < 4; i++) //platform
	{
		platform.push_back(sf::Sprite(blockTexture));
		platform[i].setPosition(sf::Vector2f(i * 120, 700));
		platform[i].setScale(sf::Vector2f(1, 0.5));
	}

	int numBlock = 0;
	vector<sf::Sprite> block;
	while (numBlock < 100)
	{
		block.push_back(sf::Sprite(blockTexture));
		//block[numBlock].setOrigin(sf::Vector2f(60, 20));
		block[numBlock].setPosition(sf::Vector2f((rand() % 280 + 1), 600 - (120 * numBlock)));
		block[numBlock].setScale(sf::Vector2f(1, 0.5));
		numBlock++;
	}

	//initialing game state to opening screen
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

	float moveSpeed = 25.0f, jumpSpeed = 50.0f;
	srand(time(NULL));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//move the block down at the same speed (we are not using view)
		for (int k = 0; k < 4; k++)
		{
			platform[k].move(sf::Vector2f(0, 5));
		}
		for (int k = 0; k < 100; k++)
		{
			block[k].move(sf::Vector2f(0, 5));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //stops when the key is not pressed
			velocity.x = -moveSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			velocity.x = moveSpeed;
		else
			velocity.x = 0;

		sf::Vector2f currPos = mainCharacterSprite.getPosition();

		//when shape touches the block the shape should jump
		for (int x = 0; x < 4; x++)
		{
				if (overlap(mainCharacterSprite, platform[x]))
				{
					velocity.y = -jumpSpeed;
				}
				if (mainCharacterSprite.getPosition().y < platform[x].getPosition().y) // groundposition
				{
					velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
				}
				else if (velocity.y > 0 && overlap(mainCharacterSprite, platform[x])) //if overlap with platform setposition
				{
					//mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, platform[x].getPosition().y);
					mainCharacterSprite.setPosition(currPos);
				}
		}

		for (int elem = 0; elem < 100; elem++)
		{
			if(mainCharacterSprite.getPosition().y < block[elem].getPosition().y) // groundposition
			{
				velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
			}
			else if (velocity.y > 0 && overlap(mainCharacterSprite, block[elem])) //if overlap with one of the blocks setposition velocity.y>0 means the mainCharacterSprite is falling
			{
				//mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, block[elem].getPosition().y);
				mainCharacterSprite.setPosition(currPos);
			}
			if (velocity.y > 0 && overlap(mainCharacterSprite, block[elem])) //when the velocity is greater than 0 and character overlap with one of the block, jump
			{
				velocity.y = -jumpSpeed;
			}
		}
		mainCharacterSprite.move(velocity.x, velocity.y);
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
//do not need. this is already taken care in the play mode as in velocity.x
//void MCMovement(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite)
//{
//	sf::Event event;
//	while (window.pollEvent(event))
//	{
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//		{
//			mainCharacterSprite.move(-5, 0);
//		}
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//		{
//			mainCharacterSprite.move(5, 0);
//		}
//		if (event.type == sf::Event::Closed)
//			window.close();
//	}			//end of while (window.pollEvent)
//
//		window.clear();
//		window.draw(mainCharacterSprite);
//		window.display();
//	
//}