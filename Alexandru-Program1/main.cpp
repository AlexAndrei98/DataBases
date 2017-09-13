#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>


using namespace std;
void AddElement(char);
void List(char);
void AddGame();
string getCityName(char, char[]);
void getStats(char[]);
void getStandings();
int getPF(char[]);
int getPA(char[]);
int getWins(char[]);
int getLosses(char[]);
void printUpperLines();
void printLowerLines();



int main()
{
    //      Quit signal
    bool quit = false;
    //      User's input
    char input;
    //      team name
    char team[255];
    //      run the program until the useres triggers the quit action
    while (!quit) {
        //       Let the user know when to input
        cout << ">>> ";
        //       read the input action
        cin>> input;
        //        chose an action
        switch (input) {
                //         adding fucntion
            case 'a':
                //                receive the second input
                cin >> input;
                //                chose an action based on the input
                switch (input) {
                        //                        add a team
                    case 't':
                        //                        add a city
                    case 'c':
                        AddElement(input);
                        break;
                        //                        add a game
                    case 'g':
                        AddGame();
                        break;
                        //                        if there is an error
                    default:
                        cout<< "Input Error. Try Again" << endl;
                        break;
                }
                break;
                //          listing function
            case 'l':
                //                receive secondary input
                cin >> input;
                //                chose an action based on the input
                switch (input) {
                        //                        list a city
                    case 'c':
                        List(input);
                        break;
                        //                        list a team
                    case 't':
                        List(input);
                        break;
                        //                        list a game
                    case 'g':
                        List(input);
                        break;
                        //                        if there is an error
                    default:
                        cout<< "Input Error. Try Again" << endl;
                        break;
                }
                break;
                //             print statistics for on team function
            case'r':
                //                input the city
                cin>>team;
                printUpperLines();
                getStats(team);
                printLowerLines();
                break;
            case 's':
                //                print all of the statistics for every team
                printUpperLines();
                getStandings();
                printLowerLines();
                break;
                //                quit the program
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


void getStandings(){
    //    the function returns all of the team with their respective
    //    statistics in order of winnig percentage
    
    //    Data file
    ifstream file("database.txt", ios::in);
    
    //    single char of data
    char c;
    //    string of data
    string in;
    //    teams's city
    char city[255];
    //    team queried
    char teamQ[255];
    //    a string that holds a team name
    string TeamString;
    
    //    points socred for a team
    int PF=0;
    //    points scored against a team
    int PA=0;
    //    wins of a team
    float wins=0;
    //    losses of a team
    float losses = 0;
    //    index of the array that holds the team
    int arrayIndex=0;
    //    winning percentage of a certain team
    float indexWinPercentage=0.0;
    
    //    size of the array that holds the teams
    int sizeArray=50;
    
    //    structure for the teams that need to be sorted
    struct teamIndexed{
        float winPercentage;
        int totalPoints;
        string teamName;
    };
    
    //    array of teams
    teamIndexed teamList[sizeArray];
    
    //    go thorugh the data file
    while (file.get(c)) {
        //        if the current line is a team
        if(c == 't'){
            //            read in the information of the team
            file.get();
            file.get(city, 255, '|');
            file.get();
            file.get(teamQ, 255, '|');
            
            //            calculate the various statistics for the team found earlier
            PF = getPF(teamQ);
            PA = getPA(teamQ);
            wins = getWins(teamQ);
            losses = getLosses(teamQ);
            if(wins ==0 && losses==0){
                indexWinPercentage=0;
            }
            else
                indexWinPercentage = (wins/(wins + losses));
            
            //conver the name of the team from a Char array to string to fit the strucutre
            string TeamString(teamQ);
            
            //            populate the array with the sorting parameters of the team found earlier
            teamList[arrayIndex].teamName = TeamString;
            teamList[arrayIndex].winPercentage = indexWinPercentage*100;
            teamList[arrayIndex].totalPoints= PF;
            
            //            increase the index of the array
            arrayIndex++;
            
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;}
        
    }
    
    
    //    temporary string used for sorting puropses
    string tempIndex;
    //    index of the winning team  used for sorting puropses
    int tempIndexWin;
    //    temporary int used for sorting purposes
    int tempIndexPoints=0;
    
    //    temporary string used for comparison purposes
    string stringCompare1;
    //    temporary string used for comparison purposes
    string stringCompare2;
    //    loop thorugh the array
    for(int k=0; k<arrayIndex; k++){
        for(int i=0; i<arrayIndex; i++){
            for (int j=arrayIndex-1; j>i; j--) {
                //                comapring winning percentages
                if(teamList[j].winPercentage > teamList[j-1].winPercentage){
                    //                swpping if nexessary
                    tempIndex = teamList[i].teamName;
                    teamList[i].teamName = teamList[j].teamName;
                    teamList[j].teamName = tempIndex;
                    
                    tempIndexWin = teamList[i].winPercentage;
                    teamList[i].winPercentage = teamList[j].winPercentage;
                    teamList[j].winPercentage = tempIndexWin;
                    
                    tempIndexPoints = teamList[i].totalPoints;
                    teamList[i].totalPoints = teamList[j].totalPoints;
                    teamList[j].totalPoints = tempIndexPoints;
                    
                }
                //                comapring if two teams have the same winning percentage
                if(teamList[j].winPercentage == teamList[j-1].winPercentage){
                    //                    swap if ncessesary
                    tempIndex = teamList[j].teamName;
                    teamList[j].teamName = teamList[j-1].teamName;
                    teamList[j-1].teamName = tempIndex;
                    
                    tempIndexWin = teamList[j].winPercentage;
                    teamList[j].winPercentage = teamList[j-1].winPercentage;
                    teamList[j-1].winPercentage = tempIndexWin;
                    
                    tempIndexPoints = teamList[j].totalPoints;
                    teamList[j].totalPoints = teamList[j-1].totalPoints;
                    teamList[j-1].totalPoints = tempIndexPoints;
                    
                }
                //                comapring if two teams have the same winning percentage
                if(teamList[j].winPercentage == teamList[j-1].winPercentage){
                    //                    comparing the total points of the teams
                    if(teamList[j].totalPoints > teamList[j-1].totalPoints){
                        //                        swap if necessary
                        tempIndex = teamList[j].teamName;
                        teamList[j].teamName = teamList[j-1].teamName;
                        teamList[j-1].teamName = tempIndex;
                        
                        tempIndexWin = teamList[j].winPercentage;
                        teamList[j].winPercentage = teamList[j-1].winPercentage;
                        teamList[j-1].winPercentage = tempIndexWin;
                        
                        tempIndexPoints = teamList[j].totalPoints;
                        teamList[j].totalPoints = teamList[j-1].totalPoints;
                        teamList[j-1].totalPoints = tempIndexPoints;
                        
                    }
                }
                //                if both the team percentage and the total points of both teams are the same
                if(teamList[j].winPercentage == teamList[j-1].winPercentage && teamList[j].totalPoints == teamList[j-1].totalPoints){
                    
                    //                    convert from char array to string
                    string stringCompare1(teamList[j].teamName);
                    //                    convert from char array to string
                    string stringCompare2(teamList[j-1].teamName);
                    
                    //                    check if the strings are in alphabetical order
                    if (stringCompare1 < stringCompare2)
                    {
                        //                        swap if necessary
                        tempIndex = teamList[j].teamName;
                        teamList[j].teamName = teamList[j-1].teamName;
                        teamList[j-1].teamName = tempIndex;
                        
                        tempIndexWin = teamList[j].winPercentage;
                        teamList[j].winPercentage = teamList[j-1].winPercentage;
                        teamList[j-1].winPercentage = tempIndexWin;
                        
                        tempIndexPoints = teamList[j].totalPoints;
                        teamList[j].totalPoints = teamList[j-1].totalPoints;
                        teamList[j-1].totalPoints = tempIndexPoints;
                        
                    }
                }
                
                
            }
        }
    }
    
    //    temporary string used for converting puropses
    string teamName;
    //    temporary string used for printing
    char teamChar[255];
    
    //    loop thorugh the exact number of teams
    for (int n=0; n<arrayIndex; n++) {
        
        //        convert string back to char array
        strncpy(teamChar,teamList[n].teamName.c_str(), sizeof(teamChar));
        //        calling getStats for every team in the array
        getStats(teamChar);
        
    }
    
    
}





int getPF(char team[]){
    //    the function returns the points the team queried scored
    
    
    //    data file
    ifstream file("database.txt", ios::in);
    
    //    single string of character
    char c;
    
    string in;
    //    name of the team home
    char teamHome[255];
    //    score of the team home
    int score1;
    //    name of the team away
    char teamAway[255];
    //    score of the team away
    int score2;
    
    //    total wins for a certain team
    int totalWins=0;
    //    total losses for a certain team
    int totalLoss = 0;;
    //    points scored for a team
    int pointsScore = 0;
    //    points scored against the team
    int pointsReceived=0;;
    
    //    scan thorugh the data file
    while (file.get(c)) {
        //        check if line is a game
        if(c =='g'){
            //            read all of the data for a game
            file.get();
            file.get(teamHome,255,'|');
            file.get();
            file >> score1;
            file.get();
            file.get(teamAway,255,'|');
            file.get();
            file >> score2;
            
            //            if the team read matches the team queried
            if (strcmp(teamHome, team)==0) {
                //                increase the total score of the home team by the amount of points scored
                pointsScore+=score1;
                //                increase the total points scored against of the home team by the amount of points scored
                pointsReceived+= score2;
                //                if the team at home scored more than the team away
                if (score1>score2) {
                    //                    increase the total wins by 1 of the home team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the home team
                    totalLoss++;
                }
                
                
            }
            //            if the team away matches the team queried
            else if(strcmp(teamAway, team)==0) {
                //                add the points to the total of wins of the away team
                pointsScore+=score2;
                //                add the points to the total of losses of the away team
                pointsReceived+= score1;
                if (score2>score1) {
                    //                    increases the total wins by 1 of the away team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the away team
                    totalLoss++;
                }
                
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    }
    
    file.close();
    return pointsScore;
}

int getPA(char team[]){
    //    the function returns the points the team queried had scored aginst them
    
    
    //        data file
    ifstream file("database.txt", ios::in);
    
    //        single string of character
    char c;
    
    string in;
    //  name of the team home
    char teamHome[255];
    //  score of the team home
    int score1;
    //  name of the team away
    char teamAway[255];
    //  score of the team away
    int score2;
    
    //  total wins for a certain team
    int totalWins=0;
    //  total losses for a certain team
    int totalLoss = 0;;
    //  points scored for a team
    int pointsScore = 0;
    //  points scored against the team
    int pointsReceived=0;;
    
    //  scan thorugh the data file
    while (file.get(c)) {
        //      check if line is a game
        if(c =='g'){
            //          read all of the data for a game
            file.get();
            file.get(teamHome,255,'|');
            file.get();
            file >> score1;
            file.get();
            file.get(teamAway,255,'|');
            file.get();
            file >> score2;
            
            //          if the team read matches the team queried
            if (strcmp(teamHome, team)==0) {
                //                increase the total score of the home team by the amount of points scored
                pointsScore+=score1;
                //                increase the total points scored against of the home team by the amount of points scored
                pointsReceived+= score2;
                //                if the team at home scored more than the team away
                if (score1>score2) {
                    //                    increase the total wins by 1 of the home team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the home team
                    totalLoss++;
                }
                
                
            }
            //            if the team away matches the team queried
            else if(strcmp(teamAway, team)==0) {
                //                add the points to the total of wins of the away team
                pointsScore+=score2;
                //                add the points to the total of losses of the away team
                pointsReceived+= score1;
                if (score2>score1) {
                    //                    increases the total wins by 1 of the away team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the away team
                    totalLoss++;
                }
                
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    }
    
    file.close();
    
    return pointsReceived;
}


int getWins(char team[]){
    //    return the wins for a certain team
    
    //    data file
    ifstream file("database.txt", ios::in);
    
    //    single string of character
    char c;
    
    string in;
    //    name of the team home
    char teamHome[255];
    //    score of the team home
    int score1;
    //    name of the team away
    char teamAway[255];
    //    score of the team away
    int score2;
    
    //    total wins for a certain team
    int totalWins=0;
    //    total losses for a certain team
    int totalLoss = 0;;
    //    points scored for a team
    int pointsScore = 0;
    //    points scored against the team
    int pointsReceived=0;;
    
    //    scan thorugh the data file
    while (file.get(c)) {
        //        check if line is a game
        if(c =='g'){
            //            read all of the data for a game
            file.get();
            file.get(teamHome,255,'|');
            file.get();
            file >> score1;
            file.get();
            file.get(teamAway,255,'|');
            file.get();
            file >> score2;
            
            //            if the team read matches the team queried
            if (strcmp(teamHome, team)==0) {
                //                increase the total score of the home team by the amount of points scored
                pointsScore+=score1;
                //                increase the total points scored against of the home team by the amount of points scored
                pointsReceived+= score2;
                //                if the team at home scored more than the team away
                if (score1>score2) {
                    //                    increase the total wins by 1 of the home team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the home team
                    totalLoss++;
                }
                
                
            }
            //            if the team away matches the team queried
            else if(strcmp(teamAway, team)==0) {
                //                add the points to the total of wins of the away team
                pointsScore+=score2;
                //                add the points to the total of losses of the away team
                pointsReceived+= score1;
                if (score2>score1) {
                    //                    increases the total wins by 1 of the away team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the away team
                    totalLoss++;
                }
                
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    }
    
    file.close();
    
    return totalWins;
}




int getLosses(char team[]){
    //    returns the number of losses for a certain team
    
    //    data file
    ifstream file("database.txt", ios::in);
    
    //    single string of character
    char c;
    
    string in;
    //    name of the team home
    char teamHome[255];
    //    score of the team home
    int score1;
    //    name of the team away
    char teamAway[255];
    //    score of the team away
    int score2;
    
    //  total wins for a certain team
    int totalWins=0;
    //    total losses for a certain team
    int totalLoss = 0;;
    //    points scored for a team
    int pointsScore = 0;
    //    points scored against the team
    int pointsReceived=0;;
    
    //    scan thorugh the data file
    while (file.get(c)) {
        //        check if line is a game
        if(c =='g'){
            //          read all of the data for a game
            file.get();
            file.get(teamHome,255,'|');
            file.get();
            file >> score1;
            file.get();
            file.get(teamAway,255,'|');
            file.get();
            file >> score2;
            
            //          if the team read matches the team queried
            if (strcmp(teamHome, team)==0) {
                //                increase the total score of the home team by the amount of points scored
                pointsScore+=score1;
                //                increase the total points scored against of the home team by the amount of points scored
                pointsReceived+= score2;
                //                if the team at home scored more than the team away
                if (score1>score2) {
                    //                    increase the total wins by 1 of the home team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the home team
                    totalLoss++;
                }
                
                
            }
            //            if the team away matches the team queried
            else if(strcmp(teamAway, team)==0) {
                //                add the points to the total of wins of the away team
                pointsScore+=score2;
                //                add the points to the total of losses of the away team
                pointsReceived+= score1;
                if (score2>score1) {
                    //                    increases the total wins by 1 of the away team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the away team
                    totalLoss++;
                }
                
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    }
    
    file.close();
    
    return totalLoss;
}







//*********************GETSTATS****************************************
void getStats(char team[]){
    //    returns the statistics for the team searched
    
    //    data file
    ifstream file("database.txt", ios::in);
    
    //    single string of character
    char c;
    
    string in;
    //    name of the team home
    char teamHome[255];
    //    score of the team home
    int score1;
    //    name of the team away
    char teamAway[255];
    //    score of the team away
    int score2;
    
    //  total wins for a certain team
    int totalWins=0;
    //    total losses for a certain team
    int totalLoss = 0;;
    //    points scored for a team
    int pointsScore = 0;
    //    points scored against the team
    int pointsReceived=0;;
    
    //    scan thorugh the data file
    while (file.get(c)) {
        //        check if line is a game
        if(c =='g'){
            //          read all of the data for a game
            file.get();
            file.get(teamHome,255,'|');
            file.get();
            file >> score1;
            file.get();
            file.get(teamAway,255,'|');
            file.get();
            file >> score2;
            //          if the team read matches the team queried
            if (strcmp(teamHome, team)==0) {
                //                increase the total score of the home team by the amount of points scored
                pointsScore+=score1;
                //                increase the total points scored against of the home team by the amount of points scored
                pointsReceived+= score2;
                //                if the team at home scored more than the team away
                if (score1>score2) {
                    //                    increase the total wins by 1 of the home team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the home team
                    totalLoss++;
                }
                
                
            }
            //            if the team away matches the team queried
            else if(strcmp(teamAway, team)==0) {
                //                add the points to the total of wins of the away team
                pointsScore+=score2;
                //                add the points to the total of losses of the away team
                pointsReceived+= score1;
                if (score2>score1) {
                    //                    increases the total wins by 1 of the away team
                    totalWins++;
                }
                else {
                    //                    increases the total losses by 1 of the away team
                    totalLoss++;
                }
                
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    }
    
    
    if(totalLoss==0 && totalWins==0){
//      formatting the score appropriately using tabs based on the length of the team name
        if (strlen(team)<4){
            cout<< team<< "\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
        }
//      formatting the score appropriately using tabs based on the length of the team name
        else if (strlen(team)<5){
            cout<< team<< "\t\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
        }
//    formatting the score appropriately using tabs based on the length of the team name
        else if (strlen(team)<6){
            cout<< team<< "\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
        }
//      formatting the score appropriately using tabs based on the length of the team name
        else{
            cout<< team<< "\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
            
        }
    }
    
//    formatting the score appropriately using tabs based on the length of the team name
    else if (strlen(team)<4){
        cout<< team<< "\t\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
    }
//    formatting the score appropriately using tabs based on the length of the team name
    else if (strlen(team)<5){
        cout<< team<< "\t\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
    }
//    formatting the score appropriately using tabs based on the length of the team name
    else if (strlen(team)<6){
        cout<< team<< "\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
    }
//    formatting the score appropriately using tabs based on the length of the team name
    else{
        cout<< team<< "\t\t\t\t"<<totalWins<<"\t"<<totalLoss<<"\t"<<pointsScore<<"\t"<<pointsReceived<<endl;
        
    }
    file.close();
    return;
}



void printUpperLines(){
    //    prints some formatting lines
    cout<<"__________________________________________________________________"<<endl;
    cout<< "    Team\t\t\t\tWins\tLosses\tPF\tPA"<< endl;
}

void printLowerLines(){
    //    prints some formatting lines
    cout<<"------------------------------------------------------------------"<<endl;
    
}


void AddElement(char type){
    //    fucntion adds an element to the file
    
    //    data file
    ofstream file("database.txt", ios::app);
    //    3-character code
    char code[255];
    //    the element name to be added
    char name[255];
    //    read the input
    cin.get();
    cin.get(code, 255, ' ');
    cin.get();
    cin.get(name, 255);
    
    //    print the element in the file
    file << type << "|";
    file << code << "|";
    file << name << "|";
    file << endl;
    
    file.close();
    
    
    return;
}


void AddGame(){
    //    add game to the data file
    
    // Data file
    ofstream file("database.txt", ios::app);
    
    char team1[255];
    int score;
    char team2[255];
    int score2;
    
    //   read the input game
    cin.get();
    cin.get(team1, 255, ' ');
    cin.get();
    cin>>score;
    cin.get();
    cin.get(team2, 255, ' ');
    cin.get();
    cin >> score2;
    
    //    write the game to the file
    file << 'g' << "|";
    file << team1 << "|";
    file << score << "|";
    file << team2 << "|";
    file << score2<< "|";
    file << endl;
    
    // Close the file
    file.close();
    
    return;
}



void List(char query){
    //    list elements based on the query input
    
    ifstream file("database.txt", ios::in);
    
    //    string of data
    string in;
    //    single character of data
    char c;
    //    character array of data
    char data[255];
    //     Result counter
    int counter = 0;
    
    //     Name of the first team in a game
    char team1[255];
    //    score of the first team
    int score;
    //     Name of the second team in a game
    char team2[255];
    //    score of the second team
    int score2;
    
    //    scan thorugh the data file
    while (file.get(c)) {
        //        check if the line matches the query
        if (c == query) {
            switch (c) {
                    //                    if the line is a team
                case 't':
                    //                    read in the '|' char
                    file.get();
                    //                    read in the city code
                    file.get(data, 255, '|');
                    //                    print out the city name
                    cout << getCityName('c', data);
                    //                    read in the '|' char
                    file.get();
                    //                    read the name of the team
                    file.get(data, 255, '|');
                    //                    print out the team related to that city
                    cout << " " << data;
                    file >> ws;
                    //                    keep track if elements are listed
                    counter++;
                    break;
                    //                    if the line represents a city
                case 'c':
                    //                    read in the '|' char
                    file.get();
                    file.get(data, 255, '|');
                    //                    print the city code
                    cout << data;
                    //                    read in the '|' char
                    file.get();
                    //                    read in the city name
                    file.get(data, 255, '|');
                    //                    print the name of the city related to that city code
                    cout << " " << data;
                    file >> ws;
                    //                    keep track if elements are listed
                    counter++;
                    break;
                    //                    if the line is a game
                case 'g':
                    //                    read in the '|' char
                    file.get();
                    //                    read in the home team
                    file.get(team1, 255, '|');
                    //                    print the home team
                    if (strlen(team1) <8) {
                        cout<< team1<< "\t" << "\t";
                    }
                    else {
                        cout<< team1<< "\t";
                    }
                    //                    read in the '|' char
                    file.get();
                    //                    read in the score of the home team
                    file >> score;
                    //                    print the score of the home team
                    cout<< score << "\t";
                    //                    read in the '|' char
                    file.get();
                    //                    read in the away team 
                    file.get(team2, 255, '|');
                    //                    print the away team 
                    if (strlen(team2) <8) {
                        cout<< team2<< "\t" << "\t";
                    }
                    else {
                        cout<< team2<< "\t";
                    }
                    //                    read in the '|' char
                    file.get();
                    //                    read in the score of the away team 
                    file >> score2;
                    //                    print out the score of the away team 
                    cout << score2;
                    file >> ws;
                    counter++;
            }
            
            // New line to console
            cout << endl;
        }
        // Current line is not equal to query
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file
    // If no result were found, print message to console
    if (counter == 0) {
        cout << "[No results.]" << endl;
    }
    // Close the file
    file.close();
    
    return;
}


string getCityName(char type, char code[]){
    //    return the city name based on the city code 
    
    // Input file
    ifstream file("database.txt", ios::in);
    
    //    single character array
    char c;
    //    character array 
    char data[255];
    
    //    string of data 
    string in;
    
    //    string for the name of the element 
    string name = "";
    
    //    loop through the file 
    while (file.get(c)) {
        //        if the line matches the code of the query 
        if (c == type) {
            //            read in the line 
            file.get();
            file.get(data, 255, '|');
            //            if the line matches the city code 
            if (strcmp(data, code) == 0) {
                //                read the rest of the line 
                file.get();
                getline(file, name, '|');
                file.close();
                //                return the name of the city
                return name;
            }
            else {
                //                skip to the next line
                getline(file, in);
                file >> ws;
            }
        }
        else {
            //            skip to the next line
            getline(file, in);
            file >> ws;
        }
    }
    
    file.close();
    
    return "Uknown city.";
}
//********************* END OF FUNCTION getCityName ******************
