//Improvement: better commenting (comment prior to every line of code and function's purpose
//if the program runs slow, maybe you are using too many unneccessary loops


#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "ResourcePath.h"
#include "OpeningScreen.h"
#include "EndScreen.h"
#include "Overlap.h"
#include "main.h"
#include "MovingBackground.h"
#include "CreditScreen.h"
using namespace std;

struct Block
{
	sf::Sprite sprite;
	bool pointsReceived;
};

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite,
	vector<Block>& blocks, sf::Text& score, MovingBackground &movingBackground, int& points);
void checkMainCharacterBound(sf::Sprite &mainCharacterSprite, sf::Vector2f &velocity);
void moveBlocks(vector<Block> &block);
void changeBlockPosition(vector<Block>&);
void gravity(sf::Sprite &mainCharacterSprite, vector<Block> &blocks, int &points, sf::Vector2f &velocity);
void EndScreenMode(sf::RenderWindow &, typeName& , sf::Text& score, int& points);

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

	//initializing a vector of blocks
	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile(resourcePath() + "assets/block.png"))
		std::cout << "Can't load block image to VS";
	sf::Sprite blockSprite(blockTexture);
	blockSprite.setScale(sf::Vector2f(1, 0.5));
	//set the origin of the block to be in the middle of the block
	blockSprite.setOrigin((blockSprite.getGlobalBounds().width) / 2, (blockSprite.getGlobalBounds().height) / 2);

	Block block;					//this is one block
	block.sprite = blockSprite;
	block.pointsReceived = false;

	vector<Block> blocks;			//vector of blocks
	//add 1 block to the vector and set the position of each block in the vector equally on the screen (8 blocks and 800 window length => 1 block per 100 vertical pixel  
	for (int i = 0; i < NUMBLOCKS; ++i) {
		blocks.push_back(block);	//add 1 block to the vector
		blocks[i].sprite.setPosition(rand() % 400, i * (LengthWindow / NUMBLOCKS));		//every blocks is 100 pixel higher than the other		
	}
	//define score (print to screen) and points (int)
	sf::Font Scorefont;
	if (!Scorefont.loadFromFile(resourcePath() + "assets/snap.ttf"))
		std::cout << "Error: File not found" << std::endl;

	sf::Text score;
	score.setFont(Scorefont);
	score.setCharacterSize(30);
	score.setColor(sf::Color::Black);
	score.setPosition(350, 50);
	stringstream displayPointsOnScreen;
	//initialing game state to opening screen
	gameState = OPENING;
	int points = 0;
	//OPEN THE WINDOW

	while (window.isOpen())
	{
		switch (gameState)
		{
		case OPENING:
			OpeningSceneMode(window);
			break;
		case PLAY:
			PlayMode(window, mainCharacterSprite, blocks, score, movingBackground, points);
			break;
		case GAME_OVER:
			EndScreenMode(window, gameState, score, points);
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
		vector<Block>& blocks, sf::Text &score, MovingBackground &movingBackground, int &points)
{
	if (savedToFile)
		savedToFile = false;
	
	sf::Event event;
	srand(time(NULL));
	sf::Vector2f velocity(sf::Vector2f(0, 0));
	//reset main character position to replay game
	mainCharacterSprite.setPosition(200.f, 200.f);

	//update game
	while (window.isOpen() && gameState == PLAY)
	{
		movingBackground.moving();

		moveBlocks(blocks);
		changeBlockPosition(blocks);
		
		//main character movement and limit character from moving out of screen
		gravity(mainCharacterSprite, blocks, points, velocity);
		mainCharacterSprite.move(velocity.x, velocity.y);
		checkMainCharacterBound(mainCharacterSprite, velocity);
		

		// when the main character disappears from the bottom of the screen, gameover
		if (mainCharacterSprite.getPosition().y > LengthWindow)
			gameState = GAME_OVER;
		while (window.pollEvent(event)){		
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//cout <<"!!!!!Points   " <<points;
		//***********initialize variable outside of loop for better performance ****************************
		
		score.setString(std::to_string(points));

		//display everything on screen
		window.clear();
		window.draw(movingBackground.getBackground(0));
		window.draw(movingBackground.getBackground(1));
		window.draw(movingBackground.getBackground(2));
		window.draw(mainCharacterSprite);
		//draw blocks
		for (int k = 0; k < blocks.size(); k++)
			window.draw(blocks[k].sprite);
		window.draw(score);

		window.display();
	}
}

/*************************************************
			moveBlocks()
move the block down at the same speed
**************************************************/
void moveBlocks(vector<Block> &blocks)
{
	for (int i = 0; i < NUMBLOCKS; ++i){
		blocks[i].sprite.move(sf::Vector2f(0, 5));
	}
}

/***********************************************************
				changeBlockPosition()
if any blocks inside the vector block runs below the length of the window
100 pixels vertically, move it back to the top of the screen without overlaping existing blocks 
***************************************************************/
void changeBlockPosition(vector<Block> &blocks)
{
	//cout << "\nInside changeBlockPositionBlock\n ";

	for (int i = 0; i < blocks.size(); ++i) {
		//cout << "\n\nBlock #" << i << " x  position is: " << blocks[i].sprite.getPosition().x;
		//cout << "\n\nBlock #" << i << " y  position is: " << blocks[i].sprite.getPosition().y;

		if (blocks[i].sprite.getPosition().y > (LengthWindow)){ //- 200)) {
			blocks[i].sprite.setPosition(rand() % 400 + 1, rand() % 100);
			//if overlap x-position, reset position
			if (overlap(blocks[NUMBLOCKS - 1 - i].sprite, blocks[i].sprite)) {
				blocks[i].sprite.setPosition(rand() % 400 + 1, rand() % 100);
			}
			blocks[i].pointsReceived = false;
		}
	}
	
}

/********************************************************
this function will influence gravity on the main character
also, if MC overlap with blocks, user earn 1 point
*********************************************************/
void gravity(sf::Sprite &mainCharacterSprite, vector<Block> &blocks, 
							int &points, sf::Vector2f &velocity)
{
	float moveSpeed = 30.0f, jumpSpeed = 40.0f;
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
			velocity.y += GRAVITY; // if the character is above the block, add the gravity to the y-value in velocity so it will go down.
		
		//when the velocity is greater than 0 and character overlap with one of the block, jump
		if (velocity.y > 0 && overlap(mainCharacterSprite, blocks[i].sprite)) //if overlap with one of the blocks setposition velocity.y>0 means the mainCharacterSprite is falling
		{
			mainCharacterSprite.setPosition(currPos);
			velocity.y = -jumpSpeed;
			if (blocks[i].pointsReceived == false)
			{
				points++;
			}
			blocks[i].pointsReceived = true;
		}
	}		//end for loop
	//cout << "!!!!!Points   " << points;
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
void EndScreenMode(sf::RenderWindow &window, typeName& gameState, sf::Text& score, int& points)
{

	window.clear();
	EndScreen(window, points, score);
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