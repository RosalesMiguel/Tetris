#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#define ESC 27
//for reference
/*#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77*/

using namespace std;

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
void chooseStart(){
    cout << "PRISONER TETRIS" << endl;
    cout << "MAIN MENU" << endl;
    cout << "Use UP and DOWN arrow keys to control main menu. Press ENTER to select. Press ESC to exit." << endl;
    cout << "###############" << endl;
    cout << "##...Start...##" << endl;
    cout << "###############" << endl;
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
}
void chooseHowtoPlay(){
    cout << "PRISONER TETRIS" << endl;
    cout << "MAIN MENU" << endl;
    cout << "Use UP and DOWN arrow keys to control main menu. Press ENTER to select. Press ESC to exit." << endl;
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    cout << "###############" << endl;
    cout << "##How to Play##" << endl;
    cout << "###############" << endl;
    cout << "..............." << endl;
    cout << ".....Story....." << endl;
    cout << "..............." << endl;
}
void chooseStory(){
    cout << "PRISONER TETRIS" << endl;
    cout << "MAIN MENU" << endl;
    cout << "Use UP and DOWN arrow keys to control main menu. Press ENTER to select. Press ESC to exit." << endl;
    cout << "..............." << endl;
    cout << ".....Start....." << endl;
    cout << "..............." << endl;
    cout << "..............." << endl;
    cout << "..How to Play.." << endl;
    cout << "..............." << endl;
    cout << "###############" << endl;
    cout << "##...Story...##" << endl;
    cout << "###############" << endl;
}

int main(){
    int choice = 1;
    int ch;
    chooseStart();
    while ((ch = getch()) != ESC){
        if(ch == 72){
                choice--;
                if (choice == 0) choice = 1;
            }
        else if(ch == 80){
                choice++;
                if (choice == 4) choice = 3;
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
        //If ENTER was pressed
        if (ch == 13) {
            if(choice == 1){
                system("cls");
                cout << "wala pa na butang" << endl;
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

            }
    }
    cout << "The program is now exiting" << endl;
    return 0;
}
