//
// Created by jul3x on 24.03.2020.
//

#ifndef RAG3_GAME_INCLUDE_MISC_JOURNALENTRIES_H
#define RAG3_GAME_INCLUDE_MISC_JOURNALENTRIES_H

#include <common/Map.h>
#include <common/Fire.h>


using namespace r3e;

class Journal;

class JournalEntry {
public:
    JournalEntry();

    explicit JournalEntry(Journal* father);

    virtual void executeEntryReversal() = 0;

protected:
    Journal* father_;

};

class TimeReversal : public JournalEntry {
public:
    explicit TimeReversal(Journal* father, void* placeholder);

    void executeEntryReversal() override;
private:
    std::string picked_weapon_;

};


class CharacterEntry : public JournalEntry {
public:
    CharacterEntry(Journal* father, Character* ptr);

    void executeEntryReversal() override;

protected:
    Character* ptr_;

    sf::Vector2f pos_;

    float rotation_;
    float life_;
    float ammo_state_;

    Character::GlobalState state_;

};


class DestroyCharacter : public JournalEntry {
public:
    DestroyCharacter(Journal* father, Character* ptr);

    void executeEntryReversal() override;

private:
    Character* ptr_;

    int u_id_;
    std::string id_;
    std::string activation_;
    std::vector<std::string> funcs_;
    std::vector<std::string> datas_;

    std::list<std::string> talk_scenario_;

};


class BulletEntry : public JournalEntry {
public:
    BulletEntry(Journal* father, Bullet* bullet);

    void executeEntryReversal() override;

private:
    Bullet* ptr_;

    sf::Vector2f pos_;

};

class DestroyBullet : public JournalEntry {
public:
    DestroyBullet(Journal* father, Bullet* bullet);

    void executeEntryReversal() override;

private:
    Bullet* ptr_;

    std::string id_;
    sf::Vector2f pos_;
    float direction_;
    Character* user_;

};

class SpawnBullet : public JournalEntry {
public:
    SpawnBullet(Journal* father, Bullet* bullet);

    void executeEntryReversal() override;

private:
    Bullet* ptr_;

};

class FireEntry : public JournalEntry {
public:
    FireEntry(Journal* father, Fire* fire);

    void executeEntryReversal() override;

private:
    Fire* ptr_;

    sf::Vector2f pos_;
    float r_;
    float alpha_;

};

class DestroyFire : public JournalEntry {
public:
    DestroyFire(Journal* father, Fire* fire);

    void executeEntryReversal() override;

private:
    Fire* ptr_;

    sf::Vector2f pos_;
    float direction_;
    Character* user_;

};

class SpawnFire : public JournalEntry {
public:
    SpawnFire(Journal* father, Fire* fire);

    void executeEntryReversal() override;

private:
    Fire* ptr_;

};

class DestroyObstacle : public JournalEntry {
public:
    DestroyObstacle(Journal* father, Obstacle* ptr);

    void executeEntryReversal() override;

private:
    Obstacle* ptr_;

    int u_id_;
    std::string id_;
    sf::Vector2f pos_;
    std::string activation_;
    std::vector<std::string> funcs_;
    std::vector<std::string> datas_;

};

class ShotObstacle : public JournalEntry {
public:
    ShotObstacle(Journal* father, Obstacle* obstacle);

    void executeEntryReversal() override;

private:
    Obstacle* ptr_;

    float life_;

};

class DestroyObstacleTile : public JournalEntry {
public:
    DestroyObstacleTile(Journal* father, ObstacleTile* ptr);

    void executeEntryReversal() override;

private:
    ObstacleTile* ptr_;

    std::string id_;
    sf::Vector2f pos_;

};

class ShotObstacleTile : public JournalEntry {
public:
    ShotObstacleTile(Journal* father, ObstacleTile* obstacle);

    void executeEntryReversal() override;

private:
    ObstacleTile* ptr_;

    float life_;

};


class SpawnDecoration : public JournalEntry {
public:
    SpawnDecoration(Journal* father, Decoration* ptr);

    void executeEntryReversal() override;

private:
    Decoration* ptr_;

};

class DestroyDecoration : public JournalEntry {
public:
    DestroyDecoration(Journal* father, Decoration* ptr);

    void executeEntryReversal() override;

private:
    Decoration* ptr_;

    int u_id_;
    std::string id_;
    sf::Vector2f pos_;

};

class SpawnSpecial : public JournalEntry {
public:
    SpawnSpecial(Journal* father, Special* ptr);

    void executeEntryReversal() override;

private:
    Special* ptr_;

};

class DestroySpecial : public JournalEntry {
public:
    DestroySpecial(Journal* father, Special* ptr);

    void executeEntryReversal() override;

private:
    Special* ptr_;

    int u_id_;
    std::string id_;
    sf::Vector2f pos_;
    std::string activation_;
    std::vector<std::string> funcs_;
    std::vector<std::string> datas_;

};

class WeaponActivation : public JournalEntry {
public:
    WeaponActivation(Journal* father, PlacedWeapon* ptr);

    void executeEntryReversal() override;

private:
    PlacedWeapon* ptr_;

    bool activate_;

};

class ChangeOpenState : public JournalEntry {
public:
    ChangeOpenState(Journal* father, Special* ptr);

    void executeEntryReversal() override;

private:
    Special* ptr_;

    bool open_state_;

};

class DoorOpen : public JournalEntry {
public:
    DoorOpen(Journal* father, Obstacle* ptr);

    void executeEntryReversal() override;

private:
    Obstacle* ptr_;

};


#endif //RAG3_GAME_INCLUDE_MISC_JOURNALENTRIES_H
