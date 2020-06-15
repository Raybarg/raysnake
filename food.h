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

	public:
		Food(int size, olc::vi2d area);

		void add();
		bool collision(olc::vi2d head);

	};

	Food::Food(int size, olc::vi2d area)
		: foodsize{size}
		, foods{new olc::vi2d[size]}
		, gamearea{area}
	{
		for (int i = 0; i < foodsize; i++)
		{
			foods[i].x = -1;
			foods[i].y = -1;
		}
	}

	void Food::add()
	{
		olc::vi2d pos{ rand() % gamearea.x, rand() % gamearea.y };

		for (int i = 0; i < foodsize; i++)
		{
			if (foods[i].x >= 0)
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
				break;
			}
		}
		return found;
	}
}

#endif RAYSNAKE_FOOD_H_