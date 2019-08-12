#include <string>
#include <cmath>
#include <gl/glut.h>    //include glut library (automatically includes gl and glu libraries)
int time;
#define MY_PI 		(3.14159265359)    //declare PI value
#define DEG2RAD(a) 	(MY_PI/180*(a))    //convert degrees into radians
#define TWO_PI	(2*MY_PI)
using namespace std;
///Globals///
GLint windowsID[2];
GLint dListID[16];
bool isDancing = false;
bool menuIsOn = false;
GLint shadeMode = GL_SMOOTH;
GLint polyMode = GL_FILL;
GLint swingSpd = 1;
GLfloat swingRot = 0.0f;
GLint discoSpd = 1;
GLfloat danceSpd = 0.1f;
//Camera globals//
GLfloat cAngle[] = {0.0f, 0.0f, 0.0f};
GLfloat cPos[] = {0.0f, 5.0f, 30.0f};
GLfloat cAim[] = {0.0f, 0.0f, 0.0f};
GLfloat cUp[] = {0.0f, 1.0f, 0.0f};
//Material Globals//
GLfloat no_mat[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};
//Light Globals//
GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat red_light[] = {1.0, 0.0, 0.0, 1.0}; 
GLfloat green_light[] = {0.0, 1.0, 0.0, 1.0};
GLfloat blue_light[] = {0.0, 0.0, 1.0, 1.0};  
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat white_spec[] = {.4, .4, .4, 1.0};
GLfloat white_diff[] = {.8, .8, .8, 1.0};
GLfloat disco_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] = {-25.0, 13.0, 0.0, 0.0}; 
GLfloat right_light_position[] = {25.0, 13.0, 0.0, 0.0};
GLfloat swing_light_position[] = {0.0, 15.0, 0.0, 1.0};
GLfloat rand_light_position[] = {0.0, 15.0, 0.0, 0.0};
GLfloat greenEmission[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat redEmission[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat blueEmission[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat whiteEmission[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat rot[3][3] = {0.0};
float spotDir[] = {0.0, -1.0, 0.0};
GLfloat swing[] = {20.0, 0.0, 40.0},
trans[3][3] = {{-20.0, 15.0, -20.0},
               {20.0, 15.0, -20.0},
               {0.0, 15.0, 20.0}},

arcIncr[] = {TWO_PI / 70.0, 0.0, TWO_PI / 140.0},
arc[3][3] = {0.0};
GLenum discoLightID[] = {GL_LIGHT3, GL_LIGHT4, GL_LIGHT5};
//GLfloat discoLights
///Prototypes///
void init();
void renderScene();
void resize(int, int);   //window width and height
void setSpKey(int, int, int);
void setKey(unsigned char, int, int);
void drawCube(GLfloat, GLfloat, GLfloat);
void drawRoom(GLfloat, GLfloat, GLfloat);
void drawTable();
void aimSpotLight(int);
void drawSpotLight();
void initDisco(int);
void drawDisco();
void printBmpStr(int, int, GLfloat, GLfloat, GLfloat, string, void *);
inline string getFPS();
void renderTxt();
void idle(){getFPS(); glutPostRedisplay();} 

int main(int argc, char** argv)
{
   //srand(time);
	glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);      //requests properties for the window (sets up the rendering context)
  glutInitWindowSize(500, 500);			//set window size in pixels
  glutInitWindowPosition(50, 50);       //set window position from top-left corner of display
  glutCreateWindow("LIGHTNING AND MOVING OBJECT BY ABHIYAN BHATTARAI AND BISHESH SHAKYA");		//set window title
  init();					            //call function to setup states 
  glutDisplayFunc(renderScene);			//register display callback function
  glutReshapeFunc(resize);		//register resize callback function
  glutKeyboardFunc(setKey);		    //register non-ASCII keypress callback function
  glutSpecialFunc(setSpKey);		//register non-ASCII keypress callback function
  glutIdleFunc(idle);
  glutMainLoop();                       //enter event loop
}

void init()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //sets clear background colour to black
   glEnable(GL_DEPTH_TEST);	//enable depth test
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   glShadeModel(shadeMode);	//set shading model
   glPolygonMode(GL_FRONT, polyMode);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    
   glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, green_light);
   
   glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
   glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, red_light);
   
   glLightfv(GL_LIGHT2, GL_POSITION, swing_light_position);
   glLightfv(GL_LIGHT2, GL_SPECULAR, white_spec);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, white_diff);
   glLightfv(GL_LIGHT2, GL_AMBIENT, no_mat);
   glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);    	// set cutoff angle
   glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 60.0);   	// set focusing strength
   glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
   glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0);
   glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0);
   
   for(int i=0; i < 3; i++)
   {
      if(i == 0)
      {
         disco_light[1] = disco_light[2] = 0.0f;
         disco_light[0] = 1.0f;
      }
      else if (i == 1)
      {
         disco_light[0] = disco_light[2] = 0.0f;
         disco_light[1] = 1.0f;
      }
      else if (i == 2)
       {
         disco_light[0] = disco_light[1] = 0.0f;
         disco_light[2] = 1.0f;
      }   

      glLightfv(discoLightID[i], GL_SPECULAR, disco_light);
      glLightfv(discoLightID[i], GL_DIFFUSE, disco_light);
      glLightfv(discoLightID[i], GL_POSITION, swing_light_position);
      glLightf(discoLightID[i], GL_SPOT_CUTOFF, 20.0);    	// set cutoff angle
      glLightf(discoLightID[i], GL_SPOT_EXPONENT, 90.0);   	// set focusing strength
      glLightf(discoLightID[i], GL_CONSTANT_ATTENUATION, 1.0f);
      glLightf(discoLightID[i], GL_LINEAR_ATTENUATION, 0);
      glLightf(discoLightID[i], GL_QUADRATIC_ATTENUATION, 0);
   }
   
   dListID[0] = glGenLists (16);
   
   for( i = 0; i < 16; i++)
      dListID[i] = dListID[0] + i;
      
   glNewList(dListID[9], GL_COMPILE);
      glColor3f(0.0f, 1.0f, 0.0f);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION, greenEmission);  //set material emission component
         glTranslatef(left_light_position[0], left_light_position[1], left_light_position[2]);
         glutSolidSphere(0.5f, 16, 16);
      glPopMatrix();
   glEndList();
   
   glNewList(dListID[10], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 0.0f);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION, redEmission);  //set material emission component
         glTranslatef(right_light_position[0], right_light_position[1], right_light_position[2]);
         glutSolidSphere(0.5f, 16, 16);
      glPopMatrix();
   glEndList();
   
   glNewList(dListID[11], GL_COMPILE);
    glPushMatrix();
         glTranslatef(0, -4.0f, 0);
         glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
         glutSolidCone(2.0f, 4.0f, 16, 8);
         glutSolidSphere(0.5f, 16, 16);
    glPopMatrix();    
   glEndList();
   
    glNewList(dListID[12], GL_COMPILE);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION, whiteEmission);  //set material emission component
         glCallList(dListID[11]);
      glPopMatrix();    
   glEndList();
   
   glNewList(dListID[13], GL_COMPILE);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION,  redEmission);  //set material emission component
         glScalef(0.3, 0.3, 0.3);
         glCallList(dListID[11]);
      glPopMatrix();    
   glEndList();
   
   glNewList(dListID[14], GL_COMPILE);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION, greenEmission);  //set material emission component
         glScalef(0.3, 0.3, 0.3);
         glCallList(dListID[11]);
      glPopMatrix();    
   glEndList();
   
   glNewList(dListID[15], GL_COMPILE);
      glPushMatrix();
         glMaterialfv(GL_FRONT, GL_EMISSION, blueEmission);  //set material emission component
         glScalef(0.3, 0.3, 0.3);
         glCallList(dListID[11]);
      glPopMatrix();    
   glEndList();
      
   glNewList(dListID[0], GL_COMPILE);
       glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
       glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
       glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
      glPushMatrix();
         glTranslatef(-25.0f, -15.0f, -25.0f);
         drawRoom(50.0f, 30.0f, 50.0f);
      glPopMatrix();
      
      glPushMatrix();
         glTranslatef(-10.0f, -6.0f, -10.0f);
         drawTable();
      glPopMatrix();
      
      glPushMatrix();
         glTranslatef(10.0f, -10.5f, -2.0f);
         glScalef(0.2, 0.5, 0.2);
         drawTable();
      glPopMatrix();
      
      glPushMatrix();
         glTranslatef(-15.0f, -10.5f, -2.0f);
         glScalef(0.2, 0.5, 0.2);
         drawTable();
      glPopMatrix();
   
   glEndList();
   
   //draw cone
   glNewList(dListID[1], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
         glTranslatef(-3.5f, -5.0f, 3.5f);
         glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
         glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
         glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidCone(2.0f, 4.0f, 16, 8);
      glPopMatrix();
   glEndList();
   
    //draw sphere
   glNewList(dListID[2], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
         glTranslatef(-3.5f, -4.0f, -3.5f);
         glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
         glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidSphere(1.0f, 16, 16); 
      glPopMatrix();
   glEndList();
   
   //draw torus
   glNewList(dListID[3], GL_COMPILE);
   glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
        glTranslatef(-3.5f, -4.5f, -3.5f);
        glRotatef(90.0f, 1.0f, .0f, 0.0f);
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glutSolidTorus(0.5f, 2.0f, 8, 16);
      glPopMatrix();
   glEndList();
   
   //draw teapot
   glNewList(dListID[4], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f); 
      glPushMatrix();
         glTranslatef(1.0f, -3.7f, 1.0f);
         glRotatef(135.0f, 0.0f, 1.0f, 0.0f);
          glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
          glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
          glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
          glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
          glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidTeapot(1.5f);
      glPopMatrix();
   glEndList();
   
   //draw dodeca
   glNewList(dListID[5], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f); 
      glPushMatrix();
         glTranslatef(3.5f, -3.5f, 4.5f);
         glRotatef(45.0f, 0.0f, 1.0f, 1.0f);
         glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
         glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidDodecahedron();
      glPopMatrix();
   glEndList();
   
   //draw Octa
   glNewList(dListID[6], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
         glTranslatef(5.0f, -4.4f, -5.5f);
         glRotatef(90.0f, 0.0f, 1.0f, 1.0f);
         glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
         glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidOctahedron();
      glPopMatrix();
   glEndList();
   
   //draw tetra
   glNewList(dListID[7], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
        glTranslatef(3.0f, -4.5f, -4.5f);
        glRotatef(90.0f, 0.0f, 1.0f, 1.0f);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glutSolidTetrahedron();
      glPopMatrix();
   glEndList();
   
   //draw icosa
   glNewList(dListID[8], GL_COMPILE);
      glColor3f(1.0f, 0.0f, 1.0f);
      glPushMatrix();
         glTranslatef(5.0f, -4.0f, 1.0f);
         glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
         glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
         glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
         glutSolidIcosahedron();
      glPopMatrix();
   glEndList();
}

void renderScene()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //clear the colour buffer to the colour previously set in glClearColor 
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); 
   glLoadIdentity();     //load modelview matrix with identity matrix  
   gluLookAt(cPos[0], cPos[1], cPos[2],   //set position
            cAim[0], cAim[1], cAim[2],         //set where eye/camera is looking at
	        cUp[0], cUp[1], cUp[2]);                    //set 'up' vector

   glPushMatrix();
      glRotatef(cAngle[0], 1.0f, 0.0f, 0.0f);
      glRotatef(cAngle[1], 0.0f, 1.0f, 0.0f);
      glRotatef(cAngle[2], 0.0f, 0.0f, 1.0f);
      
      for(int i = 0; i < 11; i++)
      {
         glPushMatrix();
         if(i >0 && i <= 8 && isDancing)     
            glTranslatef(0.0f, (rand()%4)*danceSpd, 0.0f);
         glCallList(dListID[i]);
         glPopMatrix();
      }
      if(glIsEnabled(GL_LIGHT2))
         drawSpotLight();
     if(glIsEnabled(discoLightID[0]))
         drawDisco();
   glPopMatrix();
   
   renderTxt();
  glFlush();
  glutSwapBuffers();     //swap front and back buffers
}

void setSpotLight(int val)
{
    static GLfloat arc = 0.0f;
      
    swingRot = 20.0f * sin(arc);
    arc +=  (TWO_PI / 140.0) * swingSpd;
    if (arc > TWO_PI)
      arc -= TWO_PI;
      
   glutPostRedisplay();  //refresh display
   if(glIsEnabled(GL_LIGHT2)) 
      glutTimerFunc(10, setSpotLight, 0);  
}


void drawSpotLight()
{
    glPushMatrix();
       glTranslatef(swing_light_position[0],swing_light_position[1], swing_light_position[2]);
       glRotatef(swingRot, 0, 0, 1);
       glLightfv(GL_LIGHT2, GL_POSITION, swing_light_position);
       glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
    glPopMatrix();
    
    glPushMatrix();
       glTranslatef(swing_light_position[0], swing_light_position[1], swing_light_position[2]);
       glRotatef(swingRot, 0, 0, 1);
       glCallList(dListID[12]);
    glPopMatrix();
}


void setDisco(int i)
{
     
  for (int k = 0; k < 3; ++k) 
  {
    rot[k][0] = swing[0] * sin(arc[k][0]);
    arc[k][0] += arcIncr[0] * discoSpd;
    if (arc[k][0] > TWO_PI)
      arc[k][0] -= TWO_PI;

    rot[k][1] = swing[1] * sin(arc[k][1]);
    arc[k][1] += arcIncr[1] * discoSpd;
    if (arc[k][1] > TWO_PI)
      arc[k][1] -= TWO_PI;

    rot[k][2] = swing[2] * sin(arc[k][2]);
    arc[k][2] += arcIncr[2] * discoSpd;
    if (arc[k][2] > TWO_PI)
      arc[k][2] -= TWO_PI;
  }  
  glutPostRedisplay();  //refresh display
  
  if(glIsEnabled(discoLightID[i]))
      glutTimerFunc(50, setDisco, 0);  
}

void drawDisco()
{
    for (int k = 0; k < 3; ++k)
    {
       glPushMatrix();
          glTranslatef(trans[k][0], trans[k][1], trans[k][2]);
          glRotatef(rot[k][0], 1, 0, 0);
          glRotatef(rot[k][1], 0, 1, 0);
          glRotatef(rot[k][2], 0, 0, 1);
          glLightfv(discoLightID[k], GL_POSITION, swing_light_position);
          glLightfv(discoLightID[k], GL_SPOT_DIRECTION, spotDir);
       glPopMatrix();
     
      
       glPushMatrix();
          glTranslatef(trans[k][0], trans[k][1], trans[k][2]);
          glRotatef(rot[k][0], 1, 0, 0);
          glRotatef(rot[k][1], 0, 1, 0);
          glRotatef(rot[k][2], 0, 0, 1);
          glColor4fv(disco_light);
          glCallList(dListID[13+k]);    
       glPopMatrix();
       
     }
}

void renderTxt()
{
     // switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, 500, 0, 500/2);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -500/2, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
	   printBmpStr(10, 10, 1.0f, 0.0f, 0.0f, "LIGHTNING AND MOVING OBJECT PROJECT", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 20, 1.0f, 0.0f, 0.0f, "BY", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 30, 1.0f, 0.0f, 0.0f, "SANJIP MEHTA (1ST16CS739) ", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 40, 1.0f, 0.0f, 0.0f, "RAHUL KUMAR YADAV (1ST16CS733) ", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 50, 1.0f, 0.0f, 0.0f, "UNDER THE GUIDANCE OF ", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 60, 1.0f, 0.0f, 0.0f, "MRS.USHA T", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 70, 1.0f, 0.0f, 0.0f, "MRS.BILWASHREE H", GLUT_BITMAP_8_BY_13);
	   printBmpStr(10, 90, 1.0f, 0.0f, 0.0f, "0 - toggle help menu on/off", GLUT_BITMAP_8_BY_13);
       if(menuIsOn)
       {
          printBmpStr(10, 100, 1.0f, 0.0f, 0.0f, "up, down, left, right - moves camera up, down, left, right.", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 110, 1.0f, 0.0f, 0.0f, "page up - zoom in, page down - zoom out", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 120, 1.0f, 0.0f, 0.0f, "home - reset view", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 130, 1.0f, 0.0f, 0.0f, "+[plus]- increase global ambient", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 140, 1.0f, 0.0f, 0.0f, "-[minus] - decrease global ambient", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 150, 1.0f, 0.0f, 0.0f, "1 and 2 - toggle left and right lights on/off", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 160, 1.0f, 0.0f, 0.0f, "3 - toggle swinging spotlight on/off", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 170, 1.0f, 0.0f, 0.0f, "4 - increase spotlight swing", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 180, 1.0f, 0.0f, 0.0f, "5 - toggle disco lights on/off", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 190, 1.0f, 0.0f, 0.0f, "6 - increase spotlight swing", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 200, 1.0f, 0.0f, 0.0f, "7 - toggle color tracking on/off", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 210, 1.0f, 0.0f, 0.0f, "8 - toggle smooth and flat shading", GLUT_BITMAP_8_BY_13);
	      printBmpStr(10, 220, 1.0f, 0.0f, 0.0f, "9 - toggle wireframe mode on/off", GLUT_BITMAP_8_BY_13);
          printBmpStr(10, 230, 1.0f, 0.0f, 0.0f,"a - exit program", GLUT_BITMAP_8_BY_13);
       }
	  
	glPopMatrix();
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}


void resize(int w, int h)   //window width and height
{
  if(h == 0)           //prevent divide by zero
    h = 1;
  
  glViewport(0, 0, w, h);  //set the viewport

  glMatrixMode(GL_PROJECTION);	 //select projection matrix
  glLoadIdentity();		         //clear current contents to identity matrix
  gluPerspective(60.0f, 	     //set up perspective projection with 60 degrees field of view
		(GLfloat) (w)/(GLfloat) h, //set aspect ratio
        1.0f, 		             //set near clipping plane to 1.0
		1000.0f);	             //set far clipping plane to 100.0
		
  glMatrixMode(GL_MODELVIEW);	 //select modelview matrix
  glLoadIdentity();		         //clear current contents to identity matrix

  gluLookAt(cPos[0], cPos[1], cPos[2],   //set position
            cAim[0], cAim[1], cAim[2],         //set where eye/camera is looking at
	        cUp[0], cUp[1], cUp[2]);                    //set 'up' vector       	        
}

void setKey(unsigned char key, int x, int y)
{
   switch(key)
   {
      case '0': menuIsOn = menuIsOn? false: true; 
      break;
      case '1': glIsEnabled(GL_LIGHT0)? glDisable(GL_LIGHT0): glEnable(GL_LIGHT0);
      break;
      case '2': glIsEnabled(GL_LIGHT1)? glDisable(GL_LIGHT1): glEnable(GL_LIGHT1);
      break;
      case '3': if(glIsEnabled(GL_LIGHT2)) 
                   glDisable(GL_LIGHT2);
                else 
                {
                   swingSpd = 1;  
                   glEnable(GL_LIGHT2);
                   glutTimerFunc(10, setSpotLight, 0);  
                }
      break;
      case '4': swingSpd = (swingSpd + 1) >= 10 ? 1 : (swingSpd + 1); 
      break;
      case '5': if(glIsEnabled(discoLightID[0]))
                    for(int i = 0; i < 3; i++)
                    {
                       glDisable(discoLightID[i]);
                       isDancing = false;
                    }
                else
                {
                   discoSpd = 1;
                   danceSpd = 0.1f; 
                   isDancing = true; 
                   for(int i = 0; i < 3; i++)
                   {
                       glEnable(discoLightID[i]);
                       glutTimerFunc(100, setDisco, 0);  
                   }
                }
      break;
      case '6': discoSpd = (discoSpd + 1) >= 10 ? 1 : (discoSpd + 1);
                danceSpd = (danceSpd + 0.1f) >= 1 ? 0.1f : (danceSpd + 0.1f); 
      break;
      case '7': glIsEnabled(GL_COLOR_MATERIAL)? glDisable(GL_COLOR_MATERIAL): glEnable(GL_COLOR_MATERIAL);
      break;
      case '8': glShadeModel(shadeMode = shadeMode == GL_SMOOTH? GL_FLAT: GL_SMOOTH);	//set shading model 
      break;
      case '9': glPolygonMode(GL_FRONT, polyMode = polyMode == GL_FILL? GL_LINE: GL_FILL);
      break;
      case '+': if(lmodel_ambient[0] < 1)  
                   lmodel_ambient[0] = lmodel_ambient[1] = lmodel_ambient[2] += 0.1f;
      break;
      case '-': if(lmodel_ambient[0] > 0) 
                   lmodel_ambient[0] = lmodel_ambient[1] = lmodel_ambient[2] -= 0.1f;  
      break;
	  case 'a' :exit(1);     //quit
      default:; 
   };
   glutPostRedisplay();  //refresh display
}

void setSpKey(int key, int x, int y)
{
  GLfloat cMovStep = 5;      //movement sensitivity
  GLfloat cRotStep = 5;      //rotational sensitivity   
  int upDwnAngle = ((cAngle[1] > 45 && cAngle[1] <= 135) || 
                   (cAngle[1] > 225 && cAngle[1] <= 315)) ? 2 : 0; 
  int lftRhtAngle = ((cAngle[0] > 45 && cAngle[0] <= 135) || 
                   (cAngle[0] > 225 && cAngle[0] <= 315)) ? 2 : 1; 
  int i;
  switch(key)
  {
     //if home pressed
    case GLUT_KEY_HOME: for( i= 0; i < 3; i++)
                           cAngle[2] = 0.0f;
                        cPos[2] = 30.0f;
    break;         
    //if page up pressed
    case GLUT_KEY_PAGE_UP: cPos[2] -= cMovStep;
    break;
    //if page down pressed
    case GLUT_KEY_PAGE_DOWN: cPos[2] += cMovStep;
    break;
    //if arrow up pressed
    case GLUT_KEY_UP:  cAngle[upDwnAngle]+= cRotStep; 
    break;
    //if arrow down pressed
    case GLUT_KEY_DOWN: cAngle[upDwnAngle]-= cRotStep; 
    break;
    //if left pressed
    case GLUT_KEY_LEFT: cAngle[lftRhtAngle]+= cRotStep; 
                        break;
    //if right pressed
    case GLUT_KEY_RIGHT: cAngle[lftRhtAngle]-= cRotStep;
    	                break;
    default:; 
  };
  
  for( i = 0; i < 3; i++)
   if(cAngle[i] >= 360) //wrap around if rotation angle greater than 360
    cAngle[i] -= 360.0f;
   else if(cAngle[i] < 0)
    cAngle[i] += 360.0f;
    
   glutPostRedisplay();  //refresh display
}

void drawRoom(GLfloat w, GLfloat h, GLfloat d)
{   
   glBegin(GL_QUADS);
      //ceilling
      glColor3f(0.9f, 0.9f, 0.9f);
      glNormal3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, h, d);
      glVertex3f(0.0f, h, 0.0f);
      glVertex3f(w, h, 0.0f);
      glVertex3f(w, h, d);	
      
      glColor3f(.8f, .8f, .6f);
      //front wall
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(0.0f, 0.0f, d);
      glVertex3f(0.0f, h, d);
      glVertex3f(w, h, d);	
      glVertex3f(w, 0.0f, d);
      
      //left wall
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(w, h, d);
      glVertex3f(w, h, 0.0f);
      glVertex3f(w, 0.0f, 0.0f);	
      glVertex3f(w, 0.0f, d);
     
      //back wall
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(w, h, 0.0f);
      glVertex3f(0.0f, h, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(w, 0.0f, 0.0f);
     
      //right wall
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, h, 0.0f);
      glVertex3f(0.0f, h, d);
      glVertex3f(0.0f, 0.0f, d);
      glVertex3f(0.0f, 0.0f, 0.0f);	
      
      //floor
      glColor3f(0.3f, 0.2f, 0.0f);
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, d);
      glVertex3f(w, 0.0f, d);
      glVertex3f(w, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      
   glEnd();
}

void drawCube(GLfloat w, GLfloat h, GLfloat d)
{   
   glBegin(GL_QUADS);
      //top face
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(w, h, d);	
      glVertex3f(w, h, 0.0f);
      glVertex3f(0.0f, h, 0.0f);
      glVertex3f(0.0f, h, d);

      //front face
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, h, d);
      glVertex3f(0.0f, 0.0f, d);	
      glVertex3f(w, 0.0f, d);
      glVertex3f(w, h, d);
   
      //left face
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(w, h, d);	
      glVertex3f(w, 0.0f, d);
      glVertex3f(w, 0.0f, 0.0f);
      glVertex3f(w, h, 0.0f);
    
      //back face
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(w, h, 0.0f);
      glVertex3f(w, 0.0f, 0.0f);	
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, h, 0.0f);
    
      //right face
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, h, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);	
      glVertex3f(0.0f, 0.0f, d);
      glVertex3f(0.0f, h, d);
    
      //bottom face
      glNormal3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(w, 0.0f, 0.0f);
      glVertex3f(w, 0.0f, d);
      glVertex3f(0.0f, 0.0f, d);	
   glEnd();
}

void drawTable()
{
      glColor3f(1.0f, 1.0f, 1.0f);
      glPushMatrix();
          drawCube(20, 1, 20);
      glPopMatrix();
      
      glColor3f(.2f, .2f, .2f);
      glPushMatrix();
          glTranslatef(0.0f, -9.0f, 0.0f);
          drawCube(1.0f, 9.0f, 1.0f);
      glPopMatrix();
      
      glPushMatrix();
          glTranslatef(0.0f, -9.0f, 19.0f);
          drawCube(1.0f, 9.0f, 1.0f);
      glPopMatrix();
      
      glPushMatrix();
          glTranslatef(19.0f, -9.0f, 0.0f);
          drawCube(1.0f, 9.0f, 1.0f);
      glPopMatrix();
      
      glPushMatrix();
          glTranslatef(19.0f, -9.0f, 19.0f);
          drawCube(1.0f, 9.0f, 1.0f);
      glPopMatrix();   
}


void printBmpStr(int x, int y, GLfloat r, GLfloat g, GLfloat b, string txt, void *font)
{
  glColor3f(r, g, b);// set text color
  glRasterPos3f(x, y, 0);// ste position for text to be ratsrized
  for (int i = 0; i<txt.size(); i++) 
	 glutBitmapCharacter(font, txt[i]);  
}


inline string getFPS()
{
   static int frame = 0, time = 0, timebase = 0;
   static char s[30];
   frame++;
   time = glutGet(GLUT_ELAPSED_TIME);
   if (time - timebase > 1000) 
   {
      sprintf(s,"FPS: %4.2f",frame*1000.0/(time-timebase));
      timebase = time;		
      frame = 0;
   }
   
   return string(s);   
}
