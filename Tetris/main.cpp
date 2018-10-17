#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <stdio.h>
#define UNICODE
#include <Windows.h>

using namespace std;

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)
wstring tetromino[7];

// playing fields
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;


int Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: pi = py * 4 + px; // 0
        break;

	case 1: pi = 12 + py - (px * 4); // 90
		break;

	case 2: pi = 15 - (py * 4) - px; // 180
		break;

	case 3: pi = 3 - py + (px * 4); // 270
		break;
	}

	return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
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

int main()
{
    // Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) {
        screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


    // create assets
    tetromino[0].append(L"..X...X...X...X."); // I
	tetromino[1].append(L"..X..XX...X....."); // T
	tetromino[2].append(L".....XX..XX....."); // 0
	tetromino[3].append(L"..X..XX..X......"); // Z
	tetromino[4].append(L".X...XX...X....."); // S
	tetromino[5].append(L".X...X...XX....."); // L
	tetromino[6].append(L"..X...X..XX....."); // J

	pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field buffer
	for (int x = 0; x < nFieldWidth; x++) { // Board Boundary
		for (int y = 0; y < nFieldHeight; y++) {
            // set to 0 unless it's on the sides or the bottom
            if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) {
                pField[y*nFieldWidth + x] = 9;
            } else {
                pField[y*nFieldWidth + x] = 0;
            }
        }
	}

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
	int nScore = 0;
	vector<int> vLines;
	bool bGameOver = false;

	while (!bGameOver) // Main Loop
	{
	    // Timing =======================
		this_thread::sleep_for(chrono::milliseconds(50)); // Small Step = 1 Game Tick
		nSpeedCount++;
		bForceDown = (nSpeedCount == nSpeed);

		// Input ========================
		for (int k = 0; k < 4; k++)	{                           // R   L  D Z
           bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
		}

		// Game Logic ===================

		// Handle player movement
		nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		// Rotate, but latch to stop wild spinning
		if (bKey[3]) {
			nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		} else {
		    bRotateHold = true;
		}

		// Force the piece down the playfield if it's time
		if (bForceDown)
		{
			// Update difficulty every 50 pieces
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 50 == 0 && nSpeed >= 10) {
                nSpeed--;
			}

			// Test if piece can be moved down
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // It can, so do it!
			else
			{
				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++) {
                    for (int py = 0; py < 4; py++) {
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.') {
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                        }
                    }
				}

				// Check for lines
				for (int py = 0; py < 4; py++) {
                    if(nCurrentY + py < nFieldHeight - 1) {
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++) {
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
						}

						if (bLine) {
							// Remove Line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++) {
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							}
							vLines.push_back(nCurrentY + py);
						}
					}
				}

				nScore += 25;
				if(!vLines.empty())	nScore += (1 << vLines.size()) * 100;

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
		for (int x = 0; x < nFieldWidth; x++) {
            for (int y = 0; y < nFieldHeight; y++) {
                switch(pField[y*nFieldWidth + x])
                {
                case 0: screen[y*nScreenWidth + x] = L' ';
                    break;
                case 1: screen[y*nScreenWidth + x] = L'A';
                    break;
                case 2: screen[y*nScreenWidth + x] = L'B';
                    break;
                case 3: screen[y*nScreenWidth + x] = L'C';
                    break;
                case 4: screen[y*nScreenWidth + x] = L'D';
                    break;
                case 5: screen[y*nScreenWidth + x] = L'E';
                    break;
                case 6: screen[y*nScreenWidth + x] = L'F';
                    break;
                case 7: screen[y*nScreenWidth + x] = L'G';
                    break;
                case 8: screen[y*nScreenWidth + x] = L'=';
                    break;
                case 9: screen[y*nScreenWidth + x] = L'#';
                    break;
                }
            }
		}

		// Draw Current Piece
		for (int px = 0; px < 4; px++) {
            for (int py = 0; py < 4; py++) {
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.') {
                    screen[(nCurrentY + py)*nScreenWidth + (nCurrentX + px)] = nCurrentPiece + 65;
                }
            }
		}

		// Draw Score
		//swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		// Animate Line Completion
		if (!vLines.empty())
		{
			// Display Frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(chrono::milliseconds(400)); // Delay a bit

			for (auto &v : vLines)
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					pField[px] = 0;
				}

			vLines.clear();
		}

        // Display Frame (cheekily to draw lines)
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	CloseHandle(hConsole);
	cout << "Game Over!! Score:" << nScore << endl;
	system("pause");
	return 0;
}
