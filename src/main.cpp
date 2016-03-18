//Improvement: better commenting (comment prior to every line of code and function's purpose

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
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite,
	vector<Block>& blocks, sf::Text score, MovingBackground &movingBackground);
void checkMainCharacterBound(sf::Sprite &mainCharacterSprite, sf::Vector2f &velocity);

void moveBlocks(vector<Block> &block);
void changeBlockPosition(vector<Block>&);
void checkBlockCollision(vector<Block> &blocks);
void gravity(sf::Sprite &mainCharacterSprite, vector<Block> &blocks, int &points, sf::Vector2f &velocity);
void drawBlocks(sf::RenderWindow &, const vector<Block>& block);
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
	
	//initializing a vector of blocks
	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile(resourcePath() + "assets/block.png"))
		std::cout << "Can't load block image to VS";
	sf::Sprite blockSprite(blockTexture);
	blockSprite.setScale(1, 1);
	
	Block block;					//this is one block
	block.sprite = blockSprite;
	block.pointsReceived = false;
	//cout <<"****The origin of block is: " <<(block.sprite.getGlobalBounds().width) / 2 << "and "<< (block.sprite.getGlobalBounds().height) / 2;		//60 and 20

	vector<Block> blocks;			//vector of blocks
	//add 1 block to the vector and set the position of each element in the vector to default
	for (int i = 0; i < NUMBLOCKS; ++i) {
		blocks.push_back(block);	//add 1 block to the vector
		blocks[i].sprite.setTexture(blockTexture);
		blocks[i].sprite.setPosition(rand() % 400, i * 100);		//every blocks is 100 pixel higher than the other
		blocks[i].sprite.setScale(sf::Vector2f(1, 0.5));
		blocks[i].sprite.setOrigin((blocks[i].sprite.getGlobalBounds().width) / 2, (blocks[i].sprite.getGlobalBounds().height) / 2);
	}
	//checkBlockCollision(blocks);		
	
		
	
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
			PlayMode(window, mainCharacterSprite, blocks, score, movingBackground);
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

/**********************************
OPENING SCENE EVENT HANDLE
draw opening screen and prompt user click to play
************************************/
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
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite,  
		vector<Block>& blocks, sf::Text score, MovingBackground &movingBackground)
{
	sf::Event event;
	
	sf::Vector2f velocity(sf::Vector2f(0, 0));
	int points = 0;
	srand(time(NULL));

	//reset main character position to replay game
	mainCharacterSprite.setPosition(200.f, 200.f);

	//update game
	while (window.isOpen() && gameState == PLAY)
	{
		movingBackground.moving();
		//create blocks randomly, one on top of another
		//when block goes out of the screen vertically, move it back to the top
	
		moveBlocks(blocks);
		changeBlockPosition(blocks);
		//checkBlockCollision(blocks);
		
		gravity(mainCharacterSprite, blocks, points, velocity);
		
		//main character movement and limit character from moving out of screen
		mainCharacterSprite.move(velocity.x, velocity.y);
		
		checkMainCharacterBound(mainCharacterSprite, velocity);
		//when the right side of the character is beyond the right side of the game window,
		//move the character back to be at the right side of the window, and stop further right movement
		if (mainCharacterSprite.getPosition().x > WidthWindow - mainCharacterSprite.getGlobalBounds().width){
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
		drawBlocks(window, blocks);
		
		window.display();
	}
}

/*************************************************
			moveBlocks()
move the block down at the same speed
**************************************************/
void moveBlocks(vector<Block> &blocks)
{
	int i = 0;
	for (; i < NUMBLOCKS; ++i){
		blocks[i].sprite.move(sf::Vector2f(0, 2.5));
	}
}

/***********************************************************
				changeBlockPosition()
if any blocks inside the vector block runs below the length of the window
100 pixels vertically, move it back to the top of the screen 
***************************************************************/
void changeBlockPosition(vector<Block> &blocks)
{
	
	cout << "\nInside changeBlockPositionBlock\n ";

	for (int i = 0; i < blocks.size(); ++i) {
		//cout << "\n\nBlock #" << i << " x  position is: " << blocks[i].sprite.getPosition().x;
		//cout << "\n\nBlock #" << i << " y  position is: " << blocks[i].sprite.getPosition().y;
			if (blocks[i].sprite.getPosition().y > (LengthWindow - 200)) {
				
				blocks[i].sprite.setPosition(rand() % 400 + 1, rand() % 200);
				//if overlap, reset position

				blocks[i].pointsReceived = false;
			}
		}
	//checkBlockCollision(blocks);
}

/*check the vector blocks if any block elements has the same position
if yes, re-calculate the position 
unfinished 
*/
void checkBlockCollision(vector<Block> &blocks) {
	bool shouldReCheck;
	do {
		shouldReCheck = false;
		for (int i = 0; i < NUMBLOCKS; ++i) {
			for (int j = i; j < NUMBLOCKS - 1; ++j) {
				if (overlap(blocks[i].sprite, blocks[j].sprite)) {
					blocks[i].sprite.setPosition(rand() % 400, rand() % (LengthWindow + i));		//every blocks is 100 pixel higher than the other
					shouldReCheck = true;
					cout << "Should redo\n";
					break;
				}
			}

			if (shouldReCheck)
			{
				break;
			}
		}
	} while (shouldReCheck);
}
/********************************************************
this function will influence gravity on the main character
*********************************************************/
void gravity(sf::Sprite &mainCharacterSprite, vector<Block> &blocks, 
							int &points, sf::Vector2f &velocity)
{
	float moveSpeed = 30.0f, jumpSpeed = 45.0f;
	sf::Vector2f currPos = mainCharacterSprite.getPosition();

	//move left and right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //stops when the key is not pressed
		velocity.x = -moveSpeed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		velocity.x = moveSpeed;
	else
		velocity.x = 0;

	//when main character touches the block, the maincharter should jump
	for (int i = 0; i < NUMBLOCKS; ++i)
	{
		if (mainCharacterSprite.getPosition().y < blocks[i].sprite.getPosition().y) // groundposition
			velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity
		
		else if (velocity.y > 0 && overlap(mainCharacterSprite, blocks[i].sprite)) //if overlap with one of the blocks setposition velocity.y>0 means the mainCharacterSprite is falling
		{
			//mainCharacterSprite.setPosition(mainCharacterSprite.getPosition().x, block[elem].getPosition().y);
			mainCharacterSprite.setPosition(currPos);
		}
		//when the velocity is greater than 0 and character overlap with one of the block, jump
		if (velocity.y > 0 && overlap(mainCharacterSprite, blocks[i].sprite)) 
		{
			velocity.y = -jumpSpeed;
			if (blocks[i].pointsReceived == false)
			{
				points++;
			}

			blocks[i].pointsReceived = true;
		}
	}		//end for loop
}

void drawBlocks(sf::RenderWindow &window, const vector<Block>& blocks)
{
	for (int k = 0; k < blocks.size(); k++)
		window.draw(blocks[k].sprite);
}

/*************************************************************
if character goes out of the screen bound on the right or the left, move it back
******************************************************************/
void checkMainCharacterBound(sf::Sprite &mainCharacterSprite, sf::Vector2f &velocity) {
	//when the right side of the character is beyond the right side of the game window,
	//move the character back to be at the right side of the window, and stop further right movement
	if (mainCharacterSprite.getPosition().x > WidthWindow - mainCharacterSprite.getGlobalBounds().width) {
		mainCharacterSprite.setPosition(WidthWindow - mainCharacterSprite.getGlobalBounds().width, mainCharacterSprite.getPosition().y);
		velocity.x = 0;
	}
	else if (mainCharacterSprite.getPosition().x < 0) {
		//when the left side of the character is beyond the left side of the game window, move the character back to be at the left side of the window and stop further left movement
		mainCharacterSprite.setPosition(0, mainCharacterSprite.getPosition().y);
		velocity.x = 0;
	}
}

//draw the ending screen 
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
	title.setCharacterSize(60);
	title.setColor(sf::Color::Magenta);
	title.setStyle(sf::Text::Bold); 
	title.setPosition(30, 200);

	//names of developers
	names.setFont(font);
	names.setString("Anh Pham\nYe-Eun Myung\nPhillip Nguyen\nand more");
	names.setCharacterSize(24);
	names.setColor(sf::Color::Black);
	names.setPosition(100, 400);	

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