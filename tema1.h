#pragma once

#include "components/simple_scene.h"
#define MAX_LENGTH_PROJECTILE 400
#define FIRE_RATE 1
#define ENEMIES_SPAWN_TIME 6
#define SPEED_PROJECTILE 100
#define LENGTH_PROJECTILE 25
#define WIDTH_PROJECTILE 25
#define LENGTH_ENEMY 100
#define WIDTH_ENEMY 100
#define LENGTH_ENEMY2 25
#define WIDTH_ENEMY2 25
#define ENEMIES_NUMBER 2
#define MAX_HEALTH 100
#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_LENGTH 30

namespace m1
{

    class Obstacle {
    public:
        float length;
        float width;
        glm::vec3 position;

        Obstacle(float length, float width, glm::vec3 position) {
            this->length = length;
            this->width = width;
            this->position = position;
        }

        ~Obstacle() {}
    };

    class Projectile {
    public:
        float length;
        float width;
        float tx;
        float ty;
        glm::vec2 initialPosition;
        glm::vec2 currentPosition;
        glm::vec2 direction;
        float speed;
        float maxDistance;
        float fireRate;
        float angle;
        bool hasCollided;

        Projectile(float length, float width, glm::vec2 initialPposition, glm::vec2 direction, float speed, float maxDistance, float fireRate, float angle,
                    float tx, float ty) {
            this->length = length;
            this->width = width;
            this->initialPosition = initialPosition;
            this->direction = direction;
            this->speed = speed;
            this->maxDistance = maxDistance;
            this->fireRate = fireRate;
            this->angle = angle;
            this->tx = tx;
            this->ty = ty;
        }

        Projectile() {
            this->length = 0;
            this->width = 0;
          //  this->position = glm::vec3(0.f, 0.f, 0.f);
          //  this->direction = glm::vec2(0.f, 0.f);
            this->speed = SPEED_PROJECTILE;
            this->maxDistance = MAX_LENGTH_PROJECTILE;
            this->fireRate = FIRE_RATE;
            this->angle = angle;
        }

        ~Projectile() {}
    };

    class Enemy {
    public:
        float speed;
        float length;
        glm::vec2 position;
        float tx;
        float ty;
        float angle;
        bool hasCollided;
        glm::mat3 modelMatrix;

        Enemy(float speed, float length, glm::vec2 position, float tx, float ty) {
            this->speed = speed;
            this->length = length;
            this->position = position;
            this->tx = tx;
            this->ty = ty;
        }

        ~Enemy() {}
    };


    class tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     public:
        tema1();
        ~tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CreatePlayer();
        void CreateMap();
        void CreateProjectile();
        void ShootProjectile(float deltaTimeSeconds);
        void CreateEnemy();
        void LaunchEnemy(float deltaTimeSeconds);
        void CheckPlayerMapCollision();
        void CheckPlayerObstacleCollision(Obstacle &obstacle);
        void CheckPlayerEnemyCollision(float deltaTimeSeconds);
        void CheckProjectileMapCollision(Projectile &projectile);
        void CheckProjectileObstaclesCollision(Projectile& projectile);
        void CheckProjectileEnemyCollision(Projectile& projectile);
        void CheckEnemyMapCollision(Enemy& e);
        void DoObstacleCollisions();
        void ComputeProjectileAngle();
        float ComputeEnemyDirectionAngle(Enemy& e);
        void DrawHealthBarLines();
        float clamp(float value, float min, float max);
        bool lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r);
        bool linePoint(float x1, float y1, float x2, float y2, float px, float py);
        float dist(float x1, float y1, float x2, float y2);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

     protected:
        float cx, cy;
        glm::mat3 modelMatrix, modelMatrix2, modelMatrix3;
        glm::vec3 obstaclePosition1, obstaclePosition2, obstaclePosition3, obstaclePosition4, playerPosition;
        glm::vec2 centerBig, centerSmall1, centerSmall2;
        glm::vec2 enemyCenter1, enemyCenter2, enemyCenter3;
        float obstacleLength1, obstacleWidth1, obstacleLength2, obstacleWidth2, obstacleLength3, obstacleWidth3, obstacleLength4, obstacleWidth4;
        float translateX, translateY;
        float translateProjectileX, translateProjectileY;
        float translateEnemyX, translateEnemyY;
        float scaleX, scaleY;
        float angularStep;
        float enemyAngle;
        float projectileAngle;
        float circleRadius;
        float mapHeight, mapWidth;
        float mapBottomX, mapBottomY;
        bool collision, collisionUp, collisionDown, collisionLeft, collisionRight;
        bool projectileOnMap, btnPressed, enemySpawn;
        float enemySpawnTime;
        clock_t t, t2;
        int score, health;
        float scaleFactor;
        glm::vec2 healthBarCorner;

        std::vector<Obstacle> obstacles;
        std::vector<Enemy> enemies;
        std::vector<Projectile> projectiles;
        std::vector<bool> collisionWithObstacles;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 visMatrix;

    };
}   // namespace m1
