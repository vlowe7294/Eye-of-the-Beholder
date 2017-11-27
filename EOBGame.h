#pragma once

struct EOBLOCATION
{
	int x;
	int y;
};

class EOBObject
{
public:
	EOBObject();
	~EOBObject();
	bool IsHere(EOBLOCATION plrLoc);

protected:
	EOBLOCATION m_location;

};

class EOBItem : public EOBObject
{
public:
	EOBItem();
	~EOBItem();
	bool ParseCommand(wstring cmd, EOBLOCATION loc);

private:
	void AddToInventory();
};

class EOBCharacter
{
public:
	EOBCharacter();
	~EOBCharacter();
	void NewCharacter();
	void Equip();
	bool ParseCommand(wstring cmd, EOBLOCATION loc);

private:
	enum Stat { Strength, Intelligence, Wisdom, NewStat };
	const static wchar_t m_statName[NewStat][30];
	enum Class { Fighter, Ranger, NewClass };
	const static wchar_t m_classNames[NewClass + 1][30];
	enum Race { Human, Elf, NewRace };
	const static wchar_t m_raceNames[NewRace + 1][30];

	int m_stats[10];
	EOBItem* m_pMainHand;
	
	bool SelectRace();
	bool SelectClass();
	void SelectAlignment();
	void FinalEntry();


};

class EOBDoor : public EOBObject
{
public:
	EOBDoor(char dir);
	~EOBDoor();
	void Round(EOBLOCATION plrLoc);
	bool GetIsOpen() { return m_bIsOpen; };
	void SetIsOpen(bool bOpen) { m_bIsOpen = bOpen; };
	bool ParseCommand(wstring cmd, EOBLOCATION loc);

private:
	char m_direction;
	bool m_bIsOpen;

};

class EOBPlaceable : public EOBObject
{
public:
	EOBPlaceable();
	~EOBPlaceable();

private:
};

struct ROOM
{
	char ndx;
	char exits[4];
	char exitDirs[4];
	wchar_t desc[255];
	EOBDoor* pDoor;
	EOBItem* pItem;
};

class EOBGame
{
public:
	EOBGame();
	~EOBGame();
	void Start();
	void GameLoop();
	bool NewGame();
	void AddToInventory(EOBItem* pItem);
	static EOBGame& GetGame() { return *m_pTheGame;  };
	void Save();
	bool Load();
	EOBItem* GetInventory();

private:
	EOBLOCATION m_playerLoc;
	static EOBGame* m_pTheGame;
	EOBItem* m_pInventory;
	ROOM* m_currentRoom;
	EOBCharacter* m_pChar;

};