#pragma once
#include "Projectile.hpp"
#include "Math.hpp"
#include "ImageManager.hpp"
#include "Animation.hpp"
#include <iostream>

class Enemy {
    protected:
        float angle = 90;
        int cooldown = 60;
        int speed = 3;
        bool spawning = false;
        bool frame = false;
        int frameCooldown = 30;
        
    public:
        int health = 1;
        std::pair<float, float> position;
        HitBox hitBox;

        inline static float direction = 0.5;
        inline static int directionChange = 100;
        inline static std::vector<std::pair<std::pair<float, float>, Enemy*>> enemies;

        //Static variable used to call score gained from killing an enemy in Program.cpp.
        inline static int scoreGained = 0;

        Enemy() {}

        Enemy(float x, float y) {
            position.first = x;
            position.second = y;
            hitBox = HitBox(x, y, 30, 30);
        }

        virtual void draw() = 0;
        virtual void update(std::pair<float, float> pos, HitBox target) = 0;
        virtual void attack(HitBox target) = 0;

        //Member function that returns score value based on enemy type. Overrriden in Enemy member classes to return different score values.
        virtual int getScoreValue() { return 50; }
        //Static function used to set score gained from killing an enemy.
        static void setScore(int scoreGained) {Enemy::scoreGained = scoreGained;}

        void frameChange() {
            frameCooldown--;

             if (frameCooldown <= 0) {
                frame = !frame;
                frameCooldown = 30;
             }
        }

        static void ManageEnemies(HitBox target) {
            int gainedScore = 0;
            for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) { 
                p.first.first += (p.first.first == 0) ? 0 : direction;
                if (p.second) {
                    p.second->update(p.first, target);

                    for (Projectile& p2 : Projectile::projectiles) {
                        if (p2.ID != 1 && HitBox::Collision(p.second->hitBox, p2.getHitBox())) {
                            p.second->health--;
                            p2.del = true;
                        }
                    }

                    if (p.second->health <= 0) {
                        Animation::animations.push_back(
                            Animation(p.second->position.first, p.second->position.second, 155, 0, 33, 33, 30, 30, 4, ImageManager::SpriteSheet)
                        );
                        //Adds score gained from killing enemy by calling getScoreValue() from member class and sets it to Enemy::gainedScore for it to be called in Program.cpp
                        setScore(p.second->getScoreValue());
                        p.second = nullptr;
                    }
                }
            }
            
            for (int i = 0; i < Enemy::enemies.size(); i++) {
                if ((Enemy::enemies[i].second && Enemy::enemies[i].second->position.first <= -30) || 
                    (!Enemy::enemies[i].second && Enemy::enemies[i].first.first == 0 && Enemy::enemies[i].first.second == 0)) {
                    Enemy::enemies.erase(Enemy::enemies.begin() + i);
                }
            }

            directionChange++;

            if (directionChange >= 200) {
                directionChange = 0;
                direction *= -1;
            }
        }
};
