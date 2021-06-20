// (MINUTES : SECONDS)   ONLY ======================= sorry fam

#include <iostream>
#include <stdlib.h>         // For system()
#include <windows.h>        // For Sleep() and Beep()

using namespace std;
void timer(int m, int s); 

int main() {
    int m, s;
    m = 02;
    s = 00;
    timer(m,s);
    Beep(440,1000);
}

void timer(int m, int s) {  // Function for timer
    for(;;) {
        if (m==0 && s==0){
            cout<<"\b";
            cout<<s;
            break;
        }
        if (s == 0) {
            s = 60;
            m--;
        }

        system("cls");
        cout.fill('0');
        cout.width(2);
        cout<<m<<":";
        cout.fill('0');
        cout.width(2);
        cout<<s--;
        Sleep(1000);
    }
}
