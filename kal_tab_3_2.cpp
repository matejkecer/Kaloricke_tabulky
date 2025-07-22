//============================================================================
// Name        : kal_tab.cpp
// Author      :	-
// Version     :	-
// Copyright   :	-
// Description : kalorické tabuľky
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

//=====================================================================
//database path
std::string DATABASE = "/home/matej/eclipse-workspace/kal_tab/src/dat.txt";
std::string HISTORY = "/home/matej/eclipse-workspace/kal_tab/src/history.txt";

//std::string DATABASE = "/data/data/com.termux/files/home/downloads/dat.txt";
//std::string HISTORY = "/data/data/com.termux/files/home/downloads/history.txt";
//=====================================================================


class Food {
// piece of food for database

protected:
	// attributes
	std::string name;
	double calories;
	double proteins;
	double carbs;
	double fats;
	std::string comment;

public:
	// constructors
	Food() {
		this->name = "";
		this->calories = 0;
		this->proteins = 0;
		this->carbs = 0;
		this->fats = 0;
		this->comment = "";
	}

	Food(std::string _name, double _cal, double _prot, double _carbs,
			double _fats, std::string _comment) {
		this->name = _name;
		this->calories = _cal;
		this->proteins = _prot;
		this->carbs = _carbs;
		this->fats = _fats;
		this->comment = _comment;
	}

	// getters
	std::string getName() const {
		return this->name;
	}
	double getCalories() const {
		return this->calories;
	}
	double getProteins() const {
		return this->proteins;
	}
	double getCarbs() const {
		return this->carbs;
	}
	double getFats() const {
		return this->fats;
	}
	std::string getComment() const {
		return this->comment;
	}

	// setters
	void setName(std::string _name) {
		this->name = _name;
		return;
	}
	void setCalories(double _cals) {
		this->calories = _cals;
		return;
	}
	void setProteins(double _prots) {
		this->proteins = _prots;
		return;
	}
	void setCarbs(double _carbs) {
		this->carbs = _carbs;
		return;
	}
	void setFats(double _fats) {
		this->fats = _fats;
		return;
	}
	void setComment(std::string _comment) {
		this->comment = _comment;
		return;
	}

	// print
	void print() const {
		std::cout << "{" << this->name << ", " << this->calories << ", "
				<< this->proteins << ", " << this->carbs << ", " << this->fats
				<< ", " << this->comment << "}" << std::endl;
		return;
	}

	// write
	std::string write() const {
		std::stringstream result;
		 result << "{" << this->name << ", " << this->calories << ", "
				<< this->proteins << ", " << this->carbs << ", " << this->fats
				<< ", " << this->comment << "}" << "\n";
		return result.str();
	}
	
	// add to database
	void catalog(std::string _file) const {

		std::fstream writer;

		writer.open(_file, std::fstream::app);

		writer << this->name << "\t" << this->calories << "\t" << this->proteins
				<< "\t" << this->carbs << "\t" << this->fats << "\t"
				<< this->comment;
		writer << '\n';
	}

	// overloading operators
	bool operator==(const Food &rhs) const {
		// compares only the name

		if (this->name.compare(rhs.getName()) != 0) {
			return false;
		}

		return true;
	}

};

int findFoodInDatabase(Food _food, std::vector<Food> _database) {

	for (int i = 0; i < _database.size(); i++) {
		if (_food == _database.at(i)) {
			return i;
		}
	}

	return -1;
}

Food constrFoodFromDatabase(std::string _name, std::vector<Food> _database) {
	// returns food with the same name as _name from the database, if there is no such entry
	// it then returns Food made with empty constructor

	Food result;
	bool found = false;

	for (int i = 0; i < _database.size(); i++) {
		if (_database.at(i).getName().compare(_name) == 0) {
			result = Food(_database.at(i).getName(),
					_database.at(i).getCalories(),
					_database.at(i).getProteins(), _database.at(i).getCarbs(),
					_database.at(i).getFats(), _database.at(i).getComment());
			found = true;
		}
	}

	if (!found) {
		std::cout
				<< "No such food in the database. Try adding it or type \"help\" to show current database."
				<< std::endl;
				
	}
	return result;
}

std::vector<Food> read_database(std::string _path) {
	// Function scans file in _path and constructs
	// database of Food objects corresponding to
	// scanned results.
	//
	// File is supposed to be in form:
	// ------------------------------------------
	// food_name \t calories \t proteins \t carbs \t fats \t #comment
	//etc.
	// ------------------------------------------

	std::vector<Food> result;

	std::string name = "";
	double cals = 0;
	double prot = 0;
	double carbs = 0;
	double fats = 0;
	std::string comment = "";

	std::ifstream inFile;
	inFile.open(_path);

	while (true) {

		inFile >> name;
		inFile >> cals;
		inFile >> prot;
		inFile >> carbs;
		inFile >> fats;
		inFile >> comment;

		if (inFile.eof()) {
			break;
		}

		result.push_back(Food(name, cals, prot, carbs, fats, comment));
	}

	return result;
}

void showHelp() {

	std::cout << "\"data\" - shows database" << "\n";
	std::cout << "\"add\" - add food to database" << "\n";
	std::cout<< "\"change\" - changes ammount of food already in the tally of the day" << "\n";
	std::cout << "\"order\" - changes order (id) of a food in the day" << "\n";
	std::cout << "\"show\" - shows foods eaten that day" << "\n";
	std::cout << "\"remove\" - removes food from the days tally" << "\n";
	std::cout << "\"end\" - concludes day and writes tally to history file"
			<< "\n";
			
	std::cout << std::endl;
	return;
}

void showDatabase(std::vector<Food> database) {

	for (int i = 0; i < database.size(); i++) {
		database.at(i).print();
	}
	
	//std::cout<< "\n";
	std::cout << std::endl;

	return;
}

void addToDatabase(std::vector<Food> database) {

	// if adding new food
	std::string nameOfNewFood = "";
	double newCals = 0;
	double newProt = 0;
	double newCarbs = 0;
	double newFats = 0;

	std::string newComment = "";

	std::cout << "Write name: ";
	std::cin >> nameOfNewFood;

	for (int i = 0; i < database.size(); i++) {
		if (nameOfNewFood.compare(database.at(i).getName()) == 0) {
			std::cout << "Such food is already in the database. Try again."
					<< std::endl;
			return;
		}
	}
	std::cout << "Write calories: ";
	std::cin >> newCals;

	std::cout << "Write proteins: ";
	std::cin >> newProt;

	std::cout << "Write carbs: ";
	std::cin >> newCarbs;

	std::cout << "Write fats: ";
	std::cin >> newFats;

	std::cout << "Write comment (without spaces) (optional): ";
	std::cin >> newComment;

	Food newFood(nameOfNewFood, newCals, newProt, newCarbs, newFats,
			"#" + newComment);
	newFood.catalog(DATABASE);

	//std::cout<< "\n";
	std::cout << std::endl;

	return;
}

void showBalance(double currentCals, double currentProt, double currentCarbs,
		double currentFats) {
	std::cout << "Calories: " << currentCals << " kcal" << "\n";
	std::cout << "Proteins: " << currentProt << " g" << "\n";
	std::cout << "Carbs: " << currentCarbs << " g" << "\n";
	std::cout << "Fats: " << currentFats << " g" << "\n";
}

void showDay(std::vector<Food> day, std::vector<double> portions) {

	for (int i = 0; i < day.size(); i++) {
		std::cout << "ID: " << i << "\t|  " << portions.at(i) << "\t| ";
		day.at(i).print();
	}
	//std::cout<< "\n";
	std::cout << std::endl;

	return;
}

void removeFromDay(std::vector<Food> &day, std::vector<double> &portions, int id) {

	std::vector<Food> newDay;
	std::vector<double> newPortions;
	
	for (int i = 0; i < day.size(); i++) {

		if (i == id) {
			continue;
		}

		newDay.push_back(day.at(i));
		newPortions.push_back(portions.at(i));
	}

	day = newDay;
	portions = newPortions;
	return;
}

void changeInDay(std::vector<Food> &day, std::vector<double> &portions, int id, double newAmmount) {

	std::vector<Food> newDay;
	std::vector<double> newPortions;
	
	for (int i = 0; i < day.size(); i++) {

		if (i == id) {
			newPortions.push_back(newAmmount);
		}else{
			newPortions.push_back(portions.at(i));
		}

		newDay.push_back(day.at(i));

	}

	day = newDay;
	portions = newPortions;
	return;
}

void reorderInDay(std::vector<Food> &day, std::vector<double> &portions, int id1, int id2) {

	std::vector<Food> newDay;
	std::vector<double> newPortions;
	Food foodToReorder = day.at(id1);
	double portionToReorder = portions.at(id1);
	int currentIndex = 0;
	
	if(id1==id2){
		return;
	}
	
	if(id2<id1){
		for (int i = 0; i < day.size(); i++) {
			
			if(i==id2){
			newDay.push_back(foodToReorder);
			newPortions.push_back(portionToReorder);
			}
		
			if (i == id1) {
				continue;
			}
		
			newDay.push_back(day.at(i));
			newPortions.push_back(portions.at(i));	
		}
	}else{
		for (int i = 0; i < day.size(); i++) {
			
			if (i == id1) {
				continue;
			}
			newDay.push_back(day.at(i));
			newPortions.push_back(portions.at(i));	
			currentIndex++;
			
			if(currentIndex==id2){
				newDay.push_back(foodToReorder);
				newPortions.push_back(portionToReorder);
			}
		}
		
	}
	
	
	
	day = newDay;
	portions = newPortions;
	return;
}

void addToHistory(std::vector<Food> day, std::vector<double> portions, std::string _file, double currentCals, double currentProt, double currentCarbs,
		double currentFats) {

	std::time_t t = std::time(0);   // Get the time now
	std::tm* now = std::localtime(&t);

	std::stringstream date;
	date << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'
			<< now->tm_mday;

	//newFood.catalog(DATABASE);
	std::fstream writer;

	writer.open(_file, std::fstream::app);

	writer << "---------------------------------------------"<< "\n";
	writer << "-------Date: " << date.str() << "-------" << "\n";

	writer << "Foods:" << "\n";
	for (int i = 0; i < day.size(); i++) {
		writer << portions.at(i) << "\t| " << day.at(i).write();
	}
	
	writer << "\n";

	writer << "-------End of the day balance-------" << "\n";
	writer << "Calories: " << currentCals << " kcal" << "\n";
	writer << "Proteins: " << currentProt << " g" << "\n";
	writer << "Carbs: " << currentCarbs << " g" << "\n";
	writer << "Fats: " << currentFats << " g" << "\n";

	writer << "---------------------------------------------" << "\n";
	writer << "\n";
	writer << "\n";
	return;
}

void baseProgram() {

	// read database
	std::vector<Food> database = read_database(DATABASE);

	std::vector<Food> day;
	std::vector<double> portions;

	// define variables
	std::string name = "";
	double ammount = 0;
	double newAmmount = 0;
	Food currentFood;
	Food aux;

	double currentCals = 0;
	double currentProt = 0;
	double currentCarbs = 0;
	double currentFats = 0;
	std::string input = "";

	// removing
	int toChangeId = -1;
	
	// reordering
	int newId = -1;
	
	bool goodInput = false;

	while (input.compare("end") != 0) {
		
		std::cout << "\n";
		std::cout << "Write your food; type \"help\" for help: \n" << std::endl;
		//read
		std::cin >> input;
		

		// show help
		if (input.compare("help") == 0){
			showHelp();
			continue;
		}

		// show database
		if (input.compare("data") == 0) {
			showDatabase(database);
			continue;
		}

		// add to database
		if (input.compare("add") == 0) {
			addToDatabase(database);
			database = read_database(DATABASE);
			continue;
		}

		// show days tally
		if (input.compare("show") == 0) {
			std::cout << "\n" << "-------Todays foods-------" << "\n";
			showDay(day, portions);
			std::cout << "-------Current balance-------" << "\n";
			showBalance(currentCals, currentProt, currentCarbs, currentFats);
			continue;
		}
		
		// change ammount of food in the days tally
		if (input.compare("change") == 0) {
			std::cout << "Write the ID of food you wish to change ammount of: ";
			
			// ID to change input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> toChangeId;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
					goodInput = true;
				}  
			}
			
			std::cout << "Old ammount is: " << portions.at(toChangeId) << ". Please write new ammount: ";
			
			// newAmmount input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> newAmmount;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
					goodInput = true;
				}  
			}
			
			
			
			currentFood = day.at(toChangeId);
			currentCals -= (currentFood.getCalories() * portions.at(toChangeId));
			currentProt -= (currentFood.getProteins() * portions.at(toChangeId));
			currentCarbs -= (currentFood.getCarbs() * portions.at(toChangeId));
			currentFats -= (currentFood.getFats() * portions.at(toChangeId));
			changeInDay(day, portions, toChangeId, newAmmount);
			currentCals += (currentFood.getCalories() * portions.at(toChangeId));
			currentProt += (currentFood.getProteins() * portions.at(toChangeId));
			currentCarbs += (currentFood.getCarbs() * portions.at(toChangeId));
			currentFats += (currentFood.getFats() * portions.at(toChangeId));
			
			toChangeId = -1;
			newAmmount = 0;
			
			std::cout << "Change sucessfull." << "\n";
			std::cout << "\n" << "-------Todays foods-------" << "\n";
			showDay(day, portions);
			std::cout << "-------Current balance-------" << "\n";
			showBalance(currentCals, currentProt, currentCarbs, currentFats);

			continue;
			
		}
		
		// reorder food in the day
		if (input.compare("order") == 0) {
			std::cout << "Write the ID of food you wish to reorder: ";
			
			// ID to change input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> toChangeId;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
					goodInput = true;
				}  
			}
			
			std::cout << "Write the desired ID of the food: ";
			// newId input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> newId;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
					goodInput = true;
				}  
			}
			
			reorderInDay(day, portions, toChangeId, newId);
			std::cout << "Reordering sucessfull." << "\n";
			std::cout << "\n" << "-------Todays foods-------" << "\n";
			showDay(day, portions);
			std::cout << "-------Current balance-------" << "\n";
			showBalance(currentCals, currentProt, currentCarbs, currentFats);
			
			toChangeId = -1;
			newId = -1;
			continue;
		}

		// remove from days tally
		if (input.compare("remove") == 0) {
			std::cout << "Write the ID of food you wish to remove: ";
			
			// ID to change input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> toChangeId;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
					goodInput = true;
				}  
			}

			currentFood = day.at(toChangeId);
			currentCals -= (currentFood.getCalories() * portions.at(toChangeId));
			currentProt -= (currentFood.getProteins() * portions.at(toChangeId));
			currentCarbs -= (currentFood.getCarbs() * portions.at(toChangeId));
			currentFats -= (currentFood.getFats() * portions.at(toChangeId));
			removeFromDay(day, portions, toChangeId);
			toChangeId = -1;

			std::cout << "Removal sucessfull." << "\n";
			std::cout << "\n" << "-------Todays foods-------" << "\n";
			showDay(day, portions);
			std::cout << "-------Current balance-------" << "\n";
			showBalance(currentCals, currentProt, currentCarbs, currentFats);

			continue;
		}

		if (input.compare("data") != 0 && input.compare("add") != 0
				&& input.compare("show") != 0 && input.compare("remove") != 0 && input.compare("change") != 0 && input.compare("order") != 0 && input.compare("end") != 0) {
			std::cout << "Write ammount: ";
			
			// ammount input with bad input handling
			goodInput = false;
			while(!goodInput){
				std::cin >> ammount;
		 		if(std::cin.fail()){
					std::cin.clear();
					std::cin.ignore();
					std::cout<<"Wrong input. Try again."<<std::endl;
				}else{
				
					if(ammount == 0){
						std::cout<<"Really? Zero? Type in normal ammount: "<<std::endl;
					}else{
						goodInput = true;
					}
				}  
			}
			//std::cout<<"ammount = "<< ammount;

			currentFood = constrFoodFromDatabase(input, database);
			currentCals += (currentFood.getCalories() * ammount);
			currentProt += (currentFood.getProteins() * ammount);
			currentCarbs += (currentFood.getCarbs() * ammount);
			currentFats += (currentFood.getFats() * ammount);

			// if there was input of food that is not in database, then empty food is being added -> add food only if not empty
			if(currentFood.getName().size()!=0){
				day.push_back(currentFood);
			}
			portions.push_back(ammount);
			std::cout << "\n" << "-------Todays foods-------" << "\n";
			showDay(day, portions);
			std::cout << "\n" << "-------Current balance-------" << "\n";
			showBalance(currentCals, currentProt, currentCarbs, currentFats);
			continue;

		}

	}

	if (input.compare("end") == 0) {
		// results
		std::cout << "-------End of the day balance-------" << "\n";
		showBalance(currentCals, currentProt, currentCarbs, currentFats);

		addToHistory(day, portions, HISTORY,currentCals, currentProt, currentCarbs, currentFats);
	} else {
		std::cout << "Something went wrong." << std::endl;
	}

	return;
}

int main() {

	baseProgram();

	return 0;
}
