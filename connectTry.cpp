//Di mugana

#ifndef UNICODE
#define UNICODE
#endif //UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif // _UNICODE

#include <iostream>
#include <fstream>
#include <string.h>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

using namespace std;

int nScreenWidth = 80;
int nScreenHeight = 30;			// Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;
bool bGameOver;
int endGame;

int Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}

void color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void gotoxy(int x, int y){
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
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
    plyr.score = 0;
    bGameOver = false;
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

void displayTopScores(){
    FILE *fp = fopen("record.txt", "r");
    if(!fp){
        cout << "An error occurred" << endl;
        return;
    }
    int lvl;
    cout << "Display leaderboard of level: ";
    cin >> lvl;
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

void displayMenu(){
    int Set[] = {7, 7, 7}; //DEFAULT COLORS
    int counter = 3;
    char key;

    for(int i = 0 ;;){
        gotoxy(10,5);
        color(Set[0]);
        cout<<"Start ";

        gotoxy(10,6);
        color(Set[1]);
        cout<<"How to Play ";

        gotoxy(10,7);
        color(Set[2]);
        cout<<"Leaderboard ";

        gotoxy(10,8);
        color(Set[3]);
        cout<<"Exit";

        key = _getch();

        if (key == 72 && (counter >= 2 && counter <=3)){
            counter --;
        }
        if (key == 80 && (counter >= 1 && counter <=2)){
            counter ++;
        }
        if (key == '\r'){
            if(counter == 1){
                initPlayer();
            }
            if(counter == 2){
                cout<<"Menu 2 is Open";
            }
            if(counter == 3){
                displayTopScores();
            }
        }
        Set[0] = 7; // Default white color
        Set[1] = 7;
        Set[2] = 7;

        if(counter == 1){
            Set[0] = 12; //12 is color red
        }
        if(counter == 2){
            Set[1] = 12; //12 is color red
        }
        if(counter == 3){
            Set[2] = 12; //12 is color red
        }
        if(counter == 4) endGame = true;
    }
}
int main()
{
	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field buffer
	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	// Game Logic
	bool bKey[4];
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	int nSpeed = 20;
	int nSpeedCount = 0;
	bool bForceDown = false;
	bool bRotateHold = true;
	int nPieceCount = 0;
	endGame = 0;
	vector<int> vLines;

    while(!endGame){
        displayMenu();

        while (!bGameOver) // Main Loop
        {
            // Timing =======================
            this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
            nSpeedCount++;
            bForceDown = (nSpeedCount == nSpeed);

            // Input ========================
            for (int k = 0; k < 4; k++)								// R   L   D Z
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

            // Game Logic ===================

            // Handle player movement
            nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
            nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
            nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

            // Rotate, but latch to stop wild spinning
            if (bKey[3])
            {
                nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
                bRotateHold = false;
            }
            else
                bRotateHold = true;

            // Force the piece down the playfield if it's time
            if (bForceDown)
            {
                // Update difficulty every 50 pieces
                nSpeedCount = 0;
                nPieceCount++;
                if (nPieceCount % 50 == 0)
                    if (nSpeed >= 10) nSpeed--;

                // Test if piece can be moved down
                if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                    nCurrentY++; // It can, so do it!
                else
                {
                    // It can't! Lock the piece in place
                    for (int px = 0; px < 4; px++)
                        for (int py = 0; py < 4; py++)
                            if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                                pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                    // Check for lines
                    for (int py = 0; py < 4; py++)
                        if(nCurrentY + py < nFieldHeight - 1)
                        {
                            bool bLine = true;
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                            if (bLine)
                            {
                                // Remove Line, set to =
                                for (int px = 1; px < nFieldWidth - 1; px++)
                                    pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                                vLines.push_back(nCurrentY + py);
                            }
                        }

				plyr.score += 25;
				if(!vLines.empty())	plyr.score += (1 << vLines.size()) * 100;

                    // Pick New Piece
                    nCurrentX = nFieldWidth / 2;
                    nCurrentY = 0;
                    nCurrentRotation = 0;
                    nCurrentPiece = rand() % 7;

                    // If piece does not fit straight away, game over!
                    bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
                }
            }

            // Display ======================

            // Draw Field
            for (int x = 0; x < nFieldWidth; x++)
                for (int y = 0; y < nFieldHeight; y++)
                    screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

            // Draw Current Piece
            for (int px = 0; px < 4; px++)
                for (int py = 0; py < 4; py++)
                    if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                        screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

            // Draw Score
            swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", plyr.score);

            // Animate Line Completion
		if (!vLines.empty())
		{
			// Display Frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay a bit

			for (auto &v : vLines)
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					pField[px] = 0;
				}

			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	if(bGameOver){
        CloseHandle(hConsole);
        cout << "Game Over!! Score:" << plyr.score << endl;
        writeScore();
        cout << "Play Again? Type 1 for Yes 0 for No: ";
        cin >> endGame;
        system("pause");
    }
    }
	return 0;
}

