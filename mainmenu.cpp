#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <string.h>
#include <windows.h>
#define ESC 27
//for reference
/*#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77*/

using namespace std;
//colors
int highlightcolor = 1; //blue
int defaultcolor = 7; //white
int titlecolor =  12; //ed
int subcolor = 14; //yellow

void changeColor(int desiredColor){ 
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
}
void hideCursor(){
    HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);
}
// in case needed
/*void Draw(){
    cout << "PRISONER TETRIS" << endl;
    cout << "MAIN MENU" << endl;
    cout << "Use UP and DOWN arrow keys to control main menu. Press ENTER to select. Press ESC to exit." << endl;
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
}*/

void menudesc(){
    changeColor(titlecolor);
    cout << "PRISONER TETRIS" << endl;
    changeColor(subcolor);
    cout << "MAIN MENU" << endl;
    cout << "Use UP and DOWN arrow keys to control main menu. Press ENTER to select. Press ESC to exit." << endl;
}
void chooseStart(){
    menudesc();
    changeColor(highlightcolor);
    cout << "###############" << endl;
    cout << "##";
    changeColor(defaultcolor);
    cout << "...Start...";
    changeColor(highlightcolor);
    cout << "##" << endl;
    cout << "###############" << endl;
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..Leaderboard.." << endl;
    cout << "..............." << endl;
}
void chooseHowtoPlay(){
    menudesc();
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    changeColor(highlightcolor);
    cout << "###############" << endl;
    cout << "##";
    changeColor(defaultcolor);
    cout << "How to Play";
    changeColor(highlightcolor);
    cout << "##" << endl;
    cout << "###############" << endl;
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..Leaderboard.." << endl;
    cout << "..............." << endl;
}
void chooseStory(){
    menudesc();
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    changeColor(highlightcolor);
    cout << "###############" << endl;
    cout << "##";
    changeColor(defaultcolor);
    cout << "...Story...";
    changeColor(highlightcolor);
    cout << "##" << endl;
    cout << "###############" << endl;
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << "..Leaderboard.." << endl;
    cout << "..............." << endl;
}
void chooseLeaderBoard(){
    menudesc();
    changeColor(defaultcolor);
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
    changeColor(highlightcolor);
    cout << "###############" << endl;
    cout << "##";
    changeColor(defaultcolor);
    cout << "Leaderboard";
    changeColor(highlightcolor);
    cout << "##" << endl;
    cout << "###############" << endl;
    changeColor(defaultcolor);
}
struct player{
    char name[16];
    int score;
    int lvl;
}plyr, rd, temp;

int Existing;

int playerExists(){
    FILE *fp = fopen("names.txt", "r");
    if(!fp) return 0;
    while(fread(rd.name, sizeof(char)*16, 1, fp) && !feof(fp)){
        if(!strcmp(rd.name, plyr.name)){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addName(){
    FILE *fp = fopen("names.txt", "a+");
    if(!fp) cout << "An error occurred." << endl;
    fwrite(plyr.name, sizeof(char)*16, 1, fp);
    fclose(fp);
}

void initFile(){
    FILE *fp = fopen("record.txt", "w");
    if(!fp){
        cout << "An error occurred" << endl;
        return;
    }
    rd.score = -1;
    strcpy(rd.name, "--");
    for(int i = 1; i <= 16; i++){
        rd.lvl = i;
        for(int j = 1; j <= 10; j++) fwrite(&rd, sizeof(player), 1, fp);
    }
    fclose(fp);
}

void initPlayer(){
    int acceptName = 1;
    do{
        cout << "Enter your name (up to 15 characters): ";
        cin >> plyr.name;
        Existing = playerExists();
        if(Existing){
            cout << "Name already exists. Do you wish to continue with this name? (Enter 1 for Yes. Enter 0 for No): ";
            cin >> acceptName;
        }
    }while(!acceptName);
    if(!Existing) addName();
    do{
        cout << "Enter level of difficulty (1 - Easiest; 15 - Hardest): ";
        cin >> plyr.lvl;
    }while(plyr.lvl > 15 || plyr.lvl < 1);
}

int topScore(){
    FILE *fp = fopen("record.txt", "r+");
    if(!fp){
        return -1;
    }
    fseek(fp, sizeof(player)*((plyr.lvl - 1)*10), SEEK_SET);
    for(int i = 1; i <= 10; i++){
        fread(&rd, sizeof(player), 1, fp);
        if(plyr.score >= rd.score && plyr.score != 0) return i;
    }
    fclose(fp);
    return 0;
}

void writeScore(){
    int i = topScore();
    if(i == -1){
        initFile();
        i = topScore();
    }
    if(i > 0){
        FILE *fp = fopen("record.txt", "r+");
        cout << "New high score!" << endl;
        for(int j = i; j <= 10; j++){
            fseek(fp, sizeof(player)*((plyr.lvl - 1)*10 + j - 1), SEEK_SET);
            fread(&rd, sizeof(player), 1, fp);
            fseek(fp, sizeof(player)*((plyr.lvl - 1)*10 + j - 1), SEEK_SET);
            if(j == i) fwrite(&plyr, sizeof(player), 1, fp);
            else fwrite(&temp, sizeof(player), 1, fp);
            if(rd.score == -1){
                fclose(fp);
                return;
            }
            temp = rd;
        }
    }
}

void displayTopScores(int lvl){
    FILE *fp = fopen("record.txt", "r");
    if(!fp){
        cout << "An error occurred" << endl;
        return;
    }
    cout << "\n>>>>>>>>>> Level " << lvl << " <<<<<<<<<<<" << endl;
    cout << "\n>> Player >>>>>>>>>>>> Score <<" << endl;
    fseek(fp, sizeof(player)*((lvl - 1)*10), SEEK_SET);
    fread(&rd, sizeof(player), 1, fp);
    for(int i = 0; i < 10; i++){
        if(rd.score == -1 || rd.score == 0){
            fclose(fp);
            cout << "\n>>>>>>>>> End of List <<<<<<<<<" << endl;
            return;
        }
        cout << endl << rd.name << "                       " << rd.score << endl;
        fread(&rd, sizeof(player), 1, fp);
        i++;
    };
}

int main(){
    hideCursor();
    int choice = 1;
    int ch, lvl;
    chooseStart();
    while ((ch = getch()) != ESC){
        hideCursor();
        if(ch == 72){
                choice--;
                if (choice == 0) choice = 1;
            }
        else if(ch == 80){
                choice++;
                if (choice == 5) choice = 4;
        }
        if(choice == 1){
            system("cls");
            chooseStart();
        }
        else if(choice == 2){
            system("cls");
            chooseHowtoPlay();
        }
        else if(choice == 3){
            system("cls");
            chooseStory();
        }
        else if(choice == 4){
            system("cls");
            chooseLeaderBoard();
        }
        //If ENTER was pressed
        if (ch == 13) {
            if(choice == 1){
                system("cls");
                char buffer[500];
                initPlayer();
                //Sample
                cout << "score: ";
                cin >> plyr.score;
                writeScore();
            }
            else if(choice == 2){
                system("cls");
                char buffer[500];
                FILE *fp = fopen("howtoplay.txt", "r");
                if(!fp){
                    cout << "File howtoplay.txt not found" << endl;
                    return -1;
                }
                for(int i = 0; i < 31; i++){
                    fgets(buffer, 500, fp);
                    cout << buffer;
                }
            }

            else if(choice == 3){
                system("cls");
                char buffer[500];
                FILE *fp = fopen("bgstory.txt", "r");
                if(!fp){
                    cout << "File bgstory.txt not found" << endl;
                    return -1;
                }
                 for(int i = 0; i < 9; i++){
                    fgets(buffer, 500, fp);
                    cout << buffer;
                }
            }
            else if(choice == 4){
                system("cls");
                char buffer[500];
                cout << "Display leaderboard of level: ";
                cin >> lvl;
                displayTopScores(lvl);
            }
        }
    }
    cout << "The program is now exiting" << endl;
    return 0;
}
