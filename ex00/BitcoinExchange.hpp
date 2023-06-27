#pragma once

# include <iostream>
# include <string>
# include <map>
# include <fstream>
# include <ctime>
# include <cmath>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		~BitcoinExchange();
		
		float	getPrice(std::string const&) const;

	private:
		BitcoinExchange(BitcoinExchange const &rhs);
		BitcoinExchange&	operator=(BitcoinExchange const &rhs);
		
		void	Database(std::string const&);
		void	DataLine(std::string const&);
		void	DateValid(std::string const& date) const;

		std::map<std::string, float>    exchange;

};