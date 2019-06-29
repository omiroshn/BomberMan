/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialization.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <lberezyn@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 14:01:18 by lberezyn          #+#    #+#             */
/*   Updated: 2019/02/09 14:01:18 by lberezyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/Serialization.hpp"
#include <iostream>
#include <fstream>

std::vector<SquareType> Serialization::LoadCampaignMap(int stage)
{
    std::vector<SquareType> map;
    map.reserve(400);
    std::string line;
    std::ifstream f ("map1.txt");
    if(std::getline(f, line))
    {
        for (int i = 0; i < line.size(); i++)
        {
            std::cout << "line[" <<i << "]" << line[i] << std::endl;
            if (line[i] == '0')
                map.push_back(SquareType::EmptySquare);
            else if (line[i] == '1')
                map.push_back(SquareType::Wall);
            else if (line[i] == '2')
                map.push_back(SquareType::Brick);
            else
                map.push_back(SquareType::EmptySquare);
        };
        map.push_back(SquareType::Wall);
        
            std::cout << "map size = " << map.size() << std::endl;
    }
    else{
            std::cout << "Not opened file" << stage << std::endl;

    }
    // for (int i = 0; i < mHeight; ++i)
	// {
	// 	for (int j = 0; j < mWidth; ++j)
	// 	{
    //         if (j == 0 || j == mWidth - 1 || i == 0 || i == mHeight - 1)
    //             map.push_back(SquareType::Wall);
    //         else if (i % 2 == 0 && j % 2 == 0)
    //             map.push_back(SquareType::Wall);
    //         else if ((j < 3 && i < 3))
    //             map.push_back(SquareType::EmptySquare);
    //         else
    //         {
    //             // if (rand() % 5 == 0)
    //             //     map.push_back(SquareType::Brick);
    //             // else
    //                 map.push_back(SquareType::EmptySquare);
    //         }

	// 	}

	// }


    return map;
}