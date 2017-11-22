#pragma once

class EOBCharacter
{
public:
	EOBCharacter();
	~EOBCharacter();
	void NewCharacter();

private:
	enum Stat { Strength, Intelligence, Wisdom, NewStat };
	const static wchar_t m_statName[NewStat][30];
	enum Class { Fighter, Ranger, NewClass };
	const static wchar_t m_classNames[NewClass + 1][30];
	enum Race { Human, Elf, NewRace };
	const static wchar_t m_raceNames[NewRace + 1][30];

	int m_stats[10];

	bool SelectRace();
	bool SelectClass();
	void SelectAlignment();
	void FinalEntry();


};

struct EOBLOCATION
{
	int x;
	int y;
};

class EOBDoor
{
public:
	EOBDoor();
	~EOBDoor();
	bool IsHere(EOBLOCATION plrLoc);
	void Round(EOBLOCATION plrLoc);
	bool GetIsOpen() { return m_bIsOpen; };
	void SetIsOpen(bool bOpen) { m_bIsOpen = bOpen; };
	bool ParseCommand(wstring cmd, EOBLOCATION loc);

private:
	EOBLOCATION m_location;
	char m_direction;
	bool m_bIsOpen;

};

struct ROOM
{
	char exits[4];
	char exitDirs[4];	
	EOBDoor* pDoor;
};

class EOBGame
{
public:
	EOBGame();
	~EOBGame();
	void Start();
	void GameLoop();
	bool NewGame();

private:
	EOBLOCATION m_playerLoc;
	
};