#include <iostream>
#include <cstdlib>
#include <cmath>
#include "BitcoinExchange.hpp"

void	processInputRequest(std::string const& filename, BitcoinExchange const& btx);

void	processRequestLine(std::string const& line, BitcoinExchange const& btx);

void	getDateAndAmount(std::string const& line, std::string& date, float& amountf, int& amounti, bool& isFloat);

int	main(int ac, char**av)
{
	if (ac != 2 || *(av[1]) == '\0')
	{
		std::cout << "Error: need one filename as input" << std::endl;
		exit(EXIT_FAILURE);
	}	
	try
	{
		BitcoinExchange	btx;
		processInputRequest(av[1], btx);
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}

void	processInputRequest(std::string const& filename, BitcoinExchange const& btx)
{
	std::string line;
	
	std::ifstream	ifs(filename);
    if (!ifs.is_open()) 
        throw std::invalid_argument("Error: could not open the input file");
	if (!std::getline(ifs, line))
		throw	std::invalid_argument("Error while reading input header.");
	if (line.compare("date | value") != 0)
		throw	std::invalid_argument("Error: wrong input header");
	while (std::getline(ifs, line))
		processRequestLine(line, btx);
	if (ifs.bad())
		throw	std::invalid_argument("Error while reading input file");
	return ;
}

void	processRequestLine(std::string const& line, BitcoinExchange const& btx)
{
	std::string	date;
	float	amountf;
	int		amounti;
	bool	isFloat;
	float	price;

	try
	{
		getDateAndAmount(line, date, amountf, amounti, isFloat);
		price = btx.getPrice(date);
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
	if (isFloat)
		std::cout << date << " => " << amountf << " = " << price * amountf << std::endl;
	else
		std::cout << date << " => " << amounti << " = " << price * amounti << std::endl;
	return ;
}

void	getDateAndAmount(std::string const& line, std::string& date,
	float& amountf, int& amounti, bool& isFloat)
{
	if (line.find_first_of('|') != 11 || line[10] != ' ' || line[12] != ' ')
		throw std::invalid_argument(std::string("Error: bad input => ").append(line));
	date = line.substr(0, 10);
	
	std::string	amount;
	amount = line.substr(13);
	char *endptr = NULL;

	if (amount.find_first_of('.') != std::string::npos)
	{
		amountf = strtof(amount.c_str(), &endptr);
		if (*endptr)
			throw std::invalid_argument(std::string("Error: bad input => ").append(line));
		else if (amountf < 0)
			throw std::invalid_argument("Error: not a positive number.");
		else if (amountf > 1000.0f)
			throw std::invalid_argument("Error: too large a (float) number.");
		isFloat = true;
	}
	else
	{
		long tmp = strtol(amount.c_str(), &endptr, 10);
		if (*endptr)
			throw std::invalid_argument(std::string("Error: bad input => ").append(line));
		else if (tmp < 0)
			throw std::invalid_argument("Error: not a positive number.");
		else if (tmp > 1000)
			throw std::invalid_argument("Error: too large a number.");
		amounti = (int)tmp;
		isFloat = false;
	}
	return ;
}