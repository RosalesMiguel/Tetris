#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include <unistd.h>

using namespace std;

struct rect{
    int l, w, area;
}screen, field;

struct obj{
    int shape, rotation, x, y;
}piece, prisoner;

wstring tetromino[7];
unsigned char *pField = nullptr;
wchar_t *display;
bool endGame;
int ghost;

int Rotate(int px, int py, int r){
	int pi = 0;
	switch (r % 4){
        case 0: //0 degrees
            pi = py * 4 + px;
            break;
        case 1: //90 degrees
            pi = 12 + py - (px * 4);
            break;
        case 2: //180 degrees
            pi = 15 - (py * 4) - px;
            break;
        case 3: //270 degrees
            pi = 3 - py + (px * 4);
            break;
	}
	return pi;
}

bool DoesPieceFit(int i, int j, int k){
    int pi, fi;
	for (int px = 0; px < 4; px++){
		for (int py = 0; py < 4; py++){
			pi = Rotate(px, py, piece.rotation + i);
            fi = (piece.y + k + py) * field.w + (piece.x+ j + px);
			if (piece.x + j + px >= 0 && piece.x + j + px < field.w){
				if (piece.y + k + py >= 0 && piece.y + k + py < field.l){
					if (tetromino[piece.shape][pi] != L'.' && pField[fi] != 0) return false; //block collides with a surface
				}
			}
		}
    }
	return true;
}

bool DoesPrisonerFit(int i, int j){
    int fi = ((prisoner.y + j)*field.w) + prisoner.x + i;
    if (pField[fi] != 0) return false;
    return true;
}

bool DoesPieceHitPrisoner(){
    int pi, fi_1, fi_2;
	for (int px = 0; px < 4; px++){
		for (int py = 0; py < 4; py++){
			pi = Rotate(px, py, piece.rotation);
            fi_1 = (piece.y + py)*field.w + (piece.x + px);
            fi_2 = prisoner.y*field.w + prisoner.x;
			if (piece.x + px >= 0 && piece.x + px < field.w){
				if (piece.y + py >= 0 && piece.y + py < field.l){
					if (tetromino[piece.shape][pi] != L'.' && fi_1 == fi_2) return true; //block hits prisoner
				}
			}
		}
    }
	return false;
}

void hardDrop(){
    while(DoesPieceFit(0, 0, 1)){
        piece.y++;
        endGame = DoesPieceHitPrisoner();
        if(endGame) return;
    }
}

bool DoesGhostFit(int i, int j, int k){
    int pi, fi;
	for (int px = 0; px < 4; px++){
		for (int py = 0; py < 4; py++){
			pi = Rotate(px, py, piece.rotation + i);
            fi = (ghost + k + py)*field.w + (piece.x+ j + px);
			if (piece.x + j + px >= 0 && piece.x + j + px < field.w){
				if (ghost + k + py >= 0 && ghost + k + py < field.l){
					if (tetromino[piece.shape][pi] != L'.' && pField[fi] != 0) return false; //block collides with a surface
				}
			}
		}
    }
	return true;
}

void ghostDrop(){
    while(DoesGhostFit(0, 0, 1)) ghost++;
}
int main(){
	//Screen resize
	HWND hwnd = GetConsoleWindow();
    if(hwnd != NULL) MoveWindow(hwnd ,100,100,680,600 ,TRUE);
    auto start = chrono::steady_clock::now();
    screen.w = 80;
    screen.l = 30;
    screen.area = screen.l*screen.w;
    display = new wchar_t[screen.area];
    field.w = 12;
    field.l = 18;
    field.area = field.l*field.w;

	// Create Screen Buffer
	for (int i = 0; i < screen.area; i++) display[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	tetromino[0].append(L"..X...X...X...X.");   //I
	tetromino[1].append(L"..X..XX...X.....");   //T
	tetromino[2].append(L".....XX..XX.....");   //O
	tetromino[3].append(L"..X..XX..X......");   //Z
	tetromino[4].append(L".X...XX...X.....");   //S
	tetromino[5].append(L".X...X...XX.....");   //L
	tetromino[6].append(L"..X...X..XX.....");   //J

	pField = new unsigned char[field.area]; // Create play field buffer
	for (int x = 0; x < field.w; x++) // Board Boundary
		for (int y = 0; y < field.l; y++)
			pField[y*field.w + x] = (x == 0 || x == field.w - 1 || y == field.l - 1) ? 9 : 0;

	piece.shape = 0;
	piece.rotation = 0;
	piece.x = field.w / 2;
	piece.y = 0;
	prisoner.x = field.w / 2;
    prisoner.y = field.l - 2;
	int nSpeed = 20, nSpeedCount = 0, nPieceCount = 0, nScore = 0;
	bool bForceDown = false, bRotateHold = true, line, key[9];
	vector<int> vLines;
    endGame = false;

	while (!endGame){
        this_thread::sleep_for(50ms);
		nSpeedCount++;
		bForceDown = (nSpeedCount == nSpeed);

		//Player input
		for (int k = 0; k < 9; k++)								// R   L   D U spacebar A D   W   S
			key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26\x20\x41\x44\x57\x53"[k]))) != 0;

        endGame = DoesPieceHitPrisoner();
		piece.x += (key[0] && DoesPieceFit(0, 1, 0)) ? 1 : 0;
		piece.x -= (key[1] && DoesPieceFit(0, -1, 0)) ? 1 : 0;
		piece.y += (key[2] && DoesPieceFit(0, 0, 1)) ? 1 : 0;
		if(key[4]) hardDrop();
		ghost = piece.y;
		ghostDrop();
		prisoner.x += (key[6] && DoesPrisonerFit(1, 0)) ? 1 : 0;
        prisoner.x -= (key[5] && DoesPrisonerFit(- 1, 0)) ? 1 : 0;
		prisoner.y -= (key[7] && DoesPrisonerFit(0, -1))? 1 : 0;
		prisoner.y += (key[8] && DoesPrisonerFit(0, 1)) ? 1 : 0;
		if (key[3]){
			piece.rotation += (bRotateHold && DoesPieceFit(1, 0, 0))? 1 : 0;
			bRotateHold = false;
		}
		else bRotateHold = true;

		if (bForceDown){
			nSpeedCount = 0;
			nPieceCount++;
			if (DoesPieceFit(0, 0, 1)) piece.y++; //Drop piece by one unit until it collides with a surface
			else{
                //Record piece once it collides with a surface
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[piece.shape][Rotate(px, py, piece.rotation)] != L'.')
							pField[(piece.y + py) * field.w + (piece.x + px)] = piece.shape + 1;
				// Check for lines
				for (int py = 0; py < 4; py++)
					if(piece.y + py < field.l - 1){
						line = true;
						for (int px = 1; px < field.w- 1; px++) line &= (pField[(piece.y + py) * field.w + px]) != 0;
						if(line){
							//remove formed line
							for (int px = 1; px < field.w - 1; px++) pField[(piece.y + py) * field.w + px] = 8;
							vLines.push_back(piece.y + py);
						}
					}
                nScore += 0;
				if(!vLines.empty())	nScore += (1 << vLines.size()) * 50; //Add 100 points for every cleared line

				//Create new piece
				piece.x = field.w / 2;
				piece.y = 0;
				piece.rotation = 0;
				piece.shape = rand() % 7;

				//If piece cannot fit, game over!
				endGame = !DoesPieceFit(0, 0, 0);
			}
		}
		//Draw field
		for (int x = 0; x < field.w; x++)
			for (int y = 0; y < field.l; y++)
				display[(y + 2)*screen.w + (x + 2)] = L" ABCDEFG=#"[pField[y*field.w + x]];
		//Draw piece
		for (int px = 0; px < 4; px++){
			for (int py = 0; py < 4; py++){
				if (tetromino[piece.shape][Rotate(px, py, piece.rotation)] != L'.'){
                    display[(ghost + py + 2)*screen.w + (piece.x + px + 2)] = 79;
					display[(piece.y + py + 2)*screen.w + (piece.x + px + 2)] = piece.shape + 65;
				}
			}
		}
        //Draw prisoner
        display[(prisoner.y + 2)*screen.w + (prisoner.x + 2)] = 80;
		// Draw score
		swprintf_s(&display[2 * screen.w + field.w + 6], 16, L"SCORE: %8d", nScore);
        //Draw timer
        auto end = chrono::steady_clock::now();
        if(chrono::duration_cast<chrono::seconds>(end - start).count() == 120) endGame = true;
        swprintf_s(&display[5 * screen.w + field.w + 6], 16, L" TIME LEFT: %d s", 120 - chrono::duration_cast<chrono::seconds>(end - start).count());

		if (!vLines.empty()){
			WriteConsoleOutputCharacter(hConsole, display, screen.area, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay a bit

			for (auto &v : vLines)
				for (int px = 1; px < field.w - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * field.w + px] = pField[(py - 1) * field.w + px];
					pField[px] = 0;
				}

			vLines.clear();
		}
		WriteConsoleOutputCharacter(hConsole, display, screen.area, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	CloseHandle(hConsole);
	cout << "Uh oh, game over! You garnered a total of " << nScore << " points!" << endl;
	system("pause");
	return 0;
}
