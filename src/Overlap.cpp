#include "Overlap.h"

bool overlap(sf::Sprite& sprite1, sf::Sprite& sprite2)
{
	sf::FloatRect rectangle1 = sprite1.getGlobalBounds();
	sf::FloatRect rectangle2 = sprite2.getGlobalBounds();

	// Rectangles with negative dimensions are allowed, so we must handle them correctly
	// Compute the min and max of the first rectangle on both axes
	float r1MinX = std::min(rectangle1.left, rectangle1.left + rectangle1.width); //left
	float r1MaxX = std::max(rectangle1.left, rectangle1.left + rectangle1.width);//right
	float r1MinY = std::min(rectangle1.top, rectangle1.top + rectangle1.height); // top 
	float r1MaxY = std::max(rectangle1.top, rectangle1.top + rectangle1.height); // bottom of the maincharactersprite

	// Compute the min and max of the second rectangle on both axes
	float r2MinX = std::min(rectangle2.left, rectangle2.left + rectangle2.width);//left
	float r2MaxX = std::max(rectangle2.left, rectangle2.left + rectangle2.width);//right
	float r2MinY = std::min(rectangle2.top, rectangle2.top + rectangle2.height);//top of the blocksprite
	float r2MaxY = std::max(rectangle2.top, rectangle2.top + rectangle2.height); // bottom

	// Compute the intersection boundaries
	float interLeft = std::max(r1MinX, r2MinX);
	float interTop = std::max(r1MinY, r2MinY);
	float interRight = std::min(r1MaxX, r2MaxX);
	float interBottom = std::min(r1MaxY, r2MaxY);

	//want to check if bottom and top are overlaping

	// If the intersection is valid (positive non zero area), then there is an intersection
	return ((interLeft <= interRight) && (interTop <= interBottom));
}
