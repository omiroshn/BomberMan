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
#include "ResourceManagement/ResourceManager.hpp"
#include <algorithm>

Serialization::Serialization()
{

}

Serialization::~Serialization()
{

}

std::vector<SquareType> Serialization::LoadCampaignMap(int stage, unsigned& outWidth)
{
    std::string map_name = "map" + std::to_string(stage) + ".txt";
    std::vector<SquareType> map;
    map.reserve(400);
    std::string line;
    std::ifstream f{RESOURCES.getMap(map_name)};
	unsigned width = 0;
    while (std::getline(f, line))
    {
        unsigned lineSize = (unsigned)line.size();
        if (width && lineSize != width)
        {
            std::cerr << "map  " << map_name << " is mailformed\n";
            return map;
        }
        else if (!width)
        {
            width = lineSize;
        }
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] >= '0' && line[i] <= '9')
                map.push_back((SquareType)(line[i] - '0'));
            else 
                map.push_back(SquareType::EmptySquare);
        };
    }
	outWidth = width;
    return map;
}