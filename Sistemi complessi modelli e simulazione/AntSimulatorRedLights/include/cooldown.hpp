/*
MIT License

Copyright (c) 2021 Jean Tampon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#pragma once

struct Cooldown
{
	float target;
	float value;

	Cooldown()
		: target(0.0f)
		, value(0.0f)
	{}

	Cooldown(float t)
		: target(t)
		, value(0.0f)
	{}

	Cooldown(float t, float v)
		: target(t)
		, value(v)
	{}

	void update(float dt)
	{
		value += dt;
	}

	bool ready() const
	{
		return value >= target;
	}

	bool readyNext(float dt) const
	{
		return value < target && value + dt >= target;
	}

	void reset()
	{
		value = 0.0f;
	}
};
