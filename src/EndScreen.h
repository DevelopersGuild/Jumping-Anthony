#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include "main.h"
#include <iostream>
#include <fstream>
#include <cstdlib>


void printToFile(const int& points, sf::Text& score);
/*
				EndScreen
receives: window, points (user's curret points), score (high score that will be print to screen
purpose: endscreen will print the end screen and high score
return  : none
*/
void EndScreen(sf::RenderWindow &window, const int &points, sf::Text& score)
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
	text1.setPosition(30, 50);

	//Quick Instruction to change to next state
	text2.setFont(font);
	text2.setString("GAME OVER");
	text2.setCharacterSize(24);
	text2.setColor(sf::Color::Black);
	text2.setPosition(100, 450);	//WILL DEFINITELY HAVE TO ADJUST POSITIONING


	//high score to screen
	scoreText.setFont(font);
	scoreText.setString("High Score");
	scoreText.setCharacterSize(35);
	scoreText.setColor(sf::Color::Magenta);
	scoreText.setStyle(sf::Text::Bold); // | sf::Text::Underlined
	sf::FloatRect textRect = scoreText.getLocalBounds();
	scoreText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	scoreText.setPosition(WidthWindow / 2 - 50, 300);			//50 : size of score
	
	if (!savedToFile) {
		printToFile(points, score);
		savedToFile = true;		
	}
	//actual score
	score.setColor(sf::Color::Green);
	scoreText.setCharacterSize(35);
	sf::FloatRect textRect2 = score.getLocalBounds();
	score.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
	score.setPosition(WidthWindow *2/3 + 50 , 300);			
	
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
	window.draw(score);			//actual high score (int)
	window.draw(close);
	window.display();

}

/*
				printToFile
receives: points (current points from user, &score (highscore string that will be printed to screen
purpose: read from inputFile (highScore.txt) and compare the user's score and inputFile's input
		 store whichever higher score in sf::Text&score 
		 save the higher score in inputFile
return: nothing
*/
void printToFile(const int& userPoints, sf::Text& score) {
	int higherScore = 0;
	int inputScore = -1;			//score from input file "highScore.txt"
	std::fstream highScoreFile("highScore.txt", std::fstream::in );			//put this in main.h
	
	if (!highScoreFile.is_open()) {
		std::cout << "Error opening highScore.txt";
		return;
	}

	//read the integer in the last line of highScoreFile to inputScore
	while (!highScoreFile.eof())		
		highScoreFile >> inputScore;

	/*
	vector<int> myScores;
	while (highScoreFile >> inputScore)
	{
		myScores.push_back(inputScore);		
	}
	*/

	//std::cout << "\nIn printToFile function, input Score is: " <<inputScore <<std::endl;
	if (userPoints < inputScore) {	
		score.setString(std::to_string(inputScore));
		
	}
	else {
		score.setString(std::to_string(userPoints));
		highScoreFile.close();

		highScoreFile.open("highScore.txt", std::fstream::out | std::fstream::trunc);
		std::cout << "\n\n\nPrinting to output file" <<userPoints;
		highScoreFile << userPoints;
	}

	highScoreFile.close();
}
#endif