#include <iostream>
#include <cctype>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstring>
#include <windows.h> // to delay the next display as decoration and colour
using namespace std;

HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); //colour decoration

bool repeats;
const int arrSIZE = 899;

struct foodType
{
	int foodID; //food ID
	string foodName; // food name
	double costPrice; //food's cost price
	double sellPrice; // food's selling price
	int quantity; //food quantity
	int stock_io; //stock in or out quantity
	double money; //revenues or expenses for stock phase
};
foodType search[arrSIZE];

struct admin {
	string username; //username for admin
	string password; //password for admin
};

admin adminList[100];
int numAdmin = 0;
ifstream inadminFile;
ofstream  outadminFile;

//for decoration purpose
void header();
void menu();
void loading();
void exitOper();
//for checking purpose
void invalidInput();
bool repeat();
//food phase
void count_line(int&);
bool existID(foodType search[], int, int);
bool existName(foodType search[], string, int);
void noExist();
void idRange(int&);
void addFood(foodType search[], int);
void searchFood(foodType search[], int);
bool search_food(foodType search[], int, int, string);
void displayList(foodType search[], int);
void editFood(foodType search[], int);
void editFigure(foodType search[], int, int, int, double);
void edit_name(foodType search[], int, int, string);
void delFood(foodType search[], int);
//stock phase
bool stockInOut(foodType search[], int, int, int);
void displayStockIO();
void clearHistory();
//admin phase
void loginPage();
void saveAdmin(ofstream& adminFile, admin adminList[]);
void readAdmin(ifstream& adminFile, admin adminList[], int& numAdmin);
void registerUser();
bool checkUser(string i);
void deleteUser();

int main()
{
	bool endMenu = false;
	SetConsoleTextAttribute(color, 14);
	cout << "\n\n\n\n\t\t   Loading\n\t"; //system startup loading bar decoration
	char loadBar = 219;
	for (int barAmount = 0; barAmount <= 29; barAmount++)
	{
		Sleep(100);
		cout << loadBar;
	}
	SetConsoleTextAttribute(color, 11);
	loginPage();
	do
	{
		header();
		menu();

		int countMain = 0;
		count_line(countMain);
		//input the data from the files first everytime at the main menu of the system
		ifstream inSearchFile;
		inSearchFile.open("food_figure.txt", ios::in);
		ifstream inNameFile;
		inNameFile.open("food_name.txt", ios::in);
		for (int i = 0; i < countMain; i++)
		{
			//input data from figures file
			inSearchFile >> search[i].foodID >> search[i].costPrice >> search[i].sellPrice >> search[i].quantity;
			inSearchFile.ignore();
			//input name from name file
			getline(inNameFile, search[i].foodName);
		}
		inSearchFile.close();
		inNameFile.close();
		int option;
		cin >> option; //choose an option for main menu
		if (cin.fail()) //invalid input checking such as input alphabet(abc...) into numeric variable(int, double)
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
			continue;
		}
		switch (option)
		{
		case 0: //close the application
		{
			endMenu = true;
			break;
		}
		//food phase
		case 1: //add new food
		{
			system("cls");
			addFood(search, countMain);
			break;
		}
		case 2: //search food
		{
			system("cls");
			searchFood(search, countMain);
			break;
		}
		case 3: //view the food list
		{
			system("cls");
			displayList(search, countMain);
			break;
		}
		case 4: //edit a food
		{
			system("cls");
			editFood(search, countMain);
			break;
		}
		case 5: //delete a food
		{
			system("cls");
			delFood(search, countMain);
			break;
		}
		//stock phase
		case 6: //stock in a food
		{
			system("cls");
			int stock_inID;
			bool repStoIn = true;
			do
			{
				exitOper();
				cout << "\n Stock in by food ID: ";
				cin >> stock_inID;
				repStoIn = stockInOut(search, countMain, option, stock_inID);
			} while (repStoIn);
			system("cls");
			break;
		}
		case 7: //stock out a food
		{
			system("cls");
			int stock_outID;
			bool repStoOut = true;
			do
			{
				exitOper();
				cout << "\n Stock out by food ID: ";
				cin >> stock_outID;
				repStoOut = stockInOut(search, countMain, option, stock_outID);
			} while (repStoOut);
			system("cls");
			break;
		}
		case 8: //display stock history
		{
			system("cls");
			displayStockIO();
			break;
		}
		case 9: //clear stock history
		{
			system("cls");
			clearHistory();
			break;
		}
		//admin phase
		case 10: //add admin
		{
			system("cls");
			cin.get();
			registerUser();
			break;
		}
		case 11: //delete admin
		{
			system("cls");
			cin.get();
			deleteUser();
			break;
		}
		default: //invalid input checking for main menu option
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
		}
		}

	} while (endMenu == false);
	//end of system
	loading();
	cout << "\n\t";
	SetConsoleTextAttribute(color, 12);
	system("PAUSE");
	SetConsoleTextAttribute(color, 15);
	return 0;
}
void header() //banner for main menu
{
	SetConsoleTextAttribute(color, 14);
	cout << "---------------------------------------------" << endl;
	SetConsoleTextAttribute(color, 11);
	cout << "\t    Food Inventory System" << endl;
	SetConsoleTextAttribute(color, 14);
	cout << "---------------------------------------------" << endl;
}
void menu() //menu to display option in the main menu
{
	SetConsoleTextAttribute(color, 14);
	cout << "  *** ";
	SetConsoleTextAttribute(color, 11);
	cout << "Welcome to Food Inventory System";
	SetConsoleTextAttribute(color, 14);
	cout << " ***\n\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 10);
	cout << "\t\t Food Phase\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 15);
	cout << "\t1. Add Food\n";
	cout << "\t2. Search Food\n";
	cout << "\t3. View Food List\n";
	cout << "\t4. Edit Food\n";
	cout << "\t5. Delete Food\n\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 10);
	cout << "\t\t Stock Phase\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 15);
	cout << "\t6. Stock In\n";
	cout << "\t7. Stock Out\n";
	cout << "\t8. Display Stock History\n";
	cout << "\t9. Clear Stock History\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 10);
	cout << "\t\t Admin Phase\n";
	SetConsoleTextAttribute(color, 14);
	cout << "\t*****************************\n";
	SetConsoleTextAttribute(color, 15);
	cout << "\t10. Add Admin\n";
	cout << "\t11. Delete Admin\n";
	SetConsoleTextAttribute(color, 12);
	cout << "\n\t0. Close Application\n";
	SetConsoleTextAttribute(color, 15);
	cout << "\n\tEnter your option >> ";
}
void loading() //display loading... as decoration for some operations performed
{
	cout << endl;
	for (int j = 0; j < 3; j++) //loading... for three times
	{
		cout << "\r\t\tLoading   \r\t\tLoading";
		for (int i = 0; i < 3; i++) {
			cout << ".";
			Sleep(300);
		}
	}
}
void exitOper()//function to display exit message
{
	SetConsoleTextAttribute(color, 14);
	cout << "\n---------------------------------------------------------";
	SetConsoleTextAttribute(color, 15);
	cout << "\n Enter ";
	SetConsoleTextAttribute(color, 12);
	cout << "-1";
	SetConsoleTextAttribute(color, 15);
	cout << " to exit\n";
}
void invalidInput() //display invalid input message
{
	SetConsoleTextAttribute(color, 12);
	cout << "\n\t\t INVALID INPUT!\n";
	SetConsoleTextAttribute(color, 15);
	Sleep(1000);
	system("cls");
}
bool repeat() //function to check for validation input
{
	repeats = false;
	if (cin.fail()) //invalid input checking
	{
		cin.clear();
		cin.ignore(1000, '\n');
		repeats = true; //return true at the end of the function to request for a new input again
		SetConsoleTextAttribute(color, 12);
		cout << " Invalid input. Please try again.\n";
		SetConsoleTextAttribute(color, 15);
		Sleep(1500);
		system("cls");
	}
	return repeats;
}
void count_line(int& countLine)//function to calculate the no. of row in the file
{
	countLine = 0;
	string line;
	fstream inLineFile;
	inLineFile.open("food_name.txt", ios::in);
	while (getline(inLineFile, line))
	{
		countLine++;
	}
	inLineFile.close();
}
//function to check if the food ID exist in the figure file
bool existID(foodType search[], int checkID, int countID)
{
	for (int i = 0; i < countID; i++)
	{
		if (search[i].foodID == checkID)
		{
			//return true at the end of the function if the food ID exist in the file
			return true;
		}
	}
	return false;
}
//function to check if the food Name exist in the name file
bool existName(foodType search[], string checkName, int countID)
{
	for (int i = 0; i < countID; i++)
	{
		if (search[i].foodName == checkName)
		{
			//return true at the end of the function if the food name exist in the file
			return true;
		}
	}
	return false;
}
//function to tell the user that the food ID input is not exist
void noExist()
{
	SetConsoleTextAttribute(color, 12);
	cout << "\n The food ID does not exist.";
	SetConsoleTextAttribute(color, 15);
	Sleep(1500);
	system("cls");
}
//function to tell the user that the ID input is not within the range
void idRange(int& input)
{
	cin.clear();
	cin.ignore(1000, '\n');
	SetConsoleTextAttribute(color, 12);
	cout << "\n Only food ID range from 101 to 999 is allowed.\n ";
	SetConsoleTextAttribute(color, 15);
	Sleep(1500);
	system("cls");
}
//function to add new food information
void addFood(foodType search[], int count)
{
	bool repeatFood = true;
	int foodID, quantity, i = 1;
	double costPrice, sellPrice;
	string foodName;
	do
	{
		//provide information to be inputted one by one
		switch (i)
		{
		case 1:
			exitOper();
			cout << "\n Create new food ID: ";
			cin >> foodID;
			//ID range is limited in the range from 101 to 999
			if (foodID >= 101 && foodID <= 999) //the input of new food ID is within the range
			{
				//if the food ID is exist in the file
				if (existID(search, foodID, count))
				{
					SetConsoleTextAttribute(color, 12);
					cout << " The food ID is used.\n";
					SetConsoleTextAttribute(color, 15);
					Sleep(1000);
					system("cls");
				}
				else
				{
					//stop the do...while loop from asking user to input again
					i++;
				}
			}
			else if (foodID == -1) //exit
			{
				i = 7;
			}
			else
			{
				//call the function to tell the user that the input is not within the range
				idRange(foodID);
			}
			break;
		case 2:
			cout << "\n Enter food name: ";
			cin.ignore();
			getline(cin, foodName);
			i++;
			if (foodName == "-1") //exit
			{
				i = 7;
			}
			break;
		case 3:
			do
			{
				cout << "\n Enter cost price: RM ";
				cin >> costPrice;
				repeat();//call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
			} while (repeats);
			i++;
			if (costPrice == -1) //exit
			{
				i = 7;
			}
			break;
		case 4:
			do
			{
				cout << "\n Enter sell price: RM ";
				cin >> sellPrice;
				repeat();//call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
			} while (repeats);
			i++;
			if (sellPrice == -1) //exit
			{
				i = 7;
			}
			break;
		case 5:
			do
			{
				cout << "\n Enter quantity: ";
				cin >> quantity;
				repeat();//call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
			} while (repeats);
			i++;
			if (quantity == -1) //exit
			{
				i = 7;
			}
			break;
		case 7: //i is assigned to 7 when input -1 to exit back to main menu
			break;
		}
	} while (i < 6 || i > 7); //when i is 6 the information is completely inputted while 7 is exit
	if (i == 7) //exit
	{
		system("cls");
	}
	else if (i == 6) //continue to output the data
	{
		//output the data input into the files
		ofstream foodNameFile;
		ofstream foodFigureFile;
		foodNameFile.open("food_name.txt", ios::app);
		foodFigureFile.open("food_figure.txt", ios::app);
		foodNameFile << foodName << endl;
		foodFigureFile << setw(4) << foodID << setw(8) << costPrice << setw(8) << sellPrice << setw(5) << quantity << endl;
		foodNameFile.close();
		foodFigureFile.close();
		SetConsoleTextAttribute(color, 14);
		cout << "---------------------------------------------------------";
		SetConsoleTextAttribute(color, 15);
		loading();
		SetConsoleTextAttribute(color, 14);
		cout << "\n\n\tNew food is added successfully.";
		SetConsoleTextAttribute(color, 15);
		Sleep(1500);
		system("cls");
	}
}
//function to search a food's information from the files
void searchFood(foodType search[], int count)
{
	int searchFID;
	string searchName;
	int choice;
	bool repeatSearchID = true;
	bool repeatSearchName = true;
	bool repeatSearch = true;
	do
	{
		//options for user to choose whether search by ID or name
		SetConsoleTextAttribute(color, 14);
		cout << "\n---------------------------------------------------------";
		SetConsoleTextAttribute(color, 15);
		cout << "\n  1. Search by food ID\n";
		cout << "  2. Search by food name\n";
		SetConsoleTextAttribute(color, 12);
		cout << "  0. EXIT\n";
		SetConsoleTextAttribute(color, 15);
		SetConsoleTextAttribute(color, 14);
		cout << "---------------------------------------------------------";
		SetConsoleTextAttribute(color, 15);
		cout << "\n Do you want to search by food ID or food name: ";
		cin >> choice;
		if (cin.fail()) //invalid input checking
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
			continue;
		}
		switch (choice)
		{
		case 1: //search by ID
		{
			//ask user to search by inputting the ID
			do
			{
				system("cls");
				exitOper();
				cout << "\n Enter the food ID that you want to search: ";
				cin >> searchFID;
				if (searchFID >= 101 && searchFID <= 999) //input ID within the range
				{
					if (existID(search, searchFID, count)) //ID exist in the file
					{
						repeatSearchID = search_food(search, count, searchFID, "");
						//search the food and also return true or false that determine whether to repeat or not repeat the operation
					}
					else //ID not exist in the file
					{
						noExist();
					}
				}
				else if (searchFID == -1) //exit
				{
					break;
				}
				else //input is not within the range
				{
					idRange(searchFID);
				}
			} while (repeatSearchID);
			system("cls");
			break;
		}
		case 2: //search by name
		{
			cin.ignore();
			//ask user to search by inputting the name
			do
			{
				system("cls");
				exitOper();
				cout << "\n Enter the food name that you want to search: ";
				getline(cin, searchName);
				if (existName(search, searchName, count)) //input name exist in file
				{
					repeatSearchName = search_food(search, count, 0, searchName);
					//search the food and also return true or false that determine whether to repeat or not repeat the operation
				}
				else if (searchName == "-1") //exit
				{
					break;
				}
				else //food name not exist in file
				{
					SetConsoleTextAttribute(color, 12);
					cout << " The food name does not exist.\n";
					SetConsoleTextAttribute(color, 15);
					Sleep(1000);
				}
			} while (repeatSearchName);
			system("cls");
			break;
		}
		case 0: //exit the search operation
		{
			system("cls");
			break;
		}
		default: //invalid input and repeat the question
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
			continue;
		}
		}
		system("cls");
		break;
	} while (repeatSearch);
}
//function to display the food information which searched by user
bool search_food(foodType search[], int countMain, int searchFID, string searchName)
{
	for (int i = 0; i < countMain; i++)
	{
		if (search[i].foodID == searchFID) //for search by ID option
		{
			SetConsoleTextAttribute(color, 14);
			cout << "---------------------------------------------------------";
			SetConsoleTextAttribute(color, 11);
			cout << "\n\n\tFood ID\t\t:" << setw(19) << search[i].foodID << "\n\n";
			cout << "\tFood Name\t:" << setw(19) << search[i].foodName << "\n\n";
			cout << fixed << showpoint << setprecision(2);
			cout << "\tCost Price\t:" << setw(12) << "RM" << setw(7) << search[i].costPrice << "\n\n";
			cout << "\tSelling Price\t:" << setw(12) << "RM" << setw(7) << search[i].sellPrice << "\n\n";
			SetConsoleTextAttribute(color, 15);
		}
		if (search[i].foodName == searchName) //for search by name option
		{
			SetConsoleTextAttribute(color, 14);
			cout << "---------------------------------------------------------";
			SetConsoleTextAttribute(color, 11);
			cout << "\n\n\tFood ID\t\t:" << setw(19) << search[i].foodID << "\n\n";
			cout << "\tFood Name\t:" << setw(19) << search[i].foodName << "\n\n";
			cout << fixed << showpoint << setprecision(2);
			cout << "\tCost Price\t:" << setw(12) << "RM" << setw(7) << search[i].costPrice << "\n\n";
			cout << "\tSelling Price\t:" << setw(12) << "RM" << setw(7) << search[i].sellPrice << "\n\n";
			SetConsoleTextAttribute(color, 15);
		}
	}
	char s_again;
	bool askAgain = true;
	do	//search again option
	{
		SetConsoleTextAttribute(color, 14);
		cout << "---------------------------------------------------------";
		SetConsoleTextAttribute(color, 15);
		cout << "\n Do you want to search again? (Y = Yes, N = No): ";
		cin >> s_again;
		if (toupper(s_again) == 'Y')
		{
			cin.ignore();
			system("cls");
			return true; //return true to search again
		}
		else if (toupper(s_again) == 'N')
		{
			return false; //return false to not search again and back to main menu
		}
		else //invalid input and repeat the question
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
			askAgain = true;
		}
	} while (askAgain);
}
void displayList(foodType search[], int countMain)//function to display food list
{
	SetConsoleTextAttribute(color, 14);
	cout << "\n -----------------------------------------------------------------------------------------------------\n";
	SetConsoleTextAttribute(color, 11);
	cout << setw(6) << "ID" << setw(24) << "Name" << setw(29) << "Cost Price(RM)" << setw(27) << "Selling Price(RM)" << setw(13) << "Qty";
	SetConsoleTextAttribute(color, 14);
	cout << "\n -----------------------------------------------------------------------------------------------------\n";
	for (int i = 0; i < countMain; i++)	//input the data into the array of struct
	{
		SetConsoleTextAttribute(color, 15);
		cout << "   " << setw(4) << left << search[i].foodID << setw(28) << right << search[i].foodName << setw(20) << fixed << showpoint
			<< setprecision(2) << search[i].costPrice << setw(27) << search[i].sellPrice << setw(17) << search[i].quantity << endl;
	}
	SetConsoleTextAttribute(color, 14);
	cout << " -----------------------------------------------------------------------------------------------------\n\n";	SetConsoleTextAttribute(color, 14);
	SetConsoleTextAttribute(color, 12);
	cout << setw(75) << "(EXIT) ";
	system("PAUSE");
	system("cls");
}
void editFood(foodType search[], int count)//function to edit food (main)
{
	int editID;
	bool repeatEdit = true;
	do	//ask user to input food ID to edit
	{
		exitOper();
		cout << "\n Enter the food ID that you want to edit: ";
		cin >> editID;
		if (editID >= 101 && editID <= 999) //within the ID range
		{
			if (existID(search, editID, count)) //ID exist in the file
			{
				bool repeatINFO = true;
				do
				{
					SetConsoleTextAttribute(color, 14);
					cout << "---------------------------------------------------------\n";
					//display information that can be editted
					cout << "\n\t*****************************\n";
					SetConsoleTextAttribute(color, 11);
					cout << "\t\t INFORMATION\n";
					SetConsoleTextAttribute(color, 14);
					cout << "\t*****************************\n";
					SetConsoleTextAttribute(color, 15);
					cout << "\t1. Food ID\n";
					cout << "\t2. Food Name\n";
					cout << "\t3. Cost Price\n";
					cout << "\t4. Selling Price\n";
					SetConsoleTextAttribute(color, 12);
					cout << "\t0. EXIT\n";
					SetConsoleTextAttribute(color, 15);
					SetConsoleTextAttribute(color, 14);
					cout << "\n---------------------------------------------------------";
					SetConsoleTextAttribute(color, 15);
					int editINFO;
					//ask user to input the information that want to edit by inputting number
					cout << "\n Enter the information that you want to edit: ";
					cin >> editINFO;
					if (cin.fail()) //invalid input checking
					{
						cin.clear();
						cin.ignore(1000, '\n');
						invalidInput();
						continue;
					}
					system("cls");
					switch (editINFO)
					{
					case 1: //edit food ID
					{
						bool repNewID = true;
						do
						{
							int newID;
							exitOper();
							cout << "\n Enter the new food ID: ";
							cin >> newID;
							if (newID >= 101 && newID <= 999) //new ID within the range
							{
								if (existID(search, newID, count)) //new ID is used
								{
									SetConsoleTextAttribute(color, 12);
									cout << " The food ID is used.\n ";
									SetConsoleTextAttribute(color, 15);
									Sleep(1000);
									system("cls");
								}
								else //new ID not exist in the file which can be changed
								{
									//edit the ID into new ID in the file
									editFigure(search, count, editINFO, editID, newID);
									repNewID = false;
									SetConsoleTextAttribute(color, 14);
									cout << "---------------------------------------------------------";
									SetConsoleTextAttribute(color, 15);
									loading();
									SetConsoleTextAttribute(color, 14);
									cout << "\n\tFood ID is changed successfully.";
									SetConsoleTextAttribute(color, 15);
									Sleep(1500);
									system("cls");
								}
							}
							else if (newID == -1) //exit
							{
								system("cls");
								break;
							}
							else
							{
								idRange(newID); //new ID not within the range

							}
						} while (repNewID);
						break;
					}
					case 2: //edit food name
					{
						string newName;
						exitOper();
						cout << "\n Enter the new food name: ";
						cin.ignore();
						getline(cin, newName);
						if (newName == "-1") //exit
						{
							system("cls");
							break;
						}
						//edit the name into new name in the file
						edit_name(search, count, editID, newName);
						SetConsoleTextAttribute(color, 14);
						cout << "---------------------------------------------------------";
						SetConsoleTextAttribute(color, 15);
						loading();
						SetConsoleTextAttribute(color, 14);
						cout << "\n\tFood name is changed successfully.";
						SetConsoleTextAttribute(color, 15);
						Sleep(1500);
						system("cls");
						break;
					}
					case 3: //edit food's cost price
					{
						double newCost;
						do
						{
							exitOper();
							cout << "\n Enter the new cost price: ";
							cin >> newCost;
							repeat();//call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
						} while (repeats);
						if (newCost == -1) //exit
						{
							system("cls");
							break;
						}
						//edit cost price into new cost price in the file
						editFigure(search, count, editINFO, editID, newCost);
						SetConsoleTextAttribute(color, 14);
						cout << "---------------------------------------------------------";
						SetConsoleTextAttribute(color, 15);
						loading();
						SetConsoleTextAttribute(color, 14);
						cout << "\n\tFood's cost price is changed successfully.";
						SetConsoleTextAttribute(color, 15);
						Sleep(1500);
						system("cls");
						break;
					}
					case 4: //edit food's selling price
					{
						double newSell;
						do
						{
							exitOper();
							cout << "\n Enter the new selling price: ";
							cin >> newSell;
							repeat();//call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
						} while (repeats);
						if (newSell == -1) //exit
						{
							system("cls");
							break;
						}
						//edit selling price into new selling price in the file
						editFigure(search, count, editINFO, editID, newSell);
						SetConsoleTextAttribute(color, 14);
						cout << "---------------------------------------------------------";
						SetConsoleTextAttribute(color, 15);
						loading();
						SetConsoleTextAttribute(color, 14);
						cout << "\n\tFood's selling price is changed successfully.";
						SetConsoleTextAttribute(color, 15);
						Sleep(1500);
						system("cls");
						break;
					}
					case 0: //exit the edit operation
					{
						system("cls");
						break;
					}
					default: //invalid input
					{
						cin.clear();
						cin.ignore(1000, '\n');
						invalidInput();
						Sleep(1000);

						continue;
					}
					}
					repeatEdit = false;
					repeatINFO = false;
				} while (repeatINFO);
			}
			else
			{
				noExist(); //food ID not exist in the file to be editted
			}
		}
		else if (editID == -1) //exit
		{
			system("cls");
			break;
		}
		else
		{
			idRange(editID); //not within the ID range
		}
	} while (repeatEdit);
}
//function which is used in edit food to edit food ID, cost price, and selling price
void editFigure(foodType search[], int countE, int optionEdit, int editID, double newFigure)
{
	//the existing content of the figure file is replaced by the new content
	ofstream editFigureFile;
	editFigureFile.open("food_figure.txt", ios::trunc);
	//add data into the stock history file (for stock in / out)
	ofstream stockFile;
	stockFile.open("food_stock.txt", ios::app);
	for (int i = 0; i < countE; i++)
	{
		//output back the original data back into the file when the data in the position of array is not equal to edit ID
		if (editID != search[i].foodID)
		{
			editFigureFile << setw(4) << search[i].foodID << setw(8) << search[i].costPrice << setw(8) << search[i].sellPrice << setw(5) << search[i].quantity << endl;
		}
		else //edit the information when the data in the position of array is equal to edit ID
		{
			switch (optionEdit)
			{
			case 1: //for new food ID
				search[i].foodID = newFigure;
				break;
			case 3: //for new cost price
				search[i].costPrice = newFigure;
				break;
			case 4: //for new selling price
				search[i].sellPrice = newFigure;
				break;
			case 6: //for new quantity after stock in
				search[i].quantity += newFigure;
				for (int i = 0; i < countE; i++)
				{
					if (editID == search[i].foodID) //stock in ID is same as the data in the position of array
					{
						//add the information to take note the infomation for stock in operation
						stockFile << search[i].foodName << endl;
						stockFile << setw(4) << search[i].foodID << setw(8) << search[i].costPrice << setw(5) << newFigure << " 1" << endl;
					}
				}
				SetConsoleTextAttribute(color, 14);
				cout << "---------------------------------------------------------";
				SetConsoleTextAttribute(color, 15);
				loading();
				SetConsoleTextAttribute(color, 14);
				cout << "\n\t Stock in successfully!";
				SetConsoleTextAttribute(color, 15);
				Sleep(1500);
				break;
			case 7://for new quantity after stock out
				search[i].quantity -= newFigure;
				for (int i = 0; i < countE; i++)
				{
					if (editID == search[i].foodID) //stock out ID is same as the data in the position of array
					{
						//add the information to take note the infomation for stock out operation
						stockFile << search[i].foodName << endl;
						stockFile << setw(4) << search[i].foodID << setw(8) << search[i].sellPrice << setw(5) << newFigure << " 2" << endl;
					}
				}
				SetConsoleTextAttribute(color, 14);
				cout << "---------------------------------------------------------";
				SetConsoleTextAttribute(color, 15);
				loading();
				SetConsoleTextAttribute(color, 14);
				cout << "\n\t Stock out successfully!";
				SetConsoleTextAttribute(color, 15);
				Sleep(1500);
				break;
			}
			editFigureFile << setw(4) << search[i].foodID << setw(8) << search[i].costPrice << setw(8) << search[i].sellPrice << setw(5) << search[i].quantity << endl;
			//output back the edited data back into the figure file
		}
	}
	editFigureFile.close();
	stockFile.close();
}
//function which is used in edit food to edit name
void edit_name(foodType search[], int countE, int editID, string newName)
{
	//the existing content of the file is replaced by the new content
	ofstream editNameFile;
	editNameFile.open("food_name.txt", ios::trunc);
	for (int i = 0; i < countE; i++)
	{
		if (editID != search[i].foodID) //the edit ID is not equal to the data in the position of array
		{
			//output back the name into the file
			editNameFile << search[i].foodName << endl;
		}
		else //the edit ID is not equal to the data in the position of array
		{
			//output the new name into the file
			search[i].foodName = newName;
			editNameFile << search[i].foodName << endl;
		}
	}
	editNameFile.close();
}
void delFood(foodType search[], int countD1)//function to delete food
{
	int delID, countD2;
	bool repeatDel = true;
	//ask user to input the food ID to delete
	do
	{
		exitOper();
		cout << "\n Enter the food ID that you want to delete: ";
		cin >> delID;
		if (delID >= 101 && delID <= 999) //within the range
		{
			//the existing content of the file is replaced by the new content
			ofstream delFigureFile;
			delFigureFile.open("food_figure.txt", ios::trunc);
			ofstream delNameFile;
			delNameFile.open("food_name.txt", ios::trunc);
			for (int i = 0; i < countD1; i++)
			{
				if (delID != search[i].foodID) //if the delete ID is not equal to the data in the postion of array
				{
					//output back the data into the files
					delNameFile << search[i].foodName << endl;
					delFigureFile << setw(4) << search[i].foodID << setw(8) << search[i].costPrice << setw(8) << search[i].sellPrice << setw(5) << search[i].quantity << endl;
				}
			}
			delNameFile.close();
			delFigureFile.close();

			countD2 = 0;
			count_line(countD2);
			if (countD1 == countD2) //the number of row before delete operation and after delete operation are the same
			{
				noExist(); //the ID input not exist since nothing is deleted
			}
			else //number of row before delete and after delete are different
			{
				repeatDel = false;
				SetConsoleTextAttribute(color, 14);
				cout << "---------------------------------------------------------";
				SetConsoleTextAttribute(color, 15);
				loading();
				SetConsoleTextAttribute(color, 14);
				cout << "\n\t    The food is deleted.";
				SetConsoleTextAttribute(color, 15);
				Sleep(1500);
				system("cls");
			}
		}
		else if (delID == -1) //exit
		{
			system("cls");
			break;
		}
		else //not within the range
		{
			idRange(delID);
		}
	} while (repeatDel);
}
//function to stock in/stock out
bool stockInOut(foodType search[], int countMain, int option, int stockID)
{
	if (stockID >= 101 && stockID <= 999) //within the range
	{
		if (existID(search, stockID, countMain)) //ID input exist in the file
		{
			bool askAgain = false;
			int ioAmount;
			do
			{
				do
				{
					system("cls");
					exitOper();
					if (option == 6) //stock in
					{
						cout << "\n Enter the amount that you want to stock in: ";
						cin >> ioAmount;
						if (repeat()) //call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
						{
							system("cls");
						}
					}
					else if (option == 7) //stock out
					{
						for (int i = 0; i < countMain; i++)
						{
							if (stockID == search[i].foodID)
							{
								if (search[i].quantity == 0) //quantity of the food is 0, means unable to perform stock out
								{
									SetConsoleTextAttribute(color, 12);
									cout << "\n\t\tOUT OF STOCK!";
									SetConsoleTextAttribute(color, 15);
									Sleep(1500);
									return false;
								}
							}
						}
						cout << "\n Enter the amount that you want to stock out: ";
						cin >> ioAmount;
						if (repeat()) //call function to check the validation of the input, repeat do...while loop again if invalid (return repeats=true)
						{
							system("cls");
						}
						for (int i = 0; i < countMain; i++)
						{
							if (stockID == search[i].foodID)
							{
								if (search[i].quantity < ioAmount) //stock out amount exceed the quantity of food
								{
									SetConsoleTextAttribute(color, 12);
									cout << "\n\t\tInsuffient stock.";
									SetConsoleTextAttribute(color, 15);
									Sleep(1500);
									return false;
								}
							}
						}
					}
					if (ioAmount == 0 && repeats == false) //nothing to stock out
					{
						repeats = true;
						SetConsoleTextAttribute(color, 12);
						cout << " Invalid input. Please try again.\n";
						SetConsoleTextAttribute(color, 15);
						Sleep(1500);
						system("cls");
					}
					else if (ioAmount == -1) //exit
					{
						break;
					}
				} while (repeats);

				if (ioAmount <= 0)
				{
					break;
				}
				else
				{
					editFigure(search, countMain, option, stockID, ioAmount); //also edit the quantity of the food in the figure file
					char s_again;
					bool askContAgain = true;
					//ask user to stock in/stock out the food again or not
					do
					{
						SetConsoleTextAttribute(color, 14);
						cout << "\n---------------------------------------------------------";
						SetConsoleTextAttribute(color, 15);
						cout << "\n Do you want to continue? (Y = Yes, N = No): ";
						cin >> s_again;
						if (toupper(s_again) == 'Y') //Yes to ask again the stock in/out amount
						{
							cin.ignore();
							askAgain = true;
							break;
						}
						else if (toupper(s_again) == 'N') //exit and back to main menu
						{
							askAgain = false;
							break;
						}
						else //invalid input
						{
							cin.clear();
							cin.ignore(1000, '\n');
							invalidInput();
						}
					} while (askContAgain);
				}
			} while (askAgain);
			return false;
		}
		else //ID not exist in file
		{
			noExist();
			return true;
		}
	}
	else if (stockID == -1) //exit
	{
		return false;
	}
	else //not within range
	{
		idRange(stockID);
		return true;
	}
}
void displayStockIO()//function to display stock history
{
	int countHistory = 0;
	string line;
	fstream inLineFile;
	inLineFile.open("food_stock.txt", ios::in);
	//read the line in the stock history file
	while (getline(inLineFile, line))
	{
		countHistory++;
	}
	inLineFile.close();
	fstream stockFile;
	//input the data from the stock history file
	stockFile.open("food_stock.txt", ios::in);
	for (int i = 0; i < (countHistory / 2); i++)
	{
		getline(stockFile, search[i].foodName);
		stockFile >> search[i].foodID >> search[i].money >> search[i].quantity >> search[i].stock_io;
		stockFile.ignore();
	}
	stockFile.close();
	SetConsoleTextAttribute(color, 14);
	cout << "\n -------------------------------------------------------------------------------------------------------\n";
	SetConsoleTextAttribute(color, 11);
	cout << setw(6) << "ID" << setw(24) << "Name" << setw(24) << "Expenses(RM)" << setw(22) << "Revenue(RM)" << setw(13) << "Qty" << setw(12) << "Status";
	SetConsoleTextAttribute(color, 14);
	cout << "\n -------------------------------------------------------------------------------------------------------\n";
	ifstream read("food_stock.txt");
	if (read.peek() == EOF) //the history file is empty
	{
		SetConsoleTextAttribute(color, 12);
		cout << "\n" << setw(63) << "NO RECORDS FOUND\n\n";
		read.close();
	}
	else //file has data
	{
		SetConsoleTextAttribute(color, 15);
		for (int i = 0; i < (countHistory / 2); i++)
		{
			if (search[i].stock_io == 1) //1 represent stock in
			{
				search[i].money *= search[i].quantity;
				cout << setw(6) << search[i].foodID << setw(26) << right << search[i].foodName << setw(18) << fixed << showpoint
					<< setprecision(2) << search[i].money << setw(22) << "" << setw(14) << "+" << setw(3) << search[i].quantity << setw(10) << "IN" << endl;
			}
			else //others than 1, which is 2 that represent stock out
			{
				search[i].money *= search[i].quantity;
				cout << setw(6) << search[i].foodID << setw(26) << right << search[i].foodName << setw(18) << fixed << showpoint
					<< setprecision(2) << "" << setw(22) << search[i].money << setw(14) << "-" << setw(3) << search[i].quantity << setw(10) << "OUT" << endl;
			}
		}
	}
	SetConsoleTextAttribute(color, 14);
	cout << " -------------------------------------------------------------------------------------------------------\n\n";
	SetConsoleTextAttribute(color, 12);
	cout << setw(73) << "(EXIT) ";
	system("PAUSE");
	system("cls");
}
void clearHistory() //function to clear stock history file
{
	bool askAgain;
	char clear;
	do //ask user want to clear or not
	{
		SetConsoleTextAttribute(color, 14);
		cout << "\n---------------------------------------------------------";
		SetConsoleTextAttribute(color, 15);
		cout << "\n\nDo you want to clear stock history? (Y = Yes, N = No): ";
		cin >> clear;
		if (toupper(clear) == 'Y')
		{
			cin.ignore();
			ofstream clearHistoryFile;
			//clear all the data in the stock history file
			clearHistoryFile.open("food_stock.txt", ios::out | ios::trunc);
			clearHistoryFile.close();
			SetConsoleTextAttribute(color, 14);
			cout << "\n---------------------------------------------------------";
			SetConsoleTextAttribute(color, 15);
			loading();
			SetConsoleTextAttribute(color, 14);
			cout << "\n     Stock history is cleared successfully!";
			SetConsoleTextAttribute(color, 15);
			Sleep(1500);
			askAgain = false;
		}
		else if (toupper(clear) == 'N') //exit and back to main menu
		{
			break;
		}
		else //invalid input
		{
			cin.clear();
			cin.ignore(1000, '\n');
			invalidInput();
			askAgain = true;
		}
	} while (askAgain);
	system("cls");
}
//function to add new admin
void saveAdmin(ofstream& adminFile, admin adminList[]) {
	//the existing content of the file is replaced by the new content
	adminFile.open("admin.txt", ios::trunc);
	//output the data into the file
	for (int i = 0; i < numAdmin; i++) {
		adminFile << adminList[i].username << endl << adminList[i].password << endl;
	}
	adminFile.close();
}
//function to input the admin data
void readAdmin(ifstream& adminFile, admin adminList[], int& numAdmin) {
	adminFile.open("admin.txt");
	int counter = 0;
	string temp;
	//admin's username temporarily store in temp variable to run the while loop
	while (getline(adminFile, temp)) {
		adminList[counter].username = temp; //temp variable is the admin's username 
		getline(adminFile, adminList[counter].password);
		counter++;
	}
	numAdmin = counter;
	adminFile.close();
}
//fucntion to check whether it contain spaces of the input of admin name and password in the login page
bool checkUser(string i) {
	int j = 0;
	while (i[j] != '\0') {
		if (isspace(i[j])) { //there is space in the input

			return  false;
		}
		j++;
	}
	return true;
}
//function to delete the admin
void deleteUser() {
	readAdmin(inadminFile, adminList, numAdmin); //call function to input admin data from file
	bool repeat = true;
	string userInput;
	do {	//ask user to input the username
		exitOper();
		cout << "\n Enter the username of the admin you want to delete: ";
		getline(cin, userInput);
		if (userInput == "-1") //exit
		{
			system("cls");
			break;
		}
		int noFound = 0;
		for (int i = 0; i < numAdmin; i++) {
			if (adminList[i].username == userInput) { //the username that want to delete equal to the data in the position of array
				repeat = false;
				noFound++; //nofound = 1, means username exist
				ofstream outadminFile("admin.txt", ios::trunc); //new content replace existing content
				for (int i = 0; i < numAdmin; i++) {
					if (userInput != adminList[i].username) //the username that want to delete not equal to the data in the position of array
					{
						//output back the data back into the file
						outadminFile << adminList[i].username << endl << adminList[i].password << endl;
					}
				}
				SetConsoleTextAttribute(color, 14);
				cout << "---------------------------------------------------------";
				SetConsoleTextAttribute(color, 15);
				loading();
				SetConsoleTextAttribute(color, 14);
				cout << "\n\n    The admin infomation ";
				SetConsoleTextAttribute(color, 10);
				cout << userInput;
				SetConsoleTextAttribute(color, 14);
				cout << " is deleted.";
				SetConsoleTextAttribute(color, 15);
				Sleep(1500);
				system("cls");
			}
		}
		if (noFound == 0) { //input username not exist in file
			SetConsoleTextAttribute(color, 12);
			cout << " Username does not exist" << endl;
			SetConsoleTextAttribute(color, 15);
			Sleep(1000);
			system("cls");
		}
	} while (repeat);
}
void registerUser() {//function to add new admin
	string userinput;
	string passinput;
	//obtain data from files
	readAdmin(inadminFile, adminList, numAdmin);
	int step = 1;
	do {
		switch (step) {
		case 1:
			exitOper();
			//ask user to input a username
			cout << "\n Register a username: ";
			getline(cin, userinput);
			if (!checkUser(userinput)) { //there is space in the input
				SetConsoleTextAttribute(color, 12);
				cout << " Username cannot contain spaces" << endl;
				SetConsoleTextAttribute(color, 15);
				step--;
				Sleep(1500);
				system("cls");
			}
			if (userinput == "admin") { //input username is a default username
				SetConsoleTextAttribute(color, 12);
				cout << " This username already exist" << endl;
				SetConsoleTextAttribute(color, 15);
				step--;
				Sleep(1500);
				system("cls");
			}
			else if (userinput == "-1") //exit
			{
				step++;
				system("cls");
			}
			else {
				for (int i = 0; i < numAdmin; i++) {
					if (adminList[i].username == userinput) { //input username exist in the file
						SetConsoleTextAttribute(color, 12);
						cout << " This username already exist" << endl;
						SetConsoleTextAttribute(color, 15);
						step--;
						Sleep(1500);
						system("cls");
					}
				}
				adminList[numAdmin].username = userinput;
			}
			step++;
			break;
		case 2:
			//ask user to input a password for the new username
			cout << "\n Password for " << userinput << ": ";
			getline(cin, passinput);
			if (!checkUser(passinput)) { // there is space in the input
				SetConsoleTextAttribute(color, 12);
				cout << " Password cannot contain spaces " << endl;
				SetConsoleTextAttribute(color, 15);
				step--;
				Sleep(1500);
				system("cls");
			}
			else if (passinput == "-1") //exit
			{
				step++;
				system("cls");
			}
			else {
				adminList[numAdmin].password = passinput;
				numAdmin++;
				saveAdmin(outadminFile, adminList); //call function to save the data into the file
				step++;
				SetConsoleTextAttribute(color, 14);
				cout << "---------------------------------------------------------";
				SetConsoleTextAttribute(color, 15);
				loading();
				SetConsoleTextAttribute(color, 14);
				cout << "\n\n    The admin with username ";
				SetConsoleTextAttribute(color, 10);
				cout << userinput;
				SetConsoleTextAttribute(color, 14);
				cout << " is created successfully!";
				SetConsoleTextAttribute(color, 15);
				Sleep(1500);
				system("cls");
			}
			break;
		}
	} while (step != 3);

}
//function to display the login page and ask user to input username and password to login
void loginPage() {
	int c = 0;
	do
	{
		system("cls");
		readAdmin(inadminFile, adminList, numAdmin);
		//R raw string literal wraps multiline strings
		cout << R"(
 _       __________    __________  __  _________
| |     / / ____/ /   / ____/ __ \/  |/  / ____/
| | /| / / __/ / /   / /   / / / / /|_/ / __/   
| |/ |/ / /___/ /___/ /___/ /_/ / /  / / /___   
|__/|__/_____/_____/\____/\____/_/  /_/_____/   
                                                
  __________ 
 /_  __/ __ \
  / / / / / /
 / / / /_/ / 
/_/  \____/  
		)";
		SetConsoleTextAttribute(color, 10);
		cout << R"(
    __ __ ___    ____  ______  ___
   / //_//   |  / __ \/  _/  |/  /
  / ,<  / /| | / /_/ // // /|_/ / 
 / /| |/ ___ |/ _, _// // /  / /  
/_/ |_/_/  |_/_/ |_/___/_/  /_/   
		)";
		SetConsoleTextAttribute(color, 11);
		cout << R"(
    __________  ____  ____     _____   ___    _________   ____________  ______  __
   / ____/ __ \/ __ \/ __ \   /  _/ | / / |  / / ____/ | / /_  __/ __ \/ __ \ \/ /
  / /_  / / / / / / / / / /   / //  |/ /| | / / __/ /  |/ / / / / / / / /_/ /\  / 
 / __/ / /_/ / /_/ / /_/ /  _/ // /|  / | |/ / /___/ /|  / / / / /_/ / _, _/ / /  
/_/    \____/\____/_____/  /___/_/ |_/  |___/_____/_/ |_/ /_/  \____/_/ |_| /_/   
                                                                                  
		)";
		string username;
		string password;
		SetConsoleTextAttribute(color, 15);
		//ask user to input username and password
		cout << "\n Enter username to login : ";
		cin >> username;
		cout << "\n Enter password : ";
		cin >> password;
		if (username == "admin" && password == "admin") { //default username and password
			c++; 				//default password if no admin is registered in the file
			loading();
		}
		for (int i = 0; i < numAdmin; i++) {
			if ((adminList[i].username == username && adminList[i].password == password))
			{
				//input username and password exist in the file (correct)
				c++;
				loading();
			}
		}
		if (c == 0) { //input username or passsword not exist in the file (wrong)
			SetConsoleTextAttribute(color, 12);
			cout << " Username or password is invalid" << endl;
			SetConsoleTextAttribute(color, 15);
			Sleep(1000);
		}
	} while (c == 0);
	system("cls");
}