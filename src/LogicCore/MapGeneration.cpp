/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapGeneration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liudmila <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 13:53:31 by lberezyn          #+#    #+#             */
/*   Updated: 2019/03/02 13:24:57 by liudmila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/MapGeneration.hpp"
#include <iostream>

MapGenerator::MapGenerator(int )
{

}

MapGenerator::MapGenerator(int width, int height, int ) :
	mWidth(width),
	mHeight(height)
{

}

MapGenerator::~MapGenerator() {

}

std::vector<SquareType> MapGenerator::GenerateNewMap()
{
	std::vector<SquareType> map;
	int size = mWidth * mHeight;
	map.reserve(size);

	for (int i = 0; i < mHeight; ++i)
	{
		for (int j = 0; j < mWidth; ++j)
		{
            if (j == 0 || j == mWidth - 1 || i == 0 || i == mHeight - 1)
                map.push_back(SquareType::Wall);
            else if (i % 2 == 0 && j % 2 == 0)
                map.push_back(SquareType::Wall);
            else if ((j < 3 && i < 3))
                map.push_back(SquareType::EmptySquare);
            else
            {
                if (rand() % 5 == 0)
                    map.push_back(SquareType::Brick);
                else
                    map.push_back(SquareType::EmptySquare);
            }

		}

	}
	return map;
}

int MapGenerator::GetWidth() const {
	return mWidth;
}