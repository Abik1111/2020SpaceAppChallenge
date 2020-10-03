#define GLEW_STATIC
#define FPS 30

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "include/Engine.cpp"
#include "include/spacetime.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

OpenGL::Shader shader;
OpenGL::Shader colorless;
//Place for gui
//OpenGL::Shader GUI::shader;
Renderer renderer;
Tools::BloomPostProcessor bloomPostProcessor;
SkyBox skybox;
//PlanetInClose closeEarth;

Planet sun;
Planet mercury;
Planet venus;
Planet earth;
Planet mars;
Planet jupiter;
Planet saturn;
Planet uranus;
Planet neptune;

Planet moon;
//Terrain terrain;
//Terrain to be drawn for each planet
//Billboard billboard;
//Shader Billboard::shader;

Spaceship ship;
Tools::Camera camera;
glm::mat4 projectionMatrix;

//GUI gui;
//OpenGL::Shader GUI::shader;

Tools::MousePicker picker;
int screenWidth,screenHeight;
bool initial = true;

Tools::ShadowLoader shadow;
Tools::FPSManager fpsManager(FPS);

bool keyBuffer[128];
bool mouseBuffer[2];

Spacetime spacetime;
Spacetime earthSystem;
Vector3 pos;
double radius_scale = 30.0/695842e3;//6e-6;
double position_scale = 75e-11;//15e-11;
double moons_position_scale = 120e-10;//15e-11;
int physicsInitiate(){
	Matter Mercury;
	Matter Earth;
	Matter Mars;
	Matter Venus;
	Matter Jupiter;
	Matter Saturn;
	Matter Uranus;
	Matter Neptune;
	Matter Pluto;
    Matter Sun;
    Sun.setMass(1.9e30);
    Sun.setRadius(695842e3);

    Mercury.setMass(3.3011e23);
    Mercury.setPosition(Vector3::getVector(46e9,0,0));
    Mercury.setVelocity(Vector3::getVector(0,0,1).scale(57.59e3));
    Mercury.setRadius(2439.7e3);

	Venus.setMass(4.8675e24);
	Venus.setPosition(Vector3::getVector(107.48e9, 0, 0));
	Venus.setVelocity(Vector3::getVector(0, 0, 1).scale(34.3e3));
	Venus.setRadius(6052e3);

    Earth.setMass(5.97237e24);
    Earth.setPosition(Vector3::getVector(147.09e9,0,0));
    Earth.setVelocity(Vector3::getVector(0,0,1).scale(29.5e3));
    Earth.setRadius(6357e3);

    Mars.setMass(6.4171e23);
    Mars.setPosition(Vector3::getVector(206.62e9,0,0));
    Mars.setVelocity(Vector3::getVector(0,0,1).scale(25.80e3));
    Mars.setRadius(3390e3);

	Jupiter.setMass(1898.19e24);
	Jupiter.setPosition(Vector3::getVector(740.52e9, 0, 0));
	Jupiter.setVelocity(Vector3::getVector(0, 0, 1).scale(13.23e3));
	Jupiter.setRadius(69911e3);

	Saturn.setMass(568.34e24);
	Saturn.setPosition(Vector3::getVector(1352.55e9, 0, 0));
	Saturn.setVelocity(Vector3::getVector(0, 0, 1).scale(10.18e3));
	Saturn.setRadius(58232e3);

	Uranus.setMass(86.813e24);
	Uranus.setPosition(Vector3::getVector(2741.30e9, 0, 0));
	Uranus.setVelocity(Vector3::getVector(0, 0, 1).scale(7.11e3));
    Uranus.setRadius(25362e3);

	Neptune.setMass(102.413e24);
	Neptune.setPosition(Vector3::getVector(4444.45e9, 0, 0));
	Neptune.setVelocity(Vector3::getVector(0, 0, 1).scale(6.10e3));
    Neptune.setRadius(24622e3);

//	Pluto.setMass(1.303e22);
//	Pluto.setPosition(Vector3::getVector(7375.93e9, 0, 0));
//	Pluto.setVelocity(Vector3::getVector(0, 0, 1).scale(3.71e3));

	spacetime.addMatter(0,Sun);
    spacetime.addMatter(1,Mercury);
	spacetime.addMatter(2, Venus);
    spacetime.addMatter(3,Earth);
    spacetime.addMatter(4,Mars);
    spacetime.addMatter(5, Jupiter);
	spacetime.addMatter(6, Saturn);
	spacetime.addMatter(7, Uranus);
	spacetime.addMatter(8, Neptune);
//	spacetime.addMatter(9, Pluto);

    Matter Moon;
    Moon.setMass(7.342e22);
    Moon.setPosition(Vector3::getVector(36200e3,0,0));
    Moon.setVelocity(Vector3::getVector(0,0,1).scale(1.002e3));
    Moon.setRadius(1734.4e3);

    earthSystem.addMatter(0, Earth);
    earthSystem.addMatter(1, Moon);
    return 0;
}

float cam[]{
    0, 10.0, 0
};

void resetPointer(){
    glutWarpPointer(screenWidth/2, screenHeight/2);
    camera.setYaw(0);
    camera.setPitch(10);
}

void init(){
    //Initiating the physics
    physicsInitiate();

    shader.loadShader("res/shaders/Basic.shader");
    shader.bind();

    int i[] = {0,1};
    shader.addUniform1iv("u_Textures", i);

    {
        float view[]= {0.0f, 0.0f, 0.0f};
        float d_position[] = {0.0f, 0.0f, 0.0f};
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
        float d_position[] = {5.0f, 0.0f, 0.0f};
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
        float direction[] = {-1.0f, 0.0f, 0.0f};
        float a_color[] = {1.0f, 1.0f, 1.0f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {1.0f, 1.0f, 1.0f};

        shader.addUniform3f("dlight.direction", direction);
        shader.addUniform3f("dlight.ambient", a_color);
        shader.addUniform3f("dlight.diffuse", d_color);
        shader.addUniform3f("dlight.specular", s_color);
    }

    {
        float shininess = 16.0f;
        float color[] = {1.0f, 1.0f, 1.0f};
        float a_color[] = {0.1f, 0.1f, 0.1f};
        float d_color[] = {1.0f, 1.0f, 1.0f};
        float s_color[] = {0.0f, 0.0f, 0.0f};

        shader.addUniform3f("u_color", color);
        shader.addUniform1f("material.shininess", 32.0f);
        shader.addUniform3f("material.ambient", a_color);
        shader.addUniform3f("material.diffuse", d_color);
        shader.addUniform3f("material.specular", s_color);
        shader.addUniform1f("material.shininess", shininess);
    }

    {
        shader.addUniform1i("u_useLighting", 1);
        shader.addUniform1i("u_lightType", 0);
        shader.addUniform1i("u_textureMap", 0);
        shader.addUniform1i("u_texSlot", 0);
    }

//    earth.setPosition(glm::vec3(0.0, 0.0, -5.0));
//    closeEarth.loadPlanet(1.0f, 90.0, 64, 256, 0.1f, "res/textures/Earth/Diffuse_2K.png", "res/textures/Earth/Bump_2K.png", 42);
//    closeEarth.setPosition(glm::vec3(0.0, 0.0, -5.0));

//    sun.loadPlanet("res/textures/sun.png", 9.0f , 32);
    sun.loadPlanet("res/textures/sun.png", radius_scale*spacetime.getMatter(0).getRadius(), 16);

    mercury.loadPlanet("res/textures/mercury.png", radius_scale*spacetime.getMatter(1).getRadius(), 8);
    venus.loadPlanet("res/textures/venus.png", radius_scale*spacetime.getMatter(2).getRadius(), 8);
    earth.loadPlanet("res/textures/Earth.png", radius_scale*spacetime.getMatter(3).getRadius(), 8);
        moon.loadPlanet("res/textures/moon.png", radius_scale*earthSystem.getMatter(1).getRadius(), 8);
    mars.loadPlanet("res/textures/mars.png", radius_scale*spacetime.getMatter(4).getRadius(), 8);
    jupiter.loadPlanet("res/textures/jupiter.png", radius_scale*spacetime.getMatter(5).getRadius(), 8);
    saturn.loadPlanet("res/textures/saturn.png", radius_scale*spacetime.getMatter(6).getRadius(), 8);
    uranus.loadPlanet("res/textures/uranus.png", radius_scale*spacetime.getMatter(7).getRadius(), 8);
    neptune.loadPlanet("res/textures/neptune.png", radius_scale*spacetime.getMatter(8).getRadius(), 8);


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
        ship.loadFiles("res/models/SpaceShip.obj", "res/textures/spaceship_texture.png", 0.125);
        ship.setPosition(glm::vec3(12.0, 0.0, 0.0));
        ship.setDirection(glm::vec3(1.0,0.0,1.0));
    }

    camera.setFov(60);

    {
        float fog[]={0.40, 0.50, 0.60};
        std::vector<std::string> locations;
//Tesseract waala texture
//        locations.push_back("res/textures/Space/right.png");
//        locations.push_back("res/textures/Space/left.png");
//        locations.push_back("res/textures/Space/top.png");
//        locations.push_back("res/textures/Space/bottom.png");
//        locations.push_back("res/textures/Space/back.png");
//        locations.push_back("res/textures/Space/front.png");
//Milky way waala texture
        locations.push_back("res/textures/Milky Way/PositiveX.png");
        locations.push_back("res/textures/Milky Way/NegativeX.png");
        locations.push_back("res/textures/Milky Way/PositiveY.png");
        locations.push_back("res/textures/Milky Way/NegativeY.png");
        locations.push_back("res/textures/Milky Way/PositiveZ.png");
        locations.push_back("res/textures/Milky Way/NegativeZ.png");
        skybox.loadSkyBox(locations, "res/shaders/Skybox.shader", 200, fog);
        locations.clear();
    }
    std::vector<std::string> processes;
    processes.push_back("res/shaders/Dummy.shader");
    processes.push_back("res/shaders/BrightFilter.shader");
    processes.push_back("res/shaders/HorizotalBlur.shader");
    processes.push_back("res/shaders/VerticalBlur.shader");
    processes.push_back("res/shaders/Bloom.shader");
    bloomPostProcessor.load(800, 600, screenWidth, screenHeight, processes, 0.963);
}

static void resize_display(int width, int height){
    screenHeight = height;
    screenWidth = width;
    glViewport(0, 0, width, height);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width/height, 0.01f, 10000.0f);
}

static void closeWindow(){
    shader.deleteShader();
//    terrain.cleanUP();
    ship.cleanUP();
    skybox.cleanUP();
//    gui.cleanUP();
//    GUI::deleteShader();

    sun.cleanUp();
    mercury.cleanUp();
    venus.cleanUp();
    earth.cleanUp();
        moon.cleanUp();
    mars.cleanUp();
    jupiter.cleanUp();
    saturn.cleanUp();
    uranus.cleanUp();
    neptune.cleanUp();

    shadow.cleanUP();
    exit(0);
}

static void handleKey(){
    if(keyBuffer[27]){
        closeWindow();
        return;
    }

    bool accelerated = false;
    if(keyBuffer[' ']){
        spacetime.toggleLock();
    }
    if(keyBuffer['w']){
        ship.moveForward();
        accelerated = true;
    }
    if(keyBuffer['a']){
        ship.moveLeft();
        accelerated = true;
    }
    if(keyBuffer['s']){
        ship.moveBackward();
        accelerated = true;
    }
    if(keyBuffer['d']){
        ship.moveRight();
        accelerated = true;
    }
    if(accelerated){
        ship.setDirection(-camera.getDirection());
        spacetime.accelerate(ship.getAcceleration());
    }
}

static void handleMouse(){
    if(mouseBuffer[0]){
        ship.setDirection(-camera.getDirection());
    }
    else if(mouseBuffer[1]){
        spacetime.forceShipToStop();
    }
}

static void display(void){
    fpsManager.initiateTimer();
    fpsManager.updateCounter();

    handleKey();
    handleMouse();

    spacetime.update();
    ship.setPosition(spacetime.getPosition(position_scale));

    glm::dvec3 ship_position = spacetime.getPosition(1.0);

    camera.follow(ship);
    glm::mat4 view = camera.getViewMatrix();

    shader.bind();
    shader.addUniform3f("u_viewPos", cam);

    glm::mat4 viewProj = projectionMatrix*view;
    //earth.increaseRotation(0.1f);
    //earth.drawPlanet(shader, viewProj);

    shader.unbind();
    bloomPostProcessor.startSampling();
        renderer.clear();

        shader.bind();
        shader.addUniform1i("u_useLighting", 0);

        pos=spacetime.getMatter(0).getPosition();
        pos=pos.scale(position_scale);
        sun.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        sun.drawPlanet(shader, viewProj);
        ship.draw(shader, viewProj);
        shader.addUniform1i("u_useLighting", 1);

        pos=spacetime.getMatter(1).getPosition();
        pos=pos.scale(position_scale);
        mercury.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mercury.increaseRotation(360.0/72.0);
        mercury.drawPlanet(shader, viewProj);

        pos=spacetime.getMatter(2).getPosition();
        pos=pos.scale(position_scale);
        venus.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mercury.increaseRotation(360.0/72.0);
        venus.drawPlanet(shader, viewProj);

        pos=spacetime.getMatter(3).getPosition();
        pos=pos.scale(position_scale);
        earth.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //earth.increaseRotation(360.0/72.0);
        earth.drawPlanet(shader, viewProj);

            pos = pos + earthSystem.getMatter(1).getPosition().scale(moons_position_scale);
            moon.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
            //mars.increaseRotation(360.0/72.0);
            moon.drawPlanet(shader, viewProj);

        pos=spacetime.getMatter(4).getPosition();
        pos=pos.scale(position_scale);
        mars.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mars.increaseRotation(360.0/72.0);
        mars.drawPlanet(shader, viewProj);

        pos = spacetime.getMatter(5).getPosition();
        pos = pos.scale(position_scale);
        jupiter.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mars.increaseRotation(360.0/72.0);
        jupiter.drawPlanet(shader, viewProj);

        pos = spacetime.getMatter(6).getPosition();
        pos = pos.scale(position_scale);
        saturn.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mars.increaseRotation(360.0/72.0);
        saturn.drawPlanet(shader, viewProj);

        pos = spacetime.getMatter(7).getPosition();
        pos = pos.scale(position_scale);
        uranus.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mars.increaseRotation(360.0/72.0);
        uranus.drawPlanet(shader, viewProj);

        pos = spacetime.getMatter(8).getPosition();
        pos = pos.scale(position_scale);
        neptune.setPosition(glm::vec3(pos.getValue1(), pos.getValue2(), pos.getValue3()));
        //mars.increaseRotation(360.0/72.0);
        neptune.drawPlanet(shader, viewProj);


//        closeEarth.increaseRotation(0.5f);
//        closeEarth.drawPlanet(shader, viewProj);
//
//        skybox.increaseRotation();
//        skybox.render(view, projectionMatrix);
    bloomPostProcessor.stopSampling(screenWidth, screenHeight);

    bloomPostProcessor.drawProcessed(screenWidth, screenHeight);
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

static void key(unsigned char key, int x, int y){
    keyBuffer[key] = true;
    glutPostRedisplay();
}

static void keyUp(unsigned char key, int x, int y){
    keyBuffer[key] = false;
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

void mouseFunc(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
            if(state == GLUT_DOWN)
            {
                mouseBuffer[0] = true;
            }
            else if(state == GLUT_UP){
                mouseBuffer[0] = false;
            }
    }

    else if(button == GLUT_RIGHT_BUTTON){
            if(state == GLUT_DOWN)
            {
                mouseBuffer[1] = true;
            }
            else if(state == GLUT_UP){
                mouseBuffer[1] = false;
            }
    }

}

int main(int argc, char *argv[]){
    Window::createGlutWindow(argc, argv,
                        "Solar Sailer",//Title
                        glm::vec2(250, 75),//Position
                        glm::vec2(800, 600));//Size
    Window::initializeGLEW(false);//Start extension wrangle without showing errors
    Window::enableBackfaceCulling();
    Window::enableDepthTest();

    init();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutReshapeFunc(resize_display);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUp);
    glutMotionFunc(mouseMoved);
    glutMouseFunc(mouseFunc);
	glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(mouseMoved);

    resetPointer();
    glutMainLoop();

    return EXIT_SUCCESS;
}
