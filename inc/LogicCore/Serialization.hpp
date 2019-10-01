/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialization.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <lberezyn@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 14:01:17 by lberezyn          #+#    #+#             */
/*   Updated: 2019/02/09 14:01:17 by lberezyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBERMAN_SERIALIZATION_H
#define BOMBERMAN_SERIALIZATION_H
#include "LogicCore/MapGeneration.hpp"

class Serialization
{
    public:
        Serialization();
        ~Serialization();
        Serialization(Serialization const&) = delete;
        Serialization& operator=(Serialization const&) = delete;
        std::vector<SquareType> LoadCampaignMap(int stage, unsigned& outWidth);
};

#endif //BOMBERMAN_SERIALIZATION_H
