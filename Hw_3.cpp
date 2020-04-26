/*******************
STUDENT : Arda ERSOY
ID      :   21502589
SECTION :          1
HOMEWORK:          3
*******************/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  650
#define WINDOW_HEIGHT 650
#define TIMER_PERIOD   15 // Period for the timer.
#define TIMER_ON        1 // 0:disable timer, 1:enable timer
#define D2R     0.0174532
#define PI         3.1415

typedef struct {

	float x;
	float y;
	float distance;

} point_t;
point_t pos;

typedef struct {

	float x;
	float y;
	float angle;

} position_t;
position_t mouse;

typedef struct {

	float A;
	float t[3];

}angle_t;
angle_t angle;

typedef struct {

	bool status;
	float x;
	float y;
	float angle;

}bullet_t;
bullet_t bullet;

typedef struct {

	int speed;
	int size;
	int color;
	int start;
	bool status = true;
	point_t pos;

}ballon_t;
ballon_t ballon[3];

typedef struct {

	int counter;
	bool status;

}game_t;
game_t game;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  // to draw circle, center at (x,y)
						  // radius r	  
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);

	for (i = 0; i < len; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);

	glPopMatrix();
}

void initializations()
{
	srand(time(NULL));

	// to initialize bullet and mouse coordinates
	angle.A = 100;
	mouse.x = 0;
	mouse.y = 0;
	bullet.x = 0;
	bullet.y = 0;
	game.counter = 0;

	bullet.status = false;
	game.status = false;

	// to give each ballon random starting point, radius, color and speed
	for (int i = 0; i < 3; i++)
	{
		ballon[i].status = true;

		ballon[i].start = rand() % 360 + 1;
		angle.t[i] = ballon[i].start;

		ballon[i].size = rand() % 14 + 12;

		ballon[i].color = rand() % 255 + 1;

		ballon[i].speed = (rand() % (1 + 1 - -1)) + -1;
		while (ballon[i].speed == 0)
			ballon[i].speed = (rand() % (1 + 1 - -1)) + -1;
	}
}

void drawBackground()
{
	// to draw background which consists of lines
	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(-350, 0);
	glVertex2f(350, 0);
	if (game.status == false)
	{
		glVertex2f(0, 350);
		glVertex2f(0, -55);
		glVertex2f(-70, -55);
		glVertex2f(70, -55);
		glVertex2f(-70, -95);
		glVertex2f(70, -95);
		glVertex2f(-70, -55);
		glVertex2f(-70, -95);
		glVertex2f(70, -55);
		glVertex2f(70, -95);
		glVertex2f(0, -95);
		glVertex2f(0, -350);
	}
	else
	{
		glVertex2f(0, 350);
		glVertex2f(0, -350);
	}
	glEnd();
}

void drawCirle()
{
	// to draw 3 circles different each other
	float r = fabs(angle.A);

	glColor3f(0.3, 0.3, 1.0);
	circle_wire(0, 0, r + 70);
	circle_wire(0, 0, r + 120);
	circle_wire(0, 0, r + 170);
}

void drawBullet()
{
	// to draw the bullet for the user to hit ballons
	glColor3ub(255, 0, 0);
	if (bullet.status)
		circle(bullet.x, 0 + bullet.y, 10);
}

void drawHook()
{
	// to draw the hook which consists of triangles and polygon at the bottom 
	float ang = mouse.angle * D2R;

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-15 * cos(ang) - 0 * sin(ang), -15 * sin(ang) + 0 * cos(ang));
	glVertex2f(0 * cos(ang) - -15 * sin(ang), 0 * sin(ang) + -15 * cos(ang));
	glVertex2f(15 * cos(ang) - 0 * sin(ang), 15 * sin(ang) + 0 * cos(ang));
	glVertex2f(0 * cos(ang) - 15 * sin(ang), 0 * sin(ang) + 15 * cos(ang));
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(cos(ang) - 10 * sin(ang), sin(ang) + 10 * cos(ang));
	glVertex2f(20 * cos(ang) - -10 * sin(ang), 20 * sin(ang) + -10 * cos(ang));
	glVertex2f(40 * cos(ang) - -15 * sin(ang), 40 * sin(ang) + -15 * cos(ang));
	glVertex2f(0 * cos(ang) - -10 * sin(ang), 0 * sin(ang) + -10 * cos(ang));
	glVertex2f(20 * cos(ang) - 10 * sin(ang), 20 * sin(ang) + 10 * cos(ang));
	glVertex2f(40 * cos(ang) - 15 * sin(ang), 40 * sin(ang) + 15 * cos(ang));
	glEnd();
}

void drawBallon()
{
	// to draw 3 ballons with their initial values
	float r = fabs(angle.A);

	if (game.status == true)
	{
		if (ballon[0].status)
		{
			ballon[0].pos.x = (r + 70)*cos(angle.t[0] * D2R);
			ballon[0].pos.y = (r + 70)*sin(angle.t[0] * D2R);
			glColor3ub(ballon[0].color, ballon[1].color, ballon[2].color);
			circle(ballon[0].pos.x, ballon[0].pos.y, ballon[0].size);
			glColor3f(1, 1, 1);
			vprint((r + 70)*cos(angle.t[0] * D2R) - 12, (r + 70)*sin(angle.t[0] * D2R) - 5, GLUT_BITMAP_8_BY_13, "%0.f", angle.t[0]);
		}
		if (ballon[1].status)
		{
			ballon[1].pos.x = (r + 120)*cos(angle.t[1] * D2R);
			ballon[1].pos.y = (r + 120)*sin(angle.t[1] * D2R);
			glColor3ub(ballon[1].color, ballon[2].color, ballon[0].color);
			circle(ballon[1].pos.x, ballon[1].pos.y, ballon[1].size);
			glColor3f(1, 1, 1);
			vprint((r + 120)*cos(angle.t[1] * D2R) - 12, (r + 120)*sin(angle.t[1] * D2R) - 5, GLUT_BITMAP_8_BY_13, "%0.f", angle.t[1]);
		}
		if (ballon[2].status)
		{

			ballon[2].pos.x = (r + 170)*cos(angle.t[2] * D2R);
			ballon[2].pos.y = (r + 170)*sin(angle.t[2] * D2R);
			glColor3ub(ballon[2].color, ballon[0].color, ballon[1].color);
			circle(ballon[2].pos.x, ballon[2].pos.y, ballon[2].size);
			glColor3f(1, 1, 1);
			vprint((r + 170)*cos(angle.t[2] * D2R) - 12, (r + 170)*sin(angle.t[2] * D2R) - 5, GLUT_BITMAP_8_BY_13, "%0.f", angle.t[2]);
		}
	}
}

void displayAngle()
{
	// to display mouse's angle
	float r = fabs(angle.A);
	float ang = mouse.angle * D2R;

	mouse.angle = atan2(mouse.y, mouse.x) * 180 / PI;

	if (mouse.angle < 0)
		vprint((r + 190)*cos(ang) - 12, (r + 190)*sin(ang) - 5, GLUT_BITMAP_8_BY_13, "%0.f", mouse.angle + 360);
	else
		vprint((r + 190)*cos(ang) - 12, (r + 190)*sin(ang) - 5, GLUT_BITMAP_8_BY_13, "%0.f", mouse.angle);
}

void fire()
{
	// to increase bullet's coordinates to hit ballons
	bullet.x += 5 * cos(bullet.angle * D2R);
	bullet.y += 5 * sin(bullet.angle * D2R);
}

void bulletReset()
{
	// to reset bullet's values when a bullet exceeds game coordinates 
	game.counter = 1;
	bullet.x = 0;
	bullet.y = 0;
	bullet.status = false;
}

void GameReset()
{
	// to reset game, bullet and ballon values when 3 ballon are hitted
	game.counter = 0;
	bullet.x = 0;
	bullet.y = 0;

	bullet.status = false;
	game.status = false;
	ballon[0].status = true;
	ballon[1].status = true;
	ballon[2].status = true;
}

void checkFire()
{
	// to control whether a ballon is hitted or not
	for (int i = 0; i < 3; i++)
	{
		pos.distance = sqrt((bullet.x - ballon[i].pos.x) * (bullet.x - ballon[i].pos.x) + (bullet.y - ballon[i].pos.y) * (bullet.y - ballon[i].pos.y));

		if (pos.distance <= ballon[i].size + 7)
			ballon[i].status = false;
	}
}

// To display onto window using OpenGL commands
void display()
{
	// clear window to black
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	drawCirle();
	drawBallon();
	drawHook();
	displayAngle();
	
	if (game.status == true)
		drawBullet();

	// to fire a bullet to hit a ballon
	if (bullet.status == true)
	{
		fire();
		checkFire();

		if (bullet.x >= 350 || bullet.x <= -350 || bullet.y >= 350 || bullet.y <= -350)
			bulletReset();
	}
	else
	{
		bullet.angle = mouse.angle;
		bullet.x = bullet.y = 0;
	}

	// to show notes related to game
	glColor3ub(0, 255, 0);
	if (game.status == false)
		vprint(-55, -80, GLUT_BITMAP_HELVETICA_18, "Click to Start");
	vprint(-310, 300, GLUT_BITMAP_9_BY_15, "Homework #3");
	vprint(-310, 280, GLUT_BITMAP_9_BY_15, "by ARDA ERSOY");

	glutSwapBuffers();
}

// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key)
	{
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key)
	{
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// When a click occurs in the window,
// It provides which button
//buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	if (button == GLUT_LEFT_BUTTON)
	{
		game.counter += 1;
		// to determine mouse's coordinates to start the game
		if (mouse.x >= -70 && mouse.x <= 70 && mouse.y >= -95 && mouse.y <= -55)
			game.status = true;

		// to arrange second click to fire a bullet to hit a ballon via game.counter
		if (game.counter == 3 && button == GLUT_LEFT_BUTTON)
		{
			bullet.status = true;
			bullet.angle = mouse.angle;
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y)
{
	// Write your codes here.

	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y)
{
	// Write your codes here.

	// to specify mouse's coordinates
	mouse.x = x - winWidth / 2;
	mouse.y = winHeight / 2 - y;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	// to move ballons according to their random speed 
	for (int i = 0; i < 3; i++)
	{
		if (ballon[i].status)
		{
			angle.t[i] += ballon[i].speed;

			if (angle.t[i] > 360)
				angle.t[i] -= 360;

			if (angle.t[i] < 0)
				angle.t[i] += 360;
		}

		// to check if 3 of the ballon are hitted or not
		if (ballon[0].status == false && ballon[1].status == false && ballon[2].status == false)
			GameReset();

	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init()
{
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-3: \"Rotating Targets\" by Arda ERSOY");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// keyboard registration
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	// mouse registration
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

	initializations();

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
