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

	EOBCharacter* pChar = new EOBCharacter();
	pChar->NewCharacter();
	delete pChar;

	return;

	
	while (chc != 3)
	{
		_wsystem(L"CLS\n");
		wprintf(L"1>  Start a new game\n");
		wprintf(L"2>  Load a saved game\n");
		wprintf(L"3>  Exit\n\n");

		while (chc < 1 || chc > 3)
		{
			wprintf(L"Enter choice:  ");
			cin >> chc;
			cout << endl;
		}

		if (chc == 1)
		{
			if (!NewGame())
				chc = 0;
		}
	}

}

/**********************
Created 11/17/2017
Starts a brand new EOB game
***********************/
bool EOBGame::NewGame()
{
	int chc = 0;

	_wsystem(L"CLS\n");
	wprintf(L"1>  <empty>\t2>  <empty>\n");
	wprintf(L"3>  <empty>\t4>  <empty>\n");
	wprintf(L"5>  CANCEL\n\n");

	while (chc < 1 || chc > 5)
	{
		wprintf(L"Enter a character slot:  ");
		cin >> chc;
		cout << endl;
	}

	EOBCharacter* pChar = new EOBCharacter();
	pChar->NewCharacter();
	delete pChar;

	return false;

}

/**********************
Created 11/17/2017
returns a sum total of random numbers
***********************/
int diceRoll(int sides, int nRolls)
{
	static bool init = false;
	int roll = 0;

	if (!init)
	{
		srand(201711190819);
		init = true;
	}

	for (int i = 0; i < nRolls; i++)
		roll += rand() % sides + 1;

	return roll;
}


EOBCharacter::EOBCharacter()
{

}

EOBCharacter::~EOBCharacter()
{

}


/**********************
Created 11/18/2017
returns a sum total of random numbers
***********************/
void EOBCharacter::NewCharacter()
{
	int chc = 1;
	const int nStats = 2;
	const wchar_t statName[nStats][30] = { {L"Strength" }, { L"Intelligence" }};
	int stats[nStats];

	SelectAlignment();
	return;

	while (chc == 1)
	{
		_wsystem(L"CLS\n");

		for (int i = 0; i < nStats; i++)
		{
			stats[i] = diceRoll(6, 3);
			wprintf(L"%-30.30s\t%02d\n", statName[i], stats[i]);
		}

		wprintf(L"\n1>  Reroll\n");
		wprintf(L"2>  Keep\n");
		wprintf(L"3>  Cancel\n\n");
		chc = 0;

		while (chc < 1 || chc > 3)
		{
			wprintf(L"Select:  ");
			cin >> chc;
			cout << endl;
		}
	}

	if (chc == 3)
		return;

	if (!SelectRace())
		return;

	if (!SelectClass())
		return;
}


/**********************
Created 11/18/2017
User selects the race of the new character
***********************/
bool EOBCharacter::SelectRace()
{
	int chc = 0, nStats;
	enum Race { Human, New };
	const wchar_t raceNames[New + 1][30] = { { L"Human" },{ L"<new>" } };

	nStats = New + 1;
	_wsystem(L"CLS\n");

	for (int i = 0; i < nStats; i++)
	{
		wprintf(L"%2d\t%-30.30s\n", i + 1, raceNames[i]);
	}

	wprintf(L"%2d\tCancel\n", nStats + 1);
	wprintf(L"Random race = %d\n", diceRoll(6, 1));

	while (chc < 1 || chc > nStats + 1)
	{
		wprintf(L"Select:  ");
		cin >> chc;
		cout << endl;
	}

	if (chc == nStats + 1)
		return false;
	else
		return true;
}

/**********************
Created 11/19/2017
User selects the class of the new character
***********************/
bool EOBCharacter::SelectClass()
{
	int chc = 0, nClasses;
	enum Class { Fighter, New };
	const wchar_t classNames[New + 1][30] = { { L"Fighter" },{ L"<new>" } };

	nClasses = New + 1;
	_wsystem(L"CLS\n");

	for (int i = 0; i < nClasses; i++)
	{
		wprintf(L"%2d\t%-30.30s\n", i + 1, classNames[i]);
	}

	wprintf(L"%2d\tCancel\n", nClasses + 1);
	wprintf(L"Random class = %d\n", diceRoll(6, 1));

	while (chc < 1 || chc > nClasses + 1)
	{
		wprintf(L"Select:  ");
		cin >> chc;
		cout << endl;
	}

	if (chc == nClasses + 1)
		return false;
	else
		return true;

}

/**********************
Created 11/19/2017
User selects the Alignment of the character
***********************/
void EOBCharacter::SelectAlignment()
{
	int chc = 0, nAlignments;
	enum Alignment { LawfulGood, New };
	const wchar_t alignmentNames[New + 1][30] = { { L"Lawful Good" },{ L"<new>" } };

	nAlignments = New + 1;
	_wsystem(L"CLS\n");

	for (int i = 0; i < nAlignments; i++)
	{
		wprintf(L"%2d\t%-30.30s\n", i + 1, alignmentNames[i]);
	}

	wprintf(L"%2d\tCancel\n", nAlignments + 1);
	wprintf(L"Random alignment = %d\n", diceRoll(6, 1));

	while (chc < 1 || chc > nAlignments + 1)
	{
		wprintf(L"Select:  ");
		cin >> chc;
		cout << endl;
	}

}