#include <iostream> //cout
#include <fstream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <algorithm> // Shuffle
#include <vector> //Vector
#include <random> // default_random_engine
#include <chrono> // chrono::system_clock
#include "sqlite/sqlite3.h" // SQLITE DATABASE

using namespace std;

char* err;
sqlite3* db;
sqlite3_stmt* stmt;
unsigned int highscore = 0,score = 0;
string username,name;

void play_game();
void show_Ques(string genre);
const unsigned char* Display(const unsigned char* ques, const unsigned char* op1, const unsigned char* op2, const unsigned char* op3, const unsigned char* op4, int no);

void game_Menu(){
    system("Color 0F");    
    int choice;
    system("cls");
    cout<<"        ................................... Welcome To The SQUIRZY ...................................\n";
    cout<<"\n           1.  PLAY\n           2.  HIGHSCORE\n           3.  EXIT\n\n           Enter your Choice : ";
    cin>>choice;
    switch(choice){
        case 1: play_game();
                break;
        case 2: system("cls");
                system("Color 03");
                cout<<"        ................................... High Score ...................................\n";
                cout<<"\n\n          "<<username<<"    -------------------------------------------------    "<<highscore<<"                                        ";
                Sleep(2050);
                game_Menu();
                break;
        case 3: cout<<"\n  Thanks For Trying Our Quiz Game!";
                Sleep(1200);
                exit(0);
                break;
        default: cout<<"\n  You have Selected an Invalid Option Try Again !!";
                 Sleep(1500);
                 game_Menu();
    }

}

void play_game(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n                                                      Enter Your Name : ";
    cin.ignore();
    getline(cin, name);
    system("cls");
    int choice;
    cout<<"        ....................................... CHOOSE ANY QUIZ GENRE .......................................\n";
    cout<<"\n           1.  COMPUTER\n           2.  SCIENCE\n           3.  GENERAL KNOWLEDGE\n           4.  RANDOM GENRE\n           5.  Go Back To Game Menu\n\n           Enter Your Choice : ";
    cin>>choice;
    switch(choice){
        case 1: system("Color 0B");
                show_Ques("COMPUTER");
                break;
        case 2: system("Color 0C");
                show_Ques("SCIENCE");
                break;
        case 3: system("Color 0D");
                show_Ques("GK");
                break;
        case 4: system("Color 0E");
                show_Ques("MIXED");
                break;
        case 5: game_Menu();
        default: cout<<"\nInvalid Choice !!";
                 play_game();
    }
    
}

void show_Ques(string genre){
        system("cls");
        string gen[3] = {"COMPUTER","SCIENCE","GK"};
        if(genre == "MIXED"){
                vector<int> arr = {0,1,2};
                unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
                shuffle(arr.begin(), arr.end(), default_random_engine(seed));
                genre = gen[arr[1]];
        }
        int no = 1;
        cout<<"        -------------------\n";
        cout<<"        | Genre : "<<genre<<" | -->"<<endl;
        cout<<"        -------------------\n";
        string cmd = "select * from 'squirzy' where GENRE like '";
        cmd.append(genre);
        cmd.append("'");
        sqlite3_prepare_v2(db, cmd.c_str(), -1, &stmt,0);
        const unsigned char *ques,*correct_Option,*op2,*op3,*op4,*answer;
        while(sqlite3_step(stmt) != SQLITE_DONE){
                ques = sqlite3_column_text(stmt , 4);
                correct_Option = sqlite3_column_text(stmt , 0);
                op2 = sqlite3_column_text(stmt , 2);
                op3 = sqlite3_column_text(stmt , 3);
                op4 = sqlite3_column_text(stmt , 1);
                answer = Display(ques, correct_Option, op2, op3, op4, no);
                no++;
                if(answer == correct_Option){
                        cout<<"\n            Correct Answer !!\n";
                        score += 10;
                        if(score >= highscore){
                                highscore = score;
                                username = name;
                                ofstream file("HighScore.txt");
                                file << to_string(highscore);
                                file.close();
                                ofstream fileu("UserName.txt");
                                fileu << username;
                                fileu.close();
                        }
                }
                else{
                        cout<<"\n            Wrong Answer !!\n";
                }
                Sleep(250);
        }
        cout<<"\n                 Final Score : "<<score;
        cout<<"\n\n      :------------------------------- You Have Played Well !! -------------------------------:";
        score = 0;
        no = 1;
        Sleep(2150);
        game_Menu();
}

const unsigned char* Display(const unsigned char* ques, const unsigned char* op1, const unsigned char* op2, const unsigned char* op3, const unsigned char* op4, int no){
        const unsigned char* options[4];
        int choice;
        options[0] = op1;
        options[1] = op2;
        options[2] = op3;
        options[3] = op4;
        vector<int> arr = {0,1,2,3};
        unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(arr.begin(), arr.end(), default_random_engine(seed));
        cout<<"       :-------------------------------------------------------------------------- SCORE : "<<score<<" ------------>\n";
        cout << "\n       Ques. "<<no<<"         Question : "<< ques << endl << endl;
        cout << "                       Options : 1. "<< options[arr[0]];
        cout << "  2. "<< options[arr[1]];
        cout << "  3. "<< options[arr[2]];
        cout << "  4. "<< options[arr[3]] << endl << endl;
        cout<<"       :--------------------------------:\n";
        cout<<"        Enter Your Choice (1-4) : ";
        cin>>choice;
        return options[arr[choice - 1]];
}

int main(){
        system("cls");
        ofstream MyFile("HighScore.txt", ios::app);
        ifstream MyFileR("HighScore.txt");
        ofstream User("UserName.txt",ios::app);
        ifstream UserR("UserName.txt");
        sqlite3_open("dataB.db",&db);
        int rc = sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS squirzy(Correct_Option VARCHAR(100), Option_2 VARCHAR(100), Option_3 VARCHAR(100), Option_4 VARCHAR(100), QUESTION VARCHAR(200), GENRE VARCHAR(100));", NULL, NULL, &err);
        if(rc != SQLITE_OK){
        cout<< "Error : " << err;
        }
        MyFileR.seekg(0, ios::end);
        if(MyFileR.tellg() == 0){
                highscore = 0;
                MyFile.close();
        }
        MyFileR.close();
        UserR.seekg(0, ios::end);
        if(UserR.tellg() == 0){
                username = "No Name";
                User.close();
        }
        UserR.close();
        ifstream MyFileRT("HighScore.txt");
        ifstream UserRT("UserName.txt");
        string temp;
        getline(MyFileRT, temp);
        highscore = atoi(temp.c_str());
        MyFileRT.close();
        getline(UserRT,temp);
        username = temp;
        UserRT.close();
        game_Menu();
        return 0;
}