#include "main.h"
#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>



#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int map[mapWidth][mapHeight] = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


int main(int /*argc*/, char*/*argv*/[])
{
	// Start position of the player
	double posX = 22;
	double posY = 12; 

	// Initial player direction
	double dirX = -1;
	double dirY = 0;

	// Camera plane
	double planeX = 0;
	double planeY = 0.66;

	double time = 0; // Time of current frame
	double oldTime = 0; // Time of previous frame

	// Creating the screen
	sf::VideoMode videoMode(640, 680);
	sf::RenderWindow* window = new sf::RenderWindow(videoMode, "Raycastor", sf::Style::Titlebar | sf::Style::Close);

	bool done = false;

	while (!done)
	{
		// Raycasting Loop
		for (unsigned x = 0; x < mapWidth; x++)
		{
			// Calculate ray position and direction
			double cameraX = ((2 * x) / double(mapWidth)) - 1; // x-coord in camera space
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			// Determines which box of the map we are in
			int mapX = int(posX);
			int mapY = int(posY);

			// Length of ray from current position
			double sideDistX;
			double sideDistY;

			// Length of ray from one x-side or y-side to the next x-side or y-side
			double deltaDistX = std::abs(1 / rayDirX);
			double deltaDistY = std::abs(1 / rayDirY);
			double perpWallDist;

			// What direction to step in (x or y [going to be +1 or -1])
			int stepX;
			int stepY;

			// Determines whether or not a wall was hit
			int hit = 0;

			// Determines what side was hit (x or y [0 for x-side, 1 for y-side])
			int side;

			// Calculate initial step and sideDist (having some trouble understanding the calculations on this part...)
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = ((mapX + 1.0) - posX) * deltaDistX;
			}

			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = ((mapY + 1.0) - posY) * deltaDistY;
			}

			// ********** THIS IS WHERE THE DDA ALGORITHM ACTUALLY STARTS **********

		}
	}


}

