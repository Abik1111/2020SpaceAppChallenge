
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

Matter Mercury;
Matter Earth;
Matter Mars;
Matter Halley;
Spacetime spacetime;
int physicsMain(){

    Matter Sun;
    Sun.setMass(1.9e30);

    Mercury.setMass(3.3011e23);
    Mercury.setPosition(Vector3::getVector(69.8169e9,0,0));
    Mercury.setVelocity(Vector3::getVector(0,0,1).scale(47.362e3));

    Earth.setMass(5.97237e24);
    Earth.setPosition(Vector3::getVector(147.10e9,0,0));
    Earth.setVelocity(Vector3::getVector(0,0,1).scale(30.29e3));

    Mars.setMass(6.4171e23);
    Mars.setPosition(Vector3::getVector(227.939e9,0,0));
    Mars.setVelocity(Vector3::getVector(0,0,1).scale(24.077e3));

    Halley.setMass(2.2e14);
    Halley.setPosition(Vector3::getVector(0.57*AU,0,0));
    Halley.setVelocity(Vector3::getVector(0,0,1).scale(54e3));

    spacetime.addMatter(0,Sun);

    return 0;
}

bool start=true;
double frame=0;
double scale=3e-11;

static void display(void)
{
    frame++;
    Vector3 pos;
    spacetime.update();
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    gluLookAt(0,18,0,0,0,0,0,0,-1);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(frame>=0){
        if(start){
            spacetime.addMatter(1,Mercury);
            spacetime.addMatter(3,Earth);
            spacetime.addMatter(4,Mars);
            spacetime.addMatter(10,Halley);
            start=false;
        }
        glColor3d(1,0.5,0);
        pos=spacetime.getMatter(1).getPosition();
        pos=pos.scale(scale);
        glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.3,18,18);
        glPopMatrix();

        glColor3d(0,1,0);
        pos=spacetime.getMatter(3).getPosition();
        pos=pos.scale(scale);
        glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.3,18,18);
        glPopMatrix();

        glColor3d(1,0,0);
        pos=spacetime.getMatter(4).getPosition();
        pos=pos.scale(scale);
        glPushMatrix();
            glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
            glutSolidSphere(0.3,18,18);
        glPopMatrix();

        glColor3d(0,0,0);
        pos=spacetime.getMatter(10).getPosition();
        pos=pos.scale(scale);
        glPushMatrix();
            glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
            glutSolidSphere(0.3,18,18);
        glPopMatrix();
    }

    glColor3d(1,1,0);
    pos=spacetime.getMatter(0).getPosition();
    pos=pos.scale(scale);
    glPushMatrix();
        glTranslated(pos.getValue1(),pos.getValue2(),pos.getValue3());
        glutSolidSphere(0.6,18,18);
    glPopMatrix();

    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3d(0,1,1);
        glVertex3d(-100,0,0);
        glVertex3d(100,0,0);
    glPopMatrix();
    glEnd();


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
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 45.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            double temp = 60*60*24*365;
            cout<<spacetime.getMatter(3).getTime()/temp<<endl;
            cout<<spacetime.getMatter(4).getTime()/temp<<endl;
            cout<<spacetime.getMatter(10).getTime()/temp<<endl;
            exit(0);
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
