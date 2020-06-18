#pragma once

#ifndef RAYSNAKE_SNAKE_H_
#define RAYSNAKE_SNAKE_H_

#define MAXSPARKS 1000

#include <cstdlib>
#include <memory>
#include "olcPixelGameEngine.h"
#include "food.h"
#include "spark.h"

namespace raysnake
{
	class Snake
	{
	public:
		enum class Snake_Direction { Up, Down, Left, Right };
		std::unique_ptr<raysnake::Food> foods;

	private:
		int snakesize;
		std::unique_ptr<olc::vi2d[]> body;
		int scale;

		std::unique_ptr<olc::Sprite> sprhead;
		std::unique_ptr<olc::Sprite> sprbody;

		raysnake::Spark sparks[MAXSPARKS];

		olc::vi2d gamearea;
		Snake_Direction direction;
		int tail;
		int head;

	public:
		Snake(int size, olc::vi2d, int scale);

		bool testmode;

		void handleinput(olc::PixelGameEngine*, bool*);
		void draw(olc::PixelGameEngine*);
		int move();
		void reset();
		void makeitspark(int);
	};

	Snake::Snake(int size, olc::vi2d area, int gamescale)
		: snakesize{size}
		, body{new olc::vi2d[size]}
		, gamearea{area}
		, foods{new raysnake::Food(size, area, gamescale)}
		, scale{gamescale}
	{
		testmode = false;

		sprhead = std::make_unique<olc::Sprite>("./img/head.png");
		sprbody = std::make_unique<olc::Sprite>("./img/body.png");

		reset();
	}

	void Snake::handleinput(olc::PixelGameEngine* pge, bool* changed)
	{
		// Set new direction based on input and prevent going against the direction we are coming from
		if (pge->GetKey(olc::DOWN).bPressed)
		{
			if (direction != Snake_Direction::Up)
			{
				direction = Snake_Direction::Down;
				*changed = true;
			}
		}
		else if (pge->GetKey(olc::UP).bPressed)
		{
			if (direction != Snake_Direction::Down)
			{
				direction = Snake_Direction::Up;
				*changed = true;
			}
		}
		else if (pge->GetKey(olc::LEFT).bPressed)
		{
			if (direction != Snake_Direction::Right)
			{
				direction = Snake_Direction::Left;
				*changed = true;
			}
		}
		else if (pge->GetKey(olc::RIGHT).bPressed)
		{
			if (direction != Snake_Direction::Left)
			{
				direction = Snake_Direction::Right;
				*changed = true;
			}
		}
	}

	void Snake::draw(olc::PixelGameEngine* pge)
	{
		olc::vi2d drawPos;
		int head_tmp = head;

		while(true)
		{
			drawPos = body[head_tmp] * scale;
			if (head_tmp == head)
			{
				pge->DrawSprite(drawPos, sprhead.get());
			}
			else
			{
				pge->DrawSprite(drawPos, sprbody.get());
			}
			if (head_tmp == tail) break;
			head_tmp++;
			if (head_tmp > (snakesize-1)) head_tmp = 0;
		}
		foods->draw(pge);

		// Draw sparks
		for (int i = 0; i < MAXSPARKS; i++)
			sparks[i].draw(pge);
	}

	int Snake::move()
	{
		int retval = 0;
		int i = 0;
		olc::vi2d temp = body[head];
		switch (direction)
		{
		case Snake_Direction::Up:
			temp.y--;
			break;
		case Snake_Direction::Down:
			temp.y++;
			break;
		case Snake_Direction::Left:
			temp.x--;
			break;
		case Snake_Direction::Right:
			temp.x++;
			break;
		}

		// Body collision
		for (i = 0; i < snakesize; i++)
		{
			if ((body[i].x == temp.x) && (body[i].y == temp.y) && (head != i) && (body[i].x > -1))
				if (testmode)
					makeitspark(1);
				else
					retval = 1;
		}

		if (!foods->collision(body[head]))
		{
			body[tail].x = -1;
			body[tail].y = -1;
			tail--;

		}
		else
		{
			makeitspark(0);
			retval = 2;
		}

		head--;
		if (head < 0) head += snakesize;
		body[head] = temp;
		if (tail < 0) tail += snakesize;

		// Area boundaries
		if (body[head].x > gamearea.x || body[head].y > gamearea.y ||
			body[head].x <= 0 || body[head].y <= 0)
		{
			retval = 1;
		}

		return retval;
	}

	void Snake::reset()
	{
		direction = Snake_Direction::Down;
		for (int i = 0; i < snakesize; i++)
			body[i] = olc::vi2d{ 0,0 };
		body[0] = olc::vi2d{ 20, 21 };
		body[1] = olc::vi2d{ 20, 20 };
		body[2] = olc::vi2d{ 20, 19 };
		body[3] = olc::vi2d{ 20, 18 };
		body[4] = olc::vi2d{ 20, 17 };
		head = 0;
		tail = 4;
		foods->reset();
	}

	void Snake::makeitspark(int style)
	{
		int nr = 0;
		for (int i = 0; i < MAXSPARKS; i++)
			if (!sparks[i].isalive())
			{
				sparks[i].set(body[head], scale, style);
				if (++nr > 100) break;
			}
	}
}

#endif // RAYSNAKE_SNAKE_H_