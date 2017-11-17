// EyeOfTheBeholder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EOBGame.h"


int _tmain(int argc, _TCHAR* argv[])
{
	EOBGame* pGame = new EOBGame();

	pGame->Start();

	delete pGame;

	return 0;
}

