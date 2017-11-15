#include <iostream>
#include <string>

#include <my_global.h>
#include <mysql.h>

#include <conio.h>

using namespace std;

int AddCity(string, string, MYSQL*, MYSQL);
int AddTeam(string, string, MYSQL*, MYSQL);

string citiesTable = "table_c";
string teamTable = "table_t";
string gamesTable = "table_g";

// reads in a password without echoing it to the screen
string myget_passwd()
{
	string passwd;

	for (;;)
	{
		char ch;
		ch = _getch();           // get char WITHIOUT echo!
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}

int main()
{


	// mysql connection and query variables
	MYSQL *conn, // actual mysql connection
		mysql;   // local mysql data
	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // one row of a table (result)

					// strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;

	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	mysql_init(&mysql);
	cout << "Done!" << endl;

	cout << "Done!" << endl;
	db_host = "localhost";
	db_user = "andreia";
	db_password = "07261111";
	db_name = "andreia";
	// go out and connect to the mysql server
	cout << "Connecting to remote DB ..."; cout.flush();
	conn = mysql_real_connect(&mysql,
		db_host.c_str(), db_user.c_str(), db_password.c_str(), db_user.c_str(),
		0, 0, 0); // last three are usually 0's

	// if we couldn't setup the connection ...
	if (conn == NULL)
	{
		// print out the error message as to why ...
		cout << mysql_error(&mysql) << endl;
		return 1; // ... and exit the program. 
	}
	else
		cout << "DB connection established" << endl;

	int status;
	//-------------- create the cities table------------------------ 
	string myQuery = "create table if not exists ";
	myQuery += citiesTable;
	myQuery += " (cityCode char(100) NOT NULL, CityName char(100) NOT NULL, ";
	myQuery += "PRIMARY KEY(cityCode)); ";
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1;
	}
	
	//-------------- create the team table------------------------ 
	myQuery = "create table if not exists ";
	myQuery += teamTable;
	myQuery += " (cityCode char(100) NOT NULL, teamName char(100) NOT NULL,  ";
	myQuery += "PRIMARY KEY(cityCode, teamName), ";
	myQuery += "FOREIGN KEY(cityCode) REFERENCES ";
	myQuery += citiesTable;
	myQuery += " (cityCode) ";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1;
	}


	//-------------- create the team table------------------------ 
	myQuery = "create table if not exists ";
	myQuery += gamesTable;
	myQuery += " (team1 char(100) NOT NULL, score1 int, team2 char(100) NOT NULL, score2 int, ";
	myQuery += "PRIMARY KEY(team1, score1, team2, score2 ), ";
	myQuery += "FOREIGN KEY(team1) REFERENCES ";
	myQuery += teamTable;
	myQuery += "(cityCode),";
	myQuery += "FOREIGN KEY(team2) REFERENCES ";
	myQuery += teamTable;
	myQuery += " (cityCode) ";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1;
	}
	//------------------------------ END OF SQL COMMANDS---------------------------------------

	bool quit = false;
	char input;
	char input2;

	while (!quit) {

		cout << ">>> ";
		cin >> input;
		string cityCode;
		string cityName;

		switch (input)
		{

		case 'a':
			cin >> input2;
			cin.get();
			switch (input2)
			{
				case 'c':
				cin >> cityCode;
				//get white space after the city code
				cin.get();
				//get the team name 
				getline(cin, cityName);
				//get the status of the function
				status = AddCity(cityCode, cityName, conn, mysql);
				if (status != 0) {
					// Print error message and quit
					cout << mysql_error(&mysql) << endl;
					return 1;
				}
				break;

				case 't':
					cin >> cityCode;
					//get white space after the city code
					cin.get();
					//get the team name 
					getline(cin, cityName);
					//get the status of the function
					status = AddTeam(cityCode, cityName, conn, mysql);
					if (status != 0) {
						// Print error message and quit
						cout << mysql_error(&mysql) << endl;
						return 1;
					}
					break;
			}
			break;

		case 'q':
			quit = true;
			cout << "[Goodbye.]";
			break;
			//                if there is an error
		default:
			cout << "I do not understand \"" << input << "\".";
			cout << endl;
			break;
		}


	}
	
	return 0;
}

int AddCity(string cityCode, string cityName, MYSQL *conn, MYSQL mysql) {
	
	int status;
	string myQuery = "insert into ";
	myQuery += citiesTable;
	myQuery += " VALUES(' ";
	myQuery += cityCode;
	myQuery += " ',  ' ";
	myQuery += cityName;
	myQuery += " ');";
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());

	return status;
}

int AddTeam(string cityCode, string teamName, MYSQL *conn, MYSQL mysql) {

	int status;
	string myQuery = "insert into ";
	myQuery += teamTable;
	myQuery += " VALUES(' ";
	myQuery += cityCode;
	myQuery += " ',  ' ";
	myQuery += teamName;
	myQuery += " ');";

	cout << myQuery << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());

	return status;
}