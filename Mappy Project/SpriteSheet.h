//Nathan Walter
//CPSC 440
//Program 4: Hedge Maze

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
using namespace std;
/*The sprite class handle all of the functions of the player sprite. It is responsible for drawing, updating, and checking collision detection 
* based on the direction of the sprite. It also handles the sprite sheet grabbing and end-block collision detection.
*/
class Sprite
{
	friend int collided(int x, int y);
	friend bool endValue( int x, int y );
public:
	Sprite();	//constructor
	~Sprite();	//deconstructor
	/*Initializes the sprite and loads in the sprite sheet.
	@param int, width
	@param int, height
	@return void*/
	void InitSprites(int width, int height);
	/*Updates the sprite's position based on the animation direction. It also determines the collision detection between the sprite and the blocks.
	@param int, width
	@param int, height
	@param int, dir
	@return void*/
	void UpdateSprites(int width, int height, int dir); //dir 0 = left, 1 = right, 2 = up, 3 = down, 4 = hovering
	/*Draws the sprite to the screen based on the animation direction and last key pressed.
	@param int, xoffset
	@param int, yoffset
	@return void*/
	void DrawSprites(int xoffset, int yoffset);
	/*Gets the sprite's x position.
	@param none
	return float, x*/
	float getX() {return x;}
	/*Gets the sprite's y position.
	@param none
	return float, y*/
	float getY() {return y;}
	/*Gets the sprite's width.
	@param none
	return int, frameWidth*/
	int getWidth() {return frameWidth;}
	/*Gets the sprite's height.
	@param none
	return int, frameHeight*/
	int getHeight() {return frameHeight;}
	/*Checks if the player has collided with an end block.
	@param none
	@return bool, 0 if no collision*/
	bool CollisionEndBlock();
	/*Resets the player's position back to the start after completing a level.
	@param none
	@return void*/
	void resetPosition() { x = 0, y = 64; }
private:
	float x;
	float y;
	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	float frameWidth;
	float frameHeight;
	int animationColumns;
	int animationRows;
	int animationDirection;
	ALLEGRO_BITMAP *image;
	int lastKey;	//last key pressed, regardless of current animation direction
};
