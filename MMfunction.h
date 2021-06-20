#include <windows.h>
#include <stdio.h>
#include <conio.h>

void SetColorA(int color,int colorf);
void SetColorB(int color,int colorf);
void TextColor(int color);
void Gotoxy(int x,int y);
void Say(const char *txt);
void Say(int x,int y, const char *txt);
void Replicate(const char c,int can);

void ReplicateH(int x,int y,const char *c,int can);
void ReplicateV(int x,int y,const char *c,int can);

void Rectangle(int x,int y,int w,int h,const char *c);

void MSGBOX(const char *m1,const char *m2);
void Cls();

struct OPC
	{
		int id;
		char name[128];
	};

class MENU
{

	public :
       MENU();
       virtual ~MENU();
       void Set_Color(int color,int colorb);
       void Set_Space(int s);
       void Add_Items(const char *txt, int index);
       int Draw(int x,int y);

    private:
		int COLOR,COLORB,SPACE;

		int con;
		OPC Opc[10];
		int OPC_SEL;
};
