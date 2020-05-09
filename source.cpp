#include <iostream>
#include <string>
#include <cctype>
#include <sqlite3.h>
#include <stdio.h>
#include "Account.h"
#include <cmath>
using std::string;
using std::cout;
using std::cin;
using std::endl;


int main()
{
	Account acct;
	int input = -1;
	bool active = true;

	while(active == true)
	{
		cout << "\n\n|++++++++++++++++++++++++++++++++++++++++++++++++++++++++|" << endl;
		cout << "Select an option below: \n";
		cout << "\t1) Add Record to System\n";
		cout << "\t2) Show Current Record in System\n";
		cout << "\t3) Search Record in System\n";
		cout << "\t4) Update Record\n";
		cout << "\t5) Delete Record\n";
		cout << "\t6) Quit\n";
		cout << "|++++++++++++++++++++++++++++++++++++++++++++++++++++++++|" << endl;
		
		while(true)
		{
			cout << "Your input: ";
			cin >> input;
			if(cin.good())
			{
				break;
			}
			else
			{
				cout << "You did not enter a numeric value..try again.\n";
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
		}
		cin.ignore(1000, '\n');
		input = round(input);

		switch(input)
		{
			case 1:
				cout << "\nYou chose to add a record to the system\n";
				acct.insert_record();
				break;
			case 2:
				cout << "\nYou chose to display the current record information\n";
				acct.display_data();
				break;
			case 3:
				cout << "\nYou chose to search for a record in the system\n";
				acct.search_record();
				break;
			case 4:
				cout << "\nYou chose to update a record in the system\n";
				acct.update_record();
				break;
			case 5:
				cout << "\nYou chose to delete a record in the system\n";
				acct.delete_record();
				break;
			case 6:
				cout << "\nThank you for using the system. Good bye.\n";
				active = false;
				break;
			default:
				cout << "\nInput not understood\n";
		}
	}
	return 0;
}