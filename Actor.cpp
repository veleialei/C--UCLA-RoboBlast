#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>  
using namespace std;

int randomNumber(int n) {
	srand((unsigned)time(NULL));
	return rand() % n;
}

#pragma region Actor

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, bool passable, bool fireable, GraphObject::Direction dir, int sub_level, int HP)
	:GraphObject(imageID, startX, startY, dir, sub_level), m_HP(HP), m_world(world), m_passable(passable), m_fireable(fireable) {

}

bool Actor::isAlive() {
	if (m_HP <= 0)
		return false;
	else
		return true;
}

bool Actor::isPassable() {
	return m_passable;
}

bool Actor::isFireable() {
	return m_fireable;
}

int Actor::getHP() {
	return m_HP;
}

StudentWorld* Actor::getWorld() {
	return m_world;
}

bool Actor::checkStuck(int nextX, int nextY) {
	bool stuck = false;
	for (int i = 0; i < getWorld()->size(); i++) {
		Actor* temp = getWorld()->getActor(i);
		if (!temp->isPassable())
			if (temp->getX() == nextX && temp->getY() == nextY)
				stuck = true;
	}
	return stuck;
}

bool Actor::checkShot(int nextX, int nextY) {
	for (int i = 0; i < getWorld()->size(); i++) {
		Actor* temp = getWorld()->getActor(i);
		if (temp->isFireable())
			if (temp->getX() == nextX && temp->getY() == nextY) {
				temp->decHealth(2);
				if (temp->isAlive())
					getWorld()->playSound(SOUND_ENEMY_IMPACT);
				return true;
			}
	}
	Actor* Player = getWorld()->getPlayer();
	if (Player->getX() == nextX && Player->getY() == nextY) {
		Player->decHealth(2);
		if (Player->isAlive())
			getWorld()->playSound(SOUND_PLAYER_IMPACT);
		return true;
	}
	return false;
}

bool Actor::checkDir(int X, int Y, int nextX, int nextY, GraphObject::Direction dir) {
	if (nextX == X) {
		if (dir == GraphObject::Direction::up) {
			if (nextY > Y)
				return true;
		}
		else if (dir == GraphObject::Direction::down) {
			if (nextY < Y)
				return true;
		}
	}
	else if (nextY == Y) {
		if (dir == GraphObject::Direction::left) {
			if (nextX < X)
				return true;
		}
		else if (dir == GraphObject::Direction::right) {
			if (nextX > X)
				return true;
		}
	}
	return false;
}

bool Actor::checkPlayer(int X, int Y, GraphObject::Direction dir) {
	bool blind = false;
	int playerX = getWorld()->getPlayer()->getX();
	int playerY = getWorld()->getPlayer()->getY();
	bool find = checkDir(X, Y, playerX, playerY, dir);
	if (find) {
		for (int i = 0; i < getWorld()->size(); i++) {
			Actor* temp = getWorld()->getActor(i);
			if (!temp->isPassable()
			||	temp->getID() == IID_FAKE_WALL) {
				int wallX = temp->getX();
				int wallY = temp->getY();
				if (checkDir(X, Y, wallX, wallY, dir)) {
					switch (dir) {
					case GraphObject::Direction::down:
						if (Y > wallY && wallY > playerY)
							blind = true;
						break;
					case GraphObject::Direction::up:
						if (Y < wallY && wallY < playerY)
							blind = true;
						break;
					case GraphObject::Direction::left:
						if (X > wallX && wallX > playerX)
							blind = true;
						break;
					case GraphObject::Direction::right:
						if (X < wallX && wallX < playerX)
							blind = true;
						break;
					}
				}
			}
		}
	}
	return find && !blind;
}

bool Actor::stuckPlayer(int X, int Y) {
	bool stuck = false;
	int playerX = getWorld()->getPlayer()->getX();
	int playerY = getWorld()->getPlayer()->getY();
	if (playerX == X && playerY == Y)
		stuck = true;
	return stuck;
}

Actor* Actor::checkBully() {
	int nextX = getX();
	int nextY = getY();
	for (int i = 0; i < getWorld()->size(); i++) {
		Actor* temp = getWorld()->getActor(i);
		if (temp->getID() == IID_BULLY)
			if (temp->getX() == nextX && temp->getY() == nextY) {
				return temp;
			}
	}
	return nullptr;
}

bool Actor::incHealth(int amount) {
	if ((m_HP + amount) < 20)
		m_HP += amount;
	else
		m_HP = 20;
	return true;
}

bool Actor::decHealth(int amount) {
	if ((m_HP - amount) > 0) {
		m_HP -= amount;
		return true;
	}
	else {
		m_HP = 0;
		return false;
	}
}

bool Actor::die() {
	if (!isAlive())
		return false;
	else {
		m_HP = 0;
		return true;
	}
}

void Actor::release(Actor* actor) {
	return;
}

GraphObject::Direction Actor::goOppDir(GraphObject::Direction dir) {
	switch (dir) {
	case GraphObject::Direction::left:
		return GraphObject::Direction::right;
		break;
	case GraphObject::Direction::right:
		return GraphObject::Direction::left;
		break;
	case GraphObject::Direction::up:
		return GraphObject::Direction::down;
		break;
	case GraphObject::Direction::down:
		return GraphObject::Direction::up;
		break;
	default:
		return GraphObject::Direction::none;
		break;
	}
}

void moveByDir(GraphObject::Direction dir, int& X, int& Y) {
	switch (dir) {
	case GraphObject::Direction::left:
		X--;
		break;
	case GraphObject::Direction::right:
		X++;
		break;
	case GraphObject::Direction::up:
		Y++;
		break;
	case GraphObject::Direction::down:
		Y--;
		break;
	default:
		break;
	}
	return;
}

#pragma endregion

#pragma region Player

Player::Player(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Actor(imageID, startX, startY, world, false, true, dir, sub_level, HP) {
}

bool Player::doSomething() {
	int ch;
	getWorld()->getKey(ch);
	int nextX = getX();
	int nextY = getY();
	GraphObject::Direction dir = GraphObject::Direction::none;
	switch (ch) {
	case KEY_PRESS_LEFT:
		setDirection(GraphObject::Direction::left);
		dir = GraphObject::Direction::left;
		break;
	case KEY_PRESS_RIGHT:
		setDirection(GraphObject::Direction::right);
		dir = GraphObject::Direction::right;
		break;
	case KEY_PRESS_UP:
		setDirection(GraphObject::Direction::up);
		dir = GraphObject::Direction::up;
		break;
	case KEY_PRESS_DOWN:
		setDirection(GraphObject::Direction::down);
		dir = GraphObject::Direction::down;
		break;
	case KEY_PRESS_SPACE:
		if (getWorld()->ammo() > 0) {
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			getWorld()->fire(getX(), getY(), getDirection());
			getWorld()->decAmmo();
		}
		break;
	case KEY_PRESS_ESCAPE:
		die();
		break;
	default:
		break;
	}
	moveByDir(dir, nextX, nextY);

	bool stuck = checkStuck(nextX, nextY);
	if (!stuck)
		moveTo(nextX, nextY);
	return true;
}

bool Player::decHealth(int amount) {
	for (int i = 0; i < getWorld()->sublevel(); i++)
		getWorld()->getPlayer(i)->Actor::decHealth(amount);
	return true;
}

bool Player::incHealth(int amount) {
	for (int i = 0; i < getWorld()->sublevel(); i++)
		getWorld()->getPlayer(i)->Actor::incHealth(amount);
	return true;
}

#pragma endregion

#pragma region Bullets

Bullets::Bullets(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Actor(imageID, startX, startY, world, true, false, dir, sub_level, HP) {

}

bool Bullets::doSomething() {
	int nextX = getX();
	int nextY = getY();
	moveByDir(getDirection(), nextX, nextY);
	bool stuck = checkStuck(nextX, nextY);
	bool shot = checkShot(nextX, nextY);
	if (shot)
		die();
	else if (!stuck)
		moveTo(nextX, nextY);
	else
		die();
	return true;
}

#pragma endregion

#pragma region Walls_&_FakeWalls

FxNPO::FxNPO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Actor(imageID, startX, startY, world, false, false, dir, sub_level, HP) {

}

Wall::Wall(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: FxNPO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Wall::doSomething() {
	return true;
}

BullyNest::BullyNest(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: FxNPO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool BullyNest::doSomething() {
	if (!isAlive())
		return true;
	else {
		int bullyCount = 0;
		bool bullySquare = false;
		for (int i = 0; i < getWorld()->size(); i++) {
			Actor* tempActor = getWorld()->getActor(i);
			if (tempActor->getID() == IID_BULLY) {
				int targetX = tempActor->getX();
				int targetY = tempActor->getY();
				if (abs(targetX - getX()) <= 3 && abs(targetY - getY()) <= 3)
					bullyCount++;
				if (targetX == getX() && targetY == getY())
					bullySquare = true;
			}
		}
		if (bullyCount < 3 && !bullySquare) {
			if (randomNumber(50) == 0) {
				getWorld()->createBully(getX(), getY());
				getWorld()->playSound(SOUND_BULLY_BORN);
			}
		}
		return true;
	}
}

FxPO::FxPO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Actor(imageID, startX, startY, world, true, false, dir, sub_level, HP) {

}

FakeWall::FakeWall(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: FxPO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool FakeWall::doSomething() {
	return true;
}

#pragma endregion

#pragma region Moveable_Actors

Waterpool::Waterpool(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: FxNPO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Waterpool::doSomething() {
	decHealth(1);
	return true;
}

MO::MO(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Actor(imageID, startX, startY, world, false, true, dir, sub_level, HP) {

}

bool MO::firePlayer(int nextX, int nextY) {
	if (checkPlayer(nextX, nextY, getDirection())) {
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		getWorld()->fire(getX(), getY(), getDirection());
	}
	return true;
}

bool MO::walk(int nextX, int nextY) {
	moveByDir(getDirection(), nextX, nextY);
	bool stuck = checkStuck(nextX, nextY);
	bool stuckP = stuckPlayer(nextX, nextY);
	if (!stuck && !stuckP)
		moveTo(nextX, nextY);
	else if (stuckP)
		getWorld()->fire(getX(), getY(), getDirection());
	else if (stuck)
		setDirection(goOppDir(getDirection()));
	return true;
}

Gangster::Gangster(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: MO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Gangster::doSomething() {
	if (getWorld()->time() % getWorld()->ticks() != 0)
		return true;
	int nextX = getX();
	int nextY = getY();
	firePlayer(nextX, nextY);
	walk(nextX, nextY);
	return true;
}

RobotBoss::RobotBoss(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: MO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool RobotBoss::doSomething() {
	if (getWorld()->time() % getWorld()->ticks() != 0)
		return true;
	int nextX = getX();
	int nextY = getY();
	firePlayer(nextX, nextY);
	walk(nextX, nextY);
	return true;
}

Bully::Bully(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: MO(imageID, startX, startY, world, dir, sub_level, HP), m_parameter(randomNumber(6)) {
	
}

bool Bully::walk(int nextX, int nextY) {
	int curX = nextX;
	int curY = nextY;
	moveByDir(getDirection(), nextX, nextY);
	bool stuck = checkStuck(nextX, nextY);
	bool stuckP = stuckPlayer(nextX, nextY);
	if (!stuck && !stuckP) {
		m_parameter--;
		moveTo(nextX, nextY);
	}
	else if (stuckP)
		getWorld()->fire(getX(), getY(), getDirection());
	if (stuck || stuckP || m_parameter == 0) {
		m_parameter = randomNumber(6);
		int direction = 1 + randomNumber(4);
		int d = direction;
		for (int i = 0; i < 3; i++) {
			int prevX = curX;
			int prevY = curY;
			direction = direction % 4 + 1;
			GraphObject::Direction dir = (GraphObject::Direction) direction;
			moveByDir(dir, curX, curY);
			bool stuck = checkStuck(curX, curY);
			bool stuckP = stuckPlayer(curX, curY);
			if ((!stuck && !stuckP)) {
				setDirection(dir);
				moveTo(curX, curY);
				break;
				return true;
			}
			curX = prevX;
			curY = prevY;
		}
		setDirection((GraphObject::Direction) d);
	}

	return true;
}

bool Bully::doSomething() {
	if (getWorld()->time() % getWorld()->ticks() != 0)
		return true;
	int nextX = getX();
	int nextY = getY();
	firePlayer(nextX, nextY);
	walk(nextX, nextY);
	return true;
}

#pragma endregion

#pragma region Goodie

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: FxPO(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Goodie::checkPlayer()
{
	int x = getWorld()->getPlayer()->getX();
	int y = getWorld()->getPlayer()->getY();
	if (x == getX() && y == getY())
		return true;
	else
		return false;
}

ExtraLife::ExtraLife(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {
	m_pickUp = nullptr;
}

void ExtraLife::pickup(Actor* actor) {
	setVisible(false);
	getWorld()->playSound(SOUND_BULLY_MUNCH);
	m_pickUp = actor;
}

void ExtraLife::release(Actor* actor) {
	if (actor == m_pickUp) {
		setVisible(true);
		m_pickUp = nullptr;
	}
}

bool ExtraLife::doSomething() {
	Actor* bully = checkBully();
	if (bully != nullptr) {
		pickup(bully);
		return true;
	}
	if (!isAlive() || !checkPlayer() || !isVisible()) {
		return true;
	}
	else {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(150);
		getWorld()->incLives();
		die();
		return true;
	}
}

RestoreHealth::RestoreHealth(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {
	m_pickUp = nullptr;
}

void RestoreHealth::pickup(Actor* actor) {
	setVisible(false);
	getWorld()->playSound(SOUND_BULLY_MUNCH);
	m_pickUp = actor;
}

void RestoreHealth::release(Actor* actor) {
	if (actor == m_pickUp) {
		setVisible(true);
		m_pickUp = nullptr;
	}
}

bool RestoreHealth::doSomething() {
	Actor* bully = checkBully();
	if (bully != nullptr) {
		pickup(bully);
		return true;
	}
	if (!isAlive() || !checkPlayer() || !isVisible()) {
		return true;
	}
	else {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(250);
		getWorld()->getPlayer()->incHealth(20);
		die();
		return true;
	}
}

Ammo::Ammo(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {
	m_pickUp = nullptr;
}

void Ammo::pickup(Actor* actor) {
	setVisible(false);
	getWorld()->playSound(SOUND_BULLY_MUNCH);
	m_pickUp = actor;
}

void Ammo::release(Actor* actor) {
	if (actor == m_pickUp) {
		setVisible(true);
		m_pickUp = nullptr;
	}
}

bool Ammo::doSomething() {
	Actor* bully = checkBully();

	if (bully != nullptr) {
		pickup(bully);
		return true;
	}
	if (!isAlive() || !checkPlayer() || !isVisible())
		return true;
	else {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(150);
		getWorld()->incAmmo(25);
		die();
		return true;
	}
}

FarplaneGun::FarplaneGun(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool FarplaneGun::doSomething() {
	if (!isAlive() || !checkPlayer())
		return true;
	else {
		getWorld()->playSound(SOUND_GOT_FARPLANE_GUN);
		for (int i = 0; i < getWorld()->size(); i++) {
			Actor* tempActor = getWorld()->getActor(i);
			if (tempActor->getID() == IID_BULLY
				|| tempActor->getID() == IID_GANGSTER) {
				int targetX = tempActor->getX();
				int targetY = tempActor->getY();
				if (abs(targetX - getX()) <= 4 && abs(targetY - getY()) <= 4)
					tempActor->die();
			}
		}
		getWorld()->getPlayer()->decHealth(10);
		cout << getWorld()->getPlayer(0)->getHP() << getWorld()->getPlayer(1)->getHP() << getWorld()->getPlayer(2)->getHP() << endl;
		die();
		return true;
	}
}

Gold::Gold(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Gold::doSomething() {
	if (!isAlive() || !checkPlayer())
		return true;
	else {
		getWorld()->playSound(SOUND_GOT_GOLD);
		getWorld()->increaseScore(100);
		getWorld()->decGold();
		die();
		return true;
	}
}

Hostage::Hostage(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Hostage::doSomething() {
	if (!isAlive() || !checkPlayer())
		return true;
	else {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->decHostage();
		die();
		return true;
	}
}

Gate::Gate(int imageID, int startX, int startY, StudentWorld* world, int targetsub, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP), m_target(targetsub) {

}

bool Gate::doSomething() {
	if (!isAlive() || !checkPlayer())
		return true;
	else {
		bool result = getWorld()->changeSubLevel(m_target);
		die();
		return result;
	}
}

Exit::Exit(int imageID, int startX, int startY, StudentWorld* world, GraphObject::Direction dir, int sub_level, int HP)
	: Goodie(imageID, startX, startY, world, dir, sub_level, HP) {

}

bool Exit::doSomething() {
	if (!isAlive() || !checkPlayer() || !isVisible())
		return true;
	else {
		getWorld()->enterExit();
		getWorld()->increaseScore(1500);
		die();
		return true;
	}
	return true;
}

#pragma endregion