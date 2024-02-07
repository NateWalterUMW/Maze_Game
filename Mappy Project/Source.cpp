//Nathan Walter
//CPSC 440
//Program 4: Hedge Maze

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
using namespace std;

int collided(int x, int y);  //Tile Collision
bool endValue( int x, int y );	//End Block with the User Value = 8
void getGameInfo(const int WIDTH, const int HEIGHT, int msRemain, const int FPS, int level, bool levelOver);
/*This is main. It is responsible for the creating an instance of the sprite and for loading in the maze maps from mappy. It creates the game loop and 
* determines the key bindings. It also controls the x and y offsets such that the screen will scrolll horizontally or vertically depending on the player's 
* position. It calls on the SpriteSheet.h functions to draw sprite to the screen and update it's position each frame.*/
int main(void)
{
	const int WIDTH = 900;
	const int HEIGHT = 480;
	bool keys[] = {false, false, false, false, false};
	enum KEYS{UP, DOWN, LEFT, RIGHT};

	//variables
	bool done = false;
	bool render = false;
	bool levelOver = false;
	bool introScreen = true;
	const int FPS = 60;
	int level = 1;
	char name[80];

	//Player Variable
	Sprite player;

	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;

	//program init
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	player.InitSprites(WIDTH,HEIGHT);
	int xOff = 0;
	int yOff = 0;
	int msRemain = 61 * FPS;	//set the countdown timer (61 instead of 60 just to account for loading times, to give the player a true 60 seconds instead of 59 or 58)
	if(MapLoad("map1.fmp", 1))		//load into the first level
		return -5;

	ALLEGRO_FONT* font24 = al_load_ttf_font("GROBOLD.ttf", 24, 0);	//load the fonts
	ALLEGRO_FONT* font18 = al_load_ttf_font("GROBOLD.ttf", 18, 0);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//display a brief intro screen before starting the game, lasts 15 seconds
	while (introScreen)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font24, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) - 200, ALLEGRO_ALIGN_CENTER, "Welcome to the maze game!");
		al_draw_text(font24, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) - 100, ALLEGRO_ALIGN_CENTER, "You have 60 seconds to get through each level of the maze.");
		al_draw_text(font24, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) - 65, ALLEGRO_ALIGN_CENTER, "Use the arrow keys to navigave the maze. Good luck!");
		al_draw_text(font18, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT - 25, ALLEGRO_ALIGN_CENTER, "(The game will begin shortly...)");
		al_flip_display();
		al_rest(10);	//show this page for 10 seconds, then start the game
		introScreen = false;
	}
	al_start_timer(timer);
	al_flip_display();
	
	while (!done)	//game loop
	{
		if (levelOver)	//if current level is complete, start the next level
		{
			player.resetPosition();		//reset player's position to start on next map
			timer = 0;
			msRemain = 61 * FPS;
			levelOver = false;
			level++;
			sprintf(name, "map%i.fmp", level);

			MapLoad(name, 1);	//load the next map
		}
		if (msRemain == 0 || (levelOver && level == 4))	//if the countdown hits 0, or if the player beats all 3 levels
		{
			done = true;
		}
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			MapUpdateAnims();	//update animated tiles
			if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 2);

			else if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 3);

			else if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 0);

			else if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 1);

			else
				player.UpdateSprites(WIDTH, HEIGHT, 4);

			if (player.CollisionEndBlock())		//if the player reaches the end block
			{
				levelOver = true;
			}
			render = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			//yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);
			player.DrawSprites(xOff, yOff);

			al_draw_textf(font24, al_map_rgb(255, 255, 255), 8, 8, ALLEGRO_ALIGN_LEFT, "Level %d", level);	//draw the current level

			if ((msRemain / FPS) <= 10)
				al_draw_textf(font24, al_map_rgb(255, 0, 0), WIDTH - 8, 8, ALLEGRO_ALIGN_RIGHT, "%d s", msRemain / FPS);	//draw the countdown timer in red if countdown < 10 seconds
			else
				al_draw_textf(font24, al_map_rgb(255, 255, 255), WIDTH - 8, 8, ALLEGRO_ALIGN_RIGHT, "%d s", msRemain / FPS);	//draw the countdown timer

			getGameInfo(WIDTH, HEIGHT, msRemain, FPS, level, levelOver);	//print game information (out of time, or beat the game)

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		msRemain--;		//decrement the countdown each loop through
	}
	al_rest(5.0);	//rest for 5 seconds
	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_font(font24);
	al_destroy_font(font18);
	al_destroy_display(display);	//destroy our display object

	return 0;
}
/*Given x,y values (corresponding to each corner of the block), checks if the player has collided with a side of the block.
@param int, x
@param int, y
@return int, 0 if no collision*/
int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}
/*Checks if a block is an end block by checking its user data value. If the block has a matching user data value as whatever is in the if statement, return true.
@param int, x
@param int, y
@return bool, true if block is an end block, false otherwise*/
bool endValue( int x, int y )
{
	BLKSTR* data;
	data = MapGetBlock( x/mapblockwidth, y/mapblockheight );

	if( data->user1 == 8 )
	{
		return true;
	}else
		return false;
}
/*Prints the appropriate message to the screen, depending on whether the player beat all 3 levels or if they ran out of time.
@param const int, WIDTH
@param const int, HEIGHT
@param int, msRemain
@param const int, FPS
@param int, level
@param bool, levelOver
@return void*/
void getGameInfo(const int WIDTH, const int HEIGHT, int msRemain, const int FPS, int level, bool levelOver)
{
	ALLEGRO_FONT* font24 = al_load_ttf_font("GROBOLD.ttf", 24, 0);	//load the fonts
	ALLEGRO_FONT* font18 = al_load_ttf_font("GROBOLD.ttf", 18, 0);

	if (msRemain == 0)	//if the player is out of time
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font24, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "-- Time's up! --");

		if (level == 1)
			al_draw_text(font18, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) + 40, ALLEGRO_ALIGN_CENTER, "( You didn't get past level 1 )");
		else 
			al_draw_textf(font18, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT / 2) + 40, ALLEGRO_ALIGN_CENTER, "( You reached level %d )", level);

		al_draw_text(font18, al_map_rgb(255, 255, 255), 8, HEIGHT - 25, ALLEGRO_ALIGN_LEFT, "Closing shortly...");
	}
	if (level == 4)		//if the player beat all 3 levels
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font24, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "-- You beat the game! --");
		al_draw_text(font18, al_map_rgb(255, 255, 255), 8, HEIGHT - 25, ALLEGRO_ALIGN_LEFT, "Press ESC to quit...");
	}
	al_destroy_font(font24);
	al_destroy_font(font18);
}
