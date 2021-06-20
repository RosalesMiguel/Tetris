#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <Windows.h>

int WidthofField = 12;
int WidthofScreen = 80;
int HeightofField = 18;
int HeightofScreen = 30;			// Console Screen Size Y (rows)
wstring tetromino[7];

unsigned char *pField = nullptr;
int Rotate(int px, int py, int r);
bool PieceValidation(int nTetromino, int nRotation, int nPosX, int nPosY);
// int timer ()

int main()
{
    HWND hwnd = GetConsoleWindow();
    if( hwnd != NULL ){ MoveWindow(hwnd ,100,100,680,600 ,TRUE); }

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[WidthofScreen*HeightofScreen];
	for (int i = 0; i < WidthofScreen*HeightofScreen; i++) screen[i] = L' ';
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

	pField = new unsigned char[WidthofField*HeightofField]; // Create play field buffer
	for (int x = 0; x < WidthofField; x++) // Board Boundary
		for (int y = 0; y < HeightofField; y++)
			pField[y*WidthofField + x] = (x == 0 || x == WidthofField - 1 || y == HeightofField - 1) ? 9 : 0;

	// Game Logic
	bool bKey[4];
	int PlayingPiece = 0;
	int PieceRotation = 0;
	int InitialX = WidthofField / 2;
	int InitialY = 0;
	int DropSpeed = 20;
	int nDropSpeed = 0;
	bool HardDrop = false;
	bool FastRotate = true;
	int PieceNumber = 0;
	int Score = 0;
	vector<int> vLines;
	bool GameOver = false;

	while (!GameOver) // Main Loop
	{
		// Timing =======================
		this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
		nDropSpeed++;
		HardDrop = (nDropSpeed == DropSpeed);

		// Input ========================
		for (int k = 0; k < 4; k++)								// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// Game Logic ===================

		// Handle player movement
		InitialX += (bKey[0] && PieceValidation(PlayingPiece, PieceRotation, InitialX + 1, InitialY)) ? 1 : 0;
		InitialX -= (bKey[1] && PieceValidation(PlayingPiece, PieceRotation, InitialX - 1, InitialY)) ? 1 : 0;
		InitialY += (bKey[2] && PieceValidation(PlayingPiece, PieceRotation, InitialX, InitialY + 1)) ? 1 : 0;

		// Rotate, but latch to stop wild spinning
		if (bKey[3])
		{
			PieceRotation += (FastRotate && PieceValidation(PlayingPiece, PieceRotation + 1, InitialX, InitialY)) ? 1 : 0;
			FastRotate = false;
		}
		else
			FastRotate = true;

		// Force the piece down the playfield if it's time
		if (HardDrop)
		{
			// Update difficulty every 50 pieces
			nDropSpeed = 0;
			PieceNumber++;
			if (PieceNumber % 50 == 0)
				if (DropSpeed >= 10) DropSpeed--;

			// Test if piece can be moved down
			if (PieceValidation(PlayingPiece, PieceRotation, InitialX, InitialY + 1))
				InitialY++; // It can, so do it!
			else
			{
				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[PlayingPiece][Rotate(px, py, PieceRotation)] != L'.')
							pField[(InitialY + py) * WidthofField + (InitialX + px)] = PlayingPiece + 1;

				// Check for lines
				for (int py = 0; py < 4; py++)
					if(InitialY + py < HeightofField - 1)
					{
						bool bLine = true;
						for (int px = 1; px < WidthofField - 1; px++)
							bLine &= (pField[(InitialY + py) * WidthofField + px]) != 0;

						if (bLine)
						{
							// Remove Line, set to =
							for (int px = 1; px < WidthofField - 1; px++)
								pField[(InitialY + py) * WidthofField + px] = 8;
							vLines.push_back(InitialY + py);
						}
					}

				Score += 25;
				if(!vLines.empty())	Score += (1 << vLines.size()) * 100;

				// Pick New Piece
				InitialX = WidthofField / 2;
				InitialY = 0;
				PieceRotation = 0;
				PlayingPiece = rand() % 7;

				// If piece does not fit straight away, game over!
				GameOver = !PieceValidation(PlayingPiece, PieceRotation, InitialX, InitialY);
			}
		}

		// Display ======================

		// Draw Field
		for (int x = 0; x < WidthofField; x++)
			for (int y = 0; y < HeightofField; y++)
				screen[(y + 2)*WidthofScreen + (x + 2)] = L" ABCDEFG=#"[pField[y*WidthofField + x]];

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetromino[PlayingPiece][Rotate(px, py, PieceRotation)] != L'.')
					screen[(InitialY + py + 2)*WidthofScreen + (InitialX + px + 2)] = PlayingPiece + 65;

		// Draw Score
		swprintf_s(&screen[2 * WidthofScreen + WidthofField + 6], 16, L"SCORE: %8d", Score);

		// Animate Line Completion
		if (!vLines.empty())
		{
			// Display Frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, WidthofScreen * HeightofScreen, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay a bit

			for (auto &v : vLines)
				for (int px = 1; px < WidthofField - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * WidthofField + px] = pField[(py - 1) * WidthofField + px];
					pField[px] = 0;
				}

			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, WidthofScreen * HeightofScreen, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	CloseHandle(hConsole); 
	cout << "Game Over!! Score:" << Score << endl;
	system("pause");
	return 0;
}

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


bool PieceValidation(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * WidthofField + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < WidthofField)
			{
				if (nPosY + py >= 0 && nPosY + py < HeightofField)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}

// int timer ()
