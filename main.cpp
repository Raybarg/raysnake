#define OLC_PGE_APPLICATION
#include <memory>
#include "olcPixelGameEngine.h"
#include "snake.h"

#define SNAKE_SIZE 200
#define GAME_WIDTH 200
#define GAME_HEIGHT 160

class Example : public olc::PixelGameEngine
{
private:
	enum class gamestate {Menu, Game};
	float FrameTime = 1.0f / 50.0f; // Virtual FPS of 100fps
	float AccumulatedTime = 0.0f;
	olc::Pixel bgc = olc::BLACK;
	std::unique_ptr<olc::Sprite> btnPlay;
	std::unique_ptr<olc::Sprite> btnPlayHover;
	std::unique_ptr<olc::Sprite> btnPlayPressed;
	gamestate state = gamestate::Menu;

public:
	Example()
	{
		sAppName = "japa snake";
	}

public:

	raysnake::Snake* snake;


public:
	bool OnUserCreate() override
	{
		snake = new raysnake::Snake(SNAKE_SIZE, olc::vi2d{ GAME_WIDTH, GAME_HEIGHT });

		btnPlay = std::make_unique<olc::Sprite>("./img/play_button.png");
		btnPlayHover = std::make_unique<olc::Sprite>("./img/play_button_hover.png"); 
		btnPlayPressed = std::make_unique<olc::Sprite>("./img/play_button_pressed.png");	
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		switch (state)
		{
		case gamestate::Game:
			// Handle snake input
			snake->handleinput(this);

			// Choose if we want to do anything this frame, running at FrameTime fps
			AccumulatedTime += fElapsedTime;
			if (AccumulatedTime >= FrameTime)
			{
				AccumulatedTime -= FrameTime;
				fElapsedTime = FrameTime;
			}
			else
				return true; // Don't do anything this frame

			Clear(bgc);

			// Draw and move snake
			snake->draw(this);

			if (snake->move())
			{
				bgc = olc::RED;
			}

			break;
		case gamestate::Menu:
			Clear(bgc);
			SetPixelMode(olc::Pixel::MASK);

			if (GetMouseX() >= 50 && GetMouseX() <= 150 &&
				GetMouseY() >= 50 && GetMouseY() <= 100)
			{
				if (GetMouse(0).bHeld)
				{
					DrawSprite(olc::vi2d{ 50, 50 }, btnPlayPressed.get());
				}
				else
				{
					if (GetMouse(0).bReleased) {
						state = gamestate::Game;
					}
					else
					{
						DrawSprite(olc::vi2d{ 50, 50 }, btnPlayHover.get());
					}
				}

			}
			else
			{
				DrawSprite(olc::vi2d{ 50, 50 }, btnPlay.get());
			}

			break;
		}



		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(GAME_WIDTH, GAME_HEIGHT, 2, 2))
		demo.Start();
	return 0;
}

