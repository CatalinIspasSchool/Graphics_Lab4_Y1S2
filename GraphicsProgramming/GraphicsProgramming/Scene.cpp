#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	

	// Initialise scene variables
	
}

void Scene::handleInput(float dt)
{
	// Handle user input
}

void Scene::update(float dt)
{
	// update scene related variables.

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.0f, -10.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	//lighting
	//placeDiffuseLight();

	placePointLight();

	// Render geometry/scene here -------------------------------------
	

	drawSpheres();

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	//lighting initialization
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void Scene::useMaterialPlainRed()
{
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_ambient_colour[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diff_red[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diff_blue[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_shininess = 0.0;
	GLfloat low_shininess = 50;
	GLfloat high_shininess = 100;
	GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diff_red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMateriali(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

}

void Scene::useMaterialMildShineRed()
{
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_ambient_colour[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diff_red[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diff_blue[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat low_shininess = 16.f;
	GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diff_red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

}

void Scene::useMaterialHighShineRed()
{
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_ambient_colour[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diff_red[] = { 1.0, 0.0, 0.0, 1.0 }; //
	GLfloat mat_diff_yellow[] = { 0.75, 0.75, 0.1, 1.0 }; //
	GLfloat mat_diff_blue[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 0.75, 0.75, 1, 1.0 };//
	GLfloat high_shininess = 16.f;//
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diff_yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff_yellow);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

}

void Scene::placeDiffuseLight()
{
	glPushMatrix();	//StartCube
		glTranslatef(0, 0, 2);
		glRotatef(time / 50, 1, 0, 0);
		glTranslatef(0, 0, 4.5);

		GLfloat Light_Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		GLfloat Light_Diffuse[] = { 1.f, 1.f, 1.f, 0.0f };
		GLfloat Light_DiffuseDirection[] = { 0.f, 1.f, 0.f, 0.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, Light_DiffuseDirection);
		glEnable(GL_LIGHT0);

		glutWireSphere(0.3, 10, 7);

	glPopMatrix(); //EndCube

}

void Scene::placePointLight()
{
	glPushMatrix();	//StartCube
		glTranslatef(0, 0, 2);
		glRotatef(time / 50, 0, 0, 1);
		glTranslatef(0, 2.5, 0);

		GLfloat Light_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat Light_Point[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat Light_PointLocation[] = { 0.0f, 0.f, 0.0f, 1.0f };
		//GLfloat Spot_Direction[] = {0.0f, -1.0f, 0.0f};

		glLightfv(GL_LIGHT1, GL_AMBIENT, Light_Ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Point);
		glLightfv(GL_LIGHT1, GL_SPECULAR, Light_Point);
		glLightfv(GL_LIGHT1, GL_POSITION, Light_PointLocation);

		//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Spot_Direction);
		//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
		//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 50.0f);


		//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.7);
		//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
		//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);
		//glEnable(GL_LIGHT0);

		glEnable(GL_LIGHT1);
		//glDisable(GL_LIGHT0);

		glutWireSphere(0.3, 10, 7);

	glPopMatrix(); //EndCube

}


void Scene::drawSpheres()
{
	glPushMatrix();
		useMaterialPlainRed();
		glTranslatef(-3.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 0.6, 40, 40);
	glPopMatrix();
	glPushMatrix();
		useMaterialMildShineRed();
		glTranslatef(0.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 0.6, 40, 40);
	glPopMatrix();
	glPushMatrix();
		useMaterialHighShineRed();
		glTranslatef(3.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 0.6, 40, 40);
	glPopMatrix();

}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
