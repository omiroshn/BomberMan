/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 23:37:00 by vkozlov           #+#    #+#             */
/*   Updated: 2018/06/27 23:37:00 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOMEXCEPTION_H
# define CUSTOMEXCEPTION_H

#include <exception>
#include <string>

class CustomException : public std::exception
{
public:
	CustomException(std::string const &err);
	virtual const char* what() const throw();
	~CustomException() throw();
	CustomException();
	CustomException(CustomException const &);
	std::string text;
	CustomException &operator=(CustomException const &);
};

#endif