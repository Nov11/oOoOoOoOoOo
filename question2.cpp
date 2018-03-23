//
// Created by c6s on 18-3-23.
//
/**
 * 测试题二：
实现如下接口及对象(可以直接打印一点东西)：
战斗对象（血量、攻击力、攻击速度、坐标、移动速度）
场景对象（坐标、移动速度）
怪物（血量、攻击力、攻击速度、坐标、移动速度、技能（示例用，只有一个技能。以下皆同）。操作：攻击、改变血量、移动、释放技能。怪物可以被召唤成宠物）
英雄（力量、敏捷、智力、体力、血量、攻击力、攻击速度、坐标、移动速度、技能。操作：加点（改变力量、敏捷、智力等）、攻击、改变血量、移动、释放技能。）
宠物（血量、攻击力、攻击速度、坐标、移动速度。操作：攻击、改变血量、移动、释放技能。）
NPC（血量、攻击力、攻击速度、坐标、移动速度、可购买物品列表、任务ID。方法：攻击、改变血量、移动、释放技能、购买物品、完成任务。）
注意对象之间的继承关系


 */

#include <cstddef>
#include <iostream>
#include <vector>

struct Position {
  double x;
  double y;
};

struct Spell {
  std::string name() const {
    return "ice bolt";
  }
};

class CombatObject {
protected:
  size_t healthPoint;
  int damage;
  double attackSpeed;
  Position combatObjectPosition;
  double speed;
  Spell spell;
public:
  virtual void attack()=0;
  virtual void changeHealthPoint(int) = 0;
  virtual void move(Position &to) = 0;
  virtual void castSpell() = 0;
  CombatObject() {}
  //如果是对象语义，这个东西不应该被拷贝
  CombatObject(const CombatObject &) = delete;
  CombatObject &operator=(const CombatObject &)= delete;
};

class Scenario {
public:
  Position scenarioPosition;
  double speed;
};

class Monster : public CombatObject {
  bool summoned;
public:
  Monster(bool summoned = false) : summoned(summoned) {}
  void attack() override {
    std::cout << "monster attack" << std::endl;
  }
  void changeHealthPoint(int delta) override {
    std::cout << "monster change health point :" << delta << std::endl;
  }
  void move(Position &to) override {
    std::cout << "monster move to " << combatObjectPosition.x << " " << combatObjectPosition.y << std::endl;
  }
  void castSpell() override {
    std::cout << "monster cast spell : " << spell.name() << std::endl;
  }
};

class Hero : public CombatObject {
  size_t strength;
  size_t agile;
  size_t vitality;
  size_t intelligence;
public:
  void spendSkillPoint(int category, int cnt) {
    std::cout << "use " << cnt << " points on " << category << std::endl;
  }

  void attack() override {
    std::cout << "hero attack" << std::endl;
  }
  void changeHealthPoint(int delta) override {
    std::cout << "hero change health point :" << delta << std::endl;
  }
  void move(Position &to) override {
    std::cout << "hero move to " << combatObjectPosition.x << " " << combatObjectPosition.y << std::endl;
  }
  void castSpell() override {
    std::cout << "hero cast spell : " << spell.name() << std::endl;
  }
};

class Pet : public Monster {
public:
  Pet() : Monster(true) {}
  void attack() override {
    std::cout << "pet attack" << std::endl;
  }
  void changeHealthPoint(int delta) override {
    std::cout << "pet change health point :" << delta << std::endl;
  }
  void move(Position &to) override {
    std::cout << "pet move to " << combatObjectPosition.x << " " << combatObjectPosition.y << std::endl;
  }
  void castSpell() override {
    std::cout << "hero cast spell : " << spell.name() << std::endl;
  }
};

struct Item {

};

struct Request {

};
class NPC : public CombatObject {
  std::vector<Item> itemList;
  std::vector<Request> questList;
public:
  void attack() override {
    std::cout << "NPC attack" << std::endl;
  }
  void changeHealthPoint(int delta) override {
    std::cout << "NPC change health point :" << delta << std::endl;
  }

  void move(Position &to) override {
    std::cout << "NPC stands there as nothing happened " << std::endl;
  }
  void castSpell() override {
    std::cout << "NPC cast spell : " << spell.name() << std::endl;
  }
  void buyItem(int itemId) {
    std::cout << "NPC sold item : " << itemId << std::endl;
  }
  void completeQuest(int requestId) {
    std::cout << "You completed request: " << requestId << std::endl;
  }
};

int main() {
  Pet pet;
  NPC npc;
  pet.attack();
  npc.buyItem(11);
}