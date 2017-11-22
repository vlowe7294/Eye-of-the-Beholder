#include "stdafx.h"
#include "EOBGame.h"

ROOM map[2] = 
{  
	{  {2, 0, 0, 0}, {'n', 'x', 'x', 'x'}, NULL},
	{  {1, 0, 0, 0}, {'x', 's', 'x', 'x'}, NULL}
};





EOBGame::EOBGame()
{
	m_playerLoc.y = m_playerLoc.x = 0;
}


EOBGame::~EOBGame()
{
	delete map[0].pDoor;
	map[0].pDoor = NULL;
	delete map[1].pDoor;
	map[1].pDoor = NULL;
}



/**********************
Created 11/17/2017
Begins the EOB game
***********************/
void EOBGame::Start()
{
	int chc = 0;

	if (map[0].pDoor == NULL)
	{
		map[0].pDoor = new EOBDoor();
		map[1].pDoor = new EOBDoor();
	}
	
	GameLoop();
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

void EOBGame::GameLoop()
{
	bool bGameEnded = false, bLook = true, bIsValid = false;
	wstring input;
	EOBDoor* pDoor;
	ROOM thisRoom = map[0];

	pDoor = thisRoom.pDoor;

	while (!bGameEnded)
	{
		if (bLook)
		{
			wprintf(L"You are on the first level of the sewers of Waterdeep.  Behind you, the exit tunnel has caved in.\nYou have no choice but to go forward.\n");
			pDoor->Round(m_playerLoc);
			bLook = false;

		}

		bIsValid = false;
		wprintf(L"Your action?  ");
		wcin >> input;
		cout << endl;

		if (pDoor != NULL)
			bIsValid = pDoor->ParseCommand(input, m_playerLoc);

		if (!bIsValid)
		{
			if (input.length() < 1)
				input = L"xxx";

			switch (input[0])
			{
				case 'q':
					if (input == L"quit")
					{
						bGameEnded = true;
						bIsValid = true;
					}
					break;

				case 'n':
					bIsValid = true;

					if (thisRoom.exitDirs[0] == 'n')
						bLook = true;
					else
						wprintf(L"You can't go that way\n");

					break;

				case 's':
					bIsValid = true;

					if (thisRoom.exitDirs[1] == 's')
						bLook = true;
					else
						wprintf(L"You can't go that way\n");

					break;

			}
		}

		if (!bIsValid)
			wprintf(L"I dont understand \"%s\"\n", input.c_str());

	}
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
		srand(201711200908);
		init = true;
	}

	for (int i = 0; i < nRolls; i++)
		roll += rand() % sides + 1;

	return roll;
}


EOBDoor::EOBDoor()
{
	m_location.x = m_location.y = 0;
	m_direction = 'n';
	m_bIsOpen = false;
}

EOBDoor::~EOBDoor()
{

}

void EOBDoor::Round(EOBLOCATION plrLoc)
{
	wstring openStr, dirStr, dirStr2;

	if (!IsHere(plrLoc))
		return;

	if (m_bIsOpen)
		openStr = L"open";
	else
		openStr = L"closed";

	if (m_direction == 'n')
		dirStr = L"North";
	else
		dirStr = L"South";

	wprintf(L"%s is a %s door.\n", dirStr.c_str(), openStr.c_str());;

}

bool EOBDoor::IsHere(EOBLOCATION plrLoc)
{
	return (plrLoc.x == m_location.x && plrLoc.y == m_location.y);		

}

bool EOBDoor::ParseCommand(wstring cmd, EOBLOCATION loc)
{
	if (!IsHere(loc) )
		return false;

	wchar_t dir = m_direction;
	wstring str = L" ";

	str[0] = dir;

	if (cmd == L"open")
	{
		m_bIsOpen = true;
		wprintf(L"The door is open.\n");
		return true;
	}

	
	if (cmd == str)
	{
		if (!m_bIsOpen)
		{
			wprintf(L"That way is blocked\n");
			return true;
		}
	}

	return false;
}



const wchar_t EOBCharacter::m_statName[EOBCharacter::NewStat][30] = { {L"Strength" }, { L"Intelligence" }, { L"Wisdom" }};
const wchar_t EOBCharacter::m_classNames[EOBCharacter::NewClass + 1][30] = { { L"Fighter" },{ L"Ranger" },{ L"<new>" } };
const wchar_t EOBCharacter::m_raceNames[EOBCharacter::NewRace + 1][30] = { { L"Human" },{ L"Elf" },{ L"<new>" } };

EOBCharacter::EOBCharacter()
{
	for (int i = 0; i < NewStat; i++)
		m_stats[i] = diceRoll(6, 3);

	m_stats[7] = diceRoll(6, 1);

	if (m_stats[7] > NewClass)
		m_stats[7] = Ranger;

	m_stats[8] = diceRoll(6, 1);

	if (m_stats[8] > NewRace)
		m_stats[8] = Elf;

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
	int nStats = NewStat;
	
	FinalEntry();
	return;

	while (chc == 1)
	{
		_wsystem(L"CLS\n");

		for (int i = 0; i < nStats; i++)
		{
			m_stats[i] = diceRoll(6, 3);
			wprintf(L"%-30.30s\t%02d\n", m_statName[i], m_stats[i]);
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

	SelectAlignment();
}


/**********************
Created 11/18/2017
User selects the race of the new character
***********************/
bool EOBCharacter::SelectRace()
{
	int chc = 0, nStats;
	
	nStats = NewRace + 1;
	_wsystem(L"CLS\n");

	for (int i = 0; i < nStats; i++)
	{
		wprintf(L"%2d\t%-30.30s\n", i + 1, m_raceNames[i]);
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
	

	nClasses = NewClass + 1;
	_wsystem(L"CLS\n");

	for (int i = 0; i < nClasses; i++)
	{
		wprintf(L"%2d\t%-30.30s\n", i + 1, m_classNames[i]);
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
	enum Alignment { LawfulGood, ChaoticGood, New };
	const wchar_t alignmentNames[New + 1][30] = { { L"Lawful Good" },{ L"Chaotic Good" },{ L"<new>" } };

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

/**********************
Created 11/19/2017
Shows character stats and enters name, last chance to cancel
***********************/
void EOBCharacter::FinalEntry()
{
	int chc = 0;
	wstring name;
	_wsystem(L"CLS\n");

	for (int i = 0; i < NewStat; i++)
		wprintf(L"%-30.30s\t%02d\n", m_statName[i], m_stats[i]);

	wprintf(L"\n%s %s", m_raceNames[ m_stats[8] ], m_classNames[ m_stats[7] ] );

	wprintf(L"\n1>  Keep\n");
	wprintf(L"2>  Cancel\n");
	
	while (chc < 1 || chc > 2)
	{
		wprintf(L"Enter choice:  ");
		cin >> chc;
		cout << endl;
	}

	wprintf(L"Character Name:  ");
	wcin >> name;
	cout << endl;

}
