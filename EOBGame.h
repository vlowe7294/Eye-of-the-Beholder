#pragma once

class EOBCharacter
{
public:
	EOBCharacter();
	~EOBCharacter();
	void NewCharacter();

private:
	bool SelectRace();
	bool SelectClass();
	void SelectAlignment();

};

class EOBGame
{
public:
	EOBGame();
	~EOBGame();
	void Start();
	bool NewGame();
	
};

