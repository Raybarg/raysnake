#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "snake.h"

#define SNAKE_SIZE 200
#define GAME_WIDTH 200
#define GAME_HEIGHT 160

class Example : public olc::PixelGameEngine
{
private:
	float FrameTime = 1.0f / 50.0f; // Virtual FPS of 100fps
	float AccumulatedTime = 0.0f;
	olc::Pixel bgc = olc::BLACK;

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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

		// Clear screen
		FillRect(olc::vi2d{ 0,0 }, olc::vi2d{ 200,160 }, bgc);

		// Draw and move snake
		snake->draw(this);
		
		if (snake->move()) 
		{
			bgc = olc::RED;
		}

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(GAME_WIDTH, GAME_HEIGHT, 4, 4))
		demo.Start();
	return 0;
}

