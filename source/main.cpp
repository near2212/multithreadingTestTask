
/*
	DO NOT MODIFY THIS FILE.
	Use test.cpp instead.
*/

#include <stdio.h>
#include <windows.h>

#include "GL/glut.h"
#include "nvToolsExt.h"

#include "test.h"

static int currentWindow;

void platform::drawPoint(float x, float y, float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glVertex2f(x, y);
}

static void display(void)
{
	//Specifies how the red, green, blue, and alpha source-blending factors are computed
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//If enabled, blend the incoming RGBA color values with the values in the color buffers
	glEnable(GL_BLEND);
	//The glClearColor function specifies clear values for the color buffers.
	glClearColor(0.0,0.0,0.0,0.0); 
	//The glClear function clears buffers to preset values. The buffers currently enabled for color writing.
	glClear(GL_COLOR_BUFFER_BIT);
	//Starts a nested thread range.The event message associated to this range event.
	nvtxRangePush("test::render()");
	//	Treats each vertex as a single point. Vertex n defines point n. N points are drawn.
	glBegin(GL_POINTS);
	//рендеринг все этого великолепия должна я написать сама видимо
	test::render();
	//The glBegin and glend functions delimit the vertices of a primitive or a group of like primitives.
	glEnd();
	//Ends a nested thread range.
	nvtxRangePop();
	//glutSwapBuffers swaps the buffers of the current window if double buffered. - какая то фигня
	glutSwapBuffers();
};

static void idle(void)
{
	//Starts a nested thread range.The event message associated to this range event.
	nvtxRangePush(__FUNCTION__);
	//Number of milliseconds since glutInit called (or first call to glutGet(GLUT_ELAPSED_TIME)).
	//glutInit вызывается в мейне
	int time = glutGet(GLUT_ELAPSED_TIME);
	static int lastTime = 0;
	int delta = time - lastTime;
	lastTime = time;

	// check memory
	{
		static int nextStats = -1;
		if (nextStats < time)
		{
			MEMORYSTATUSEX memStatus;
			//Fills a block of memory with zeros.
			ZeroMemory(&memStatus, sizeof(MEMORYSTATUSEX));
			memStatus.dwLength = sizeof(MEMORYSTATUSEX);
			//Retrieves information about the system's current usage of both physical and virtual memory.
			GlobalMemoryStatusEx(&memStatus);
			int totalVirtual = (memStatus.ullTotalVirtual / 1024) / 1024;
			int availVirtual = (memStatus.ullAvailVirtual / 1024) / 1024;
			int usedVirtual = totalVirtual - availVirtual;
			
			printf("Virtual memory info. Total: %dMb, Free: %dMb, Used: %dMb\n", totalVirtual, availVirtual, usedVirtual);
			nextStats = time + 3000;
		}
	}

	{
		static int nextAnim = -1;
		static int animPos = 0;
		const char *animation = "-\\|/-\\|/";

		if (nextAnim < time)
		{
			printf("\b\b%c ", animation[animPos]);
			animPos = (animPos+1) % (8);
			nextAnim = time + 60;
		}
	}
	//Starts a nested thread range.The event message associated to this range event.
	nvtxRangePush("test::update()");

	test::update(delta);
	//Ends a nested thread range.
	nvtxRangePop();
	//Mark an indicated window as needing a redisplay.
	glutPostWindowRedisplay(currentWindow);
	//Ends a nested thread range.

	nvtxRangePop();
};

static void visible(int vis)
{
	//sets the global idle callback.
	//Passing NULL to glutIdleFunc disables the generation of the idle callback.

	glutIdleFunc((vis == GLUT_VISIBLE) ? idle : NULL);
};

static void mouse(int button, int state, int x, int y)
{

	if (state == GLUT_UP && button == 0)
	{
		//Starts a nested thread range.The event message associated to this range event.
		nvtxRangePush("test::on_click()");

		test::on_click(x, y);
		//Ends a nested thread range.3
		nvtxRangePop();
	}
};

static void initWindow(void)
{
	//glutDisplayFunc sets the display callback for the current window. 
	//When GLUT determines that the normal plane for the window needs to be redisplayed,
	//the display callback for the window is called.
	glutDisplayFunc(display);
	//sets the visibility callback for the current window.
	//The visibility callback for a window is called when the visibility of a window changes.
	//The state callback parameter is either GLUT_NOT_VISIBLE or GLUT_VISIBLE
	glutVisibilityFunc(visible);
	//glutMouseFunc sets the mouse callback for the current window.
	//When a user pressesand releases mouse buttons in the window, each pressand each release generates a mouse callback.
	glutMouseFunc(mouse);
	//glMatrixMode — specify which matrix is the current matrix
	//GL_PROJECTION Applies subsequent matrix operations to the projection matrix stack.
	glMatrixMode(GL_PROJECTION);
	//replaces the current matrix with the identity matrix.
	//It is semantically equivalent to calling glLoadMatrix with the identity matrix.
	glLoadIdentity();
	//multiply the current matrix with an orthographic matrix
	glOrtho(0, test::SCREEN_WIDTH, 0, test::SCREEN_HEIGHT, 0, 40);
	//GL_MODELVIEW Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW); 
	//specifies the rasterized diameter of points
	glPointSize(3.0);
	//returns the identifier of the current window.
	currentWindow = glutGetWindow();
};

int main(int argc, char **argv)
{
//is used to initialize the GLUT library.
	glutInit(&argc, argv);
	//sets the initial display mode.

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize((int)test::SCREEN_WIDTH, (int)test::SCREEN_HEIGHT);
	glutCreateWindow("particles");
	initWindow();

	test::init();
	//The atexit module defines functions to register and unregister cleanup functions

	atexit(test::term);
	//glutMainLoop enters the GLUT event processing loop.This routine should be called at most once in a GLUT program.
	//Once called, this routine will never return.It will call as necessary any callbacks that have been registered.
	glutMainLoop();

	return 0;
}
