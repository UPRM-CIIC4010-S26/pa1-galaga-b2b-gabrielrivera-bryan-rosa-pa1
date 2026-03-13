#pragma once
#include "Enemy.hpp"

class DyEnemy : public Enemy {
    private: 
        float angle = 135;
        float aimAngle = 225;
        bool loop = false;

    public:
        DyEnemy(float x, float y) : Enemy(x, y) { 
            this->cooldown = GetRandomValue(90, 300);
            this->health = 1; 
        }

        void draw() override;
        void update(std::pair<float, float> pos, HitBox target) override;
        void attack(HitBox target) override;
        
        //Returns score value of DyEnemy. DyEnemy is worth 150 points when killed.
        int getScoreValue() override { return 150; }
};