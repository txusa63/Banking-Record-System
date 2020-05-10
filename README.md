# Banking-Record-System
A console C++ Banking Record System that works with a SQLite database.
The user is able to create, read, update, and delete records in the database via the terminal. 

Create a record with the user's information by using a unique account number. If the chosen account number is already in use with another account, the system will not register the new account.

Search for an existing record by inputting an account number. The system will return the account information associated with that account number. If no record is present in the database, the system will return an error.

Update an existing record by inputting an account number. The system will then let you update the necessary fields. If no record is present in the database, the system will return an error.

Delete an existing record by inputting an account number. If no record is present in the database, the system will return an error.

To run the program, compile with the following
```
g++ -std=c++11 source.cpp Account.cpp -l sqlite3
```

then execute with

```
./a.out
```
