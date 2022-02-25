#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <time.h>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema1::tema1()
{
}


tema1::~tema1()
{
}


void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    float squareSide = 100;
    float rectangleLength = 100;
    float rectangleWidth = 50;
    circleRadius = 60;

    mapHeight = resolution.y + resolution.y * 0.3;
    mapWidth = resolution.x + resolution.x * 0.3;
    mapBottomX = 0;
    mapBottomY = 0;

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = resolution.x;   // logic width
    logicSpace.height = resolution.y;  // logic height

    collision = false;
    collisionUp = false;
    collisionDown = false;
    collisionLeft = false;
    collisionRight = false;

    obstaclePosition1 = glm::vec3(mapWidth - 400, 100.f, 0.f);
    obstaclePosition2 = glm::vec3(mapWidth / 3, mapHeight - 200, 0.f);
    obstaclePosition3 = glm::vec3(0, mapHeight - 150, 0.f);
    obstaclePosition4 = glm::vec3(mapWidth - 50, 300.f, 0.f);
    playerPosition = glm::vec3(0, 0, 0.f);

    obstacleLength1 = 85;
    obstacleLength2 = 200;
    obstacleLength3 = 150;
    obstacleLength4 = 50;
    obstacleWidth1 = 70;
    obstacleWidth2 = 35;
    obstacleWidth3 = 150;
    obstacleWidth4 = 200;

    centerBig.x = logicSpace.x + logicSpace.width/2;
    centerBig.y = logicSpace.y + logicSpace.height /2;

    centerSmall1.x = logicSpace.x + logicSpace.width / 2;
    centerSmall1.y = logicSpace.y + logicSpace.height / 2 + circleRadius;

    centerSmall2.x = logicSpace.x + logicSpace.width / 2 + circleRadius;
    centerSmall2.y = logicSpace.y + logicSpace.height / 2;

    projectileOnMap = false;

    btnPressed = false;
    enemySpawn = true;

    enemyCenter1.x = 0;
    enemyCenter1.y = 0;

    enemyCenter2.x = LENGTH_ENEMY - LENGTH_ENEMY2;
    enemyCenter2.y = 0;

    enemyCenter3.x = LENGTH_ENEMY - LENGTH_ENEMY2;
    enemyCenter3.y = WIDTH_ENEMY - WIDTH_ENEMY2;

    enemySpawnTime = 0;

    t2 = 0;

    health = MAX_HEALTH;
    score = 0;
    scaleFactor = 1;

    healthBarCorner.y = logicSpace.height - HEALTH_BAR_LENGTH;
    healthBarCorner.x = logicSpace.width - HEALTH_BAR_WIDTH;


    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    translateProjectileX = 0;
    translateProjectileY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    //map
    Mesh* rectangle = object2D::CreateRectangle("rectangle", glm::vec3(mapBottomX, mapBottomY, 0.f), mapWidth, mapHeight, glm::vec3(0.5f, 1.0f, 1.0f));
    AddMeshToList(rectangle);

    //obstacles

    Mesh* obstacle1 = object2D::CreateRectangle("obstacle1", obstaclePosition1, obstacleLength1, obstacleWidth1, glm::vec3(0.7f, 0.6f, 0.4f));
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = object2D::CreateRectangle("obstacle2", obstaclePosition2, obstacleLength2, obstacleWidth2, glm::vec3(0.7f, 0.6f, 0.4f));
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = object2D::CreateRectangle("obstacle3", obstaclePosition3, obstacleLength3, obstacleWidth3, glm::vec3(0.7f, 0.6f, 0.4f));
    AddMeshToList(obstacle3);

    Mesh* obstacle4 = object2D::CreateRectangle("obstacle4", obstaclePosition4, obstacleLength4, obstacleWidth4, glm::vec3(0.7f, 0.6f, 0.4f));
    AddMeshToList(obstacle4);

    //player
    Mesh* circle1 = object2D::CreateCircle("circle1", glm::vec3(centerBig.x, centerBig.y, 0.f), circleRadius, glm::vec3(2.0f, 0.5f, 1.0f), true);
    AddMeshToList(circle1);

    Mesh* circle2 = object2D::CreateCircle("circle2", glm::vec3(centerSmall1.x, centerSmall1.y, 0.f), circleRadius/4, glm::vec3(2.0f, 0.5f, 1.0f), true);
    AddMeshToList(circle2);

    Mesh* circle3 = object2D::CreateCircle("circle3", glm::vec3(centerSmall2.x, centerSmall2.y, 0.f), circleRadius/4, glm::vec3(2.0f, 0.5f, 1.0f), true);
    AddMeshToList(circle3);

    //projectile

    Mesh* projectile = object2D::CreateRectangle("projectile", glm::vec3(0.f, 0.f, 0.f), LENGTH_PROJECTILE, WIDTH_PROJECTILE, glm::vec3(0, 0, 0));
    AddMeshToList(projectile);

    //enemy

    Mesh* enemy1 = object2D::CreateRectangle("enemy1", glm::vec3(enemyCenter1.x, enemyCenter1.y, 0.f), LENGTH_ENEMY, WIDTH_ENEMY, glm::vec3(1.f, 0.f, 0.f));
    AddMeshToList(enemy1);

    Mesh* enemy2 = object2D::CreateRectangle("enemy2", glm::vec3(enemyCenter2.x, enemyCenter2.y, 0.f), LENGTH_ENEMY2, WIDTH_ENEMY2, glm::vec3(0, 0, 0));
    AddMeshToList(enemy2);

    Mesh* enemy3 = object2D::CreateRectangle("enemy3", glm::vec3(enemyCenter3.x, enemyCenter3.y, 0.f), LENGTH_ENEMY2, WIDTH_ENEMY2, glm::vec3(0, 0, 0));
    AddMeshToList(enemy3);

    // healthBar

    Mesh* healthBarLines = object2D::CreateRectangle("healthBarLines", glm::vec3(healthBarCorner.x, healthBarCorner.y, 0.f), HEALTH_BAR_WIDTH, HEALTH_BAR_LENGTH, glm::vec3(0, 0, 0));
    AddMeshToList(healthBarLines);

    Mesh* healthBarFilling = object2D::CreateRectangle("healthBarFilling", glm::vec3(healthBarCorner.x, healthBarCorner.y, 0.f), HEALTH_BAR_WIDTH, HEALTH_BAR_LENGTH, glm::vec3(0, 1, 0));
    AddMeshToList(healthBarFilling);

}


void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

glm::mat3 tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw 
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0, 0, 0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    //create map
    CreateMap();

    //create health bar
    DrawHealthBarLines();

    //create and draw player
    CreatePlayer();

    //create and draw obstacles
    ShootProjectile(deltaTimeSeconds);

    //spawn a certain number of enemies at a certain period of time
    if (enemySpawn == true) {
        CreateEnemy();
        t2 = clock();
        enemySpawn = false;
    }

    if ((double)(clock() - t2) / CLOCKS_PER_SEC >= ENEMIES_SPAWN_TIME) {
        enemySpawn = true;
        t2 = clock() - t2;
    }

    //draw the enemies
    LaunchEnemy(deltaTimeSeconds);

    //implement player's collisions
    CheckPlayerMapCollision();
    DoObstacleCollisions();
    CheckPlayerEnemyCollision(deltaTimeSeconds);

    //if the player has no life left, game is over
    if (health == 0) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cout << "Game over with score " << score << " !" << endl;
        exit(0);
    }
    
}


void tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
    //the player can move with W A S D as long as 
    //there is no collision with the map or with an obstacle

    if (window->KeyHold(GLFW_KEY_W)) {
        if(collisionUp == false){
            translateY += deltaTime * SPEED_PROJECTILE;
            logicSpace.y += deltaTime * SPEED_PROJECTILE;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (collisionDown == false) {
            translateY -= deltaTime * SPEED_PROJECTILE;
            logicSpace.y -= deltaTime * SPEED_PROJECTILE;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
       if (collisionLeft == false){
            translateX -= deltaTime * SPEED_PROJECTILE;
            logicSpace.x -= deltaTime * SPEED_PROJECTILE;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (collisionRight == false) {
            translateX += deltaTime * SPEED_PROJECTILE;
            logicSpace.x += deltaTime * SPEED_PROJECTILE;
        }
    }

}


void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    glm::ivec2 cursorPos = window->GetCursorPosition();
    glm::vec3 aux = glm::vec3(centerBig.x , centerBig.y , 1);
    aux = aux * visMatrix;
    double coordinateX = cursorPos.x - aux.x;
    double coordinateY = resolution.y - cursorPos.y - aux.y;
    angularStep = atan2(coordinateY, coordinateX);
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    // a projectile is created if the fire rate conditionis respected
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        if (btnPressed == false) {
            btnPressed = true;
            t = clock();
            ComputeProjectileAngle();
            CreateProjectile();
        }
        if (btnPressed == true) {
            if (((float)(clock() - t) / CLOCKS_PER_SEC) >= FIRE_RATE) {
                btnPressed = false;
            }
        }
   }
  
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}
void tema1::CreatePlayer()
{
    //draw player's body

    glm::ivec2 resolution = window->GetResolution();
    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);

    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);

    //draw player's eyes

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(centerBig.x, centerBig.y) *
        transform2D::Rotate(angularStep - (45 * M_PI / 180)) * transform2D::Translate(-centerBig.x, -centerBig.y);

    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(centerBig.x, centerBig.y) *
        transform2D::Rotate(angularStep - (45 * M_PI / 180)) * transform2D::Translate(-centerBig.x, -centerBig.y);

    RenderMesh2D(meshes["circle3"], shaders["VertexColor"], modelMatrix);

}

void tema1::CreateMap()
{

    //draw map
    glEnable(GL_DEPTH_TEST);

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

    glDisable(GL_DEPTH_TEST);

    //draw obstacles

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
    Obstacle obstacle1 = Obstacle(obstacleLength1, obstacleWidth1, obstaclePosition1);
    obstacles.push_back(obstacle1);

    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);
    Obstacle obstacle2 = Obstacle(obstacleLength2, obstacleWidth2, obstaclePosition2);
    obstacles.push_back(obstacle2);

    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);
    Obstacle obstacle3 = Obstacle(obstacleLength3, obstacleWidth3, obstaclePosition3);
    obstacles.push_back(obstacle3);

    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);
    Obstacle obstacle4 = Obstacle(obstacleLength4, obstacleWidth4, obstaclePosition4);
    obstacles.push_back(obstacle4);

}

void tema1::ComputeProjectileAngle()
{
    // compute the angle for a projectile
    glm::ivec2 resolution = window->GetResolution();
    glm::ivec2 cursorPos = window->GetCursorPosition();
    glm::vec3 aux = glm::vec3(centerBig.x , centerBig.y , 1);
    aux = aux * visMatrix;
    double coordinateX = cursorPos.x - aux.x;
    double coordinateY = resolution.y - cursorPos.y - aux.y;
    projectileAngle = atan2(coordinateY, coordinateX);
}


void tema1::CreateProjectile()
{
    glm::vec2 projectileCreatePosition;
    Projectile currentProjectile;

    currentProjectile.tx = translateX;
    currentProjectile.ty = translateY;

    projectileCreatePosition.x = logicSpace.x + logicSpace.width / 2 - LENGTH_PROJECTILE / 2;
    projectileCreatePosition.y = logicSpace.y + logicSpace.height / 2 - WIDTH_PROJECTILE / 2;

    glm::vec2 direction;
    float angle = projectileAngle;

    direction.x = cos(angle);
    direction.y = sin(angle);

    currentProjectile = Projectile(LENGTH_PROJECTILE, WIDTH_PROJECTILE,
        projectileCreatePosition, direction, SPEED_PROJECTILE,
        MAX_LENGTH_PROJECTILE, FIRE_RATE, angle, translateX , translateY);

    currentProjectile.hasCollided = false;
    projectiles.push_back(currentProjectile);
}

void tema1::ShootProjectile(float deltaTimeSeconds)
{
    for (int i = 0; i < projectiles.size(); i++) {

        //compute the projectile's direction
        projectiles[i].tx += projectiles[i].direction.x * deltaTimeSeconds * SPEED_PROJECTILE;
        projectiles[i].ty += projectiles[i].direction.y * deltaTimeSeconds * SPEED_PROJECTILE;

        modelMatrix = glm::mat3(1);
        modelMatrix = visMatrix * transform2D::Translate(centerBig.x - LENGTH_PROJECTILE / 2,  centerBig.y - WIDTH_PROJECTILE / 2); 
        modelMatrix *= transform2D::Translate(projectiles[i].tx, projectiles[i].ty);
        modelMatrix *= transform2D::Translate(LENGTH_PROJECTILE / 2, WIDTH_PROJECTILE / 2);
        modelMatrix *= transform2D::Rotate(projectiles[i].angle);
        modelMatrix *= transform2D::Translate(-LENGTH_PROJECTILE / 2, -WIDTH_PROJECTILE / 2);

        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        glm::vec2 distance;
        distance.x = projectiles[i].tx - translateX;
        distance.y = projectiles[i].ty - translateY;

        //compute the distance from the projectile's initial position
        //to the projectile's current position
        float d = sqrt(distance.x * distance.x + distance.y * distance.y);

        //check if any collision occuress
        CheckProjectileMapCollision(projectiles[i]);

        CheckProjectileObstaclesCollision(projectiles[i]);

        CheckProjectileEnemyCollision(projectiles[i]);

        if (d >= MAX_LENGTH_PROJECTILE) {
            projectiles[i].hasCollided = true;
            
        }

        //if the projectile collides, it is destroyed
        if (projectiles[i].hasCollided == true) {
            projectiles.erase(projectiles.begin() + i);
        }

    }

}

void tema1::CheckProjectileObstaclesCollision(Projectile& projectile)
{
    //rectangle - rectangle collision
    for (int i = 0; i < obstacles.size(); i++) {

        bool collisionX = projectile.tx + centerBig.x >= obstacles[i].position.x &&
            obstacles[i].position.x + obstacles[i].length >= projectile.tx + centerBig.x;
        bool collisionY = projectile.ty + centerBig.y >= obstacles[i].position.y &&
            obstacles[i].position.y + obstacles[i].width >= projectile.ty + centerBig.y;

        if (collisionX && collisionY) {
            projectile.hasCollided = true;
        }
    }
}

void tema1::CheckProjectileMapCollision(Projectile& projectile)
{
    //check if the projectile is still on map using the coordinates
    if (projectile.tx + centerBig.x >= mapWidth) {
        projectile.hasCollided = true;
    }

    if (projectile.ty + centerBig.y >= mapHeight) {
        projectile.hasCollided = true;
    }

    if (projectile.tx + centerBig.x <= mapBottomX) {
        projectile.hasCollided = true;
    }

    if (projectile.ty + centerBig.y <= mapBottomY) {
        projectile.hasCollided = true;
    }
}

void tema1::CheckProjectileEnemyCollision(Projectile& projectile)
{
    //due to the fact that the enemy and the projectile may have different
    //directions, I implemented a circle - circle collision for the 
    //two objects
    for (int i = 0; i < enemies.size(); i++) {

        double projectileRadius = projectile.length  / 2;
        double enemyRadius = enemies[i].length  / 2;

        glm::vec2 center;
        center.x = centerBig.x + projectile.tx - projectile.length / 2;
        center.y = centerBig.y + projectile.ty - projectile.length / 2;

        glm::vec2 center2;
        center2.x = enemies[i].tx + enemies[i].length / 2;
        center2.y = enemies[i].ty + enemies[i].length / 2;
        
        glm::vec2 difference = center2 - center;

        if (glm::length(difference) < projectileRadius + enemyRadius) {
            enemies[i].hasCollided = true;
            projectile.hasCollided = true;
            score++;
            cout << "Score is " << score << endl;
        }
    }
}


void tema1::CreateEnemy()
{
    glm::vec2 position;
    glm::vec2 direction;
    float speed;

    for (int i = 0; i < ENEMIES_NUMBER; i++) {
        position.x = rand() % (int)mapWidth ;
        position.y = rand() % (int)mapHeight ;
        speed = rand() % 100 + 50;
        float tx = position.x;
        float ty = position.y;
        Enemy enemy = Enemy(speed, LENGTH_ENEMY, position, tx, ty);
        enemy.hasCollided = false;
        enemies.push_back(enemy);
    }

}

void tema1::LaunchEnemy(float deltaTimeSeconds)
{

    for (int i = 0; i < enemies.size(); i++) {

        enemies[i].angle = ComputeEnemyDirectionAngle(enemies[i]);

        //modelMatrix = glm::mat3(1);
        modelMatrix = visMatrix * transform2D::Translate(enemies[i].length / 2, enemies[i].length / 2);
        //tx and ty are initialized with the enemy's initial position,
        //so this translation firstly puts the enemy at a randomly 
        //generated location
        modelMatrix *= transform2D::Translate(enemies[i].tx, enemies[i].ty);
        modelMatrix *= transform2D::Translate(enemies[i].length / 2,  enemies[i].length / 2 );
        modelMatrix *= transform2D::Rotate(enemies[i].angle);
        modelMatrix *= transform2D::Translate(- enemies[i].length / 2, - enemies[i].length / 2 ) ;

        //computes the enemy's position
        enemies[i].tx += cos(enemies[i].angle * 180 / M_PI) * deltaTimeSeconds * enemies[i].speed;
        enemies[i].ty += sin(enemies[i].angle * 180 / M_PI) * deltaTimeSeconds * enemies[i].speed;

        RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemy2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemy3"], shaders["VertexColor"], modelMatrix);

        //check if there is any collision
        CheckEnemyMapCollision(enemies[i]);

        //if the enemy collided, destroy it
        if (enemies[i].hasCollided == true) {
            enemies.erase(enemies.begin() + i);
        }

    }

}

void tema1::CheckEnemyMapCollision(Enemy& e)
{
    //check the enemy's coordinates to see if
    //it is still on the map
    if (e.tx >= mapWidth) {
        e.hasCollided = true;
    }

    if (e.ty  >= mapHeight) {
        e.hasCollided = true;
    }

    if (e.tx  <= mapBottomX) {
        e.hasCollided = true;
    }

    if (e.ty <= mapBottomY) {
        e.hasCollided = true;
    }
}

float tema1::ComputeEnemyDirectionAngle(Enemy& e)
{
    double coordinateX = (logicSpace.x + logicSpace.width / 2) - (e.tx + e.length/2);
    double coordinateY = (logicSpace.y + logicSpace.height / 2) - (e.ty + e.length/2);
    return atan2(coordinateY, coordinateX);
}



void tema1::CheckPlayerEnemyCollision(float deltaTimeSeconds) {

    //used a square-circle collision 
    for (int i = 0; i < enemies.size(); i++) {

        glm::vec2 center;
        center.x = centerBig.x + translateX ;
        center.y = centerBig.y + translateY ;

        glm::vec2 aabb_half_extents(enemies[i].length / 2.0f, enemies[i].length / 2.0f);
        glm::vec2 aabb_center(
            enemies[i].tx + aabb_half_extents.x,
            enemies[i].ty + aabb_half_extents.y
        );

        glm::vec2 difference = center - aabb_center;
        glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
        glm::vec2 closest = aabb_center + clamped;

        difference = closest - center;
        if (glm::length(difference) < circleRadius) {
            enemies[i].hasCollided = true;
            health = health - 5;
            scaleFactor = (double)(health % MAX_HEALTH) / 100;
        }
    }
}


void tema1::DrawHealthBarLines()
{
    // Turn on wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Draw the healthBar
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["healthBarLines"], shaders["VertexColor"], modelMatrix);

    // Turn off wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    modelMatrix *= transform2D::Translate(healthBarCorner.x, healthBarCorner.y);
    modelMatrix *= transform2D::Scale(scaleFactor, 1);
    modelMatrix *= transform2D::Translate(-healthBarCorner.x, -healthBarCorner.y);

    RenderMesh2D(meshes["healthBarFilling"], shaders["VertexColor"], modelMatrix);
}

void tema1::CheckPlayerMapCollision()
{   
    //for player-map collision I used a circle-line collison 
    //for each side of the rectangle which represents the map

    float radius = 5 * circleRadius / 4;
    collisionLeft = lineCircle(mapBottomX, mapBottomY, mapBottomX, mapHeight, centerBig.x + translateX, centerBig.y + translateY, radius);
    collisionUp = lineCircle(mapBottomX, mapHeight, mapWidth, mapHeight, centerBig.x + translateX, centerBig.y + translateY, radius);
    collisionRight = lineCircle(mapWidth, mapHeight, mapWidth, mapBottomY, centerBig.x + translateX, centerBig.y + translateY, radius);
    collisionDown = lineCircle(mapWidth, mapBottomY, mapBottomX, mapBottomY, centerBig.x + translateX, centerBig.y + translateY, radius);

}

void tema1::DoObstacleCollisions()
{

    for (int i = 0; i < obstacles.size(); i++) {
        CheckPlayerObstacleCollision(obstacles[i]);
    }

}

void tema1::CheckPlayerObstacleCollision(Obstacle& obstacle)
{
    //rectangle - rectangle collison

    float radius = circleRadius;

    glm::vec2 center;
    center.x = centerBig.x + translateX;
    center.y = centerBig.y + translateY;

    glm::vec2 aabb_half_extents(obstacle.length, obstacle.width);
    glm::vec2 aabb_center(
        obstacle.position.x,
        obstacle.position.y
    );

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, glm::vec2(0,0), aabb_half_extents);

    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;
    if ((closest.y > center.y) && (glm::length(difference) < radius)) {
        collisionUp = true;
    }

    if ((closest.x > center.x) && (glm::length(difference) < radius)) {
        collisionRight = true;
    }

    if ((closest.y < center.y) && (glm::length(difference) < radius)) {
        collisionDown = true;
    }

    if ((closest.x < center.x) && (glm::length(difference) < radius)) {
        collisionLeft = true;
    }
}

float tema1::clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

float tema1::dist(float x1, float y1, float x2, float y2) {
    float distance = sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
    return distance;
}

bool tema1::linePoint(float x1, float y1, float x2, float y2, float px, float py) {

    // get distance from the point to the two ends of the line
    float d1 = dist(px, py, x1, y1);
    float d2 = dist(px, py, x2, y2);

    // get the length of the line
    float lineLen = dist(x1, y1, x2, y2);

    // since floats are so minutely accurate, add
    // a little buffer zone that will give collision
    float buffer = 0;    // higher # = less accurate

    // if the two distances are equal to the line's
    // length, the point is on the line!
    // note we use the buffer here to give a range,
    // rather than one #
    if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
        return true;
    }
    return false;
}

bool tema1::lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {

    // get length of the line
    float distX = x1 - x2;
    float distY = y1 - y2;
    float len = sqrt((distX * distX) + (distY * distY));

    // get dot product of the line and circle
    float dot = (((cx - x1) * (x2 - x1)) + (float(cy - y1) * (y2 - y1))) / pow(len, 2);

    // find the closest point on the line
    float closestX = x1 + (dot * (x2 - x1));
    float closestY = y1 + (dot * (y2 - y1));

    // is this point actually on the line segment?
    // if so keep going, but if not, return false
    bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
    if (!onSegment) return false;

    // get distance to closest point
    distX = closestX - cx;
    distY = closestY - cy;
    float distance = sqrt((distX * distX) + (distY * distY));

    if (distance <= r) {
        return true;
    }
    return false;
}


