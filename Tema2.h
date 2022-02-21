#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab5/lab_camera.h"
#define SIZE 16


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
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
                 : x(x), y(y),  width(width), height(height) {}
             float x;
             float y;
             float z;
             float width;
             float height;
         };


         struct Cell
         {
             bool visited;
             bool top_wall;
             bool bot_wall;
             bool left_wall;
             bool right_wall;
             char display;
         };



         struct Player {
             float centerX, centerY, centerZ;
         };


        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void DrawPlayer(float deltaTimeSeconds);
        void DrawEnemy(float deltaTimeSeconds, float enemyX, float enemyY, float enemyZ,float scale);
        void Initialize(Cell Level[][SIZE]);
        void ClearScreen();

        void GenerateMaze(Cell Level[][SIZE], int& posX, int& posY, int& goalX, int& goalY);
        void SaveMaze(Cell Level[][SIZE]);

        //void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
        void RenderSimpleMesh2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;



     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix, visMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        bool proiectiaOrtografica;

        float z1;
        float z2;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        bool enemy_off;
        int lightOn;
        float centerX, centerY, centerZ;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        GLfloat zNear, zFar;
        float mouse_x, mouse_y, angle;

        

        
        int start, start1, start2, counter;
        bool attackModeOn;
        bool shoot;
        int life;




    };
}   // namespace m1
