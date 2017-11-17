#include "stdafx.h"
#include "EOBGame.h"



EOBGame::EOBGame(void)
{
}


EOBGame::~EOBGame(void)
{
}



/**********************
Created 11/17/2017
Begins the EOB game
***********************/
void EOBGame::Start()
{
	int chc = 0;

	_wsystem(L"CLS\n");
	wprintf(L"1>  Start a new game\n");
	wprintf(L"2>  Load a saved game\n\n");

	while (chc < 1 || chc > 2)
	{
		wprintf(L"Enter choice:  ");
		cin >> chc;
		cout << endl;
	}

	if (chc == 1)
		NewGame();

}

void EOBGame::NewGame()
{
	int chc = 0;

	_wsystem(L"CLS\n");
	wprintf(L"1>  <empty>\t2>  <empty>\n");
	wprintf(L"3>  <empty>\t4>  <empty>\n\n");
	
	while (chc < 1 || chc > 4)
	{
		wprintf(L"Enter a character slot:  ");
		cin >> chc;
		cout << endl;
	}

}
