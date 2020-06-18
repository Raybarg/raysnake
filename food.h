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

		std::unique_ptr<olc::Sprite> sprfood;

	public:
		Food(int size, olc::vi2d area, int);
		~Food();

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
		sprfood = std::make_unique<olc::Sprite>("./img/food.png");
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
				pge->DrawSprite(foods[i] * scale, sprfood.get());
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

	Food::~Food()
	{

	}
}

#endif RAYSNAKE_FOOD_H_
