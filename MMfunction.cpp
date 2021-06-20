#include "MMfunction.h"

void Gotoxy(int x,int y){
      HANDLE hcon;
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dwPos;
      dwPos.X = x;
      dwPos.Y= y;
      SetConsoleCursorPosition(hcon,dwPos);
 }

void Say(const char *txt){
	printf("%s",txt);
}

void Say(int x,int y,const char *txt){
	Gotoxy(x,y);printf("%s",txt);
}

void Replicate(const char c,int can){
	for(int i=0;i<can;i++)
	 {
		printf("%c",c);
	 }
}

void ReplicateH(int x,int y,const char *c,int can){
	for(int i=0;i<can;i++)
	 {
		Say(x+i,y,c);
	 }
}

void ReplicateV(int x,int y,const char *c,int can){
	for(int i=0;i<can;i++)
	 {
		Say(x,y+i+1,c);
	 }
}

void Rectangle(int x,int y,int w,int h,const char *c){
		ReplicateH(x,y,c,w);
		ReplicateH(x,y+h+2,c,w);
		ReplicateV(x,y,c,h+1);
		ReplicateV(x+w-1,y,c,h+1);

}

void SetColorA(int color,int colorf){

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | colorf | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
}

void SetColorB(int color,int colorf){

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | colorf | BACKGROUND_INTENSITY );
}

void SetColorC(int color,int colorf){

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | colorf);
}


void TextColor(int color){
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}



void MSGBOX(const char *m1,const char *m2){
     MessageBox(NULL,m1 ,m2,MB_ICONEXCLAMATION);
}


void Cls(){
	system("cls");
}


MENU::MENU(){
	con=0;
	COLOR=15;
	COLORB=21;
	OPC_SEL=1;
	SPACE=1;
}

MENU::~MENU(){
}

void MENU::Set_Color(int color,int colorb){
	COLOR=color;
	COLORB=colorb;

}

void MENU::Add_Items(const char *txt, int index){
	Opc[index].id=index;
	sprintf(Opc[index].name,"%s",txt);
	con=con+1;
}

void MENU::Set_Space(int s){
	SPACE=s;
}

int MENU::Draw(int x,int y){   int key;
	bool Sel=false;
	int ROW;
	while(key!=13){
	ROW=y;
	for(int i=1;i<con+1;i++){
            SetColorA(COLOR,COLORB);

			if(OPC_SEL==i){
				SetColorA(15,2);
			}
			Gotoxy(x,ROW);printf("%s",Opc[i].name);
			ROW=ROW+SPACE;
	  }
      SetColorC(15,0);

      key=_getch();

	  switch ( key ){
	      case 72:
	          OPC_SEL=OPC_SEL-1;
	          if(OPC_SEL<1){
                    OPC_SEL=1;
              }
	  			break;
		    case 80:
		        OPC_SEL=OPC_SEL+1;
	            if(OPC_SEL>con){
	                OPC_SEL=con;
                }
	  			break;
	  }

	}

	return OPC_SEL;
}

