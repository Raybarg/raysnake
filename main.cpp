#define OLC_PGE_APPLICATION
#include <memory>
#include "olcPixelGameEngine.h"
#include "snake.h"

#define GAME_SCALE 8
#define GAME_WIDTH 130
#define GAME_HEIGHT 80
#define SNAKE_SIZE GAME_WIDTH * GAME_HEIGHT

class Example : public olc::PixelGameEngine
{
private:
	enum class gamestate {Menu, Game};
	float FrameTime = 1.0f / 20.0f; // Virtual FPS of 100fps
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
		snake = new raysnake::Snake(SNAKE_SIZE, olc::vi2d{ GAME_WIDTH, GAME_HEIGHT }, GAME_SCALE);

		btnPlay = std::make_unique<olc::Sprite>("./img/play_button.png");
		btnPlayHover = std::make_unique<olc::Sprite>("./img/play_button_hover.png"); 
		btnPlayPressed = std::make_unique<olc::Sprite>("./img/play_button_pressed.png");	
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(bgc);
		// Draw and move snake
		snake->draw(this);

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

				DrawRect(olc::vi2d{ 0,0 }, olc::vi2d{ GAME_WIDTH * GAME_SCALE - 1, GAME_HEIGHT * GAME_SCALE - 1 }, olc::DARK_GREEN);
				DrawRect(olc::vi2d{ GAME_SCALE, GAME_SCALE }, olc::vi2d{ (GAME_WIDTH - 1) * GAME_SCALE - GAME_SCALE, (GAME_HEIGHT - 1) * GAME_SCALE - GAME_SCALE }, olc::DARK_GREEN);

				if (rand() % 10 <= 1)
					snake->foods->add();

				if (snake->move())
				{
					state = gamestate::Menu;
					snake->reset();
				}
			}

			break;
		case gamestate::Menu:
			SetPixelMode(olc::Pixel::ALPHA);
			FillRect(olc::vi2d{ 0,0 }, olc::vi2d{ GAME_WIDTH * GAME_SCALE - 1, GAME_HEIGHT * GAME_SCALE - 1 }, olc::GREY);
			SetPixelMode(olc::Pixel::NORMAL);

			olc::vi2d btnPos = olc::vi2d{ (GAME_WIDTH * GAME_SCALE) / 2 - 50, 150 };

			SetPixelMode(olc::Pixel::MASK);

			MyDrawString(olc::vi2d{ GAME_WIDTH, 30 }, "Raybarg's Snake", olc::DARK_GREEN, GAME_SCALE);

			if (GetMouseX() >= ((GAME_WIDTH * GAME_SCALE) / 2 - 50) && GetMouseX() <= ((GAME_WIDTH * GAME_SCALE) / 2 + 50) &&
				GetMouseY() >= 150 && GetMouseY() <= 200)
			{
				if (GetMouse(0).bHeld)
				{
					DrawSprite(btnPos, btnPlayPressed.get());
				}
				else
				{
					if (GetMouse(0).bReleased) {
						state = gamestate::Game;
					}
					else
					{
						DrawSprite(btnPos, btnPlayHover.get());
					}
				}
			}
			else
			{
				DrawSprite(btnPos, btnPlay.get());
			}

			break;
		}
		return true;
	}

	void MyDrawString(olc::vi2d pos, std::string str, olc::Pixel color, int scale)
	{
		int length = str.length();
		olc::vi2d txtPos = { pos.x - length * GAME_SCALE, pos.y - GAME_SCALE };
		DrawString(txtPos, str, color, scale);
	}

};

int main()
{
	Example demo;
	if (demo.Construct(GAME_WIDTH * GAME_SCALE, GAME_HEIGHT * GAME_SCALE, 1, 1))
		demo.Start();
	return 0;
}

