#include "gfx3.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
int radius=100;
enum ORIENTATION {up,down,left,right};
void drawPacman(int,int,int);
void drawPacman(int x,int y,int orientation){
  static int pacLoop=1;
  int i;
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,radius/2-1);

  gfx_color(0,0,0);
  int r=radius/2-1;
  if(pacLoop%2){
    switch(orientation){
        case up:
                gfx_fill_triangle(x-r,y-r,x+r,y-r,orientation);
                break;
        case down:
                gfx_fill_triangle(x-r,y+r,x+r,y+r,orientation);
                break;
        case right:
                gfx_fill_triangle(x+r,y-r,x+r,y+r,orientation);
                break;
        case left:
                gfx_fill_triangle(x-r,y-r,x-r,y+r,orientation);
                break;
    }
  }
  pacLoop++;
}

int main(){

gfx_open(400,400,"Test");
gfx_color(255,255,0);
int x=100,y=100;
int r=radius/2-1,timer=000000;
  gfx_color(255,255,0);
  gfx_clear();
  gfx_wait();
  //drawBoard(xtl,ytl,boardHeight,boardWidth);
  printf("1\n");
  gfx_fill_circle(x,y,radius/2-1);

  printf("2\n");
  gfx_wait();
  usleep(timer);
  gfx_clear();
  //drawBoard(xtl,ytl,boardHeight,boardWidth);
  drawPacman(x,y,up);

  printf("3\n");
  gfx_wait();
  usleep(timer);
  gfx_clear();
  //drawBoard(xtl,ytl,boardHeight,boardWidth);
  gfx_color(255,255,0);
  drawPacman(x,y,0);
  gfx_color(0,0,0);
  gfx_fill_rectangle(x-radius/2-1,y-radius/2+1,radius,radius/2-1);

  printf("4\n");
  gfx_wait();
  usleep(timer); 
  gfx_clear();
  //drawBoard(xtl,ytl,boardHeight,boardWidth);
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,radius/2-1);
  gfx_color(0,0,0);
  gfx_fill_triangle(x-r,y-r,x+r,y-r,up);
  gfx_fill_triangle(x+r,y-r,x+r,y+r,right);
  gfx_fill_triangle(x-r,y-r,x-r,y+r,left);
        
  printf("5\n");
  gfx_wait();
  usleep(timer);
  gfx_clear();
  //drawBoard(xtl,ytl,boardHeight,boardWidth);
  
  printf("6\n");
  gfx_wait();
  usleep(timer); 
  gfx_clear();
  gfx_color(255,255,0);
  float angle=2*M_PI/5;           
  int i,xnew[5],ynew[5];
  for(i=1;i<=5;i++){
    xnew[i-1]=x+r*cos(i*angle+3*M_PI/2);
    ynew[i-1]=y+r*sin(i*angle+3*M_PI/2);
    gfx_line(x,y,xnew[i-1],ynew[i-1]);
  }
  
  printf("7\n");
  gfx_wait();
  usleep(timer); 
  gfx_clear();
  gfx_color(255,255,0);
  angle=2*M_PI/5;                
  for(i=1;i<=5;i++){
    xnew[i-1]=x+r*cos(i*angle+3*M_PI/2);
    ynew[i-1]=y+r*sin(i*angle+3*M_PI/2);
    gfx_line((x+xnew[i-1])/2,(y+ynew[i-1])/2,xnew[i-1],ynew[i-1]);
  }
gfx_wait();

//gfx_fill_triangle(3,300,250,79,20,20);
//gfx_wait();//pacman
/*gfx_fill_circle(x,y,radius/2);
gfx_color(0,0,0);
switch(orientation){
	case up:
		gfx_fill_triangle(x-radius/2,y-radius/2,x+radius/2,y-radius/2,orientation);
		break;
	case down:
		gfx_fill_triangle(x-radius/2,y+radius/2,x+radius/2,y+radius/2,orientation);
		break;
	case right:
		gfx_fill_triangle(x+radius/2,y-radius/2,x+radius/2,y+radius/2,orientation);
		break;
	case left:
		gfx_fill_triangle(x-radius/2,y-radius/2,x-radius/2,y+radius/2,orientation);
		break;
}*
int orientation=up;

int P=radius/10;
int i,r=radius/2-P;
int ghostLoop=0;
x=300;y=300;
while(1){
gfx_color(255,255,255);//confining box
gfx_rectangle(x-radius/2,y-radius/2,radius,radius);

gfx_color(255,0,0);
gfx_fill_circle(x,y-P,r);
gfx_fill_rectangle(x-r,y-P,2*r,r);
for(i=0;i<=r;i++){//left leg
  gfx_line(x-r+i,y-P,	x-r+i,	y+r-i);
}
for(i=1;i<=r;i++){//right leg
  gfx_line(x+i,	y-P,	x+i,	y+i);
}


if(ghostLoop%2){
  printf("YES\n");
  for(i=0;i<r;i++){//right middle leg
    gfx_line(x+i, y-P,    x+i,    y+r-i);
  }
  for(i=0;i<=r;i++){//left middle leg
    gfx_line(x-r+i, y-P,    x-r+i,    y+i);
  }
} else {
  for(i=2*P;i<=r;i++){//left middle leg
    gfx_line(x-i, y-P,    x-i,    y+r+2*P-i);
  }
  for(i=2*P;i<=r;i++){//right leg
    gfx_line(x+i,   y-P,    x+i,    y+r+2*P-i);
  }
} 

*add the eyes based on orientation *
gfx_color(255,255,255);
int eye=r/4;
int pupil=r/6;
switch(orientation){
	case up:
		gfx_fill_circle(x-4*r/10,y-2*P,r/3);
		gfx_fill_circle(x-4*r/10,y-3*P/2,r/3);
		gfx_fill_circle(x+4*r/10,y-2*P,r/3);
		gfx_fill_circle(x+4*r/10,y-3*P/2,r/3);
		gfx_color(0,0,150);
		gfx_fill_circle(x-4*r/10,y-3*P/4,r/6);
		gfx_fill_circle(x+4*r/10,y-3*P/4,r/6);
		break;
	case down:
		gfx_fill_circle(x-4*r/10,y-2*P,r/3);
		gfx_fill_circle(x-4*r/10,y-3*P/2,r/3);
		gfx_fill_circle(x+4*r/10,y-2*P,r/3);
		gfx_fill_circle(x+4*r/10,y-3*P/2,r/3);
		gfx_color(0,0,150);
		gfx_fill_circle(x-4*r/10,y-11*P/4,r/6);
		gfx_fill_circle(x+4*r/10,y-11*P/4,r/6);
		break;
	case left:
		gfx_fill_circle(x-4*r/10,y-2*P,r/3);
		gfx_fill_circle(x-4*r/10,y-3*P/2,r/3);
		gfx_fill_circle(x+4*r/10,y-2*P,r/3);
		gfx_fill_circle(x+4*r/10,y-3*P/2,r/3);
		gfx_color(0,0,150);
		gfx_fill_circle(x-4*r/10-3*P/4,y-7*P/4,r/6);
		gfx_fill_circle(x+4*r/10-3*P/4,y-7*P/4,r/6);
		break;
	case right:
		gfx_fill_circle(x-4*r/10,y-2*P,r/3);
		gfx_fill_circle(x-4*r/10,y-3*P/2,r/3);
		gfx_fill_circle(x+4*r/10,y-2*P,r/3);
		gfx_fill_circle(x+4*r/10,y-3*P/2,r/3);
		gfx_color(0,0,150);
		gfx_fill_circle(x-4*r/10+3*P/4,y-7*P/4,r/6);
		gfx_fill_circle(x+4*r/10+3*P/4,y-7*P/4,r/6);
		break;
}
ghostLoop++;
usleep(200000);
gfx_clear();
}*/
}
