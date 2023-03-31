#include "main.h"
#include <iostream>
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
  {1,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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

	sf::Time time; // Time of current frame
	sf::Time oldTime; // Time of previous frame
	sf::Clock clock;

	double moveSpeed = 0.5;
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
				int hit = false;

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
				// The DDA Loop
				while (!hit)
				{
					// Jump to the next map square (either in the x-direction or the y-direction)
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}

					// Checking to see if a wall was hit in this iteration
					if (map[mapX][mapY] > 0)
						hit = true;
				}

				// Calculate distance using the camera direction instead of player direction (player direction would give the fisheye effect)
				if (side == 0)
					perpWallDist = sideDistX - deltaDistX;
				else
					perpWallDist = sideDistY - deltaDistY;

				//Calculate the height of the wall that is to be drawn
				int lineHeight = (int)((mapHeight * 2) / perpWallDist);

				// Calculate the lowest and highest pixel to fill in current stripe
				int drawStart = (-lineHeight / 2) + (mapHeight / 2);

				if (drawStart < 0)
					drawStart = 0;

				int drawEnd = (lineHeight / 2) + (mapHeight / 2);

				if (drawEnd >= mapHeight)
					drawEnd = mapHeight - 1;

				// Choose wall color (darker colors if a y-side was hit for a nicer effect)
				sf::Color color;

				switch (map[mapX][mapY])
				{
				case 1:
					color = sf::Color::Red;
					break;
				case 2:
					color = sf::Color::Green;
					break;
				case 3:
					color = sf::Color::Blue;
					break;
				case 4:
					color = sf::Color::White;
					break;
				default:
					color = sf::Color::Yellow;
					break;
				}

				if (side == 1)
					color = (sf::Color)(color.toInteger() / 2);

				for (unsigned short y = 1; y <= 26; y++)
				{
					// Draw the pixels of the stripes as vertical lines
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(x * y, drawStart * lineHeight)),
						sf::Vertex(sf::Vector2f(x * y, drawEnd * lineHeight))
					};

					line->color = color;
					//window->draw(line, 2, sf::Lines);
				}
				

				sf::RectangleShape line2;
				line2.setPosition(x * 8, drawStart * lineHeight);
				line2.setSize(sf::Vector2f(x * 8, drawEnd * lineHeight));
				line2.setScale(sf::Vector2f(1.0, 1.0));
				line2.setFillColor(color);
				/*line2.setOutlineColor(sf::Color::Black);
				line2.setOutlineThickness(3.0);*/

				
				window->draw(line2);

			}

			// Timing for input and FPS counter
			oldTime = time;
			time = clock.getElapsedTime();
			double frameTime = time.asSeconds() - oldTime.asSeconds();
			//std::cout << "FPS: " << 1.0 / frameTime << std::endl;

			window->display();
			window->clear();

			// Speed modifiers
			double movespeed = frameTime * 1.5;
			double rotSpeed =  frameTime * 30.0;

			// Input Keys
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
				else if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Up)
					{
						//std::cout << "Heeeeeeelllllllllooooooo" << std::endl;
						if (map[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
						if (map[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;

					}

					if (event.key.code == sf::Keyboard::Down)
					{
						if (map[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
						if (map[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
					}

					if (event.key.code == sf::Keyboard::Right)
					{
						//both camera direction and camera plane must be rotated
						double oldDirX = dirX;
						dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
						dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
						double oldPlaneX = planeX;
						planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
						planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
					}

					if (event.key.code == sf::Keyboard::Left)
					{
						//both camera direction and camera plane must be rotated
						double oldDirX = dirX;
						dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
						dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
						double oldPlaneX = planeX;
						planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
						planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
					}
				}
			}
			
		}

}


