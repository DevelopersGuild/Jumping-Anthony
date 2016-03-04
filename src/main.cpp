#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "ResourcePath.h"
#include "OpeningScreen.h"
#include "EndScreen.h"
#include "Overlap.h"
#include "main.h"
using namespace std;

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow&, sf::Sprite&, vector<sf::Sprite>, vector<sf::Sprite>&, sf::Text); // during game
void moveBlocks(vector<sf::Sprite> &platform, vector<sf::Sprite> &block);
void createBlock(sf::Texture*, vector<sf::Sprite>&, vector<sf::Sprite>&);
void changeBlockPosition(vector<sf::Sprite>&, int);
void drawBlocks(sf::RenderWindow &, vector<sf::Sprite> platform, vector<sf::Sprite> block);
void EndScreenMode(sf::RenderWindow &);
int main()
{
	// initalizing window
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "Jumping Anthony");

	window.setFramerateLimit(30);

	// initalizing a texture object
	sf::Texture mainCharacterTexture;
	mainCharacterTexture.loadFromFile(resourcePath() + "assets/pikachu.png");

	//initializing main character sprite
	sf::Sprite mainCharacterSprite;
	mainCharacterSprite.setScale(0.05, 0.05);
	mainCharacterSprite.setTexture(mainCharacterTexture);
	mainCharacterSprite.setPosition(100, 600);

	// score
	sf::Font font;
	sf::String points;
	
	if (!font.loadFromFile(resourcePath() + "assets/snap.ttf"))
	{
		std::cout << "Error: File not found" << std::endl;
	}
	sf::Text score(points, font, 30);
	score.setPosition(350, 600);
	
	//initializing block sprite
	sf::Texture blockTexture;
	blockTexture.loadFromFile(resourcePath() + "assets/block.png");

	vector<sf::Sprite> platform;
	vector<sf::Sprite> block;
	createBlock(&blockTexture, block, platform);
	
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
			PlayMode(window, mainCharacterSprite, platform, block, score);
			break;
		case GAME_OVER:
			EndScreenMode(window);
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
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite, vector<sf::Sprite> platform, vector<sf::Sprite> &block, sf::Text score)
{
	sf::Event event;
	int numBlock = 0;
	int points = 0;

	const float GRAVITY = 1;
	sf::Vector2f velocity(sf::Vector2f(0, 0));
	float moveSpeed = 50.0f, jumpSpeed = 45.0f;
	srand(time(NULL));

	while (window.isOpen() && gameState == PLAY)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		moveBlocks(platform, block);

		if (numBlock == SIZE)
		{
			numBlock = 0;
		}
		if (block[numBlock].getPosition().y > LengthWindow)//when the block disappears from the window
		{
			changeBlockPosition(block, numBlock);
			++numBlock;
		}

		//move left and right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //stops when the key is not pressed
			float playerLeftSide = mainCharacterSprite.getPosition().x;
			float distanceToWall = playerLeftSide;
			velocity.x = -std::min(moveSpeed, distanceToWall);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			float playerWidth = mainCharacterSprite.getGlobalBounds().width;
			float playerLeftSide = mainCharacterSprite.getPosition().x;
			float playerRightSide = playerLeftSide + playerWidth;
			float distanceToWall = WidthWindow - playerRightSide;
			velocity.x = std::min(moveSpeed, distanceToWall);
		}
		else {
			velocity.x = 0;
		}

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

		for (int elem = 0; elem < SIZE; elem++)
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
				points++;
			}
		}
		mainCharacterSprite.move(velocity.x, velocity.y);
		stringstream convertToString;
		convertToString << points;
		score.setString(convertToString.str());
		
		window.clear();
		window.draw(score);
		window.draw(mainCharacterSprite);
		drawBlocks(window, platform, block);
		if (mainCharacterSprite.getPosition().y > LengthWindow)
		{
			gameState = GAME_OVER;
		}
		window.display();
	}
}
void moveBlocks(vector<sf::Sprite> &platform, vector<sf::Sprite> &block)
{
	//move the block down at the same speed (we are not using view)
	for (int x = 0; x < 4; x++)
	{
		platform[x].move(sf::Vector2f(0, 8));
	}
	for (int k = 0; k < SIZE; k++)
	{
		block[k].move(sf::Vector2f(0, 8));
	}
}

void createBlock(sf::Texture *blockTexture, vector<sf::Sprite>&block, vector<sf::Sprite>&platform)
{
	for (int i = 0; i < 4; i++) //platform
	{
		platform.push_back(sf::Sprite(*blockTexture));
		platform[i].setPosition(sf::Vector2f(i * 120, 700));
		platform[i].setScale(sf::Vector2f(1, 0.5));
	}

	int numBlock = 0;
	while (numBlock < SIZE)
	{
		block.push_back(sf::Sprite(*blockTexture));
		//block[numBlock].setOrigin(sf::Vector2f(60, 20));
		block[numBlock].setPosition(sf::Vector2f((rand() % 280 + 1), 600 - (120 * numBlock)));
		block[numBlock].setScale(sf::Vector2f(1, 0.5));
		numBlock++;
	}

}

void changeBlockPosition(vector<sf::Sprite> &block, int numBlock)
{
	if (numBlock == 0)
	{
		block[numBlock].setPosition(sf::Vector2f((rand() % 280 + 1), block[block.size() - 1].getPosition().y - 120));
	}
	else
	{
		//place that block above the lastest block created.
		block[numBlock].setPosition(sf::Vector2f((rand() % 280 + 1), block[numBlock - 1].getPosition().y - 120)); 
	}
	block[numBlock].setScale(sf::Vector2f(1, 0.5));
}
void drawBlocks(sf::RenderWindow &window, vector<sf::Sprite> platform, vector<sf::Sprite> block)
{
	for (int x = 0; x < 4; x++)
	{
		window.draw(platform[x]);
	}
	for (int k = 0; k < SIZE; k++)
	{
		window.draw(block[k]);
	}
}

void EndScreenMode(sf::RenderWindow &window)
{
	window.clear();
	EndScreen(window);
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			window.clear();
			window.close();
		}
		else if (event.type == sf::Event::Closed)
			window.close();
	}
}