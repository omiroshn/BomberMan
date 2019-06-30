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

std::vector<SquareType> Serialization::LoadCampaignMap(int stage)
{
    std::string map_name = "map" + std::to_string(stage) + ".txt";
    std::vector<SquareType> map;
    map.reserve(400);
    std::string line;
    std::cout << map_name << std::endl;
    std::fstream f{RESOURCES.getMap(map_name)};
    if(std::getline(f, line))
    {
        for (int i = 0; i < line.size(); i++)
        {
            //std::cout << "line[" <<i << "]" << line[i] << std::endl;
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
            map = LoadCampaignMap(1);

    }

    return map;
}