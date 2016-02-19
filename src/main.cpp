#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "OpeningScreen.h"
#include "main.h"

void OpeningSceneMode(sf::RenderWindow&);
void PlayMode(sf::RenderWindow&, sf::Sprite&);

int main()
{
	// initalizing window
	sf::RenderWindow window(sf::VideoMode(WidthWindow, LengthWindow), "Jumping Anthony");
	//just a precautionary framerate limit
	window.setFramerateLimit(60);
	// initalizing a texture object
	sf::Texture mainCharacterTexture;
	// applying an image to the texture object (it can be found in your assets folder)
	mainCharacterTexture.loadFromFile(resourcePath() + "assets\\pikachu.png");

	// applying texture object to sprite
	sf::Sprite mainCharacterSprite;
	mainCharacterSprite.setScale(0.2, 0.2);
	mainCharacterSprite.setTexture(mainCharacterTexture);
	//INITIALIZATION
	gameState = OPENING;
	int counter = 0;

	//OPEN THE WINDOW
	while (window.isOpen())
	{
		switch (gameState)
		{
		case PLAY:
			PlayMode(window, mainCharacterSprite);
			break;
		case OPENING:
			OpeningSceneMode(window);
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
void PlayMode(sf::RenderWindow &window, sf::Sprite &mainCharacterSprite)
{
	sf::Event event;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	
		mainCharacterSprite.move(-1, 0);
	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mainCharacterSprite.move(1, 0);
	
	while (window.pollEvent(event))
	{
	if (event.type == sf::Event::Closed)
		window.close();
	}//end of while (window.pollEvent)

		window.clear();
		window.draw(mainCharacterSprite);
		window.display();
	
}