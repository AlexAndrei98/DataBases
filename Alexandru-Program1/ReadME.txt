Alexandru Andrei
September 12th, 2017

Developed on macOS Sierra 10.12.6 using Xcode 8.3.3

BASEBALL TEAM STATISTICS TRACKER ⚾️🥇🥈🥉

COMPILING THE PROJECT
1 - Open the terminal window
2 - Navigate to where the main.cpp file is located 
3 - Type in the following command to compile: 
	make main 
4 - Type in the following command to run: 
	./main
5 - To give a text file as an input, the text file has to be located in the same folder as main.cpp
6 - Type in the following command to give a text file (input.txt) as an input: 
	./main < input.txt

PROGRAM TESTS.
-I tested the program and it runs with the cases provided receiving the same result.
Furthermore I tested all the cases including if team had the same percentage score and total points and it seems like the sorting function sorts the team accordingly. 
-The program was tested with two running versions at the same time and the data file gets updated according to instructions. 

POSSIBLE ISSUES. 
-The only limiting factor I have set is the number of teams has to be maximum 50 since it is the size of the array used to store the teams when I need to sort. However, this constraint can be changed in line 151.

-There might be some formatting inconsistencies based on the length of the name of teams.

-Whenever using a input.txt file as input the first line of every cout to the console has:

>>> {output line of code}
{output line of code}
{output line of code}
etc…

-When inputing to the command line by hand the above does not present issue present.

-When running the command: ./main < input.txt twice in a row, whenever printing out to the console there will be a duplicate of every item displayed.


PROGRAM APPROACH
-In the main function I parse through the input string and call the appropriate functions when needed. 
-I have used switch cases to call the correct functions. 
-When prompted to add a city or team I called the function AddElement() 
-When prompted to  add a game I call the function AddGame(). 
-When prompted to list a city, team or game I call List(). 
-When prompted to get the stats for a specific team I call getStats(). 
-When prompted to get the listing of the entire set of teams I use getStandings();