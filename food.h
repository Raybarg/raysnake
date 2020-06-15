#pragma once

#ifndef RAYSNAKE_FOOD_H_
#define RAYSNAKE_FOOD_H_

#include <cstdlib>
#include <memory>
#include "olcPixelGameEngine.h"

namespace raysnake
{
	class Food
	{
	private:
		int foodsize;
		olc::vi2d gamearea;
		std::unique_ptr<olc::vi2d[]> foods;
		int scale;

	public:
		Food(int size, olc::vi2d area, int);

		void add();
		bool collision(olc::vi2d head);
		void draw(olc::PixelGameEngine*);
		void reset();
	};

	Food::Food(int size, olc::vi2d area, int gamescale)
		: foodsize{size}
		, foods{new olc::vi2d[size]}
		, gamearea{area}
		, scale{ gamescale }
	{
		reset();
	}

	void Food::add()
	{
		olc::vi2d pos{ (rand() % (gamearea.x - 2)) + 1, (rand() % (gamearea.y -2)) + 1 };

		for (int i = 0; i < foodsize; i++)
		{
			if (foods[i].x == -1)
			{
				foods[i] = pos;
				break;
			}
		}
	}

	bool Food::collision(olc::vi2d head)
	{
		bool found = false;

		for (int i = 0; i < foodsize; i++)
		{
			if (foods[i].x == head.x && foods[i].y == head.y)
			{
				found = true;
				foods[i].x = -1;
				foods[i].y = -1;
				break;
			}
		}
		return found;
	}

	void Food::draw(olc::PixelGameEngine* pge)
	{
		for (int i = 0; i < foodsize; i++)
		{
			if (foods[i].x > -1)
			{
				pge->FillRect(foods[i] * scale, olc::vi2d{ scale, scale }, olc::GREEN);
			}
		}
	}

	void Food::reset()
	{
		for (int i = 0; i < foodsize; i++)
		{
			foods[i].x = -1;
			foods[i].y = -1;
		}
	}
}

#endif RAYSNAKE_FOOD_H_