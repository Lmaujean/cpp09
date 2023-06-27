#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {
	this->Database("data.csv");
	return ; 
}

BitcoinExchange::~BitcoinExchange(){
    return ;
}

void	BitcoinExchange::_importDatabase(std::string const& filename){
    std::string line;

    std::ifstream ifs(filename); // ifstream est une classe qui représente une entrée de fichier

    if (!ifs.is_open()){ // verifie l'ouverture du fichier
        std::cout << "Error could not open the database file" << std::endl;
        return ;
    }
    if (!std::getline(ifs, line)){ // je stock le contenue de ifs dans line
        std::cout << "Error while reading database header" << std::endl;
        return ;
    }
    if (line.compare("date,exchange_rate") != 0){ // verifie si les 2 chaine de caractere line et "date,exchange_rate" sont identique
        std::cout << "Error: wrong database header" << std::endl;
        return ;
    }
    while (std::getline(ifs, line)) //  lit chaque ligne du fichier de ifs et permet de traiter chaque ligne de données
		this->DataLine(line);
    if (ifs.bad()){ // cette instruction vérifie si une erreur de lecture s'est produite
        std::cout << "Error while reading database" << std::endl;
        return ;
    }
    if (this->exchange.size() == 0){ // Cette instruction vérifie si la taille de la structure de données interne
        std::cout << "Error: database is empty" << std::endl;
        return ;
    }
    return ;
}

void BitcoinExchange::_processDataLine(std::string const& line)
{
    std::string date;
    std::string valueStr;
    float value;
    size_t indComma;
    char* endptr = NULL;

   
    indComma = line.find_first_of(",");  // trouver la position de la première virgule dans la ligne
    
    if (indComma == std::string::npos || indComma == line.size() - 1) { // vérifier si aucune virgule n'est présente ou si la virgule est à la fin de la ligne
        std::cout << "Error: wrong entry in database" << std::endl;
        return;
    }

   
    date = line.substr(0, indComma);  // extraire la partie de la ligne avant la virgule et la stocker dans la variable "date"

    try {
        // vérifier si la "date" extraite est valide
        this->DateValid(date);
    }
    catch (std::exception const& e) {
        // s'il y a une exception lors de la validation de la "date", ajouter l'erreur à un message d'erreur et la lancer
        std::string err("Error: wrong entry in database\n");
        err.append(e.what());
        throw std::invalid_argument(err);
    }

    valueStr = line.substr(indComma + 1); // extraire la partie de la ligne après la virgule et la stocker dans la variable "valueStr"

    value = strtof(valueStr.c_str(), &endptr);  // convertir la chaîne de caractères "valueStr" en valeur de type "float" en utilisant la fonction strtof, la variable "endptr" est utilisée pour vérifier si la conversion a réussi

    if (*endptr) { // vérifier si la conversion a échoué (c'est-à-dire si des caractères non numériques ont été trouvés)
        std::cout << "Error: wrong entry in database" << std::endl;
        return;
    }

    if (value < 0 || value == HUGE_VALF) { // vérifier si la valeur est négative ou égale à HUGE_VALF (une valeur spéciale pour l'infini)
        std::cout << "Error: wrong entry in database" << std::endl;
        return;
    }

    this->exchange.insert(std::pair<std::string, float>(date, value)); // insérer la paire (date, value) dans la structure de données "exchange" de la classe

    return;
}

float BitcoinExchange::getPrice(std::string const& date) const
{
    this->DateValid(date); // appelle la fonction DateValid pour valider la date passée en argument

    std::map<std::string,float>::const_iterator itup; // déclare un itérateur constant pour parcourir la structure de données exchange de type std::map<std::string,float>
    itup = this->exchange.upper_bound(date); // recherche dans exchange l'élément suivant immédiatement après la date spécifiée. La fonction upper_bound renvoie un itérateur pointant vers cet élément

    if (itup == this->exchange.begin()) { // vérifie si l'itérateur itup est égal au début de la structure de données exchange. Si c'est le cas, cela signifie que la date spécifiée est antérieure à la plus ancienne date dans la base de données
        std::cout << "Error: input date is earlier than database" << std::endl;
        return;
    }

    itup--; // décrémente l'itérateur itup pour pointer vers l'élément précédent dans la structure de données exchange.
    return (itup->second); // retourne la valeur associée à la clé (date) de l'élément pointé par l'itérateur itup.
}

void BitcoinExchange::DateValid(std::string const& date) const
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') // vérifie si la taille de la chaîne date est différente de 10 caractères et si les caractères aux positions 4 et 7 sont des tirets ("-")
        throw std::invalid_argument(std::string("Error: bad input => ").append(date)); // si une de ces conditions n'est pas satisfaite, lance une exception std::invalid_argument avec un message d'erreur

    struct tm tm;
    memset(&tm, 0, sizeof(tm)); // initialise la structure tm avec des zéros.
    char* endptr = NULL;

    endptr = strptime(date.c_str(), "%Y-%m-%d", &tm); // convertit la chaîne de caractères date en une structure tm en utilisant le format spécifié "%Y-%m-%d"
    if (endptr == NULL || *endptr != 0) // vérifie si la conversion a échoué (endptr est NULL) ou si des caractères supplémentaires existent après la date convertie (*endptr != 0)
        throw std::invalid_argument(std::string("Error: bad input => ").append(date));

    if (tm.tm_year < (2009 - 1900)) // vérifie si l'année dans la structure tm est antérieure à 2009
        throw std::invalid_argument(std::string("Error: btc value starts in 2009 => ").append(date));

    time_t now = time(0); // obtient l'heure actuelle sous forme de time_t
    time_t etime = mktime(&tm); // convertit la structure tm en un time_t représentant la date et l'heure

    if (etime > now) // vérifie si le time_t de la date convertie est supérieur au time_t de l'heure actuelle
        throw std::invalid_argument(std::string("Error: this program doesn't make future prediction => ").append(date)); // lance une exception std::invalid_argument avec un message d'erreur

    return;
}