#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "main.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

void compareHighScore(const int& points, const sf::Text& score);
/*
				EndScreen
receives: window, points (user's curret points), score (high score that will be print to screen
purpose: endscreen will print the end screen and high score
return  : none
*/
void EndScreen(sf::RenderWindow &window, const int &points, const sf::Text& score)
{
	sf::Font font;
	sf::Text text1;
	sf::Text text2;
	sf::Text text3;
	sf::Text scoreText;		//print highscore to screen
	sf::Text close; 

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


	//print high score to screen
	scoreText.setFont(font);
	scoreText.setString("High Score");
	scoreText.setCharacterSize(75);
	scoreText.setColor(sf::Color::Magenta);
	scoreText.setStyle(sf::Text::Bold); // | sf::Text::Underlined
	sf::FloatRect textRect = scoreText.getLocalBounds();
	scoreText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	scoreText.setPosition(WidthWindow / 2, 300);
	
	compareHighScore(points, score);
	
	//prompt user
	text3.setFont(font);
	text3.setString("Press R to replay\n\nPress C for credits");
	text3.setCharacterSize(24);
	text3.setColor(sf::Color::Black);
	text3.setPosition(100, 500);

	close.setFont(font);
	close.setString("Click to Close");
	close.setCharacterSize(24);
	close.setColor(sf::Color::Black);
	close.setPosition(100, 600);

	//Draws background first and then the texts
	window.clear();
	window.draw(background);
	window.draw(text1);
	window.draw(text2);
	window.draw(text3);
	window.draw(scoreText);
	window.draw(close);
	window.display();

}

/*
				compareHighScore
receives: points (current points from user, &score (highscore string that will be printed to screen
purpose: read from inputFile (highScore.txt) and compare the user's score and inputFile's input
		 store whichever higher score in sf::Text&score 
		 save the higher score in inputFile
return: nothing
*/
void compareHighScore(const int& points, const sf::Text& score) {
	int higherScore = 0;
	int inputScore = 0;			//score from input file "highScore.txt"
	std::fstream highScoreFile("highScore.txt");
	
	if (!highScoreFile.is_open()) {
		std::cout << "Error opening highScore.txt";
		return;
	}
	highScoreFile >> inputScore;
	
	std::cout << "In compareHighScore function, input Score is: " <<inputScore <<std::endl;
	if (points < inputScore) {
		stringstream convertToString;
		convertToString << inputScore;
		score.setString(convertToString.str());

	}
	//after comparison, how tf can I store higher score in highScore.txt
}
#endif