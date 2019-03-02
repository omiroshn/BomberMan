/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapGeneration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liudmila <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 13:53:31 by lberezyn          #+#    #+#             */
/*   Updated: 2019/03/02 11:11:31 by liudmila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <LogicCore/MapGeneration.hpp>

#include "LogicCore/MapGeneration.hpp"
#include <iostream>

MapGenerator::MapGenerator(int hardness) :
	mHardness(hardness)
{

}

MapGenerator::MapGenerator(int width, int height, int hardness) :
	mWidth(width),
	mHeight(height),
	mHardness(hardness)
{

}

MapGenerator::~MapGenerator() {

}

std::vector<int> MapGenerator::GenerateNewMap()
{
	std::vector<int> map;
	int size = mWidth * mHeight;
	map.reserve(size);

	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
			if (j == 0 || j == mWidth - 1 || i == 0 || i == mHeight - 1)
				map.push_back((int)SquareType::Wall);
			else if (i % 2 != 0 && j % 2 == 0)
				map.push_back((int)SquareType::Wall);
			else if ((j < 3 && i < 3))
				map.push_back((int)SquareType::EmptySquare);
			else
			{
				if (rand() % 5 == 0)
					map.push_back((int)SquareType::Brick);
				else
					map.push_back((int)SquareType::EmptySquare);
			}

		}

	}
	return map;
}

int MapGenerator::GetWidth() const {
	return mWidth;
}

int MapGenerator::GetHeight() const {
	return mHeight;
}
