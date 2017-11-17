#include <iostream>
#include <string>
#include <iomanip> 


#include <my_global.h>
#include <mysql.h>

#include <conio.h>

using namespace std;

int AddCity(string, string, MYSQL*, MYSQL);
int AddTeam(string, string, MYSQL*, MYSQL);
int AddGame(string, int, string, int, MYSQL*, MYSQL);
int AddInitialRecord(string, MYSQL*, MYSQL);
int list(char, MYSQL*, MYSQL);
int UpdateRecords(string, int, int, int, int, MYSQL*, MYSQL);
int teamStandings(string, MYSQL*, MYSQL);
string citiesTable = "table_c";
string teamTable = "table_t";
string gamesTable = "table_g";
string recordsTable = "table_r";

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
	cout << endl;
	cout << myQuery << endl;
	cout << endl;
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
	myQuery += "PRIMARY KEY(teamName,cityCode), ";
	myQuery += "FOREIGN KEY(cityCode) REFERENCES ";
	myQuery += citiesTable;
	myQuery += " (cityCode) ";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());
	cout << endl;
	cout << myQuery << endl;
	cout << endl;	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1;
	}


	//-------------- create the games table------------------------ 
	myQuery = "create table if not exists ";
	myQuery += gamesTable;
	myQuery += " (team1 char(100) NOT NULL, score1 int, team2 char(100) NOT NULL, score2 int, ";
	//myQuery += "PRIMARY KEY(team1, score1, team2, score2 ), ";
	myQuery += "FOREIGN KEY(team1) REFERENCES ";
	myQuery += teamTable;
	myQuery += " (teamName),";
	myQuery += "FOREIGN KEY(team2) REFERENCES ";
	myQuery += teamTable;
	myQuery += " (teamName) ";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());
	cout << endl;
	cout << myQuery << endl;
	cout << endl;	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1;
	}
	//-------------- create the records table------------------------ 
	myQuery = "create table if not exists ";
	myQuery += recordsTable;
	myQuery += " (team1 char(100) NOT NULL, win int, losses int, PointsF int, PointsA int, ";
	myQuery += "FOREIGN KEY(team1) REFERENCES ";
	myQuery += teamTable;
	myQuery += " (teamName)";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());
	cout << endl;
	cout << myQuery << endl;
	cout << endl;	// If error creating table
	if (status != 0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		//return 1;
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
		string standingTeam;
		string team1;
		int score1;
		string team2;
		int score2;

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
					// Print error message
					cout << mysql_error(&mysql) << endl;
					//return 1;
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
						// Print error message
						cout << mysql_error(&mysql) << endl;
						//return 1;
					}

					status = AddInitialRecord(cityName, conn, mysql);
					if (status != 0) {
						// Print error message
						cout << mysql_error(&mysql) << endl;
						//return 1;
					}
					break;

				case 'g':
					cin >> team1;
					//get white space after the city code
					cin.get();
					//get the team name 
					cin >> score1;
					cin.get();
					cin >> team2;
					cin.get();
					cin >> score2;
					//get the status of the function
					status = AddGame(team1, score1, team2, score2, conn, mysql);
					if (status != 0) {
						// Print error message
						cout << mysql_error(&mysql) << endl;
						//return 1;
					}
					int win = 0;
					int zero = 0;

					if (score1 > score2) {
						win++;
						
						status = UpdateRecords(team1, score1,score2, win,zero, conn, mysql);
						status = UpdateRecords(team2, score2, score1, zero, win, conn, mysql);
						win = 0;
					}
					else {
						win++;
						status = UpdateRecords(team1, score1, score2, zero, win, conn, mysql);
						status = UpdateRecords(team2, score2, score1, win, zero, conn, mysql);
						win = 0;
					}

					break;
			}
			break;

		case 'l':
			cin >> input2;
			status = list(input2, conn, mysql);
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
				//return 1;
			}
			break;
		case 'r':
			cin >> standingTeam;
			status = teamStandings(standingTeam, conn, mysql);
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
				//return 1;
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
	myQuery += " VALUES('";
	myQuery += cityCode;
	myQuery += "',  '";
	myQuery += cityName;
	myQuery += "');";
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());

	return status;
}

int AddTeam(string cityCode, string teamName, MYSQL *conn, MYSQL mysql) {

	int status;
	string myQuery = "insert into ";
	myQuery += teamTable;
	myQuery += " VALUES('";
	myQuery += cityCode;
	myQuery += "',  '";
	myQuery += teamName;
	myQuery += "');";


	cout << myQuery << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());



	return status;
}

int AddInitialRecord(string teamName, MYSQL *conn, MYSQL mysql) {

	int status;
	int wins=0;
	int losses = 0;
	int PA = 0;
	int PF = 0;

	//string myQuery = "insert into table_r"+
	
	string myQuery = "insert into ";
	myQuery += recordsTable;
	myQuery += " VALUES('";
	myQuery += teamName;
	myQuery += "',  '";
	myQuery += to_string(wins);
	myQuery += "',  '";
	myQuery += to_string(losses);
	myQuery += "',  '";
	myQuery += to_string(PA);
	myQuery += "',  '";
	myQuery += to_string(PF);
	myQuery += "');";
	
	cout << endl;
	cout << myQuery << endl;
	cout << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());



	return status;
}

int UpdateRecords(string teamName, int pointsFor, int pointsAgainst, int win, int zero, MYSQL *conn, MYSQL mysql) {


	//string myQuery = "insert into table_r"+
	int status;
	string myQuery = "select * from table_r where team1 = '" + teamName + "';";
	cout << endl;
	cout << myQuery << endl;
	cout << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());

	//check if query worked
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
		//return 1;
	}
	status = 0;
	MYSQL_RES *res;
	MYSQL_ROW row;
	res = mysql_store_result(conn);

	int currentFor;
	int currentAgainst;
	int currentWins;
	int currentLosses;
	
	for (row = mysql_fetch_row(res); row != NULL;
		row = mysql_fetch_row(res)) {
		cout << row[1] << " " << row[2] << " " << row[3] << " " << row[4] << endl;
		
		currentWins = atoi(row[1]);
		currentLosses = atoi(row[2]);
		currentFor = atoi(row[3]);
		currentAgainst = atoi(row[4]); 
	}
	
	cout << currentWins << " " << currentLosses << " " << currentFor << " " << currentAgainst << endl;
	//clearing up the result
	mysql_free_result(res);

	currentWins += win;
	currentLosses += zero;
	currentFor += pointsFor;
	currentAgainst += pointsAgainst;

	myQuery = "update ";
	myQuery += recordsTable;
	myQuery += " set ";
	myQuery += "win = " + to_string(currentWins) + ",";
	myQuery += "losses = " + to_string(currentLosses) + ",";
	myQuery += "PointsF = " + to_string(currentFor) + ",";
	myQuery += "PointsA = " + to_string(currentAgainst);
	myQuery += " where team1 = '" + teamName + "' ;";
	cout << "-------UPDATE RECORDSSS----------" << endl;
	cout << myQuery << endl;
	cout << "-------UPDATE RECORDSSS----------" << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());
	return status;
	
}

int teamStandings(string teamName, MYSQL *conn, MYSQL mysql) {
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;

	string myQuery = "select * from table_r where team1 = '" + teamName + "';";
	status = mysql_query(conn, myQuery.c_str());
	res = mysql_store_result(conn);
	cout << "__________________________________________________________________" << endl;
	cout << "\tTeam\t\t\t\tWins\tLosses\tPF\tPA" << endl;
	for (row = mysql_fetch_row(res); row != NULL;
		row = mysql_fetch_row(res)) {
		cout <<"\t"<< row[0] << "\t\t\t\t" << row[1] << "\t" << row[2] << "\t" << row[3] << "\t" << row[4] << endl;
	}


	return status;
}

int AddGame(string team1, int score1, string team2, int score2, MYSQL *conn, MYSQL mysql) {

	int status;
	string myQuery = "insert into ";
	myQuery += gamesTable;
	myQuery += " VALUES('";
	myQuery += team1;
	myQuery += "',  '";
	myQuery += to_string(score1);
	myQuery += "',  '";
	myQuery += team2;
	myQuery += "',  '";
	myQuery += to_string(score2);
	myQuery += "');";

	cout << myQuery << endl;
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());

	return status;
}

int list(char input2, MYSQL *conn, MYSQL mysql) {

	string myQuery;
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	switch (input2)
	{
	case('c'):
		myQuery = "select * from " + citiesTable + " ;";
		break;
	case('t'):
		myQuery = "select * from " + teamTable + " ;";
		break;
	case('g'):
		myQuery = "select * from " + gamesTable + " ;";
		break;
	default:
		cout << "Wrong query" << endl;
		break;
	}

	cout << myQuery<< endl;
	status = mysql_query(conn, myQuery.c_str());
	// Get results
	res = mysql_store_result(conn);

	switch (input2)
	{
	case('c'):
		for (row = mysql_fetch_row(res); row != NULL;
			row = mysql_fetch_row(res)) {
			cout << setw(20) << row[0] << setw(0)<< row[1] <<  endl;
		}
		break;
	case('t'):
		for (row = mysql_fetch_row(res); row != NULL;
			row = mysql_fetch_row(res)) {
			cout << setw(20) << row[0] << setw(40) << row[1] <<  endl;
		}
		break;
	case('g'):
		for (row = mysql_fetch_row(res); row != NULL;
			row = mysql_fetch_row(res)) {
			cout << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(20) << row[3] << endl;
		}
		break;
	default:
		cout << "Wrong query" << endl;
		break;
	}

	return status;

}