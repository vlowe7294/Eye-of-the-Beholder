#include "stdafx.h"
#include "EOBGame.h"

ROOM map[2] =
{
	{ 0, { 2, 0, 0, 0 },{ 'n', 'x', 'x', 'x' }, {L"You are on the first level of the sewers of Waterdeep.  The exit tunnel has caved in.\nYou have no choice but to go forward."}, NULL, NULL },
	{ 1, { 1, 0, 0, 0 },{ 'x', 's', 'x', 'x' },{ L"You have gone through the north door." }, NULL, NULL }
};



EOBGame* EOBGame::m_pTheGame = NULL;

EOBGame::EOBGame()
{
	m_playerLoc.y = m_playerLoc.x = 0;
	m_pTheGame = this;
	m_pInventory = NULL;
	m_currentRoom = map;
	m_pChar = new EOBCharacter();

	Load();
}


EOBGame::~EOBGame()
{
	delete map[0].pDoor;
	map[0].pDoor = NULL;
	delete map[1].pDoor;
	map[1].pDoor = NULL;
	m_pTheGame = NULL;
	delete m_pInventory;
	delete m_pChar;
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
		map[0].pDoor = new EOBDoor('n');
		map[0].pItem = new EOBItem();
		map[1].pDoor = new EOBDoor('s');
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

	delete m_pChar;
	m_pChar = new EOBCharacter();
	m_pChar->NewCharacter();

	return false;

}

void EOBGame::GameLoop()
{
	bool bGameEnded = false, bLook = true, bIsValid = false;
	wstring input;
	
	EOBDoor* pDoor;
	EOBItem* pItem;

	while (!bGameEnded)
	{
		pDoor = m_currentRoom->pDoor;
		pItem = m_currentRoom->pItem;

		if (bLook)
		{
			_wsystem(L"CLS\n");
			wprintf(L"%s\n", m_currentRoom->desc);

			if (pDoor != NULL)
				pDoor->Round(m_playerLoc);

			if (pItem != NULL)
				wprintf(L"There is an axe on the ground.\n");

			bLook = false;

		}

		bIsValid = false;
		wprintf(L"Your action?  ");

		std::getline(wcin, input);
		cout << endl;

		if (pDoor != NULL)
			bIsValid = pDoor->ParseCommand(input, m_playerLoc);

		if (pItem != NULL && !bIsValid)
			bIsValid = pItem->ParseCommand(input, m_playerLoc);

		if (!bIsValid)		
			bIsValid = m_pChar->ParseCommand(input, m_playerLoc);

		if (!bIsValid)
		{
			if (input.length() < 1)
				input = L"xxx";

			switch (input[0])
			{

			case 'l':
				if (input == L"look")
				{
					bLook = true;
					bIsValid = true;
				}
				break;

			case 'q':
				if (input == L"quit")
				{
					bGameEnded = true;
					bIsValid = true;
					//Save();
				}
				break;

			case 'n':
				bIsValid = true;

				if (m_currentRoom->exitDirs[0] == 'n' && m_currentRoom->exits[0] > 0)
				{
					m_currentRoom = &map[m_currentRoom->exits[0] - 1];

					if (m_currentRoom->pDoor != NULL)
						m_currentRoom->pDoor->SetIsOpen(true);
					bLook = true;
				}
				else
					wprintf(L"You can't go that way\n");

				break;

			case 's':
				bIsValid = true;

				if (m_currentRoom->exitDirs[1] == 's')
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
Created 11/222/2017
Adds item to player inventory */
void EOBGame::AddToInventory(EOBItem* pItem)
{
	wprintf(L"You pick up the axe.\n");
	delete m_pInventory;
	m_pInventory = pItem;

	if (m_currentRoom->pItem == pItem)
		m_currentRoom->pItem = NULL;

}


/**********************
Created 11/24/2017
saves game to file*/
void EOBGame::Save()
{
	ofstream fle;
	int nBytes = 0, nCount = 0;

	fle.open("C:\\Users\\Vaughn\\Documents\\eob.dat", ios::out | ios::trunc | ios::binary);

	if (!fle.is_open())
		return;

	fle.write((char*)&m_playerLoc, 8);

	if (m_currentRoom != NULL)
	{
		nBytes = 513;
		nCount = 1;
		fle.write((char*)&nBytes, 4);

		fle.write((char*)&nCount, 4);
		fle.write(&m_currentRoom->ndx, 1);

		fle.write(m_currentRoom->exits, 4);
		fle.write(m_currentRoom->exits, 4);
		fle.write(m_currentRoom->exitDirs, 4);
		fle.write((char*)m_currentRoom->desc, 500);
	}
	else
	{
		nBytes = 0;
		fle.write((char*)&nBytes, 4);
	}


	fle.close();

}

bool EOBGame::Load()
{
	ifstream fle;
	int nBytes = 0, nCount = 0;
	ROOM aRoom;

	fle.open("C:\\Users\\Vaughn\\Documents\\eob.dat", ios::in | ios::binary);

	if (!fle.is_open())
		return false;

	fle.read((char*)&m_playerLoc, 8);
	fle.read((char*)&nBytes, 4);

	if (nBytes == 513)
	{
		fle.read((char*)&nCount, 4);
		fle.read(&aRoom.ndx, 1);
		fle.read(aRoom.exits, 4);

		fle.read(aRoom.exits, 4);
		fle.read(aRoom.exitDirs, 4);
		fle.read((char*)aRoom.desc, 500);

		if (aRoom.ndx >= 0 && aRoom.ndx < 2)
		{
			aRoom.pDoor = map[aRoom.ndx].pDoor;
			aRoom.pItem = map[aRoom.ndx].pItem;

			m_currentRoom = &map[aRoom.ndx];
			map[aRoom.ndx] = aRoom;
		}
	}
	
	fle.close();
	return true;
}

EOBItem* EOBGame::GetInventory()
{
	EOBItem* ret = m_pInventory;
	m_pInventory = NULL;
	return ret;
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


EOBDoor::EOBDoor(char dir)
{
	m_location.x = m_location.y = 0;
	m_direction = dir;
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

bool EOBDoor::ParseCommand(wstring cmd, EOBLOCATION loc)
{
	if (!IsHere(loc))
		return false;

	wchar_t dir = m_direction;
	wstring str = L" ";

	str[0] = dir;

	if (cmd == L"open")
	{
		m_bIsOpen = true;
		wprintf(L"The door is now open.\n");
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



const wchar_t EOBCharacter::m_statName[EOBCharacter::NewStat][30] = { { L"Strength" },{ L"Intelligence" },{ L"Wisdom" } };
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

	m_pMainHand = NULL;

}

EOBCharacter::~EOBCharacter()
{
	delete m_pMainHand;
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

	wprintf(L"\n%s %s", m_raceNames[m_stats[8]], m_classNames[m_stats[7]]);

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

bool EOBCharacter::ParseCommand(wstring cmd, EOBLOCATION loc)
{
	if (cmd == L"equip")
	{
		Equip();
		return true;
	}


	return false;

}

/**********************
Created 11/25/2017
Equips an item from inventory
***********************/
void EOBCharacter::Equip()
{
	EOBGame& gme = EOBGame::GetGame();
	m_pMainHand = gme.GetInventory();

}



EOBObject::EOBObject()
{

}

EOBObject::~EOBObject()
{

}

bool EOBObject::IsHere(EOBLOCATION plrLoc)
{
	return (plrLoc.x == m_location.x && plrLoc.y == m_location.y);

}

EOBItem::EOBItem()
{
	m_location.x = m_location.y = 0;
}

EOBItem::~EOBItem()
{

}

bool EOBItem::ParseCommand(wstring cmd, EOBLOCATION loc)
{
	if (!IsHere(loc))
		return false;

	if (cmd == L"get")
	{
		AddToInventory();
		return true;
	}


	return false;

}

void EOBItem::AddToInventory()
{
	EOBGame::GetGame().AddToInventory(this);
}



EOBPlaceable::EOBPlaceable()
{

}

EOBPlaceable::~EOBPlaceable()
{

}