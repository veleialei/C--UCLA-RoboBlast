#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	std::string getFilename(int level, int subLevel);
	int init();
	int mapStatus(std::string filename, int sublevel, Level::LoadResult &result);
	void loadMap(int sublevel);
	virtual int move();
	virtual void cleanUp();
	void dieSound(int ID);

	// actions by actors
	bool fire(int X, int Y, GraphObject::Direction dir);
	bool createWaterpool(int X, int Y);
	bool createHostage(int X, int Y);
	bool createBully(int X, int Y);

	// utilities
	virtual int getCurrentSubLevel();
	bool changeSubLevel(int sublevel);
	int size();
	int sublevel();
	int time();
	int ticks();
	int ammo();
	bool incAmmo(int amount);
	bool decAmmo();
	void enterExit();
	Actor* getActor(int i);
	Actor* getPlayer();
	Actor* getPlayer(int sublevel);
	bool decHostage();
	bool decGold();
	bool decRobots();
private:
	// game records
	int m_time;
	int m_sublevel;
	int m_ammo;
	int m_bouns;

	// internal data structures
	std::vector<Level*>					m_level;
	std::vector<std::vector<Actor*>*>	m_actors;
	std::vector<Actor*>					m_player;

	// status variables
	std::string							m_gamePrompt;
	bool								exit_visible;
	bool								exit_entered;
	int									n_robots;
	int									n_golds;
	int									n_hostages;

	// internal functions
	Actor* putActors(int imageID, int startX, int startY, GraphObject::Direction dir, int sub_level);
	void updateDisplayText();
	bool makeExitVisible();
};

#endif // STUDENTWORLD_H_