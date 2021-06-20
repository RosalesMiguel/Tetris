#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "MMfunction.h"

int opc=0;

void Start();
int HowToPlay();
int Story();
void Leaderboard();

MENU myMenu;

int main(int argc, char *argv[]) {
  bool Exit=false;

  myMenu.Add_Items("1. START",1);
  myMenu.Add_Items("2. STORY",2);
  myMenu.Add_Items("3. HOW TO PLAY", 3);
  myMenu.Add_Items("4. LEADERBOARD",4);
  myMenu.Add_Items("5. EXIT",5);

  myMenu.Set_Space(2);

do {
	/* TextColor(10);
	Say(33,2,"PRISON TETRIS"); */

	if(opc==0) {
        TextColor(10);
	    Say(33,2,"PRISON TETRIS");
		SetColorB(2,19);
		Rectangle(20,8,40,12," ");
		SetColorA(2,22);
		Say(30,10,"WELCOME TO INFERNO!");
  		opc=myMenu.Draw(33,12);
	    }

   switch (opc) {

			case 1:
			   Cls();
			   Start();
			break;

			case 2:
				Cls();
				Story();
			break;

			case 3:
				Cls();
				HowToPlay();
			break;

			case 4:
                Cls();
                Leaderboard();
            break;

			case 5:
				Exit=true;
				std::cout<<"\n";
				std::cout<<"\n";
			break;

	  }


} while(Exit==false);

  return 0;
;

}

void Start() {

}

int HowToPlay() {
  TextColor(12); /*
  Rectangle(20,8,40,12,"%");

  SetColorA(2,22);
  Say(31,10,"*** HOW TO PLAY ***"); */
    const char* filename = "howtoplay.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        return 1;
    }
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
    int tec=getch();
    opc=0;
    return 0;
}

int Story() {
  TextColor(12);
  // Rectangle(20,8,40,12,"x");

  SetColorA(2,22);
  /* Say(31,10,"*** STORY ***");

  int tec=getch();
  opc=0; */
    const char* filename = "bgstory.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        return 1;
    }
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
    int tec=getch();
    opc=0;
    return 0;
}

void Leaderboard() {
  TextColor(12);
  Rectangle(20,8,40,12,":");

  SetColorA(2,22);
  Say(31,10,"*** LEADERBOARD ***");
  int tec=getch();
  opc=0;
}

