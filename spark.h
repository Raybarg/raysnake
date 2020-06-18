#pragma once

#ifndef RAYSNAKE_SPARK_H_
#define RAYSNAKE_SPARK_H_

#include <cstdlib>
#include "olcPixelGameEngine.h"

namespace raysnake
{
	class Spark
	{
	private:
		bool alive;
		int lifetime;
		int life;
		int style;
		olc::vf2d location;
		olc::vf2d direction;
	public:
		Spark();
		Spark(olc::vi2d, int, int);

		void set(olc::vi2d, int, int);
		void draw(olc::PixelGameEngine* pge);
		bool isalive();
	};

	Spark::Spark()
	{
		alive = false;
		lifetime = 0;
		life = 0;
		style = 0;
		location = olc::vf2d{ -1, -1 };
		direction = olc::vf2d{ 0, 0 };
	}

	Spark::Spark(olc::vi2d loc, int scale, int _style)
	{
		set(loc, scale, _style);
	}

	void Spark::set(olc::vi2d loc, int scale, int _style)
	{
		style = _style;
		alive = true;
		lifetime = ((int)rand() % 300) + 120;
		life = 0;
		location.x = float(loc.x * scale) + 6;
		location.y = float(loc.y * scale) + 6;

		float angle = (float)(rand() % 360);
		float speed = (float)((rand() % 10) + 1) / 25;

		direction.x = speed * cosf(angle);
		direction.y = speed * sinf(angle);
	}

	void Spark::draw(olc::PixelGameEngine* pge)
	{
		if (alive)
		{
			location += direction;
			life++;
			if (life >= lifetime)
				alive = false;
			
			float lifetreshold = (255 - ((float)life / (float)lifetime)) * (float)255.0;
			olc::Pixel clr = olc::Pixel(128 + int(lifetreshold / 2), int(lifetreshold), int(lifetreshold)); // when style = 0
			if (style == 1)
			{
				clr = olc::Pixel(255, int(lifetreshold), int(lifetreshold));
			}
			


			pge->Draw(location, clr);
		}
	}

	bool Spark::isalive()
	{
		return alive;
	}
}

#endif // RAYSNAKE_SPARK_H_