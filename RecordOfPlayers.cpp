#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

struct player{
    char name[16];
    int score;
    int lvl;
}plyr, read, temp;

int Existing;

int playerExists(){
    FILE *fp = fopen("names.txt", "r");
    if(!fp) return 0;
    while(fread(read.name, sizeof(char)*16, 1, fp) && !feof(fp)){
        if(!strcmp(read.name, plyr.name)){
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
    read.score = -1;
    strcpy(read.name, "--");
    for(int i = 1; i <= 16; i++){
        read.lvl = i;
        for(int j = 1; j <= 10; j++) fwrite(&read, sizeof(player), 1, fp);
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
        fread(&read, sizeof(player), 1, fp);
        if(plyr.score >= read.score && plyr.score != 0) return i;
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
            fread(&read, sizeof(player), 1, fp);
            fseek(fp, sizeof(player)*((plyr.lvl - 1)*10 + j - 1), SEEK_SET);
            if(j == i) fwrite(&plyr, sizeof(player), 1, fp);
            else fwrite(&temp, sizeof(player), 1, fp);
            if(read.score == -1){
                fclose(fp);
                return;
            }
            temp = read;
        }
    }
}

void displayTopScores(int lvl){
    FILE *fp = fopen("record.txt", "r");
    if(!fp){
        cout << "An error occurred" << endl;
        return;
    }
    cout << ">>>>>>>>>> Level " << lvl << " <<<<<<<<<<<" << endl;
    cout << "Player >>>>>>>>>>>>>>>>>>> Score" << endl;
    fseek(fp, sizeof(player)*((lvl - 1)*10), SEEK_SET);
    fread(&read, sizeof(player), 1, fp);
    for(int i = 0; i < 10; i++){
        if(read.score == -1 || read.score == 0){
            fclose(fp);
            return;
        }
        cout << read.name << "           " << read.score << endl;
        fread(&read, sizeof(player), 1, fp);
        i++;
    };
    fclose(fp);
}
void displayAll(){
    FILE *fp = fopen("record.txt", "r");
    if(!fp){
        cout << "An error occurred" << endl;
        return;
    }
    for(int i = 1; i <= 15; i++){
        for(int j = 1; j <= 10; j++){
        fread(&read, sizeof(player), 1, fp);
        cout << j << read.lvl << read.name << read.score << endl;
        }
        cout << endl;
    }
    fclose(fp);
}
int main(){
    initPlayer();
    cout << "score: ";
    cin >> plyr.score;
    writeScore();
    int lvl;
    cout << "Display leaderboard of level: ";
    cin >> lvl;
    displayTopScores(lvl);
    return 0;
}

