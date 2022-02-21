#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include<algorithm>
#include <math.h>
#include <iostream>
#include <ctime>
#include <stack>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/CreateObjects.h"

int iteratii;
int directie;
int startTimer;
bool impact, gameOver;
bool switchToThird;
bool stop1, stop2, stop3, stop4;
float enemyX, enemyY, enemyZ, adjust_scale, adjust_scale2, adjust1, adjust2;
bool shoot;
int index;
int life;
float scale;
double duration;
float unghi_player;
float timeOut;
bool enemyShot;
using namespace std;
using namespace m1;

float vec[5];

struct Cell
{
    bool visited;
    bool top_wall;
    bool bot_wall;
    bool left_wall;
    bool right_wall;
    char display;
};

struct gloante {
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
    float degree, speed, ttl;

};
typedef struct gloante Gloante;

std::vector <Gloante*> Glont[16];
Gloante bullet;

Cell Level[SIZE][SIZE];


//Generarea labirintului
void Initialize(Cell Level[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Level[i][j].display = '*';
            Level[i][j].visited = false;
            Level[i][j].top_wall = true;
            Level[i][j].bot_wall = true;
            Level[i][j].left_wall = true;
            Level[i][j].right_wall = true;
        }
    }
    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            // Border Cells have fewer accessible walls
            Level[1][j].top_wall = false;
            Level[SIZE - 2][j].bot_wall = false;
            Level[i][1].left_wall = false;
            Level[i][SIZE - 2].right_wall = false;
        }
    }
}



// REDRAW MAZE
void Redraw(Cell Level[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        cout << endl;
        for (int j = 0; j < SIZE; j++)
            cout << " " << Level[i][j].display;
    }
}

// GENERATE MAZE
void GenerateMaze(Cell Level[][SIZE], int& posX, int& posY, int& goalX, int& goalY) {
    srand((unsigned)time(NULL));                                            // Pick random start cell
    int random = 0;
    int randomX = ((2 * rand()) + 1) % (SIZE - 1);                      // Generate a random odd number between 1 and SIZE
    int randomY = ((2 * rand()) + 1) % (SIZE - 1);                      // Generate a random odd number between 1 and SIZE
    randomX = 1;
    randomY = 1;
    posX = randomX;                                             // Save player's initial X position
    posY = randomY;                                             // Save player's initial Y position
    int visitedCells = 1;
    int totalCells = ((SIZE - 1) / 2) * ((SIZE - 1) / 2);
    int percent = 0;
    stack<int> back_trackX, back_trackY;                        // Stack is used to trace the reverse path

    Level[randomY][randomX].display = 'S';                      // Set S as the start cell
    Level[randomY][randomX].visited = true;                     // Set start cell as visited;
    while (visitedCells < totalCells)
    {
        if (((Level[randomY - 2][randomX].visited == false) && (Level[randomY][randomX].top_wall == true && Level[randomY - 2][randomX].bot_wall == true)) ||
            ((Level[randomY + 2][randomX].visited == false) && (Level[randomY][randomX].bot_wall == true && Level[randomY + 2][randomX].top_wall == true)) ||
            ((Level[randomY][randomX - 2].visited == false) && (Level[randomY][randomX].left_wall == true && Level[randomY][randomX - 2].right_wall == true)) ||
            ((Level[randomY][randomX + 2].visited == false) && (Level[randomY][randomX].right_wall == true && Level[randomY][randomX + 2].left_wall == true)))
        {
            random = (rand() % 4) + 1;      // Pick a random wall 1-4 to knock down

             // GO UP
            if ((random == 1) && (randomY > 1)) {
                if (Level[randomY - 2][randomX].visited == false) {    // If not visited
                    Level[randomY - 1][randomX].display = ' ';    // Delete display
                    Level[randomY - 1][randomX].visited = true;   // Mark cell as visited
                    Level[randomY][randomX].top_wall = false;   // Knock down wall

                    back_trackX.push(randomX);          // Push X for back track
                    back_trackY.push(randomY);          // Push Y for back track

                    randomY -= 2;                   // Move to next cell
                    Level[randomY][randomX].visited = true;     // Mark cell moved to as visited
                    Level[randomY][randomX].display = ' ';      // Update path
                    Level[randomY][randomX].bot_wall = false;   // Knock down wall
                    visitedCells++;                 // Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO DOWN
            else if ((random == 2) && (randomY < SIZE - 2)) {
                if (Level[randomY + 2][randomX].visited == false) {    // If not visited
                    Level[randomY + 1][randomX].display = ' ';    // Delete display
                    Level[randomY + 1][randomX].visited = true;   // Mark cell as visited
                    Level[randomY][randomX].bot_wall = false;   // Knock down wall

                    back_trackX.push(randomX);          // Push X for back track
                    back_trackY.push(randomY);          // Push Y for back track

                    randomY += 2;                   // Move to next cell
                    Level[randomY][randomX].visited = true;     // Mark cell moved to as visited
                    Level[randomY][randomX].display = ' ';      // Update path
                    Level[randomY][randomX].top_wall = false;   // Knock down wall
                    visitedCells++;                 // Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO LEFT
            else if ((random == 3) && (randomX > 1)) {
                if (Level[randomY][randomX - 2].visited == false) {    // If not visited
                    Level[randomY][randomX - 1].display = ' ';    // Delete display
                    Level[randomY][randomX - 1].visited = true;   // Mark cell as visited
                    Level[randomY][randomX].left_wall = false;  // Knock down wall

                    back_trackX.push(randomX);          // Push X for back track
                    back_trackY.push(randomY);          // Push Y for back track

                    randomX -= 2;                   // Move to next cell
                    Level[randomY][randomX].visited = true;     // Mark cell moved to as visited
                    Level[randomY][randomX].display = ' ';      // Update path
                    Level[randomY][randomX].right_wall = false; // Knock down wall
                    visitedCells++;                 // Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO RIGHT
            else if ((random == 4) && (randomX < SIZE - 2)) {
                if (Level[randomY][randomX + 2].visited == false) {    // If not visited
                    Level[randomY][randomX + 1].display = ' ';    // Delete display
                    Level[randomY][randomX + 1].visited = true;   // Mark cell as visited
                    Level[randomY][randomX].right_wall = false; // Knock down wall

                    back_trackX.push(randomX);          // Push X for back track
                    back_trackY.push(randomY);          // Push Y for back track

                    randomX += 2;                   // Move to next cell
                    Level[randomY][randomX].visited = true;     // Mark cell moved to as visited
                    Level[randomY][randomX].display = ' ';      // Update path
                    Level[randomY][randomX].left_wall = false;  // Knock down wall
                    visitedCells++;                 // Increase visitedCells counter
                }
                else
                    continue;
            }

            percent = (visitedCells * 100 / totalCells * 100) / 100;        // Progress in percentage
            cout << endl << "   Generating a Random Maze... " << percent << "%" << endl;
        }
        else {
            randomX = back_trackX.top();
            back_trackX.pop();

            randomY = back_trackY.top();
            back_trackY.pop();
        }


        // Redraw(Level);
        iteratii++;

        if (iteratii > 100) {
            break;
        }
        cout << "iteratii: " << iteratii << endl;
    }

    goalX = randomX;
    goalY = randomY;
    Level[goalY][goalX].display =' ';
    system("cls");

    //Redraw(Level);
    cout << endl << "\a\t   Complete!" << endl;
}
// MAIN
int Maze() {

    int posX = 0;
    int posY = 0;
    int goalX = 0;
    int goalY = 0;


    system("cls");
    Initialize(Level);


    GenerateMaze(Level, posX, posY, goalX, goalY);
    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << Level[i][j].display;
            cout << " ";
        }
        cout << endl;
    }

    return 0;
}

Tema2::Tema2()
{

}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;
    glm::ivec2 resolution = window->GetResolution();

    //orientare dupa mouse
    mouse_x = 0;
    mouse_y = 0;
    angle = 0;
    impact = false;
    switchToThird = false;
    gameOver = false;

    scale = 0.3;
    adjust_scale = 0.4;
    adjust_scale2 = 0.4;
    adjust1 = 0;
    adjust2 = 0;
    life = 500;
    unghi_player = 0;
    counter = 0;
    timeOut = 0;
    enemyShot = false;
    startTimer = 0;
    enemy_off = false;
    
    for (int i = 0; i < 5; i++) {
        vec[i] = 0.5;
    }

    //coordonatele jucatorului
    //centerX = (4*0.17+2.4);
    centerX = 0.4;
    //centerY = 1.32;
    centerY = 0.3;
    //centerZ = -(0+1.2);
    centerZ = 0.95;
    attackModeOn = false;
    shoot = true;
    shoot = false;
    stop1 = false;
    stop2 = false;
    stop3 = false;
    stop4 = false;
    iteratii = 0;
    directie = 1;
    index = 0;

    enemyX = 0;
    enemyY = 0;
    enemyZ = 0;

    //coordonatele gloantelor
    bullet.min_x = centerX;
    bullet.max_x = centerX + 0.5;
    bullet.min_y = centerY;
    bullet.max_y = centerY + 0.5;
    bullet.min_z = centerZ;
    bullet.max_z = centerZ + 0.5;

    bullet.degree = 0;

    //generarea labirintului in consola
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Level[i][j].display = ' ';
        }
    }


    //masuram timpul de la inceput
    start = std::clock();

    start1 = 0;
    start2 = 0;

    //pentru camera
    fov = 50;
    top = 10.f;

    left = .01f;
    right = 10.f;

    zNear = 0.01;
    bottom = 200 / 20000;

    zFar = 200;
    fov = 50;

    camera = new implemented::Camera();
    camera->Set(
        glm::vec3(centerX , centerY + 5, 400),
        glm::vec3(centerX , centerY + 1.f, 300.f),
        glm::vec3(0.f, 1.f, 0.f)

    );
    camera->RotateThirdPerson_OX(-20 * (float)M_PI / 180);

    projectionMatrix = glm::perspective(
        RADIANS(60.f),
        window->props.aspectRatio,
        zNear,
        zFar
    );
    proiectiaOrtografica = false;

    Player p;
    p.centerX = 0;
    p.centerY = 0;
    p.centerZ = 0;

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    glm::vec3 corner = glm::vec3(0, 0, 0);
    {
        Mesh* healthBar = object2D::DrawRectangle("healthBar", corner, 15, glm::vec3(1, 0, 0), false);
        AddMeshToList(healthBar);
    }
    {
        Mesh* healthBar1 = object2D::DrawRectangle("healthBar1", corner, 15, glm::vec3(1, 0, 0), true);
        AddMeshToList(healthBar1);
    }


// TODO(student): After you implement the changing of the projection
// parameters, remove hardcodings of these parameters
projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
// Create a shader program for drawing face polygon with the color of the normal
{
    Shader* shader = new Shader("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

{
    Shader* shader = new Shader("EnemyShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Enemy_VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Enemy_FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}
 
// Light & material properties
{
    lightPosition = glm::vec3(centerX, centerY, centerZ);
    lightDirection = glm::vec3(centerX, centerY, centerZ);
    materialShininess = 30;
    materialKd = 0.5;
    materialKs = 0.5;


    //switch pentru lumina
    lightOn = 0;
}
Maze();
cout << endl << "iteratii: " << iteratii << endl;

}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    angle = 5.f * atan((mouse_x - centerX) / (mouse_y - centerY + FLT_EPSILON));

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    int index = 0;
    {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (Level[i][j].display == '*') {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.7 + (2 + 1.2 * j), 0, -(0 + 1.2 * i) + 2.7));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.91, 1.92, 0.91));
                    //modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1+i, 0));

                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
                    if (sqrt((centerX - (-2.9 + (2 + 1.2 * j))) * (centerX - (-2.9 + (2 + 1.2 * j))) +
                        (centerY - 0) * (centerY - 0) + (centerZ + (0 + 1.2 * i) - 2.7) * (centerZ + (0 + 1.2 * i) - 2.7)) <= 0.4) {
                            impact = true;
                            //cout << endl << "coliziune iminenta " << "i: " << i << " j: " << j << " directie: " << directie << endl;
                            counter = duration;
                            if (impact == true && directie == 1) {
                                stop1 = true;
                                stop2 = false;
                                stop3 = false;
                                stop4 = false;
                            }
                            else if (impact == true && directie == 2) {
                                stop2 = true;
                                stop1 = false;
                                stop3 = false;
                                stop4 = false;
                            }
                            else if (impact == true && directie == 3) {
                                stop3 = true;
                                stop1 = false;
                                stop2 = false;
                                stop4 = false;
                            }
                            else if (impact == true && directie == 4) {
                                stop4 = true;
                                stop1 = false;
                                stop3 = false;
                                stop2 = false;
                            }
                    }
                    //25
                    else if (sqrt((centerX - (-2.9 + (2 + 1.2 * j))) * (centerX - (-2.9 + (2 + 1.2 * j))) +
                        (centerY - 0) * (centerY - 0) + (centerZ + (0 + 1.2 * i) - 2.7) * (centerZ + (0 + 1.2 * i) - 2.7)) > 25) {
                            impact = false;
                            stop1 = false;
                            stop2 = false;
                            stop3 = false;
                            stop4 = false;

                    }
                }

                if (Level[i][j].display == ' ' && index < 2 && i != 1 && j != 1) {
                    
                    enemyX = -2.9 + (2 + 1.2 * j);
                    enemyY = 0.5;
                    enemyZ = -(0 + 1.2 * i) + 2.7;

                    //coliziune proiectil si inamic
                    if (shoot == true) {
                        if (sqrt((enemyX - bullet.min_x) * (enemyX - bullet.min_x) +
                            (enemyY - bullet.min_y) * (enemyY - bullet.min_y) + (enemyZ - bullet.min_z) * (enemyZ - bullet.min_z)) < 2) {
                            life++;
                            enemyShot = true;

                            vec[index] = -10;
                            timeOut = 0;

                            cout << endl << "Enemy shot " <<life<< endl;
                            //vec[index] = -10;
                        }
                    }
                     
                    //desenarea inamicilor                 
                    DrawEnemy(deltaTimeSeconds, enemyX, vec[index], enemyZ, scale);
                    
                    index++;

                    //coliziune player si inamic
                    if (sqrt((centerX - enemyX) * (centerX - enemyX) +
                        (centerY - enemyY) * (centerY - enemyY) + (centerZ - enemyZ) * (centerZ - enemyZ)) < 0.9) {
                        life--;
                        cout << endl << "Enemy encounter - current life level: " << life << endl;
                    }
                }
            }
        }
    }
    if (impact==true&& duration - counter > 1) {
        stop1 = false;
        stop2 = false;
        stop3 = false;
        stop4 = false;
        
    }
    cout << "stop1 stop2 stop3 stop4: " << stop1 << " " << stop2 << " " << stop3 << " "<<stop4<<endl;
    //desenarea proiectilelor
    if (shoot == true)
    {

        {
            bullet.degree = angle;
            float bullet_speed = 1.2;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(bullet.min_x, bullet.min_y + 0.5, bullet.min_z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.2, 0.2));

            //bullet.min_z += cos(bullet.degree) * bullet_speed * deltaTimeSeconds;
            //bullet.min_x += sin(bullet.degree) * bullet_speed * deltaTimeSeconds;

            bullet.min_z -= cos(bullet.degree) * bullet_speed * deltaTimeSeconds;
            bullet.min_x += sin(bullet.degree) * bullet_speed * deltaTimeSeconds;

            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));


            //proiectilele dispar dupa 3 secunde
            if (duration - start1 > 3) {
                bullet.min_y = -10;
            }
        }
    }



    //healthbar 
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //projectionMatrix = glm::ortho(-10.0f, 10.0f, -1.5f, 25.f, zNear, zFar);

       //playerul are 5 vieti
        if (life < 250 && life>200) {
            adjust_scale = 0.35;
            adjust1 = 1;
        }
        else if (life < 200 && life>150 ) {
            adjust_scale = 0.30;
            adjust1 = 2;
        }
        else if (life < 150 && life>100) {
            adjust_scale = 0.25;
            adjust1 = 3;
        }
        else if (life < 100 && life>50) {
            adjust_scale2 = 0.20;
            adjust1 = 4;
        }
        else if (life < 50 && life>0) {
            adjust_scale = 0.15;
            adjust1 = 5;
        }

        projectionMatrix = glm::ortho(right, top, -1.5f, 25.f, zNear, zFar);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.5 - 0.05 * adjust1, centerY + 1.56, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(adjust_scale, 0.08, 0.1));
        RenderSimpleMesh(meshes["quad"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 1, 1));

        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
    }

    //healthbar wireframe
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //projectionMatrix = glm::ortho(-10.0f, 10.0f, -1.5f, 25.f, zNear, zFar);
        projectionMatrix = glm::ortho(right, top, -1.5f, 25.f, zNear, zFar);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.5, centerY + 1.56, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.08, 0.1));
        RenderSimpleMesh(meshes["quad"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);

    }
    //timeElapsed
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        //projectionMatrix = glm::ortho(-10.0f, 10.0f, -1.5f, 25.f, zNear, zFar);
        projectionMatrix = glm::ortho(right, top, -1.5f, 25.f, zNear, zFar);

        //timpul limita este de 90 de secunde
        if (duration > 15 && duration < 30) {
            adjust_scale2 = 0.35;
            adjust2 = 1;
        }
        else if (duration > 30 & duration < 45) {
            adjust_scale2 = 0.30;
            adjust2 = 2;
        }
        else if (duration > 45 & duration < 60) {
            adjust_scale2 = 0.25;
            adjust2 = 3;
        }
        else if (duration > 60 & duration < 75) {
            adjust_scale2 = 0.20;
            adjust2 = 4;
        }
        else if (duration > 75 & duration < 90) {
            adjust_scale2 = 0.15;
            adjust2 = 5;
        }
        else if (duration > 89 & duration < 90) {
            adjust_scale2 = 0.10;
            adjust2 = 6;
        }

        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.5 - 0.05 * adjust2, centerY + 1.3, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(adjust_scale2, 0.08, 0.1));
        RenderSimpleMesh(meshes["quad"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));

        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);

    }

    DrawPlayer(deltaTimeSeconds);
    

    //verificare exit labirint
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (Level[i][j].display == ' ' && (i == 0 || i == 15
                || j == 0 || j == 15)) {

                if (sqrt((centerX - (-2.9 + (2 + 1.2 * j))) * (centerX - (-2.9 + (2 + 1.2 * j))) +
                    (centerY - 0) * (centerY - 0) + (centerZ + (0 + 1.2 * i) - 2.7) * (centerZ + (0 + 1.2 * i) - 2.7)) <= 0.7) {
                    gameOver = true;
                    cout << endl << "Jocul a luat sfarsit!!" << endl;
                }
            }
        }
    }

    //daca player-ul nu mai are viata sau a depasit timpul, se declara Game Over
    if ((life <= 0) || (duration > 90)) {
        gameOver = true;
        cout << endl << "Jocul a luat sfarsit!!" << endl;
        adjust_scale = 0;
        adjust_scale2 = 0;
    }

    //podea
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.2, -0.9, -1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(70, 1, 70));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["quad"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }



    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    
    glm::vec3 cameraTarget = glm::vec3(-15.0f, -5.0f, -1.0f);
    //glm::vec3 cameraTarget = glm::vec3(centerX+100, centerY+100, -1.0f);
    if (renderCameraTarget)
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 100, centerY + 100, -1.0f));
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());     
        modelMatrix = glm::translate(modelMatrix, cameraTarget);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.37, 0.37, 0.2));
        DrawPlayer(deltaTimeSeconds);

    }
    

    
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3& color)
{

    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    //glm::vec3 eyePosition = glm::vec3(-15.0f, -5.0f, -1.0f);
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    //glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);
    glUniform3f(eye_position, centerX, centerY, centerZ);
    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    GLint type = glGetUniformLocation(shader->program, "type_of_light");
    glUniform1i(type, lightOn);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

}

void Tema2::RenderSimpleMesh2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{

        if (!mesh || !shader || !shader->GetProgramID())
            return;

        // Render an object using the specified shader and the specified position
        glUseProgram(shader->program);

        // TODO(student): Get shader location for uniform mat4 "Model"

        int model_location = glGetUniformLocation(shader->GetProgramID(), "Model");

        // TODO(student): Set shader uniform "Model" to modelMatrix

        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // TODO(student): Get shader location for uniform mat4 "View"

        GLint location_View = glGetUniformLocation(shader->GetProgramID(), "View");

        // TODO(student): Set shader uniform "View" to viewMatrix
        glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();

        glUniformMatrix4fv(location_View, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // TODO(student): Get shader location for uniform mat4 "Projection"

        GLint location_Projection = glGetUniformLocation(shader->GetProgramID(), "Projection");

        // TODO(student): Set shader uniform "Projection" to projectionMatrix
        glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();

        glUniformMatrix4fv(location_Projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "EnemyTime");

        // Set shader uniform "Time" to elapsed time
        glUniform1f(timeLocation, timeOut);

        // Draw the object
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    

}


void Tema2::DrawPlayer(float deltaTimeSeconds)
{
    //legs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.17, centerY - 0.82, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.3));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.18, centerY - 0.82, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.3));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }


    //shirt
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(0.17, 1.32, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX, centerY, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.65, 0.7, 0.25));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.1564, 0.68, 0.5753));
        //glm::vec3(0.1564, 0.68, 0.5753)
    }

    //arms
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.52, centerY, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.2));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.53, centerY, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.2));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }
    //hands
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.52, centerY - 0.57, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.2, 0.2));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.53, centerY - 0.57, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.2, 0.2));
        modelMatrix = glm::rotate(modelMatrix, unghi_player, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));
    }


    //head
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX, centerY + 0.63, centerZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.37, 0.37, 0.2));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));
        //glm::vec3(0.5, 1.0, 0.5)
    }
}

void Tema2::DrawEnemy(float deltaTimeSeconds, float enemyX, float enemyY, float enemyZ,float scale)
{
    //legs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.17, centerY - 0.82, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX-0.17, enemyY-0.82, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.9, scale));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.17, centerY - 0.82, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX +0.18, enemyY - 0.82, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.9, scale));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));
    }


    //shirt
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(0.17, 1.32, 0));
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX, centerY, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX, enemyY , enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale+0.35, 0.8, scale-0.05));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));
        //glm::vec3(0.1564, 0.68, 0.5753)
    }

    //arms
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.52, centerY, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX - 0.52, enemyY, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.9, scale-0.1));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.53, centerY, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX +0.53, enemyY , enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.9, scale-0.1));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(0.3696, 0.3781, 0.88));
    }
    //hands
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX - 0.52, centerY - 0.57, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX - 0.52, enemyY - 0.57, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.2, scale-0.1));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX + 0.53, centerY - 0.57, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX + 0.53, enemyY - 0.57, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.2, scale-0.1));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0.99, 0.99));
    }


    //head
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(centerX, centerY + 0.63, centerZ));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyX, enemyY + 0.63, enemyZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale+0.07, 0.37, scale-0.1));
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));

        RenderSimpleMesh2(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));
        //glm::vec3(0.5, 1.0, 0.5)
    }


}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2;
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
    glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
    forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

    // Add key press event
    if (enemyShot==true)
    {   
        timeOut = 100;

    }

    float speed = 2;

        if (attackModeOn == false) {
            float speed = 2;
            camera->Set(
                glm::vec3(centerX , centerY + 10, 400),
                glm::vec3(centerX , centerY + 1.f, 400.f),
                glm::vec3(0.f, 1.f, 0.f)

            );
            if (switchToThird == true) {
                if (duration - start2 < 1) {
                    centerZ -= cameraSpeed * deltaTime;
                }
                
            }
            camera->RotateThirdPerson_OX(-20 * (float)M_PI / 180);


            if (window->KeyHold(GLFW_KEY_W)) {
                // Translate the camera forward
                if (stop1 == false) {
                    camera->TranslateForward(deltaTime * cameraSpeed);
                    centerZ -= cameraSpeed * deltaTime;
                    lightPosition -= forward * deltaTime * speed;
                    directie = 1;

                }
                if (stop1 == true) {
                    camera->TranslateForward(-deltaTime * cameraSpeed);
                    centerZ += 0.5* deltaTime;
                    lightPosition += forward * deltaTime * speed;
                    directie = 2;


                }

            }

            if (window->KeyHold(GLFW_KEY_A)) {
                // Translate the camera to the left
                if (stop3 == false) {
                    camera->TranslateRight(-deltaTime * cameraSpeed);
                    centerX -= cameraSpeed * deltaTime;
                    lightPosition -= right * deltaTime * speed;
                    directie = 3;
                }
                if (stop3 == true) {
                    camera->TranslateRight(deltaTime * cameraSpeed);
                    centerX += 0.5* deltaTime;
                    lightPosition += right * deltaTime * speed;
                    directie = 4;

                }
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                // Translate the camera backward
                if (stop2 == false) {
                    camera->TranslateForward(-deltaTime * cameraSpeed);
                    centerZ += cameraSpeed * deltaTime;
                    lightPosition += forward * deltaTime * speed;
                    directie = 2;

                }
                if (stop2 == true) {
                    camera->TranslateForward(deltaTime * cameraSpeed);
                    centerZ -= 0.5* deltaTime;
                    lightPosition -= forward * deltaTime * speed;
                    directie = 1;


                }
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                // Translate the camera to the right
                if (stop4 == false) {
                    camera->TranslateRight(deltaTime * cameraSpeed);
                    centerX += cameraSpeed * deltaTime;
                    lightPosition += right * deltaTime * speed;
                    directie = 4;
                }
                if (stop4 == true) {
                    camera->TranslateRight(-deltaTime * cameraSpeed);
                    centerX -= 0.5 * deltaTime;
                    lightPosition -= right * deltaTime * speed;
                    directie = 3;

                }

            }

            if (window->KeyHold(GLFW_KEY_Q)) {
                // Translate the camera downward
                camera->TranslateUpward(-deltaTime * cameraSpeed);
            }

            if (window->KeyHold(GLFW_KEY_E)) {
                // Translate the camera upward
                camera->TranslateUpward(deltaTime * cameraSpeed);

            }
        }

        //camera FirstPerson
        if (attackModeOn == true) {
            float speed = 2;
            camera->Set(
                glm::vec3(centerX , centerY +1, centerZ-10),
                glm::vec3(centerX + 0.f, centerY + 1.f, 300.f),
                glm::vec3(0.f, 1.f, 0.f)
            );
           
            camera->TranslateForward(deltaTime * cameraSpeed);

            if (duration - start2 < 0.99) {
                centerZ += cameraSpeed * deltaTime;
            }

            if (window->KeyHold(GLFW_KEY_W)) {
                // Translate the camera forward
                if (stop1 == false) {
                    camera->TranslateForward(deltaTime * cameraSpeed);
                    centerZ -= cameraSpeed * deltaTime;
                    lightPosition -= forward * deltaTime * speed;
                    directie = 1;

                }
                if (stop1 == true) {
                    camera->TranslateForward(-deltaTime * cameraSpeed);
                    centerZ += cameraSpeed * deltaTime;
                    lightPosition += forward * deltaTime * speed;
                    directie = 2;
                    stop1 = false;

                }

            }

            if (window->KeyHold(GLFW_KEY_A)) {
                // Translate the camera to the left
                if (stop3 == false) {
                    camera->TranslateRight(-deltaTime * cameraSpeed);
                    centerX -= cameraSpeed * deltaTime;
                    lightPosition -= right * deltaTime * speed;
                    directie = 3;
                }
                if (stop3 == true) {
                    camera->TranslateRight(deltaTime * cameraSpeed);
                    centerX += cameraSpeed * deltaTime;
                    lightPosition += right * deltaTime * speed;
                    directie = 4;
                    stop3 = false;
                }
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                // Translate the camera backward
                if (stop2 == false) {
                    camera->TranslateForward(-deltaTime * cameraSpeed);
                    centerZ += cameraSpeed * deltaTime;
                    lightPosition += forward * deltaTime * speed;
                    directie = 2;

                }
                if (stop2 == true) {
                    camera->TranslateForward(deltaTime * cameraSpeed);
                    centerZ -= cameraSpeed * deltaTime;
                    lightPosition -= forward * deltaTime * speed;
                    directie = 1;
                    stop2 = false;

                }
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                // Translate the camera to the right
                if (stop4 == false) {
                    camera->TranslateRight(deltaTime * cameraSpeed);
                    centerX += cameraSpeed * deltaTime;
                    lightPosition += right * deltaTime * speed;
                    directie = 4;
                }
                if (stop4 == true) {
                    camera->TranslateRight(-deltaTime * cameraSpeed);
                    centerX -= cameraSpeed * deltaTime;
                    lightPosition -= right * deltaTime * speed;
                    directie = 3;
                    stop4 = false;
                }

            }

            if (window->KeyHold(GLFW_KEY_Q)) {
                // Translate the camera downward
                camera->TranslateUpward(-deltaTime * cameraSpeed);
            }

            if (window->KeyHold(GLFW_KEY_E)) {
                // Translate the camera upward
                camera->TranslateUpward(deltaTime * cameraSpeed);


            }
        }

   
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }


    //nu merge tasta CTRL, asa ca am folosit tasta space
    if (key == GLFW_KEY_C) {

        // face switch intre attackMode
        attackModeOn = !attackModeOn;

        start2 = duration;

        if (attackModeOn == false) {
            switchToThird = true;
        }

    }
    if (key == GLFW_KEY_SPACE)
    {

        //durata unui glont
        start1 = duration;
        shoot = true;
        startTimer = duration;
        index++;
        bullet.min_x = centerX;
        bullet.min_y = centerY;
        bullet.min_z = centerZ;
        bullet.degree = angle;
    }
    if (key == GLFW_KEY_X) {
        enemy_off = true;
    }

}
void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mouse_x = (GLfloat)mouseX;
    mouse_y = (GLfloat)mouseY;

    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;


    
    if (attackModeOn == true) {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {


            if (window->GetSpecialKeyState() == 0)
            {
                renderCameraTarget = false;
                camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
                camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
                unghi_player += sensivityOX * (-deltaX);

            }
        }
    }
   
    else if (attackModeOn == false) {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {

            if (window->GetSpecialKeyState() == 0)
            {
                renderCameraTarget = false;
                camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
                camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
                unghi_player += sensivityOX * (-deltaX);

            }
        }
    }
       
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}



