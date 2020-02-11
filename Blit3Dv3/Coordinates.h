#pragma once
#define NULL 0

class Coords
{
public:

	short x, y;

	Coords()
	{
		x = NULL;
		y = NULL;
	}

	Coords(short positionX, short positionY)
	{
		x = positionX;
		y = positionY;
	}
};