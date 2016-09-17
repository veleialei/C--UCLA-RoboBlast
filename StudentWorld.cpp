#include "StudentWorld.h"
#include <string>
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;
const int GWSTATUS_SUBLEVEL_NOTFOUND = 5;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

#pragma region initialization

StudentWorld::StudentWorld(string assetDir) :GameWorld(assetDir){
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

string StudentWorld::getFilename(int level, int subLevel) {
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << level;
	if (subLevel != 0)
		oss << "_" << subLevel;
	oss << ".dat";
	return oss.str();
}

int StudentWorld::init() {
	m_time = 3000;
	m_sublevel = 0;
	m_ammo = 0;
	m_bouns = 0;
	exit_visible = false;
	exit_entered = false;
	n_robots = 0;
	n_golds = 0;
	n_hostages = 0;

	int sublevel = 0;
	if (getLevel() == 100)
		return GWSTATUS_PLAYER_WON;
	while (1) {
		m_level.push_back(nullptr);
		m_level[sublevel] = new Level(assetDirectory());
		Level::LoadResult ret;
		int result = mapStatus(getFilename(getLevel(), sublevel), sublevel, ret);
		if (result == GWSTATUS_LEVEL_ERROR) {
			if (!sublevel && ret == Level::LoadResult::load_fail_file_not_found)
				return GWSTATUS_PLAYER_WON;
			else
				return result;
		} else if (result == GWSTATUS_SUBLEVEL_NOTFOUND)
			break;
		else
			loadMap(sublevel);
		sublevel++;
	}
	return 0;
}

int StudentWorld::mapStatus(string filename, int sublevel, Level::LoadResult &result) {
	result = m_level[sublevel]->loadLevel(filename, sublevel);
	cerr << "For file " << filename << "\t";
	int ret;
	switch (result) {
	case Level::LoadResult::load_fail_bad_format:
		cerr << "bad format for main level file!\n";
		ret = GWSTATUS_LEVEL_ERROR;
		break;
	case Level::LoadResult::load_fail_file_not_found:
		cerr << "main level not found!\n";
		ret = GWSTATUS_LEVEL_ERROR;
		break;
	case Level::LoadResult::load_success:
		cerr << "main level load success!\n";
		ret = GWSTATUS_CONTINUE_GAME;
		break;
	case Level::LoadResult::load_sublevel_fail_bad_format:
		cerr << "bad format for sublevel file!\n";
		ret = GWSTATUS_LEVEL_ERROR;
		break;
	case Level::LoadResult::load_sublevel_fail_file_not_found:
		cerr << "sublevel not found!\n";
		ret = GWSTATUS_SUBLEVEL_NOTFOUND;
		break;
	case Level::LoadResult::load_sublevel_success:
		cerr << "sublevel load success!\n";
		ret = GWSTATUS_CONTINUE_GAME;
		break;
	}
	return ret;
}

void StudentWorld::loadMap(int sublevel) {
	m_actors.push_back(nullptr);
	m_player.push_back(nullptr);
	vector<Actor*>* temp_actors = new vector<Actor*>;
	for (int y = 0; y < VIEW_HEIGHT; y++) {
		for (int x = 0; x < VIEW_WIDTH; x++) {
			Level::MazeEntry entry = m_level[sublevel]->getContentsOf(x, y, sublevel);
			if (entry == Level::MazeEntry::player)
				m_player[sublevel] = putActors(entry, x, y, GraphObject::Direction::right, sublevel);
			else if (entry != Level::MazeEntry::player && entry != Level::MazeEntry::empty)
				temp_actors->push_back(putActors(entry, x, y, GraphObject::Direction::none, sublevel));
		}
	}
	m_actors[sublevel] = temp_actors;
}

// conversion from map symbols to the actual actor objects.
// dir and HP come from the spec, while type, x, y, sub_level come from the map data file.
Actor* StudentWorld::putActors(int type, int startX, int startY, GraphObject::Direction dir, int sub_level) {
	Actor* tempActor = nullptr;
	switch (type)
	{
	case Level::MazeEntry::gate0:
		tempActor = new Gate(IID_GATE, startX, startY, this, 0, dir, sub_level);
		break;
	case Level::MazeEntry::gate1:
		tempActor = new Gate(IID_GATE, startX, startY, this, 1, dir, sub_level);
		break;
	case Level::MazeEntry::gate2:
		tempActor = new Gate(IID_GATE, startX, startY, this, 2, dir, sub_level);
		break;
	case Level::MazeEntry::gate3:
		tempActor = new Gate(IID_GATE, startX, startY, this, 3, dir, sub_level);
		break;
	case Level::MazeEntry::gate4:
		tempActor = new Gate(IID_GATE, startX, startY, this, 4, dir, sub_level);
		break;
	case Level::MazeEntry::gate5:
		tempActor = new Gate(IID_GATE, startX, startY, this, 5, dir, sub_level);
		break;
	case Level::MazeEntry::exit:
		tempActor = new Exit(IID_EXIT, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::player:
		tempActor = new Player(IID_PLAYER, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::horiz_gangster:
		tempActor = new Gangster(IID_GANGSTER, startX, startY, this, GraphObject::Direction::right, sub_level);
		break;
	case Level::MazeEntry::vert_gangster:
		tempActor = new Gangster(IID_GANGSTER, startX, startY, this, GraphObject::Direction::down, sub_level);
		break;
	case Level::MazeEntry::bully_nest:
		tempActor = new BullyNest(IID_BULLY_NEST, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::wall:
		tempActor = new Wall(IID_WALL, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::fake_wall:
		tempActor = new FakeWall(IID_FAKE_WALL, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::hostage:
		tempActor = new Hostage(IID_HOSTAGE, startX, startY, this, dir, sub_level);
		n_hostages++;
		break;
	case Level::MazeEntry::gold:
		tempActor = new Gold(IID_GOLD, startX, startY, this, dir, sub_level);
		n_golds++;
		break;
	case Level::MazeEntry::restore_health:
		tempActor = new RestoreHealth(IID_RESTORE_HEALTH, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::extra_life:
		tempActor = new ExtraLife(IID_EXTRA_LIFE, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::ammo:
		tempActor = new Ammo(IID_AMMO, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::waterpool:
		tempActor = new Waterpool(IID_WATERPOOL, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::farplane_gun:
		tempActor = new FarplaneGun(IID_FARPLANE_GUN, startX, startY, this, dir, sub_level);
		break;
	case Level::MazeEntry::robot_boss:
		tempActor = new RobotBoss(IID_ROBOT_BOSS, startX, startY, this, GraphObject::Direction::right, sub_level);
		n_robots++;
		break;
	default:
		break;
	}
	if (type != Level::MazeEntry::exit)
		tempActor->setVisible(true);
	else
		tempActor->setVisible(exit_visible);
	return tempActor;
}

#pragma endregion

#pragma region move

int StudentWorld::move() {
	updateDisplayText();
	exit_visible = makeExitVisible();
	m_player[m_sublevel]->doSomething();

	for (int i = 0; i < m_actors[m_sublevel]->size(); i++) {
		if ((*(m_actors[m_sublevel]))[i] != nullptr) {

			// doSomething();
			if (!(*(m_actors[m_sublevel]))[i]->doSomething())
				return GWSTATUS_LEVEL_ERROR;

			// reveal exit
			if ((*(m_actors[m_sublevel]))[i]->getID() == IID_EXIT)
				(*(m_actors[m_sublevel]))[i]->setVisible(exit_visible);

			// delete dead actors();
			if (!(*(m_actors[m_sublevel]))[i]->isAlive()) {
				if ((*(m_actors[m_sublevel]))[i]->getID() == IID_GANGSTER) {
					createWaterpool((*(m_actors[m_sublevel]))[i]->getX(), (*(m_actors[m_sublevel]))[i]->getY());
					increaseScore(100);
				}

				if ((*(m_actors[m_sublevel]))[i]->getID() == IID_ROBOT_BOSS) {
					createHostage((*(m_actors[m_sublevel]))[i]->getX(), (*(m_actors[m_sublevel]))[i]->getY());
					increaseScore(100);
					decRobots();
				}

				if ((*(m_actors[m_sublevel]))[i]->getID() == IID_BULLY) {
					increaseScore(10);
					for (int j = 0; j < m_actors[m_sublevel]->size(); j++) {
						Actor* temp = (*(m_actors[m_sublevel]))[j];
						if (temp->getID() == IID_AMMO
							|| temp->getID() == IID_EXTRA_LIFE
							|| temp->getID() == IID_RESTORE_HEALTH)
							temp->release((*(m_actors[m_sublevel]))[i]);
					}
				}

				dieSound((*(m_actors[m_sublevel]))[i]->getID());
				delete (*(m_actors[m_sublevel]))[i];
				(*(m_actors[m_sublevel]))[i] = nullptr;

				vector<Actor*>* temp = new vector<Actor*>;
				for (int j = 0; j < m_actors[m_sublevel]->size(); j++)
					if ((*(m_actors[m_sublevel]))[j] != nullptr)
						temp->push_back((*(m_actors[m_sublevel]))[j]);
				delete m_actors[m_sublevel];
				m_actors[m_sublevel] = temp;
				// cerr << i << ", " << temp->size() << endl;
			}
		}
	}

	m_time--;	// time elapsed

				// player died
	if (!m_player[m_sublevel]->isAlive() || m_time <= 0) {
		playSound(SOUND_PLAYER_DIE);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	// finish level
	if (exit_entered) {
		increaseScore(m_time);
		return GWSTATUS_FINISHED_LEVEL;
	}

	// continue game
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::updateDisplayText() {
	ostringstream oss;
	oss.fill('0');
	oss << "  Score: " << setw(7) << getScore() << "  ";
	oss << "Level: " << setw(2) << getLevel() << "-";
	oss << setw(2) << m_sublevel << "  ";
	oss.fill(' ');
	oss << "Lives: " << setw(2) << getLives() << "  ";
	oss << "Health: " << setw(3) << m_player[m_sublevel]->getHP() * 100 / 20 << "%  ";
	oss << "Ammo: " << setw(3) << m_ammo << "  ";
	oss << "TimeLimit: " << setw(4) << m_time << "  ";
	string s = oss.str();
	setGameStatText(s);				// calls our provided GameWorld::setGameStatText
	return;
}

bool StudentWorld::makeExitVisible() {
	if (n_hostages == 0 && n_golds == 0 && n_robots == 0)
		return true;
	return false;
}

#pragma endregion

#pragma region clean_up

void StudentWorld::cleanUp() {
	int size = m_level.size() - 1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < m_actors[i]->size(); j++) {
			if ((*(m_actors[i]))[j] != nullptr)
				delete (*(m_actors[i]))[j];
		}
		int vector_size = m_actors[i]->size();
		for (int j = 0; j < vector_size; j++)
			m_actors[i]->pop_back();
		delete m_actors[i];
		delete m_player[i];
		delete m_level[i];
	}
	delete m_level[size];
	m_level.pop_back();
	for (int i = 0; i < size; i++) {
		m_actors.pop_back();
		m_player.pop_back();
		m_level.pop_back();
	}
	return;
}

void StudentWorld::dieSound(int ID) {
	int sound = SOUND_NONE;
	switch (ID) {
	case IID_GANGSTER:
	case IID_ROBOT_BOSS:
	case IID_BULLY:
		sound = SOUND_ENEMY_DIE;
		break;
	}
	playSound(sound);
}

#pragma endregion

#pragma region actions

bool StudentWorld::fire(int X, int Y, GraphObject::Direction dir) {
	moveByDir(dir, X, Y);
	Actor* temp = new Bullets(IID_BULLET, X, Y, this, dir, m_sublevel);
	temp->setVisible(true);
	m_actors[m_sublevel]->push_back(temp);
	return true;
}

bool StudentWorld::createWaterpool(int X, int Y) {
	Actor* temp = new Waterpool(IID_WATERPOOL, X, Y, this, GraphObject::Direction::none, m_sublevel);
	temp->setVisible(true);
	m_actors[m_sublevel]->push_back(temp);
	return true;
}

bool StudentWorld::createHostage(int X, int Y) {
	Actor* temp = new Hostage(IID_HOSTAGE, X, Y, this, GraphObject::Direction::none, m_sublevel);
	n_hostages++;
	temp->setVisible(true);
	m_actors[m_sublevel]->push_back(temp);
	return true;
}

bool StudentWorld::createBully(int X, int Y) {
	Actor* temp = new Bully(IID_BULLY, X, Y, this, GraphObject::Direction::right, m_sublevel);
	temp->setVisible(true);
	m_actors[m_sublevel]->push_back(temp);
	return true;
}

#pragma endregion

#pragma region Utilities

int StudentWorld::getCurrentSubLevel() {
	return m_sublevel;
}

bool StudentWorld::changeSubLevel(int sublevel) {
	if (sublevel < m_level.size() - 1) {
		m_sublevel = sublevel;
		return true;
	}
	else
		return false;
}

int StudentWorld::size() {
	return m_actors[m_sublevel]->size();
}

int StudentWorld::sublevel() {
	return m_actors.size();
}

int StudentWorld::time() {
	return m_time;
}

int StudentWorld::ticks() {
	int temp = (28 - getLevel()) / 4;
	if (temp < 3)
		temp = 3;
	return temp;
}

int StudentWorld::ammo() {
	return m_ammo;
}

bool StudentWorld::incAmmo(int amount) {
	if ((m_ammo + amount) < 1000) {
		m_ammo += amount;
		return true;
	}
	else
		return false;
}

bool StudentWorld::decAmmo() {
	if ((m_ammo - 1) >= 0) {
		m_ammo -= 1;
		return true;
	}
	else
		return false;
}

void StudentWorld::enterExit() {
	exit_entered = true;
}

Actor* StudentWorld::getActor(int i) {
	return (*(m_actors[m_sublevel]))[i];
}

Actor* StudentWorld::getPlayer() {
	return m_player[m_sublevel];
}

Actor* StudentWorld::getPlayer(int sublevel) {
	return m_player[sublevel];
}

bool StudentWorld::decHostage() {
	if (n_hostages > 0) {
		n_hostages--;
		return true;
	}
	else
		return false;
}

bool StudentWorld::decGold() {
	if (n_golds > 0) {
		n_golds--;
		return true;
	}
	else
		return false;
}

bool StudentWorld::decRobots() {
	if (n_robots > 0) {
		n_robots--;
		return true;
	}
	else
		return false;
}
#pragma endregion