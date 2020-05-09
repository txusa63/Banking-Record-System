#ifndef ACCOUNT_H
#define ACCOUNT_H

class Account 
{
private:
	int account_number;
	std::string first_name;
	std::string last_name;
	double balance;
	bool current = false;
public:
	static int callback1(void *count, int argc, char **argv, char **azColName);
	static int callback2(void *count, int argc, char **argv, char **azColName);
	void get_data();
	int verify_account();
	void display_data();
	void insert_record();
	void search_record();
	void update_record();
	void delete_record();
};

#endif