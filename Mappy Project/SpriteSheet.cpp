//Nathan Walter
//CPSC 440
//Program 4: Hedge Maze

#include "SpriteSheet.h"

Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 0;
	y = 64;

	maxFrame = 14;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 2;
	frameWidth = 63.5;
	frameHeight = 60;
	animationColumns = 5;
	animationRows = 3;
	animationDirection = 1;
	lastKey = NULL;

	image = al_load_bitmap("bird.png");
}
void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	int halfWidth = frameWidth / 2;
	int halfHeight = frameHeight / 2;

	if(dir == 1) //right key
	{
		frameDelay = 2;
		animationDirection = 1;
		x += 3; 
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame =1 ;
		}
		lastKey = 1;	//update last key
	}
	else if (dir == 0) //left key
	{
		frameDelay = 2;
		animationDirection = 0;
		x -= 3; 
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame = 1;
		}
		lastKey = 0;	//update last key
	}
	else if (dir == 2) //up key
	{
		frameDelay = 0;		//decrease framedelay to appear like the wings are flapping quicker
		animationDirection = 2;
		y -= 3;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame = 1;
		}
	}
	else if (dir == 3) //down key
	{
		frameDelay = 4;		//increase framedelay to appear like the wings are flapping slower
		animationDirection = 3;
		y += 3;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame = 1;
		}
	}
	else //hovering
	{
		frameDelay = 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame >= maxFrame)
				curFrame = 1;
		}
	}
	//check for collided with foreground tiles
	if (animationDirection == 0)
	{ 
		if (collided(x + 1, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y = oldy;
		}
	}
	else if (animationDirection == 1)
	{ 
		if (collided(x + frameWidth - 1, y + frameHeight)) { //collision detection to the right
			x = oldx; 
			y = oldy;
		}
	}
	else if (animationDirection == 2)
	{ 
		if (collided(x + halfWidth, y)) { //collision detection above
			x = oldx; 
			y = oldy;
		}
	}
	else if (animationDirection == 3)
	{
		if (collided(x + halfWidth, y + frameHeight)) { //collision detection below
			x = oldx;
			y = oldy;
		}
	}
	if (x <= 0) //prevent player from moving left of the starting tile
	{
		x = 0;
	}
}
bool Sprite::CollisionEndBlock()
{
	int halfWidth = frameWidth / 2;
	int halfHeight = frameHeight / 2;

	if (endValue(x + halfWidth, y + halfHeight)) //collide when player reaches end of block (left -> right)
		return true;
	else
		return false;
}
void Sprite::DrawSprites(int xoffset, int yoffset)
{
	//sprite grabbing: x,y positions of each sprite frame
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection == 1) //right
	{
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	}
	else if (animationDirection == 0) //left
	{
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) //up
	{
		if (lastKey == 0) //up, facing left
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
		else //up, facing right
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 3) //down
	{
		if (lastKey == 0) //down, facing left
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
		else //down, facing right
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 4) //hovering
	{
		al_draw_bitmap_region(image,0,0,frameWidth,frameHeight,  x-xoffset, y-yoffset, 0);
	}
}
