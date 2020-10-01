
/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include "spacetime.h"

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

Spacetime spacetime;
int physicsMain(){

    Matter Sun;
    Sun.setMass(1.9e30);
	Sun.setTemperature(5780);
	Sun.setRadius(6.96340e8);
	Sun.setBlackBody(true);

    Matter Earth;
    Earth.setMass(6e24);
    Earth.setPosition(Vector3::getVector(149.598e9,0,0));
    Earth.setVelocity(Vector3::getVector(0,0,1).scale(29.78e3));
	Earth.setRadius(6.371e6);
	Earth.setEmissivity(0.7);

    //Matter Moon;
    //Moon.setMass(7.35e22);
    //Moon.setPosition(Vector3::getVector(149.597e9,0,0));
    //Moon.setVelocity(Vector3::getVector(0,0,1).scale(-1.022e3+24.077e3));

    Matter Mars;
    Mars.setMass(6.4171e23);
    Mars.setPosition(Vector3::getVector(227.939e9,0,0));
    Mars.setVelocity(Vector3::getVector(0,0,1).scale(24.077e3));
	Mars.setRadius(3.3895e6);
	Mars.setEmissivity(0.9);

	Matter Mercury;
	Mercury.setMass(3.285e23);
	Mercury.setPosition(Vector3::getVector(67.087e9, 0, 0));
	Mercury.setVelocity(Vector3::getVector(0, 0, 1).scale(47.362e3));
	Mercury.setRadius(2.437e6);
	Mercury.setEmissivity(0.932);

	Matter Venus;
	Venus.setMass(4.867e24);
	Venus.setPosition(Vector3::getVector(107e9,0,0));
	Venus.setVelocity(Vector3::getVector(0, 0, 1).scale(35.26e3));
	Venus.setRadius(6.0518e6);
	Venus.setEmissivity(0.3);

    spacetime.addMatter(0,Sun);
    spacetime.addMatter(3,Earth);
    //spacetime.addMatter(5,Moon);
    spacetime.addMatter(4,Mars);
	spacetime.addMatter(1, Mercury);
	spacetime.addMatter(2, Venus);


//    for(int i=0;i<30;i++){
//        spacetime.update();
//        cout<<spacetime.getMatter(4).getPosition()<<endl;
//    }


    return 0;
}

double scale=3e-11;

static void display(void)
{
    spacetime.update();
	cout << "temp of Earth::" << spacetime.getMatter(3).getTemperature() << endl;
	cout << "temp of Sun::" << spacetime.getMatter(0).getTemperature() << endl;
	cout << "temp of Mars::" << spacetime.getMatter(4).getTemperature() << endl;
	cout << "temp of Venus::" << spacetime.getMatter(2).getTemperature() << endl;
	cout << "temp of Mercury::" << spacetime.getMatter(1).getTemperature() << endl;

    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(0,15,0,0,0,0,0,0,-1);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vector3 pos;

    glColor3d(0,1,0);
    pos=spacetime.getMatter(3).getPosition();
    pos=pos.scale(scale);
    glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.3,100,100);
    glPopMatrix();

	/*
   glColor3d(0,0,1);
    pos=spacetime.getMatter(5).getPosition();
    pos=pos.scale(scale);
    glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.1,18,18);
    glPopMatrix();
	*/

    glColor3d(1,0,0);
    pos=spacetime.getMatter(4).getPosition();
    pos=pos.scale(scale);
    glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.2,18,18);
    glPopMatrix();

    glColor3d(1,1,0);
    pos=spacetime.getMatter(0).getPosition();
    pos=pos.scale(scale);
    glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.6,18,18);
    glPopMatrix();

	glColor3d(1, 0.5, 0);
	pos = spacetime.getMatter(1).getPosition();
	pos = pos.scale(scale);
	glPushMatrix();
	glTranslated(pos.getValue1(), pos.getValue2(), pos.getValue3());
	glutSolidSphere(0.15, 18, 18);
	glPopMatrix();

	glColor3d(0.5, 1, 0.75);
	pos = spacetime.getMatter(2).getPosition();
	pos = pos.scale(scale);
	glPushMatrix();
	glTranslated(pos.getValue1(), pos.getValue2(), pos.getValue3());
	glutSolidSphere(0.3, 18, 18);
	glPopMatrix();


    glutSwapBuffers();
}

static void init(){
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    physicsMain();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(720,640);
    glutInitWindowPosition(150,30);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}