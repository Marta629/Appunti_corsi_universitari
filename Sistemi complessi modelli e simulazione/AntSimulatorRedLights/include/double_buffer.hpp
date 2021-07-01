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

template<typename T>
struct DoubleObject
{
	template<typename... Args>
	DoubleObject(Args&&... args)
		: current_buffer(0u)
	{
		buffers[0] = T(args...);
		buffers[1] = T(args...);
	}

	void swap()
	{
		current_buffer = !current_buffer;
	}

	T& getCurrent()
	{
		return buffers[current_buffer];
	}

	T& getLast()
	{
		return buffers[!current_buffer];
	}

	const T& getCurrent() const
	{
		return buffers[current_buffer];
	}

	const T& getLast() const
	{
		return buffers[!current_buffer];
	}

	uint8_t current_buffer;
	T buffers[2];
};


