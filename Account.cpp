#include <iostream>
#include <string>
#include <cctype>
#include <sqlite3.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <cfloat>
#include <stdlib.h>
#include "Account.h"
#include <cmath>

using std::string;
using std::cout;
using std::cin;
using std::endl;

char check_table_existence;
int records = 0;
bool found = false;


int Account::callback1(void *data, int argc, char **argv, char **azColName)
{
	for(int i=0; i<argc; i++)
	{
		if(strcmp(*azColName, "count(type)") == 0)
		{
			if(*argv[i] == '0')
			{
				check_table_existence = *argv[i];
			}
			if(*argv[i] == '1')
			{
				check_table_existence = *argv[i];
			}
		}
		if(strcmp(*azColName, "COUNT(*)") == 0)
		{
			sscanf(*argv, "%d", &records);

		}
		if(strcmp(*azColName, "COUNT(1)") == 0)
		{
			
			if(*argv[i] == '0')
			{
				found = false;
			}
			if(*argv[i] == '1')
			{
				found = true;
			}
		}
	}
	cout << endl;
	return 0;
}

int Account::callback2(void *data, int argc, char **argv, char **azColName) 
{
	cout << "Found the following results: \n";
	cout << "------------------------------\n";
	for(int i=0; i<argc; i++)
	{
		cout << azColName[i] << ":\t " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	cout << "------------------------------";
	cout << endl;
	return 0;
}

void Account::get_data()
{
	int account_number;
	string first_name;
	string last_name;
	double balance;
	bool completed = false;
	string prompt;

	while(completed == false)
	{
		while(true)
		{
			cout << "Enter account number of client: ";
			cin >> account_number;
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
		account_number = round(account_number);
		this->account_number = account_number;

		cout << "Enter first name of client: ";
		getline(cin, first_name);
		this->first_name = first_name;

		cout << "Enter last name of client: ";
		getline(cin, last_name);
		this->last_name = last_name;

		while(true)
		{
			cout << "Enter account balance of client: ";
			cin >> balance;
			if(cin.good())
			{
				break;
			}
			else
			{
				cout << "You did not enter a numeric value..try again.\n";
				cin.clear();
				cin.ignore(100000, '\n');
			}
		}
		this->balance = balance;
		cin.ignore();

		current = true;

		while(true)
		{
			cout << "Is the information entered above correct?\n";
			cout << "Enter yes or no: ";
			getline(cin, prompt);
			if(tolower(prompt[0]) == 'y' && tolower(prompt[1]) == 'e' && tolower(prompt[2]) == 's')
			{
				completed = true;
				break;
			}
			else if(tolower(prompt[0]) == 'n' && tolower(prompt[1]) == 'o')
			{
				cout << "Enter the information again: \n";
				break;
			}
			else
			{
				cout << "The response was not understood. Try again.\n";
			}
		}
	}
}

int Account::verify_account()
{
	int account_number;
	string command;
	const char *sql1;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *line;

	rc = sqlite3_open("data.db", &db);
	if(rc)
	{
		cout << "Cannot open the database: " << sqlite3_errmsg(db) << endl;
	}

	sql1 = "SELECT COUNT(*) FROM account";
	rc = sqlite3_exec(db, sql1, callback1, 0, &zErrMsg);
	cout << "The total number of records is: " << records << endl;

	while(true)
	{
		cout << "Enter account number: ";
		cin >> account_number;
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
	account_number = round(account_number);

	command = "SELECT COUNT(1) FROM account WHERE Account_Number = " + std::to_string(account_number);
	line = command.c_str();
	rc = sqlite3_exec(db, line, callback1, 0, &zErrMsg);

	if(found == true)
	{
		return account_number;
	}
	else
	{
		return 0;
	}
	sqlite3_close(db);
}

void Account::display_data()
{
	if(current == false)
	{
		cout << "No current record available...try adding a record\n";
	}
	else
	{
		cout << "\nThe current information is: \n";
		cout << "------------------------------\n";
		cout << "Account Number: " << account_number << endl;
		cout << "First Name: " << first_name << endl;
		cout << "Last Name: " << last_name << endl;
		cout << "Current Balance: $" << balance << endl;
		cout << "------------------------------\n";
	}
}

void Account::insert_record()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	const int STATEMENTS = 6;
	const char *sql[STATEMENTS];
	const char *sql1;
	const char *line;
	string command;
	int counter = 0;
	int count = 0;
	int rc;

	get_data();

	rc = sqlite3_open("data.db", &db);
	if(rc)
	{
		cout << "Cannot open the database: " << sqlite3_errmsg(db) << endl;
	}
	else
	{
		cout << "Database opened successfully\n";
	}

	sql1 = "select count(type) from sqlite_master where type='table' and name='account'"; 
	rc = sqlite3_exec(db, sql1, callback1, 0, &zErrMsg);

	if(check_table_existence == '0')
	{
		sql[counter] = "CREATE TABLE IF NOT EXISTS account(Account_Number INT PRIMARY KEY NOT NULL, First_Name varchar(30) NOT NULL, Last_Name varchar(30) NOT NULL, Balance real NOT NULL)";
		counter++;
	}

	command = "INSERT INTO account (Account_Number, First_Name, Last_Name, Balance) VALUES (" + std::to_string(account_number) + ", '" + first_name + "', '" + last_name + "', " + std::to_string(balance) + ")";
	line = command.c_str();

	sql[counter] = line;
	counter++;

	for(int i=0; i<counter; i++)
	{
		rc = sqlite3_exec(db, sql[i], callback1, 0, &zErrMsg);
		if(rc != SQLITE_OK)
		{
			cout << "SQL error: " << sqlite3_errmsg(db) << endl;
			sqlite3_free(zErrMsg);
			break;
		}
	}

	sqlite3_close(db);
}

void Account::search_record()
{
	int account_number;
	string command;
	const char *sql1;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *line;

	account_number = verify_account();

	if(account_number != 0)
	{
		rc = sqlite3_open("data.db", &db);
		if(rc)
		{
			cout << "Cannot open the database: " << sqlite3_errmsg(db) << endl;
		}
		command = "SELECT * FROM account WHERE Account_Number = " + std::to_string(account_number);
		line = command.c_str();
		rc = sqlite3_exec(db, line, callback2, 0, &zErrMsg);
	}
	else 
	{
		cout << "Error: No record found!\n";
	}
	sqlite3_close(db);
}

void Account::update_record()
{
	int account_number;
	string first_name;
	string last_name;
	double balance;
	sqlite3 *db;
	char *zErrMsg = 0;
	const char *sql1;
	const char *line;
	string command;
	string action;
	int rc;
	int returned_value;

	returned_value = verify_account();

	if(returned_value <= 0)
	{
		cout << "Error: No record found!\n";
	}
	else
	{
		cout << "Enter changes below: " << endl;

		while(true)
		{
			cout << "Enter account number of client: ";
			cin >> account_number;
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
		account_number = round(account_number);
		action = std::string("Account_Number") + " = " + std::to_string(account_number) + ", ";

		cout << "Enter first name of client: ";
		getline(cin, first_name);
		action = action + "First_Name" + " = '" + first_name + "'" + ", ";

		cout << "Enter last name of client: ";
		getline(cin, last_name);
		action = action + "Last_Name" + " = '" + last_name + "'" + ", ";
		
		while(true)
		{
			cout << "Enter account balance of client: ";
			cin >> balance;
			if(cin.good())
			{
				break;
			}
			else
			{
				cout << "You did not enter a numeric value..try again.\n";
				cin.clear();
				cin.ignore(100000, '\n');
			}
		}
		action = action + "Balance" + " = " + std::to_string(balance);
		cin.ignore();

		rc = sqlite3_open("data.db", &db);
		if(rc)
		{
			cout << "Cannot open the database: " << sqlite3_errmsg(db) << endl;
		}

		sql1 = "select count(type) from sqlite_master where type='table' and name='account'"; 
		rc = sqlite3_exec(db, sql1, callback1, 0, &zErrMsg);

		command = "UPDATE account SET " + action + " WHERE Account_Number = " + std::to_string(returned_value) + ";";
		line = command.c_str();

		rc = sqlite3_exec(db, line, callback1, 0, &zErrMsg);
		cout << "Record updated!\n";
		sqlite3_close(db);
	}
}

void Account::delete_record()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	const char *line;
	string command;
	int rc;

	int returned_value = verify_account();

	if(returned_value <= 0)
	{
		cout << "Error: No record found!\n";
	}

	else
	{
		rc = sqlite3_open("data.db", &db);
		if(rc)
		{
			cout << "Cannot open the database: " << sqlite3_errmsg(db) << endl;
		}

		command = "DELETE from account WHERE Account_Number = " + std::to_string(returned_value);
		line = command.c_str();

		rc = sqlite3_exec(db, line, callback1, 0, &zErrMsg);
		cout << "Record deleted!\n";
		sqlite3_close(db);
	}
}


