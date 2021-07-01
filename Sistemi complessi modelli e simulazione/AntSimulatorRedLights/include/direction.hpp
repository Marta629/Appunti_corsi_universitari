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
#include <SFML/System.hpp>
#include "utils.hpp"


struct Direction
{
public:
	Direction(float angle, float rotation_speed = 10.0f)
		: m_angle(angle)
		, m_target_angle(angle)
		, m_rotation_speed(rotation_speed)
	{
		updateVec();
		m_target_vec = m_vec;
	}

	void update(float dt)
	{
		updateVec();

		const sf::Vector2f dir_nrm(-m_vec.y, m_vec.x);

		const float dir_delta = dot(m_target_vec, dir_nrm);
		const float rotation_speed = 10.0f;
		m_angle += rotation_speed * dir_delta * dt;
	}

	sf::Vector2f getVec() const
	{
		return m_vec;
	}

	float getCurrentAngle() const
	{
		return getAngle(m_target_vec);
	}

	void operator+=(float a)
	{
		m_target_angle += a;
		updateTargetVec();
	}

	void operator=(float a)
	{
		m_target_angle = a;
		updateTargetVec();
	}

	void addNow(float a)
	{
		this->operator+=(a);
		m_angle = m_target_angle;
		updateVec();
	}

	void setDirectionNow(sf::Vector2f d)
	{
		m_target_vec = d;
		m_vec = d;
		m_angle = getAngle(d);
		m_target_angle = m_angle;
	}


private:
	sf::Vector2f m_vec;
	sf::Vector2f m_target_vec;
	float m_angle;
	float m_target_angle;
	float m_rotation_speed;

	void updateVec()
	{
		m_vec.x = cos(m_angle);
		m_vec.y = sin(m_angle);
	}

	void updateTargetVec()
	{
		m_target_vec.x = cos(m_target_angle);
		m_target_vec.y = sin(m_target_angle);
	}
};
