#define GLEW_STATIC
#define FPS 60

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "include/Engine.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

OpenGL::Shader shader;
OpenGL::Shader colorless;
OpenGL::Shader GUI::shader;
Renderer renderer;

SkyBox skybox;
Planet earth;
//Terrain terrain;
//Terrain to be drawn for each planet

//Billboard billboard;
//Shader Billboard::shader;

Spaceship player;
Tools::Camera camera;
glm::mat4 proj;

//GUI gui;

Tools::MousePicker picker;

int screenWidth,screenHeight;
bool initial = true;

Tools::ShadowLoader shadow;
Tools::FPSManager fpsManager(FPS);

float cam[]{
    0, 10.0, 0
};

void resetPointer(){
    glutWarpPointer(screenWidth/2, screenHeight/2);
    camera.setYaw(0);
    camera.setPitch(10);
}

void init(){
    shader.loadShader("res/shaders/Basic.shader");
    shader.bind();

    int i[] = {0,1};
    shader.addUniform1iv("u_Textures", i);

    {
        float view[]= {0.0f, 0.0f, 0.0f};
        float d_position[] = {0.0f, 0.0f, -2.0f};
        float a_color[] = {1.0f, 1.0f, 1.0f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {1.0f, 1.0f, 1.0f};

        shader.addUniform3f("u_viewPos", view);
        shader.addUniform3f("light.position", d_position);
        shader.addUniform3f("light.ambient", a_color);
        shader.addUniform3f("light.diffuse", d_color);
        shader.addUniform3f("light.specular", s_color);
    }

    {
        float d_position[] = {10.0f, 0.0f, -20.0f};
        float a_color[] = {1.0f, 1.0f, 1.0f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {1.0f, 1.0f, 1.0f};
//        float constant = 1.0f;
//        float linear = 0.09f;
//        float quadratic = 0.032f;
        float constant = 0.5f;
        float linear = 0.045f;
        float quadratic = 0.016f;

        shader.addUniform3f("plight.position", d_position);
        shader.addUniform3f("plight.ambient", a_color);
        shader.addUniform3f("plight.diffuse", d_color);
        shader.addUniform3f("plight.specular", s_color);
        shader.addUniform1f("plight.constant", constant);
        shader.addUniform1f("plight.linear", linear);
        shader.addUniform1f("plight.quadratic", quadratic);
    }

    {
        float direction[] = {0.0f, -1.0f, -1.0f};
        float a_color[] = {1.0f, 1.0f, 1.0f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {1.0f, 1.0f, 1.0f};

        shader.addUniform3f("dlight.direction", direction);
        shader.addUniform3f("dlight.ambient", a_color);
        shader.addUniform3f("dlight.diffuse", d_color);
        shader.addUniform3f("dlight.specular", s_color);
    }

    {
        float shininess = 32.0f;
        float color[] = {1.0f, 0.0f, 1.0f};
        float a_color[] = {0.1f, 0.1f, 0.1f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {1.0f, 1.0f, 1.0f};

        shader.addUniform3f("u_color", color);
        shader.addUniform1f("material.shininess", 32.0f);
        shader.addUniform3f("material.ambient", a_color);
        shader.addUniform3f("material.diffuse", d_color);
        shader.addUniform3f("material.specular", s_color);
        shader.addUniform1f("material.shininess", shininess);
    }

    {
        shader.addUniform1i("u_useLighting", 0);
        shader.addUniform1i("u_lightType", 1);
        shader.addUniform1i("u_textureMap", 0);
        shader.addUniform1i("u_texSlot", 0);
    }

    earth.loadPlanet("res/models/earth.obj", "res/textures/earth.png", 0.25);
    {
//        std::vector<std::string> locations;
//        locations.push_back("res/textures/terrain/grass2.png");
//        locations.push_back("res/textures/terrain/grass1.png");
//        locations.push_back("res/textures/terrain/mud.png");
//        locations.push_back("res/textures/terrain/path.png");
//        locations.push_back("res/textures/terrain/blendMap.png");
//        terrain.loadTerrain("res/shaders/Terrain.shader", locations,"res/textures/terrain/heightmap.png", 10, 200, 200);
//        locations.clear();
    }

    {
        //player.loadFiles("res/models/player.obj", "res/textures/player.png", 0.125);
        player.setPosition(glm::vec3(-2.0, 0.0, 0.0));
        player.setDirection(glm::vec3(1.0,0.0,0.0));
    }

    {
        float fog[]={0.40, 0.50, 0.60};
        std::vector<std::string> locations;
        locations.push_back("res/textures/Space/right.png");
        locations.push_back("res/textures/Space/left.png");
        locations.push_back("res/textures/Space/top.png");
        locations.push_back("res/textures/Space/bottom.png");
        locations.push_back("res/textures/Space/back.png");
        locations.push_back("res/textures/Space/front.png");
//        locations.push_back("res/textures/Milky Way/PositiveX.png");
//        locations.push_back("res/textures/Milky Way/NegativeX.png");
//        locations.push_back("res/textures/Milky Way/PositiveY.png");
//        locations.push_back("res/textures/Milky Way/NegativeY.png");
//        locations.push_back("res/textures/Milky Way/PositiveZ.png");
//        locations.push_back("res/textures/Milky Way/NegativeZ.png");
        skybox.loadSkyBox(locations, "res/shaders/Skybox.shader", 200, fog);
        locations.clear();
    }
    /*{
        Billboard::specifyShader("res/shaders/Particles.shader");
        billboard.loadData(glm::vec3(00, 2.0, 00), glm::vec2(0.2, 0.2));
    }*/
    {
        shadow.loadData(1024, 1024, screenWidth, screenHeight, glm::vec3(-0.5, -1.25, 0.5));
        colorless.loadShader("res/Shaders/Colorless.shader");
        colorless.bind();
        colorless.addUniform1i("texSlot",0);
    }

}

static void resize(int width, int height){
    screenHeight = height;
    screenWidth = width;
    glViewport(0, 0, width, height);
    proj = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 200.0f);
}

static void display(void){
    fpsManager.initiateTimer();
    fpsManager.updateCounter();

    glClearColor(0.5, 0.5, 0.5, 0.0);
	renderer.clear();

	camera.follow(player);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = proj * view * model;

    shader.bind();
    shader.addUniformMat4f("u_model", model);
    shader.addUniformMat4f("u_MVP", mvp);
    shader.addUniform3f("u_viewPos", cam);

    earth.drawPlanet(shader);
    model = glm::mat4(1.0f);
    skybox.increaseRotation();
    //gui.draw();
    Tools::CameraData temp_camera;
    temp_camera = shadow.getCameraData(camera, 45, 0.1, 15.0, screenWidth/screenHeight, 50.0);
    shadow.startSampling();
    {
        renderer.clear();
        colorless.bind();
        mvp = temp_camera.projection * temp_camera.view;
        colorless.addUniformMat4f("mvp", mvp);
        //terrain.draw(temp_camera.projection*temp_camera.view, colorless);
        //model = glm::translate(model, player.getPosition());
        //model = glm::rotate(model, glm::radians(player.getRotation()+180), glm::vec3(0.0, 1.0, 0.0));
        //mvp = temp_camera.projection * temp_camera.view * model;

        //colorless.addUniformMat4f("mvp", mvp);
        //player.draw(colorless);
    }
    shadow.stopSampling(screenWidth, screenHeight);

//    shadow.bindDepthTexture(terrain.shadowDepthSlot());
//    terrain.draw(view, proj, temp_camera.projection*temp_camera.view);

    skybox.render(view, proj);
    /*
    //Works currently
    picker.setMatrices(view, proj);
    glm::vec3 ray = picker.calculateRay(0, 0);
    std::cout << "Ray = " << ray.x << " " << ray.y << " " << ray.z <<std::endl;*/

    //Limiting FPS to targeted
    fpsManager.limitFPS();
    glutSwapBuffers();
    glutPostRedisplay();
}

static void closeWindow(){
    shader.deleteShader();
//    terrain.cleanUP();
    player.cleanUP();
    skybox.cleanUP();
//    gui.cleanUP();
//    GUI::deleteShader();
    shadow.cleanUP();

    exit(0);
}

static void key(unsigned char key, int x, int y){
    switch (key)
    {
        case 27 :
        case 'q':
            closeWindow();
            break;
        case 'w':
            player.setDirection(-camera.getDirection());
            player.moveForward();
            break;
        case 'a':
            player.setDirection(-camera.getDirection());
            player.moveLeft();
            break;
        case 's':
            player.setDirection(-camera.getDirection());
            player.moveBackward();
            break;
        case 'd':
            player.setDirection(-camera.getDirection());
            player.moveRight();
            break;
    }
    glutPostRedisplay();
}

void mouseMoved(int x,int y){

    if(initial){
        resetPointer();
        initial = false;
        return;
    }

    static bool just_warped = false;
    int dx = x - screenWidth/2;
    int dy = y - screenHeight/2;

    if(just_warped) {
        just_warped = false;
        return;
    }

    camera.increaseYaw(-dx);
    camera.increasePitch(dy);
    glutWarpPointer(screenWidth/2, screenHeight/2);
    just_warped = true;
}

int main(int argc, char *argv[]){
    Window::createGlutWindow(argc, argv,
                        "Solar Sailer",//Title
                        glm::vec2(250, 75),//Position
                        glm::vec2(800, 600));//Size
    Window::initializeGLEW(false);//Start extension wrangle without showing errors
    Window::enableBackfaceCulling();
    Window::enableDepthTest();
    Window::enableClipDistance0();

    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(mouseMoved);
	glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(mouseMoved);
    //glutTimerFunc(1000.0/10, idle, 0);
    //glutFullScreen();
    resetPointer();
    glutMainLoop();

    return EXIT_SUCCESS;
}
