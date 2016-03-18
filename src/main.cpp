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
#include "MovingBackground.h"
using namespace std;

struct Block
{
	sf::Sprite sprite;
	bool pointsReceived;
};

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow&, sf::Sprite&, vector<Block>& platform, vector<Block>& blocks, sf::Text, MovingBackground&); // during game
void moveBlocks(vector<Block> &platform, vector<Block> &block);
void createBlock(sf::Texture*, vector<Block>&, vector<Block>&);
void changeBlockPosition(vector<Block>&, int);
void gravity(sf::Sprite &mainCharacterSprite, vector<Block> platform, vector<Block> &blocks, int &points, sf::Vector2f &velocity);
void drawBlocks(sf::RenderWindow &, const vector<Block>& platform, const vector<Block>& block);
void EndScreenMode(sf::RenderWindow &, typeName& );
void drawCredit(sf::RenderWindow &, typeName& gameState);

int main()
{
	// initalizing window
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "Jumping Anthony");
	window.setFramerateLimit(60);
	
	//initializing main character sprite
	sf::Texture mainCharacterTexture;
	if (!mainCharacterTexture.loadFromFile(resourcePath() + "assets/pikachu.png"))
		std::cout << "Can't load main character picture";

	sf::Sprite mainCharacterSprite;
	mainCharacterSprite.setScale(0.05, 0.05);
	mainCharacterSprite.setTexture(mainCharacterTexture);
	mainCharacterSprite.setPosition(100, 600);

	//initialize background
	MovingBackground movingBackground;
	// score
	sf::Font font;
	sf::String points;
	
	if (!font.loadFromFile(resourcePath() + "assets/snap.ttf"))
	{
		std::cout << "Error: File not found" << std::endl;
	}
	sf::Text score(points, font, 30);
	score.setPosition(350, 700);
	
	//initializing block sprite
	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile(resourcePath() + "assets/block.png"))
		std::cout << "Can't load block image to VS";

	vector<Block> platform;
	vector<Block> blocks;
	createBlock(&blockTexture, blocks, platform);
	
	//initialing game state to opening screen
	gameState = OPENING;
	int counter = 0;

	//OPEN THE WINDOW
	while (window.isOpen())
	{
		switch (gameState)
		{
		case OPENING:
			OpeningSceneMode(window);
			break;
		case PLAY:
			PlayMode(window, mainCharacterSprite, platform, blocks, score, movingBackground);
			break;
		case GAME_OVER:
			EndScreenMode(window, gameState);
			break;
		case CREDIT:
			drawCredit(window, gameState);
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
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite, vector<Block> &platform, 
		vector<Block>& blocks, sf::Text score, MovingBackground &movingBackground)
{
	sf::Event event;
	

	sf::Vector2f velocity(sf::Vector2f(0, 0));
	int numBlock = 0;
	int points = 0;
	srand(time(NULL));

	//reset main character position to replay game
	mainCharacterSprite.setPosition(200.f, 200.f);

	while (window.isOpen() && gameState == PLAY)
	{
		movingBackground.moving();
	
		

		//when block goes out of the screen vertically, move it back to the top
		for (int i = 0; i < NUMBLOCKS; i++) {
			if (blocks[i].sprite.getPosition().y > LengthWindow) {
				blocks[i].sprite.setPosition(rand() % 400, rand() % 400);
				blocks[i].sprite.setScale(sf::Vector2f(1, 0.5));
				blocks[i].pointsReceived = false;

			}
		}
		
		moveBlocks(platform, blocks);
		gravity(mainCharacterSprite, platform, blocks, points, velocity);
		
		//main character movement and limit character from moving out of screen
		mainCharacterSprite.move(velocity.x, velocity.y);
		
		if (mainCharacterSprite.getPosition().x > WidthWindow - mainCharacterSprite.getGlobalBounds().width){
			//when the right side of the character is beyond the right side of the game window, move the character back to be at the right side of the window and stop further right movement
			mainCharacterSprite.setPosition(WidthWindow - mainCharacterSprite.getGlobalBounds().width, mainCharacterSprite.getPosition().y);
			velocity.x = 0;
		}
		else if (mainCharacterSprite.getPosition().x < 0){
			//when the left side of the character is beyond the left side of the game window, move the character back to be at the left side of the window and stop further left movement
			mainCharacterSprite.setPosition(0, mainCharacterSprite.getPosition().y);
			velocity.x = 0;
		}

		stringstream convertToString;
		convertToString << points;
		score.setString(convertToString.str());
		

		if (mainCharacterSprite.getPosition().y > LengthWindow)
			gameState = GAME_OVER;
		

		while (window.pollEvent(event)){		
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//display everything on screen
		window.clear();
		window.draw(score);
		window.draw(movingBackground.getBackground(0));
		window.draw(movingBackground.getBackground(1));
		window.draw(movingBackground.getBackground(2));
		window.draw(mainCharacterSprite);
		drawBlocks(window, platform, blocks);
		
		window.display();
	}
}

void moveBlocks(vector<Block> &platform, vector<Block> &block)
{
	//move the block down at the same speed (we are not using view)
	for (int x = 0; x < 4; x++)
	{
		platform[x].sprite.move(sf::Vector2f(0, 5));
	}
	
	for (int k = 0; k < NUMBLOCKS; k++)
	{
		block[k].sprite.move(sf::Vector2f(0, 5));
	}
}

/********************************************
				createBlock()
create a platform vector of 4 block object and a block vector of 10 objects
*********************************************/

void createBlock(sf::Texture *blockTexture, vector<Block>&blocks, vector<Block>&platform)
{
	for (int i = 0; i < 4; i++) //platform
	{
		platform.push_back(Block());
		platform[i].sprite.setTexture(*blockTexture);
		platform[i].sprite.setPosition(sf::Vector2f(i * 120, 700));
		platform[i].sprite.setScale(sf::Vector2f(1, 0.5));
		platform[i].pointsReceived = true;
	}

	for (int numBlock = 0; numBlock < NUMBLOCKS; numBlock++) // blocks
	{
		blocks.push_back(Block());
		//block[numBlock].setOrigin(sf::Vector2f(60, 20));
		blocks[numBlock].sprite.setTexture(*blockTexture);
		blocks[numBlock].sprite.setPosition(sf::Vector2f((rand() % 280 + 1), 600 - (120 * numBlock)));
		blocks[numBlock].sprite.setScale(sf::Vector2f(1, 0.5));
		blocks[numBlock].pointsReceived = false;
	}

}

/***********************************************************
				changeBlockPosition
if any blocks inside the vector block run out of screen, move it back to the top of the screen
*/
void changeBlockPosition(vector<Block> &block, int numBlock)
{

	for (int i = 0; i < block.size(); i++) {
		if (block[i].sprite.getPosition().y > LengthWindow) {
			block[numBlock].sprite.setPosition(sf::Vector2f((rand() % 280 + 1), block[numBlock - 1].sprite.getPosition().y - 120));
			block[i].sprite.setScale(sf::Vector2f(1, 0.5));
			block[i].pointsReceived = false;
		}
	}
}
/*
	if (numBlock == 0)
	{
		block[numBlock].sprite.setPosition(sf::Vector2f((rand() % 280 + 1), block[block.size() - 1].sprite.getPosition().y - 120));
	}
	else
	{
		//place that block above the lastest block created.
		block[numBlock].sprite.setPosition(sf::Vector2f((rand() % 280 + 1), block[numBlock - 1].sprite.getPosition().y - 120)); 
	}
	block[numBlock].sprite.setScale(sf::Vector2f(1, 0.5));
	block[numBlock].pointsReceived = false;
	*/



void gravity(sf::Sprite &mainCharacterSprite, vector<Block> platform, vector<Block> &blocks, 
	int &points, sf::Vector2f &velocity)
{
	const float GRAVITY = 1;
	float moveSpeed = 30.0f, jumpSpeed = 45.0f;

	sf::Vector2f currPos = mainCharacterSprite.getPosition();

	//move left and right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //stops when the key is not pressed
		velocity.x = -moveSpeed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		velocity.x = moveSpeed;
	else
		velocity.x = 0;

	//when shape touches the block the shape should jump
	for (int x = 0; x < 4; x++)
	{
		if (overlap(mainCharacterSprite, platform[x].sprite))
		{
			velocity.y = -jumpSpeed;
		}
		if (mainCharacterSprite.getPosition().y < platform[x].sprite.getPosition().y) // groundposition
		{
			velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
		}
		else if (velocity.y > 0 && overlap(mainCharacterSprite, platform[x].sprite)) //if overlap with platform setposition
		{
			//mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, platform[x].getPosition().y);
			mainCharacterSprite.setPosition(currPos);
		}
	}

	for (int elem = 0; elem < NUMBLOCKS; elem++)
	{
		if (mainCharacterSprite.getPosition().y < blocks[elem].sprite.getPosition().y) // groundposition
		{
			velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
		}
		else if (velocity.y > 0 && overlap(mainCharacterSprite, blocks[elem].sprite)) //if overlap with one of the blocks setposition velocity.y>0 means the mainCharacterSprite is falling
		{
			//mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, block[elem].getPosition().y);
			mainCharacterSprite.setPosition(currPos);
		}
		if (velocity.y > 0 && overlap(mainCharacterSprite, blocks[elem].sprite)) //when the velocity is greater than 0 and character overlap with one of the block, jump
		{
			velocity.y = -jumpSpeed;
			if (blocks[elem].pointsReceived == false)
			{
				points++;
			}

			blocks[elem].pointsReceived = true;
		}
	}
}

void drawBlocks(sf::RenderWindow &window, const vector<Block>& platform, const vector<Block>& block)
{
	for (int x = 0; x < 4; x++)
	{
		window.draw(platform[x].sprite);
	}
	for (int k = 0; k < NUMBLOCKS; k++)
	{
		window.draw(block[k].sprite);
	}
}

//draw the ending screen and 
void EndScreenMode(sf::RenderWindow &window, typeName& gameState)
{
	window.clear();
	EndScreen(window);
	sf::Event event;

	
	while (window.pollEvent(event))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			gameState = PLAY;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			gameState = CREDIT;

		if (event.type == sf::Event::MouseButtonPressed)
		{
			window.clear();
			window.close();
		}
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

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
	title.setCharacterSize(75);
	title.setColor(sf::Color::Magenta);
	title.setStyle(sf::Text::Bold); // | sf::Text::Underlined
	title.setPosition(30, 200);

	//names of developers
	names.setFont(font);
	names.setString("Anh Pham\nYe-Eun Myung\nPhillip Nguyen\nand more");
	names.setCharacterSize(24);
	names.setColor(sf::Color::Black);
	names.setPosition(100, 350);	//WILL DEFINITELY HAVE TO ADJUST POSITIONING

	//ask if user want to replay
	prompt.setFont(font);
	prompt.setString("Press R to replay\n\nClick to Close");
	prompt.setCharacterSize(24);
	prompt.setColor(sf::Color::Black);
	prompt.setPosition(100, 500);

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