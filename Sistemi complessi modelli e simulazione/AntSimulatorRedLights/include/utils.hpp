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
#include <SFML/Graphics.hpp>
#include <cmath>


constexpr float PI = 3.14159265f;


float getRandRange(float width);


float getRandUnder(float width);


template<typename T>
float getLength2(sf::Vector2<T> v)
{
	return v.x * v.x + v.y * v.y;
}


template<typename T>
float getLength(sf::Vector2<T> v)
{
	return sqrt(getLength2(v));
}

template<typename T>
sf::Vector2<T> getNormalized(sf::Vector2<T> v)
{
	return v / getLength(v);
}

template<typename T, typename U>
T to(const U& u)
{
	return static_cast<T>(u);
}


float getAngle(const sf::Vector2f& v);


float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);


float sign(const float f);
