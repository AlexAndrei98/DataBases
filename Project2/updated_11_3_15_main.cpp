#include <iostream>
#include <string>
#include <iomanip> 
#include <sstream>

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
int UpdateRecordsNeg(string, int, int, int, int, MYSQL*, MYSQL);
int teamStandings(string, MYSQL*, MYSQL);
int deleteGames(string, MYSQL*, MYSQL);
int deleteTeam(string, MYSQL*, MYSQL);
int deleteRecord(string, MYSQL*, MYSQL);
int moveCity(string, string, MYSQL*, MYSQL);
int allTeamsStandings(MYSQL*, MYSQL);
int UpdateDeletedGamesRecords(string, MYSQL*, MYSQL);

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
	
	// If error creating table
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
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
	// If error creating table
	if (status != 0) {
		// Print error message 
		cout << mysql_error(&mysql) << endl;
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
	// If error creating table
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//-------------- create the records table------------------------ 
	myQuery = "create table if not exists ";
	myQuery += recordsTable;
	myQuery += " (team1 char(100) NOT NULL, win int, losses int, PointsF int, PointsA int,  winPercentage float ,";
	myQuery += "PRIMARY KEY(team1), ";
	myQuery += "FOREIGN KEY(team1) REFERENCES ";
	myQuery += teamTable;
	myQuery += " (teamName)";
	myQuery += ");";
	status = mysql_query(conn, myQuery.c_str());
	// If error creating table
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}


	//------------------------------ END OF SQL COMMANDS---------------------------------------

	bool quit = false;
	char input;
	char input2;

	while (!quit) {

		//show console input 
		cout << ">>> ";
		//read in the first command
		cin >> input;

		//initialize variables
		string cityCode;
		string cityName;
		string standingTeam;
		string team1;
		int score1;
		string team2;
		int score2;
		int win = 0;
		int zero = 0;

		//switch input for the command cases
		switch (input)
		{
		case 'a':
			//read in the next command
			cin >> input2;
			//read whitespace
			cin.get();
			switch (input2)
			{
				//adding a city
				case 'c':
				// read city code
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
				}
				break;
				//adding a team
				case 't':
					//read the city code
					cin >> cityCode;
					//get white space after the city code
					cin.get();
					//get the team name 
					getline(cin, cityName);
					//add a team to the database
					status = AddTeam(cityCode, cityName, conn, mysql);
					//check if the function gets an error
					if (status != 0) {
						// Print error message
						cout << mysql_error(&mysql) << endl;
					}
					//create an initial record for the added team
					status = AddInitialRecord(cityName, conn, mysql);
					//check if the function gets an error
					if (status != 0) {
						// Print error message
						cout << mysql_error(&mysql) << endl;
					}
					break;
				//adding a game
				case 'g':
					//get the team name
					cin >> team1;
					//get white space after the city code
					cin.get();
					//get the team score 
					cin >> score1;
					//get the white space
					cin.get();
					//get the team name
					cin >> team2;
					//get the white space
					cin.get();
					//get the tea score
					cin >> score2;
					//add the game to the databse
					status = AddGame(team1, score1, team2, score2, conn, mysql);
					//check the status of the function
					if (status != 0) {
						// Print error message
						cout << mysql_error(&mysql) << endl;
					}
					//check who wins in the game
					if (score1 > score2) {
						//update the wins
						win++;
						//update records of the first team
						status = UpdateRecords(team1, score1,score2, win,zero, conn, mysql);
						//update records of the second team
						status = UpdateRecords(team2, score2, score1, zero, win, conn, mysql);
						//reset wins
						win = 0;
					}
					else {
						//update the wins
						win++;
						//update records of the first team
						status = UpdateRecords(team1, score1, score2, zero, win, conn, mysql);
						//update records of the second team
						status = UpdateRecords(team2, score2, score1, win, zero, conn, mysql);
						//reset wins
						win = 0;
					}

					break;
			}
			break;
		//listing switch
		case 'l':
			//reading the second command
			cin >> input2;
			//listing function
			status = list(input2, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			break;
		//record function of a team
		case 'r':
			//read in the standing team
			cin >> standingTeam;
			//get the standings 
			status = teamStandings(standingTeam, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;			}
			break;
		//standing function of all teams
		case 's':
			//get all of the standings
			status = allTeamsStandings( conn, mysql);
			//check the status of the funcion
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			break;
		//delete function
		case 'd':
			//get the team name 
			cin>>cityName;
			//update the records 
			status = UpdateDeletedGamesRecords(cityName, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//delete games 
			status = deleteGames(cityName, conn, mysql);
			//update the record of the team
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//delete the record of the team
			status = deleteRecord(cityName, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//delete the team from the team table
			status = deleteTeam(cityName, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			break;
		//move a city
		case 'm':
			//read in the city code
			cin >> cityCode;
			//get the whitespace
			cin.get();
			//read in the city
			cin >> standingTeam;
			//move the city command
			status = moveCity(cityCode,standingTeam, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
				//return 1;
			}
			break;
		//quite command
		case 'q':
			quit = true;
			cout << "[Goodbye.]";
			break;
			// if there is an error
		default:
			cout << "I do not understand \"" << input << "\".";
			cout << endl;
			break;
		}


	}
	
	return 0;
}

int AddCity(string cityCode, string cityName, MYSQL *conn, MYSQL mysql) {
	
	//initialize status
	int status;
	//creating the SQL query 
	string myQuery = "insert into ";
	myQuery += citiesTable;
	myQuery += " VALUES('";
	myQuery += cityCode;
	myQuery += "',  '";
	myQuery += cityName;
	myQuery += "');";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	//return status
	return status;
}

int AddTeam(string cityCode, string teamName, MYSQL *conn, MYSQL mysql) {
	
	//initialize status
	int status;
	//creating the SQL query 
	string myQuery = "insert into ";
	myQuery += teamTable;
	myQuery += " VALUES('";
	myQuery += cityCode;
	myQuery += "',  '";
	myQuery += teamName;
	myQuery += "');";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int AddInitialRecord(string teamName, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	int wins=0;
	int losses = 0;
	int PA = 0;
	int PF = 0;
	int winPercentage = 0;
	//creating the SQL query 
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
	myQuery += "',  '";
	myQuery += to_string(winPercentage);
	myQuery += "');";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int UpdateRecords(string teamName, int pointsFor, int pointsAgainst, int win, int zero, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int currentFor = 0;
	int currentAgainst = 0;
	int currentWins = 0;
	int currentLosses = 0;
	float currentPercentage;
	//creating the SQL query 
	string myQuery = "select * from table_r where team1 = '" + teamName + "';";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	//check if query worked
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//reset status
	status = 0;


	//get the result
	res = mysql_store_result(conn);


	//going thorugh the row
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		//get the stats of a team
		currentWins = atoi(row[1]);
		currentLosses = atoi(row[2]);
		currentFor = atoi(row[3]);
		currentAgainst = atoi(row[4]); 
	}
	//clearing up the result
	mysql_free_result(res);

	//update the stats of the team
	currentWins += win;
	currentLosses += zero;
	currentFor += pointsFor;
	currentAgainst += pointsAgainst;
	currentPercentage = (double)currentWins / (double)(currentWins + currentLosses);
	
	//changing the float to a string 
	stringstream winString;
	winString << fixed<< setprecision(5) << currentPercentage;
	string winP = winString.str();
	//create the SQL query to update the records
	myQuery = "update ";
	myQuery += recordsTable;
	myQuery += " set ";
	myQuery += "win = " + to_string(currentWins) + ",";
	myQuery += "losses = " + to_string(currentLosses) + ",";
	myQuery += "PointsF = " + to_string(currentFor) + ",";
	myQuery += "PointsA = " + to_string(currentAgainst) + ",";
	myQuery += "winPercentage = " + winP;
	myQuery += " where team1 = '" + teamName + "' ;";
	//send the query 
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int UpdateRecordsNeg(string teamName, int pointsFor, int pointsAgainst, int win, int zero, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int currentFor = 0;
	int currentAgainst = 0;
	int currentWins = 0;
	int currentLosses = 0;
	float currentPercentage;
	//create the sql command as a string
	string myQuery = "select * from table_r where team1 = '" + teamName + "';";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	//check if query worked
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//reset the status
	status = 0;
	//store the SQL result
	res = mysql_store_result(conn);

	//go thorugh the result
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		//get the current stats of the team
		currentWins = atoi(row[1]);
		currentLosses = atoi(row[2]);
		currentFor = atoi(row[3]);
		currentAgainst = atoi(row[4]);
	}
	//clearing up the result
	mysql_free_result(res);
	//update the stats for the team
	currentWins -= win;
	currentLosses -= zero;
	currentFor -= pointsFor;
	currentAgainst -= pointsAgainst;
	currentPercentage = (double)currentWins / (double)(currentWins + currentLosses);
	//changing the float to a string 
	stringstream winString;
	winString << fixed << setprecision(5) << currentPercentage;
	string winP = winString.str();
	//create SQL query 
	myQuery = "update ";
	myQuery += recordsTable;
	myQuery += " set ";
	myQuery += "win = " + to_string(currentWins) + ",";
	myQuery += "losses = " + to_string(currentLosses) + ",";
	myQuery += "PointsF = " + to_string(currentFor) + ",";
	myQuery += "PointsA = " + to_string(currentAgainst) + ",";
	myQuery += "winPercentage = " + winP;
	myQuery += " where team1 = '" + teamName + "' ;";
	//check if the query had an error
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int teamStandings(string teamName, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//create SQL query
	string myQuery = "select * from table_r where team1 = '" + teamName + "';";

	status = mysql_query(conn, myQuery.c_str());
	//check if query worked
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//get the result
	res = mysql_store_result(conn);
	//print out the standings 
	cout << "__________________________________________________________________" << endl;
	cout << setw(0)<<"Teams"<<setw(10)<<"tWins"<<setw(10)<< "Losses" <<setw(10)<<"PF"<<setw(10)<<"PA" << endl;
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		cout << setw(0)<< row[0] << setw(10) << row[1] << setw(10) << row[2] << setw(10) << row[3] << setw(10) << row[4] << endl;
	}
	return status;
}

int AddGame(string team1, int score1, string team2, int score2, MYSQL *conn, MYSQL mysql) {
	//initialize status
	int status;
	//create SQL query
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
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int list(char input2, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	string myQuery;
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//different commands for different kind of listing
	switch (input2){
	case('c'):
		myQuery = "select * from " + citiesTable + " ;";
		break;
	case('t'):
		myQuery = "select CityName, teamName from table_c, table_t where table_c.cityCode = table_t.cityCode; ";
		break;
	case('g'):
		myQuery = "select * from " + gamesTable + " ;";
		break;
	default:
		cout << "Wrong query" << endl;
		break;
	}
	status = mysql_query(conn, myQuery.c_str());
	//check if query worked
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	// Get results
	res = mysql_store_result(conn);
	//print the result from the SQL query
	switch (input2){
		//print the cities
	case('c'):
		for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
			cout << setw(20) << row[0] << setw(0)<< row[1] <<  endl;
		}
		break;
	case('t'):
		//print the teams
		for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
			cout << setw(20) << row[0] << setw(40) << row[1] <<  endl;
		}
		break;
	case('g'):
		//print the games
		for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
			cout << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(20) << row[3] << endl;
		}
		break;
	default:
		cout << "Wrong query" << endl;
		break;
	}
	//free the result
	mysql_free_result(res);
	return status;
}

int deleteGames(string teamName, MYSQL *conn, MYSQL mysql) {
	//initialize status
	int status;
	//create SQL query
	string myQuery = "delete from ";
	myQuery += gamesTable;
	myQuery += " where team1 = '";
	myQuery += teamName;
	myQuery += "' or team2 = '";
	myQuery += teamName;
	myQuery += "';";
	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int deleteTeam(string teamName, MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	string cityCode;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//create SQL query to get the ciry code
	string dropPK = "select cityCode from " + teamTable + " where teamName=\"";
	dropPK += teamName;
	dropPK += "\";";
	//send the query to the sql server
	status = mysql_query(conn, dropPK.c_str());
	//check the status of the query
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//store the query result
	res = mysql_store_result(conn);
	//get the city code of the team
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		cityCode = row[0];

	}
	//initiate the query to delete the team from the database
	string myQuery = "delete from ";
	myQuery += teamTable;
	myQuery += " where teamName = \"";
	myQuery += teamName;
	myQuery += "\" and cityCode =  \"";
	myQuery += cityCode;
	myQuery += "\";";
	
	// Send the query to the database
	status = mysql_query(conn, myQuery.c_str());

	return status;
}

int deleteRecord(string teamName, MYSQL *conn, MYSQL mysql) {
	//initialize status
	int status;
	//creating the SQL query to delete the record
	string myQuery = "delete from ";
	myQuery += recordsTable;
	myQuery += " where team1 = '";
	myQuery += teamName;
	myQuery += "';";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int moveCity(string cityCode, string cityName, MYSQL *conn, MYSQL mysql) {
	//initialize status
	int status;
	//creating the SQL query 
	string myQuery = "update ";
	myQuery += teamTable;
	myQuery += " set ";
	myQuery += " cityCode ='";
	myQuery += cityCode;
	myQuery += "' where teamName = '";
	myQuery += cityName;
	myQuery += "' ;";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	return status;
}

int allTeamsStandings(MYSQL *conn, MYSQL mysql) {
	//initialize variables
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//create sql query to get all team standings
	string myQuery = "select team1,win,losses, PointsF, PointsA from table_r order by winPercentage,PointsF,team1 ASC;";
	//send  the query to the server
	status = mysql_query(conn, myQuery.c_str());
	//check the status of the function
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
		//return 1;
	}
	//store the result of the sql query
	res = mysql_store_result(conn);
	//go thorugh the result
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		//print the team and their statistics 
		cout << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << row[4] << endl;
	}
	return status;
}

int UpdateDeletedGamesRecords(string teamName, MYSQL *conn, MYSQL mysql) {

	//initialize variables
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int currentFor = 0;
	int currentAgainst = 0;
	int currentWins = 0;
	int currentLosses = 0;
	float currentPercentage;
	//create sql query
	string myQuery = "select * from table_g where team1 = '" + teamName + "';";
	//send  the query to the server
	status = mysql_query(conn, myQuery.c_str());
	//check the status of the function
	if (status != 0) {
		// Print error message
		cout << mysql_error(&mysql) << endl;
	}
	//store the result of the query
	res = mysql_store_result(conn);

	//go thorugh the result
	for (row = mysql_fetch_row(res); row != NULL; row = mysql_fetch_row(res)) {
		//get the current stats of the team
		int score1= atoi(row[1]);
		int score2 = atoi(row[3]);
		string team2 = row[2];
		string team1 = teamName;
		//reset wins and losses
		int win = 0;
		int zero = 0;

		//chech which team won
		if (score1 > score2) {
			win++;
			//make sure the teams have a standings
			status = teamStandings(team1, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//store the result of the query
			status = teamStandings(team2, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//update standings of the teams
			status = UpdateRecordsNeg(team1, score1, score2, win, zero, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//update standings of the teams
			status = UpdateRecordsNeg(team2, score2, score1, zero, win, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//reset wins
			win = 0;
		}
		else {
			//make sure the teams have a standings
			status = teamStandings(team1, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//make sure the teams have a standings
			status = teamStandings(team2, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//update wins
			win++;
			//update standings of the teams
			status = UpdateRecordsNeg(team1, score1, score2, win, zero, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//update standings of the teams
			status = UpdateRecordsNeg(team2, score2, score1, win, zero, conn, mysql);
			//check the status of the function
			if (status != 0) {
				// Print error message
				cout << mysql_error(&mysql) << endl;
			}
			//reset wins
			win = 0;
		}

	}



	return status;
}