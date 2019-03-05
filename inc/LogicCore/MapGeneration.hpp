/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapGeneration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liudmila <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 13:53:30 by lberezyn          #+#    #+#             */
/*   Updated: 2019/03/03 14:16:52 by liudmila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBERMAN_MAPGENERATION_H
#define BOMBERMAN_MAPGENERATION_H

#include <LogicCore/SquareInstance.h>
#include <vector>

class MapGenerator
{
public:
	MapGenerator(int hardness);
	MapGenerator(int width, int height, int hardness);
	~MapGenerator();
	std::vector<SquareType> GenerateNewMap();
	int GetWidth() const;
	int GetHeight() const;

private:
	int mHardness = 0;
	int mWidth = 30;
	int mHeight = 20;
};


#endif //BOMBERMAN_MAPGENERATION_H
