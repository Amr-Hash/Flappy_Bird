#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <math.h>
#include <string> 

int last_time=0;
float vel = 0;
float pos = 240;
boolean up = false;
boolean first = false;
int h[10]={250,150,180,100,130,30,50,150,200,120};
int i=640;
int n = 0;
boolean iscoll = false;
int score = 0;

void type(GLdouble x , GLdouble y ,const std::string &string)
{
	glColor3f(1.0,1.0,0.0);
	glRasterPos2d(x,y);
	for(int i=0;i<string.size();i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
	}
}

void bird(int x,int y)
{
	//body
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x+10,y+10);
	glVertex2d(x+10,y-10);
	glVertex2d(x-10,y-10);
	glVertex2d(x-10,y+10);
	glEnd();
	//eye
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glVertex2d(x+2,y);
	glVertex2d(x+10,y);
	glVertex2d(x+10,y+10);
	glVertex2d(x+2,y+10);
	glEnd();
	//iris
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x+4,y+3);
	glVertex2d(x+7,y+3);
	glVertex2d(x+7,y+7);
	glVertex2d(x+4,y+7);
	glEnd();
	//mouth
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x-5,y-10);
	glVertex2d(x+10,y-10);
	glVertex2d(x+10,y-5);
	glVertex2d(x-5,y-5);
	glEnd();
	//beak
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2d(x+10,y);
	glVertex2d(x+20,y-10);
	glVertex2d(x+10,y-10);
	glEnd();
}

void mouse(int button , int state , int x , int y)
{
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !iscoll)
	{
		up = true;
		first = true;
	}
	else if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		last_time=0;
		vel = 0;
		pos = 240;
		up = false;
		first = false;
		i=640;
		n = 0;
		iscoll = false;
		score = 0;
	}
}


void coll(int pos , int i)
{
	if(i>260 && i<330)
	{
		if(!((pos>(h[n]+10) && pos<h[n]+190)))iscoll=true;
	}
	else if(i>260-197 && i<330-197)
	{
		if(n==9)
		{
			if(!((pos>(h[0]+10) && pos<h[0]+190)))iscoll=true;
		}
		else
		{
			if(!((pos>(h[n+1]+10) && pos<h[n+1]+190)))iscoll=true;
		}
	}
	if(pos<10 || pos>470)iscoll=true;
}

void init()
{
	glClearColor(0.0,0.74,1.0,1);
	glColor3f(0.0,0.0,0.0);
	glPointSize(1.5);
	glLineWidth(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,480.0);
}

void block(int h , int f)
{
	glColor3f(0.5,0.5,0.0);
	glBegin(GL_POLYGON);
	glVertex2d(f,0);
	glVertex2d(f+50,0);
	glVertex2d(f+50,h);
	glVertex2d(f,h);
	glEnd();	
	glBegin(GL_POLYGON);
	glVertex2d(f,h+200);
	glVertex2d(f+50,h+200);
	glVertex2d(f+50,480);
	glVertex2d(f,480);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int cur_time= glutGet(GLUT_ELAPSED_TIME);
	int delta = (cur_time - last_time);
	if(last_time>0)
	{
		if(!up)
		{
			vel = vel + (-0.005 * delta);
			pos = pos + ((vel * delta)/8);
			bird(320,pos);
		}
		else
		{
			if(first)
			{
				vel = 2.5;
			}
			else
			{
				vel = vel - (0.005 * delta);
			}
			pos = pos + ((vel * delta)/8);
			bird(320,pos);
			first = false;
			if(vel<0.2) up = false; 
		}
	}
	last_time = cur_time;
	if(!iscoll)i=i-1;
	if(i>443)
	{
		block(h[n],i);
	}
	else if(i>246)
	{
		block(h[n],i);
		block(h[n+1],i+197);
	}
	else if(i>50)
	{
		block(h[n],i);
		block(h[(n+1)%10],i+197);
		block(h[(n+2)%10],i+394);
	}
	else
	{
		block(h[n],i);
		block(h[(n+1)%10],i+197);
		block(h[(n+2)%10],i+394);
		block(h[(n+3)%10],i+591);
	}
	if(i==-50)
	{
		i=i+197;
		n=n+1;
		if(n==10)n=0;
	}
	std::string s = std::to_string(score);
	type(50,450,s);
	if(iscoll)
	{
		type(290,260,"Game over");
		type(240,220,"Press right click to reset");
	}
	glFlush();
	Sleep(4);
	coll(pos,i);
	if(i==260 || i==260-197)score=score+1;
}


void main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100,150);
	glutInitWindowSize(640,480);
	glutCreateWindow("Flappy Bird");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	init();
	glutMainLoop();
}