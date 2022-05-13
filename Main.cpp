//==============================================
// Name        : Faraz Majid ( L20 - 1162 )
// Description : BS(CS) - 1D Semester 1 Project
//==============================================

/*
**************************************************
-> All Previous Comments are removed
-> The given comments are explanations of my code
**************************************************
*/

#ifndef BSLAYER_CPP_
#define BSLAYER_CPP_
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
using namespace std;


#define MAX(A,B) ((A) > (B) ? (A):(B))
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))

#define FPS 80

#define KEY_ESC 27

// I am not using default colors

/*
float colors[6][3] = { { 1 / 255.0, 164 / 255.0, 164 / 255.0 }, { 215 / 255.0, 0
		/ 255.0, 96 / 255.0 }, { 247 / 255.0, 159 / 255.0, 95 / 255.0 }, { 137
		/ 255.0, 130 / 255.0, 251 / 255.0 }, { 50 / 255.0, 116 / 255.0, 44
		/ 255.0 },};
*/

// My Custom Color Palette
float mycolors[4][3] = {
{ 247 / 255.0, 159 / 255.0, 95 / 255.0 }, // Orange = 0
{ 112 / 255.0, 112 / 255.0, 219 / 255.0 }, // Lilac = 1
{ 0 / 255.0 , 138 / 255.0 , 255 / 255.0 }, // Blue = 2

};
// Easier color coding :)
const int orange = 0;
const int lilac = 1;
const int blue = 2;

// Creating required Objects with certin properties
struct board
{
	int x = 800/2;
	const int y = 0;
	const int width = 150;
	const int height = 8;
};
board Board;

struct Sphere
{
	int x = 800/2;
	int y = 50;
	int radius = 12;
	int velocity_y = 2;
	int velocity_x = 2;
};
Sphere Ball;

const int no_bricks = 168; // Number of bricks to be printed
struct Brick
{
	int x , y;
	int color;
	bool visible = true;
};
Brick Bricks[no_bricks];
const int Brick_height = 20;
const int Brick_width = 40;
// Setting space b/w Bricks
const int Brick_space_x = 50;
const int Brick_space_y = 40;

float Deg2rad(float degree)
{
	return (degree / 180.0) * M_PI;
}

void InitRandomizer()
{
	srand((unsigned int) time(0));
}

long GetRandInRange(const long &rmin, const long &rmax)
{
	long range = rmax - rmin;
	long value = (rand() % (long) range) + rmin;

	return value;
}

/*
************************************************
This Fuction is not used by me.
My functions are adaptations of DrawTriangle().
************************************************
*/

/*
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,float color[])
{
	glColor3fv(color); // Set the triangle colour
	// ask library to draw triangle at given position...

	glBegin(GL_TRIANGLES);
	//Draw triangle using given three vertices
	glVertex4i(x1, y1, 0, 1);
	glVertex4i(x2, y2, 0, 1);
	glVertex4i(x3, y3, 0, 1);
	glEnd();
}
*/


/*
*************************************************************************
My function only requies the bottom left cordinate of the rectangle and
it's desired width and height the rest of the required cordinates are
calculated by the function
*************************************************************************
*/

void DrawRectangle(int x1, int y1, int block_width, int block_height,float color[])
{
	glColor3fv(color);

	//Calculating required coordinates
  int x2 = x1 + block_width;
	int y2 = y1 + block_height;

 	glBegin(GL_TRIANGLES);

 	/*Drawing lower half of rectangle*/
	glVertex4i(x1, y1, 0, 1);
	glVertex4i(x1, y2, 0, 1);
 	glVertex4i(x2, y1, 0, 1);

  /*Drawing upper half of rectangle*/
  glBegin(GL_TRIANGLES);
  glVertex4i(x2, y2, 0, 1);
  glVertex4i(x1, y2, 0, 1);
  glVertex4i(x2, y1, 0, 1);

 	glEnd();

}


/*
********************************************
Drawing triangles Repeatedly while rotating
them about a fixed point to make a circle.
-> ox is x-cordinate of origin of the circle
-> oy is y-cordinate of origin of the circle
********************************************
*/
void DrawSphere( int ox, int oy, int radius ,float color[] )
{

 float angle, angle1;
 int x1 , y1 , x2 , y2;

 glColor3fv(color);

 for ( float i = 0.0 ; i <= 360.0 ; i+= 0.1 )
 {
   angle = Deg2rad(i);
   angle1 = Deg2rad(i+0.1);
   x1 = ( radius * cos (angle) + ox );
   y1 = ( radius * sin (angle) + oy );
   x2 = ( radius * cos (angle1) + ox );
   y2 = ( radius * sin (angle1) + oy );

   glBegin(GL_TRIANGLES);
   glVertex4i(ox, oy, 0, 1);
   glVertex4i(x1, y1, 0, 1);
   glVertex4i(x2, y2, 0, 1);

	 glEnd();
 }


}

/*
******************************
Animation & Collision of ball
******************************
*/
void moveball()
{

 if ( Ball.x > 800 ) // Ball rebound from right boundary
 {
 	Ball.velocity_x *= -1; // Changes direction of ball
 }
 else if ( Ball.x < 0) // Ball rebound from left boundary
 {
 	Ball.velocity_x *= -1; // Changes direction of ball
 }
 else if ( Ball.y > 600) // Ball rebound from top boundary
 {
 	Ball.velocity_y *= -1; // Changes direction of ball
 }

// This moves the ball according to the assigned velocity
 Ball.x += Ball.velocity_x;
 Ball.y += Ball.velocity_y;

 DrawSphere( Ball.x , Ball.y , Ball.radius , mycolors[blue] );
}

/*
*******************************************************************
brickCollision() rebounds ball for all visible bricks by checking
if the ball cordinates lie in the range of brick cordinates
*******************************************************************
*/
void brickCollision()
{
	for ( int i = 0 ; i < no_bricks ; i++)
	{
		if (Bricks[i].visible == true)
		{
			 if ( Ball.y - Bricks[i].y >= (-1*Ball.radius)  &&  Ball.y - Bricks[i].y <= Brick_width + Ball.radius  )
			 {
			  	if (Ball.x - Bricks[i].x >= (-1*Ball.radius)  &&  Ball.x - Bricks[i].x <= Brick_height + Ball.radius )
			 	 {
					Ball.velocity_y *= -1; // Inverting sign of velocity to rebound ball in opposite direction
					Bricks[i].visible = false;
			   }

			 }
		}
	}

}

/*
****************************************************
boardCollision() rebounds ball by checking if the
ball cordinates lie in the range of board cordinates
****************************************************
*/
void boardCollision()
{
  if ( ( Ball.x >= Board.x && Ball.x <= (Board.x + Board.width) ) && ( Ball.y >= 0 && Ball.y <= Board.height ) )
	{
  	Ball.velocity_x *= -1; // Inverting sign of velocity to rebound ball in opposite direction
		Ball.velocity_y *= -1; // Inverting sign of velocity to rebound ball in opposite direction
	}
	else if ( Ball.y <= 0)
	{
		exit(1); // exit the game if board misses the ball
		cout << "GAME OVER";
	}
}


void Display()
{
	//           Red  , Green , Blue , Alpha
	glClearColor(0.15 , 0.15 , 0.15 , 0 );
	// I have set a Dark Grey background
	glClear(GL_COLOR_BUFFER_BIT);



/*
***************************************************************
-> Drawing Bricks repeatedly by using a loop.
-> Recording each bricks cordinates in Bricks[].x & Bricks[].y
-> Drawing Brick only if it is visible
-> Resetting x to 60 after Printing each row
***************************************************************
*/

int counter = 0;
// Initial cordinates of Brick
int x = 60;
int y = 80;
// Printing Rowwise
while ( counter < no_bricks )
{

  if ( Bricks[counter].visible == true)
	{
  	DrawRectangle( x , y, Brick_width, Brick_height, mycolors[orange]);
  }

	Bricks[counter].x = x;
	Bricks[counter].y = y;
	x += Brick_space_x; // Calculating next bricks x-cordinate

	if ( x >= (800 - 60) )
	{
		x = 60;
		y += Brick_space_y; // Calculating next bricks ***rows's y-cordinate
	}

	counter++;

}


// Drawing Board

// DrawRectangle(int x1, int y1, int block_width, int block_height,float color[])
 DrawRectangle( Board.x , Board.y , Board.width , Board.height , mycolors[lilac] );

// Drawing Ball

//DrawSphere( int ox, int oy, int radius ,float color[] )
 DrawSphere( Ball.x , Ball.y , Ball.radius , mycolors[blue] );

//Functions controlling Physics of the game
 moveball();
 boardCollision();
 brickCollision();

glutSwapBuffers();

}

void SetCanvasSize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
}

/*
*******************************************
Redrwaing the board according to user input
*******************************************
*/
void NonPrintableKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
			Board.x -= 50; // redraw 50px to left
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		Board.x += 50; // redraw 50px to right
 	 glutPostRedisplay();
	}

}

void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == KEY_ESC)
	{
		exit(1);
	}
}

void Timer(int m)
{

/*
****************************************************
Repeatedly calling Display() function for animation
****************************************************
*/
   Display();

	glutTimerFunc(1000.0 / FPS, Timer, 0);

}

int main(int argc, char*argv[])
{

// I have set my window size to be 800 x 600
	int width = 800, height = 600;
	InitRandomizer();
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height);
	glutCreateWindow("Faraz Majid L20-1162 : Project Brick Slayer");
	SetCanvasSize(width, height);

	glutDisplayFunc(Display);
	glutSpecialFunc(NonPrintableKeys);
	glutKeyboardFunc(PrintableKeys);

	glutTimerFunc(1000.0 / FPS, Timer, 0);

	glutMainLoop();
	return 1;

}
#endif

//=============
// Faraz Majid
// L20-1162
// BS(CS) 1D
//=============
