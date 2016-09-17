#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"

class StudentWorld;
void moveByDir(GraphObject::Direction dir, int& X, int& Y);
int randomNumber(int n);

#pragma region Actor

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, bool passable, bool fireable, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	virtual bool doSomething() = 0;

	// status
	bool isAlive();
	bool isPassable();
	bool isFireable();

	// getter
	StudentWorld* getWorld();
	int getHP();
	bool checkStuck(int nextX, int nextY);
	bool checkShot(int nextX, int nextY);
	bool checkDir(int X, int Y, int nextX, int nextY, GraphObject::Direction dir);
	bool checkPlayer(int X, int Y, GraphObject::Direction dir);
	bool stuckPlayer(int X, int Y);
	Actor* checkBully();

	// action
	virtual bool incHealth(int amount);
	virtual bool decHealth(int amount);
	virtual void release(Actor* actor);
	bool die();
	GraphObject::Direction goOppDir(GraphObject::Direction dir);
private:
	int m_HP;
	bool m_passable;
	bool m_fireable;
	StudentWorld* m_world;
};

#pragma endregion

#pragma region Player

class Player : public Actor {	// Player
public:
	Player(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
	virtual bool decHealth(int amount);
	virtual bool incHealth(int amount);
};

#pragma endregion

#pragma region Bullets

class Bullets : public Actor {	// Bullets
public:
	Bullets(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();

};

#pragma endregion

#pragma region Walls_&_FakeWalls

class FxNPO : public Actor {	// Fixed, non-passable objects	--	purely virtual!
public:
	FxNPO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	virtual bool doSomething() = 0;
};

class FxPO : public Actor {		// Fixed, passable objects		--	purely virtual!
public:
	FxPO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	virtual bool doSomething() = 0;
};

class Wall : public FxNPO {			// Wall
public:
	Wall(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();

};

class BullyNest : public FxNPO {	// Bully Nest
public:
	BullyNest(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();

};

class FakeWall : public FxPO {		// Fake Wall
public:
	FakeWall(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();

};

#pragma endregion

#pragma region Moveable_Actors

class MO : public Actor {		// Moveable Objects				--	purely virtual!
public:
	MO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	virtual bool doSomething() = 0;
	bool firePlayer(int nextX, int nextY);
	virtual bool walk(int nextX, int nextY);
};

class Gangster : public MO {		// Gangster
public:
	Gangster(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 10);
	bool doSomething();

};

class RobotBoss : public MO {		// Robot Boss
public:
	RobotBoss(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 50);
	bool doSomething();

};

class Bully : public MO {			// Bully
public:
	Bully(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 5);
	bool doSomething();
	bool walk(int nextX, int nextY);
private:
	int m_parameter;
};

class Waterpool : public FxNPO {		// Waterpool
public:
	Waterpool(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 30);
	bool doSomething();
};

#pragma endregion

#pragma region Goodie

class Goodie : public FxPO {		// Goodie					--	purely virtual!
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool checkPlayer();
	virtual bool doSomething() = 0;
};

class ExtraLife : public Goodie {			// Extra Life Goodie
public:
	ExtraLife(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
	void pickup(Actor* actor);
	void release(Actor* actor);
private:
	Actor* m_pickUp;
};

class RestoreHealth : public Goodie {		// Restore Health Goodie
public:
	RestoreHealth(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
	void pickup(Actor* actor);
	void release(Actor* actor);
private:
	Actor* m_pickUp;
};

class Ammo : public Goodie {				// Ammo Goodie
public:
	Ammo(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
	void pickup(Actor* actor);
	void release(Actor* actor);
private:
	Actor* m_pickUp;
};

class FarplaneGun : public Goodie {			// FarplaneGun Goodie
public:
	FarplaneGun(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
};

class Gold : public Goodie {				// Gold
public:
	Gold(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
};

class Hostage : public Goodie {				// Hostage
public:
	Hostage(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
};

class Gate : public Goodie {		// Gate
public:
	Gate(int imageID, int startX, int startY, StudentWorld* world, int targetsub, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
private:
	int m_target;
};

class Exit : public Goodie {		// Exit
public:
	Exit(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir = none, int sub_level = 0, int HP = 20);
	bool doSomething();
};

#pragma endregion

#endif // ACTOR_H_