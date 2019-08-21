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
    std::cout << map_name << std::endl;
    std::fstream f{RESOURCES.getMap(map_name)};
    while (std::getline(f, line))
    {
        outWidth = std::max(outWidth, (unsigned)line.size());
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] >= '0' && line[i] <= '9')
                map.push_back((SquareType)(line[i] - '0'));
            else if(line[i] >= 'a' && line[i] <= 'z')
                map.push_back((SquareType)(line[i] - 'a' + 10));
            else 
                map.push_back(SquareType::EmptySquare);
        };
        //map.push_back(SquareType::Wall);
    }
    return map;
}