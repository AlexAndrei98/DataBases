#include <iostream>
#include <string>

#include <my_global.h>
#include <mysql.h>

#include <conio.h>

using namespace std;

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

	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;

	cout << "Enter MySQL database username:";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password = myget_passwd();

	// could also prompt for this, if desired
	db_name = db_user;


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


	// now, send mysql our query ...
	int status;


	/*
	string myQuery = "select * from brandNewTable";
	cout << "What is the author name?";
	string authName;
	cin >> authName;

	myQuery += " where Name= \"";
	myQuery += authName + "\"";
	*/

	string myQuery = "insert into brandNewTable values(\'";
	
	string bookName;
	string authName;
	string ISBN;

	cout << "insert book Name" << endl;
	cin >> bookName;
	
	myQuery += bookName;
	myQuery += "','";

	cout << "insert auth Name" << endl;
	cin >> authName;

	myQuery += authName;
	myQuery += "',";

	cout << "insert ISBN" << endl;
	cin >> ISBN;

	myQuery += ISBN;
	myQuery += ");";


	
	cout << "The Query is:" << myQuery << endl;

	cout << "Sending query ..."; cout.flush();
	status = mysql_query(conn, myQuery.c_str());
	cout << "Done" << endl;

	// if the query didn't work ...
	if (status != 0)
	{
		// ... explain why ...
		cout << mysql_error(&mysql) << endl;
		return 1;  // ... and exit program
	}
	else
		cout << "Query succeeded" << endl;

	//exit(0); // uncomment to keep create table ....



	// get the query result(s)
	res = mysql_store_result(conn);

	// go through each line (row) of the answer table
	/*for (row = mysql_fetch_row(res);
		row != NULL;
		row = mysql_fetch_row(res))
	{

		// print out the first 2 colums; they are stored in
		//    an "array-like" manner
		cout << row[2] << "  " << row[1] << endl;
	}*/

	// clean up the query
	mysql_free_result(res);

	// clean up the connection
	mysql_close(conn);

	/*
	cout << "Press the any key (wherever that is) ...";
	char ch;
	cin >> ch;
	*/

	return 0;
}