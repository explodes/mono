#include <array>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#include <iostream>
#include <string>

// Generate a random number between min and max (exclusive)
// Assumes srand() has already been called
int getRandomNumber(int min, int max) {
  // static used for efficiency, so we only calculate this value once
  static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

  // evenly distribute the random number across our range
  return static_cast<int>(rand() * fraction * (max - min) + min);
}

class Creature {
 protected:
  std::string name_;
  char symbol_;
  int health_;
  int attack_;
  int gold_;

 public:
  Creature(std::string name, char symbol, int health, int attack, int gold)
      : name_(name),
        symbol_(symbol),
        health_(health),
        attack_(attack),
        gold_(gold) {}

  std::string getName() const { return name_; }
  char getSymbol() const { return symbol_; }
  int getHealth() const { return health_; }
  int getAttack() const { return attack_; }
  int getGold() const { return gold_; }

  bool isDead() const { return health_ <= 0; }

  void reduceHealth(int amount) { health_ -= amount; }
  void addGold(int amount) { gold_ += amount; }
};

class Player : public Creature {
 private:
  int level_ = 1;

 public:
  Player(std::string name) : Creature(name, '@', 10, 1, 0) {}

  void levelUp() {
    ++level_;
    ++attack_;
  }

  int getLevel() { return level_; }
  bool hasWon() { return level_ >= 20; }
};

class Monster : public Creature {
 private:
  enum Type { DRAGON, ORC, SLIME, MAX_TYPES };
  struct MonsterData {
    std::string name;
    char symbol;
    int health;
    int attack;
    int gold;
  };
  static MonsterData monsterData[Monster::MAX_TYPES];
  Monster(const Type type)
      : Creature(monsterData[type].name, monsterData[type].symbol,
                 monsterData[type].health, monsterData[type].attack,
                 monsterData[type].gold) {}

 public:
  static Monster getRandomMonster() {
    Type monsterType = static_cast<Type>(getRandomNumber(0, MAX_TYPES));
    return Monster(monsterType);
  }
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]{
    {"dragon", 'D', 20, 4, 100},
    {"orc", 'o', 4, 2, 25},
    {"slime", 's', 1, 1, 10}};

void init() {
  // set initial seed value to system clock
  srand(static_cast<unsigned int>(time(0)));

  // get rid of first result
  rand();
}

char getAction() {
  char c;
  while (true) {
    std::cout << "(R)un or (F)ight: ";
    std::cin >> c;
    if (c == 'f' || c == 'r' || c == 'F' || c == 'R') {
      break;
    }
  }
  // Convert to lowercase.
  if (c < 'a') {
    c += ('a' - 'A');
  }
  return c;
}

void attackMonster(Player &assailant, Monster &victim) {
  victim.reduceHealth(assailant.getAttack());
  std::cout << "You hit the " << victim.getName() << " for "
            << assailant.getAttack() << " damage." << std::endl;
}

void monsterAttack(Monster &assailant, Player &victim) {
  victim.reduceHealth(assailant.getAttack());
  std::cout << "The " << assailant.getName() << " hit you for "
            << assailant.getAttack() << " damage." << std::endl;
}

void encounter(Player &player) {
  Monster monster = Monster::getRandomMonster();
  std::cout << "You have encountered a " << monster.getName() << " ("
            << monster.getSymbol() << ")." << std::endl;
  while (!player.isDead()) {
    char action = getAction();
    switch (action) {
      case 'r':
        if (getRandomNumber(0, 100) > 50) {
          std::cout << "You successfully fled." << std::endl;
          return;
        } else {
          std::cout << "You failed to flee." << std::endl;
          monsterAttack(monster, player);
        }
        break;
      case 'f':
        attackMonster(player, monster);
        if (monster.isDead()) {
          std::cout << "You killed the " << monster.getName() << std::endl;
          player.addGold(monster.getGold());
          std::cout << "You found " << monster.getGold() << " gold."
                    << std::endl;
          player.levelUp();
          return;
        } else {
          monsterAttack(monster, player);
        }
        break;
    }
  }
}

static const int winningLevel = 20;

void game() {
  std::cout << "Enter your name: ";
  std::string playerName;
  std::cin >> playerName;

  Player player(playerName);
  std::cout << "Welcome, " << player.getName() << '.' << std::endl;
  std::cout << "You have " << player.getHealth() << " health and are carrying "
            << player.getGold() << " gold." << std::endl;

  while (!player.isDead() && player.getLevel() < winningLevel) {
    encounter(player);
  }

  if (player.isDead()) {
    std::cout << "You have died." << std::endl;
    std::cout << "You were level " << player.getLevel() << '.' << std::endl;
    std::cout << "You were carrying " << player.getGold() << " gold."
              << std::endl;
  } else {
    std::cout << "You have reached level " << player.getLevel() << " and won!"
              << std::endl;
    std::cout << "You are carrying " << player.getGold() << " gold."
              << std::endl;
  }
}

int main() {
  init();
  game();
  return 0;
}