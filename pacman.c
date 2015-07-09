/* This is a game of Pac-Man *
 * Cory Jbara - Fundamentals of Computing I Final Project - December 2014 */
#include "gfx3.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

const int rows=17, columns=15, radius=20*2; //radius is length and width of each square cell

typedef struct Location{
  int x;
  int y;
  int prevX;
  int prevY;
  int orientation;
} Location;

enum ORIENTATION {up,down,left,right};
enum GHOST {blinky,pinky,inky,clyde};
enum STATE {chase, scatter, frighten,dead,housed};

char *num2str(int);
void titleScreen(int,int,Location[],Location*,int[]);
void drawBoard(int,int,int,int,int,int,int,int,int);
void animateMotion(int,int,int,int,Location*,Location[],int,int,int,int[],int,int[],int[],int);
int movePacman(Location*,Location[],char,int,int,int,int,int,int[],int*,int[]);
void targetGhosts(Location[],Location*,int,int,int,int,int,int[]);
void moveGhost(Location*,int,int,int,int,int[]);
void drawPacman(int,int,int,int);
void drawGhost(int,int,int,int,int[],int); 
int dotsNumber(void);
double targetDistance(int,int,int,int);
int minLocation(double[]);
int checkDeath(Location*,Location[],int,int,int,int,int,int,int,int[],int*,int[],int[],int);
void deathAnimation(Location*,Location[],int,int,int,int,int,int,int,int[],int,int[],int[],int);
int activeGhosts(Location*,int[]);
void ghostState(int*,int[],int[]);
void resetBoard(void);
/* The board is a visual array of numbers as follows:
 * 0: dots that pacman has not eaten yet
 * 1: blue rectangle space that pacman cannot go through
 * 2: Pacman's location
 * 7: Big dots (one in each of four corners)
 * 8: Space ghosts can go in but Pacman cannot
 * 9: Blank space */
int board[17][15]=     {{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
			{7,1,1,0,1,1,0,1,0,1,1,0,1,1,7},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,1,1,0,1,0,1,1,1,0,1,0,1,1,0},
			{0,0,0,0,1,0,0,1,0,0,1,0,0,0,0},
			{1,1,1,0,1,1,0,1,0,1,1,0,1,1,1},
			{1,1,1,0,1,0,0,3,0,0,1,0,1,1,1},
			{0,0,0,0,0,0,8,8,8,0,0,0,0,0,0},
			{1,1,1,0,1,0,0,0,0,0,1,0,1,1,1},
			{1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},
			{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
			{0,1,1,0,1,1,0,1,0,1,1,0,1,1,0},
			{7,0,1,0,0,0,0,2,0,0,0,0,1,0,7},
			{1,0,1,0,1,0,1,1,1,0,1,0,1,0,1},
			{0,0,0,0,1,0,0,1,0,0,1,0,0,0,0},
			{0,1,1,1,1,1,0,1,0,1,1,1,1,1,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
int board2[17][15];

void resetBoard(void){
	memcpy(board,board2,sizeof(int)*rows*columns);
}

int main()
{
  memcpy(board2,board,sizeof(int)*rows*columns);
  int height=800,width=800;				//height and width of screen
  gfx_open(width,height,"Pacman");
  gfx_clear_color(0,0,0);
  gfx_clear();
 
  int boardHeight=radius*rows,boardWidth=radius*columns;//Height of the board
  int xtopleft=width/2-boardWidth/2;			//x coord of top left corner of board
  int ytopleft=height/2-boardHeight*9/16; 		//y coord of top left corner of board
  char movement;  
  int i,lives=3;//start with 3 lives
  int win=0;
  int active=0;	//this changes depending on number of dots left
  int initialDots=dotsNumber();
  int remainingDots=dotsNumber();
  int score=0;
  int loop[4]={0,0,0,0};
  int frightenLoop[4]={0};
  int newScore[3]={0,0,0};//values [0]:new score to display(when ghost is killed),[1]:xvalue,[2]:yvalue
  Location pacman;
  Location ghosts[4];// enumerated to blinky, pinky, inky, clyde;
/*There are four states: 0: Chase, 1: Scatter, 2: Frighten, 3: Dead, and 4: Housed */
  int state[4]={scatter,scatter,scatter,scatter};

while(1){ 
 titleScreen(height,width,ghosts,&pacman,state);
 score=0;
 lives=3;
 for(i=0;i<=3;i++) loop[i]=0;	//reset the game
 resetBoard();
 gfx_wait();
/* This is the gameplay loop, will repeat every time a life is lost */
 while(lives>0){
  gfx_clear();
/* Initialize pacman's location */
  pacman.x=pacman.prevX=7;
  pacman.y=pacman.prevY=12;
  pacman.orientation=right;
/* Initialize ghost's locations */
  for(i=blinky;i<=clyde;i++){state[i]=scatter;}
  
  ghosts[blinky].x=7; 
  ghosts[blinky].prevX=8;
  ghosts[blinky].y=ghosts[blinky].prevY=6;
  ghosts[blinky].orientation=left;

  ghosts[pinky].x=7;
  ghosts[pinky].prevX=7;
  ghosts[pinky].y=7;
  ghosts[pinky].prevY=8;
  ghosts[pinky].orientation=up;
  
  ghosts[inky].x=ghosts[inky].prevX=6;
  ghosts[inky].y=7;
  ghosts[inky].prevY=8;
  ghosts[inky].orientation=right;

  ghosts[clyde].x=ghosts[clyde].prevX=8;
  ghosts[clyde].y=7;
  ghosts[clyde].prevY=8;
  ghosts[clyde].orientation=left; 
 
/* These two statements draw pacman and the board to begin the program */
  drawBoard(xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,2,score);
  drawPacman(xtopleft+radius*pacman.x+radius/2,ytopleft+radius*pacman.y+radius/2,pacman.orientation,0);
  for(i=blinky;i<=clyde;i++){
    drawGhost(xtopleft+radius*ghosts[i].x+radius/2,ytopleft+radius*ghosts[i].y+radius/2,i,ghosts[i].orientation,state,frightenLoop[i]);
  }

/* This loop is the gameplay after all initialization */
  while(1){
    /*if(gfx_event_waiting()){prevMovement=movement;*/ movement=gfx_wait();//}
    
  /* This block updates pacman position, checks for death
   * then updates ghosts' positions, then checks for death again */
    movePacman(&pacman,ghosts,movement,xtopleft,ytopleft,boardHeight,boardWidth,active,state,&score,frightenLoop);
    if(checkDeath(&pacman,ghosts,xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,state,&score,newScore,frightenLoop,loop[0])){	//pacman's death?
	for(i=0;i<=3;i++) loop[i]=0;	//reset the game
	lives--;			//decrease the lives
	printf("LIVES: %i\n",lives);
	break;
    }
    targetGhosts(ghosts,&pacman,xtopleft,ytopleft,boardHeight,boardWidth,active,state);
    if(checkDeath(&pacman,ghosts,xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,state,&score,newScore,frightenLoop,loop[0])){	//pacman's death?
	for(i=0;i<=3;i++) loop[i]=0;	//reset the game
	lives--;			//decrease the lives
	printf("LIVES: %i\n",lives);
	break;
    }
    ghostState(loop,state,frightenLoop);
    active=activeGhosts(ghosts,loop);

/* The next function animates the motion of all of the objects (pacman and ghosts */
    animateMotion(xtopleft,ytopleft,boardHeight,boardWidth,&pacman,ghosts,height,width,lives,state,score,newScore,frightenLoop,loop[0]);

/* The case to exit the loop is winning, when there are no dots left */
    if(dotsNumber()==0){	//The player got all the dots, they won the game
	win=1;
	break;
    }
  }
  if(lives<=0){ 
	gfx_clear();
	printf("\n\nGAME OVER\n\n");	
  	drawBoard(xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,0,score);
	if(tolower(gfx_wait())=='n'){ return 0; }//This will start the entire game over including the title screen
	else break;}
  else if(win){ printf("\n\nWINNER!\n\n");	
	drawBoard(xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,win,score);
	if(tolower(gfx_wait())=='n'){ return 0; }//This will end the game 
	else break;}
  }
 }
}

int activeGhosts(Location ghosts[],int loop[]){
/* This program returns the number of ghosts that should be active, as well as updates the 
 * values of the ghosts in the ghost house */
/* A new ghost is put in play after 10+ghost*20 non-frighten loops */
  int active,i;
  if(loop[0]<10)	active=0;
  else if(loop[0]<30)	active=1;
  else if(loop[0]<50)	active=2;
  else			active=3;

/* update starting locations of ghosts, but only once, when the ghost should leave the ghost house */
  switch(active){
	case 0:
		loop[1]=0;
		ghosts[1].x=ghosts[1].prevX=7;
                ghosts[1].y=7;
                ghosts[1].prevY=8;
                ghosts[1].orientation=up;
		
		ghosts[2].x=ghosts[2].prevX=6;
	        ghosts[2].y=7;
		ghosts[2].prevY=8;
        	ghosts[2].orientation=right;
		
                ghosts[3].x=ghosts[3].prevX=8;
                ghosts[3].y=7;
		ghosts[3].prevY=8;
                ghosts[3].orientation=left;
		break;
	case 1:
		loop[2]=0;
		ghosts[2].x=ghosts[2].prevX=7;
	        ghosts[2].y=7;
		ghosts[2].prevY=8;
        	ghosts[2].orientation=right;

                ghosts[3].x=ghosts[3].prevX=8;
                ghosts[3].y=7;
                ghosts[3].prevY=8;
                ghosts[3].orientation=left;
		break;
	case 2:	
		loop[3]=0;
                ghosts[3].x=ghosts[3].prevX=7;
                ghosts[3].y=7;
		ghosts[3].prevY=8;
                ghosts[3].orientation=left;
                break;	
  }
  return active;
}

void ghostState(int loop[],int state[],int frightenLoop[]){
  int i;
  i=clyde;
  for(i=0;i<=3;i++){
    if(state[i]==frighten){
	frightenLoop[i]++;
	if(frightenLoop[i]<=30){//30 is the default number of loops the ghosts are frightened for
		continue;		//This state stays frightened
	} else { 
		frightenLoop[i]=0; //reinitialize this variable
	}
    } else if(state[i]>=dead){ continue;}
  //If the ghosts are not frightened, then they go back to their previous state
  if(loop[i]<=5)	{state[i]=chase;}//chase for 5 loops
  else if(loop[i]<=10)	{state[i]=scatter;}//scatter for 5 loops
  else if(loop[i]<=30)	{state[i]=chase;}//chase for 20 loops
  else if(loop[i]<=40)	{state[i]=scatter;}//scatter for 10 loops
  else if(loop[i]<=60)	{state[i]=chase;}//chase for 20 loops
  else if(loop[i]<=70)	{state[i]=scatter;}//scatter for 10
  else if(loop[i]<=90)	{state[i]=chase;}//chase for 20
  else if(loop[i]<=100)  	{state[i]=scatter;}//scatter for 10
  else			{state[i]=chase;}//chase permanantly
 }
 if(state[0]!=frighten && state[1]!=frighten && state[2]!=frighten && state[3]!=frighten){  
   for(i=0;i<=3;i++) (loop[i])++;	//only increase loop when all of ghosts are not frightened
 }  
}

void drawBoard(int x,int y,int height,int width,int h,int w,int lives,int win,int score){	
/* This function draws the board in blue, one pixel wide borders
 * x and y are the coordinates of the top left corner of the board */
  gfx_color(0,0,255);
 
/* This loop draws the rectangles (1) in blue to prevent pacman from going over them
 * It also draws the dots (0) if pacman has not eaten them yet 
 * Also draws the big dots (7) if pacman has not eaten them yet */
  int i,j;
  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
	if(board[i][j]==1){
	  gfx_color(0,0,255);
	  gfx_fill_rectangle(x+radius*j,y+radius*i,radius,radius);
	} else if(board[i][j]==0){//draws dots in place
	  gfx_color(255,200,0);
	  gfx_fill_rectangle(x+radius*(j+0.5)-2,y+radius*(i+0.5)-2,4,4);
	} else if (board[i][j]==7){//draw big dots
	  gfx_color(255,200,0);
          gfx_fill_circle(x+radius*(j+.5),y+radius*(i+.5),8);
	} else if(board[i][j]==8){
	  gfx_color(0,0,255);
          gfx_rectangle(x+radius*j,y+radius*i,radius,radius);
	}//printf("%i",board[i][j]); 
    }//printf("\n");
  }
/* This block draws borders for the board */
  gfx_color(0,0,255);
  gfx_line(x,y,x+width,y);			//top border
  gfx_line(x,y+height,x+width,y+height);	//bottom border
  gfx_line(x,y,x,y+radius*7);				//left top border    (above gap)
  gfx_line(x,y+radius*8,x,y+height);			//left bottom border (below gap)
  gfx_line(x+width,y,x+width,y+radius*7);		//right top border    (above gap)
  gfx_line(x+width,y+radius*8,x+width,y+height);	//right bottom border (below gap)

/* This block draws the score and number of lives left */
  gfx_color(255,255,255);
  gfx_text((w-width)/4-radius/2,h*1/2-2*radius/3,"LIVES:");
  gfx_text((w-width)/4-radius/2,h*.3-16,"SCORE:");
  gfx_text((w-width)/4-radius/2,h*.3,num2str(score));
  for(i=0;i<lives;i++){
    drawPacman((w-width)/4,h*1/2+radius*i,right,1);
  }
  if(lives<=0){
	gfx_color(255,255,255);
	gfx_text(w/2-27,h/2-8,"GAME OVER");
        gfx_text(w/2-48,h/2+8,"Play Again? (y/n)");	
  } else if(win==1){
	gfx_color(255,255,255);
        gfx_text(w/2-18,h/2-8,"WINNER!");	
        gfx_text(w/2-48,h/2+8,"Play Again? (y/n)");	
  } else if(win==2){
        gfx_color(255,255,255);
        gfx_text(w/2-18,h/2-8,"READY?");	
  } else if(win==3){
        gfx_color(255,255,255);
        gfx_text(w/2-8,h/2-8,"GO!"); 
  } else if(win==5){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 7 times");
  } else if(win==6){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 6 times");
  } else if(win==7){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 5 times");
  } else if(win==8){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 4 times");
  } else if(win==9){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 3 times");
  } else if(win==10){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-38,h/2+8,"Click 2 times");
  } else if(win==11){
        gfx_color(255,255,255);
        gfx_text(w/2-23,h/2-8,"YOU DIED");
        gfx_text(w/2-36,h/2+8,"Click 1 time");
  }
} 

void animateMotion(int xtl, int ytl,int boardHeight,int boardWidth,Location *pacman,Location ghosts[],int height,int width, int lives,int state[],int score,int newScore[],int frightenLoop[],int loop){
/* This function is called after pacman is moved, and animates the movement of pacman,
 * and all the ghosts. While the ghosts' new positions are tabulated in moveGhosts, they
 * are not drawn until this function is called after pacman is moved */
  int i,j,loops=2;

  int x=xtl+pacman->x*radius+radius/2;	//first location to draw, will increment 5 times
  int y=ytl+pacman->y*radius+radius/2;

  Location delta;
  delta.x=(pacman->x-pacman->prevX)*radius/loops;
  delta.y=(pacman->y-pacman->prevY)*radius/loops;
/* Currently, pacman only moves one space abruptly, he cannot smoothly move from cell to cell */
  //for(j=1;j<=loops+1;j++){	//this loop allows the pacman movement to be smooth
    gfx_clear();
    if(loop==1){i=3;}
    drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,i,score);
    drawPacman(x,y,pacman->orientation,0);
    //x+=delta.x;
    //y+=delta.y;
    //usleep(10000);
  //}
  for(i=blinky;i<=clyde;i++){
    x=xtl+ghosts[i].x*radius+radius/2;
    y=ytl+ghosts[i].y*radius+radius/2;
    drawGhost(x,y,i,ghosts[i].orientation,state,frightenLoop[i]);
  }
  /* Check if there needs to be a score displayed */
  if(newScore[0]){
        gfx_color(120,120,120);
        gfx_text(xtl+newScore[1]*radius+radius/2,ytl+newScore[2]*radius+radius/2,num2str(newScore[0]));
        newScore[0]=0;
	newScore[1]=0;
	newScore[2]=0;
  }
}

void drawPacman(int x,int y,int orientation,int mouthOpen){
  static int pacLoop=0;
  int i;
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,radius/2-1);

  gfx_color(0,0,0);
  int r=radius/2-1;
  if(mouthOpen || pacLoop%2){
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
  if(!mouthOpen)  pacLoop++;
}

void drawGhost(int x,int y,int ghost,int orientation,int state[],int frightenLoop){
/* ghost is the number of ghost to draw
 * 0:blinky, 1:pinky, 2:inky, 3:clyde */
  static int ghostLoop[4]={0,1,1,0};
  int f; //f determines whether a frightened ghost is white or blue (flashes at end of frightened state)
  if(frightenLoop>=20 && state[ghost]==frighten){ f=frightenLoop%2; } else { f=-1; }
/* Choose color */
  if(state[ghost]==frighten){
     if(!f){ gfx_color(0,0,150); } else if(f==1){ gfx_color(255,255,255); } else {gfx_color(0,0,150);}
  } else {
     switch(ghost){
	case blinky: 
		gfx_color(255,0,0); 
		ghostLoop[blinky]++;
		break; 
	case pinky: 
		gfx_color(255,150,200); 
		ghostLoop[pinky]++;
		break; 	
	case inky: 	
		gfx_color(0,255,255); 
		ghostLoop[inky]++;
		break;		
	case clyde: 
		gfx_color(255,130,0); 
		ghostLoop[clyde]++;
		break;
     } 
  }

/* Draw body */
  int P=radius/10;	//value that helped make the ghosts look better
  int i,r=radius/2-P;
 if(state[ghost]!=dead){
   gfx_fill_circle(x,y-P,r);
  for(i=0;i<=r;i++){//left leg
    	gfx_line(x-r+i,y-P,   x-r+i,  y+r-i);
  }
  for(i=1;i<=r;i++){//right leg
    	gfx_line(x+i, y-P,    x+i,    y+i);
  }
  gfx_fill_rectangle(x-r,y-P,2*r,r);
    
  if(ghostLoop[ghost]%2){
    for(i=0;i<r;i++){//right middle leg
      	gfx_line(x+i, y-P,    x+i,    y+r-i);
    }
    for(i=0;i<=r;i++){//left middle leg
    	gfx_line(x-r+i, y-P,    x-r+i,    y+i);
    }
  } else {
    for(i=3*P/2;i<=r;i++){//left middle leg
    	gfx_line(x-i, y-P,    x-i,    y+r+3*P/2-i);
    }
    for(i=3*P/2;i<=r;i++){//right leg
    	gfx_line(x+i,   y-P,    x+i,    y+r+3*P/2-i);
    }
  }
 }

/* Add the eyes based on orientation *
 * Frightened ghosts will have two whit dot eyes and a mouth
 * Values for all circle centers are made by trial and error, until the ghost looked good */
  gfx_color(255,255,255);
  if(state[ghost]==frighten){
    if(f==1){gfx_color(0,0,150);}
	gfx_fill_circle(x-4*r/10,y-2*P,r/4);
        gfx_fill_circle(x+4*r/10,y-2*P,r/4);
	//next six are the mouth
	gfx_line(x-r,y+2*P,x-2*r/3,y+P);
	gfx_line(x-2*r/3,y+P,x-r/3,y+2*P);
	gfx_line(x-r/3,y+2*P,x,y+P);
	gfx_line(x,y+P,x+r/3,y+2*P);
	gfx_line(x+r/3,y+2*P,x+2*r/3,y+P);
	gfx_line(x+2*r/3,y+P,x+r,y+2*P);
  } else {
	gfx_fill_circle(x-4*r/10,y-2*P,r/3);
        gfx_fill_circle(x-4*r/10,y-3*P/2,r/3);
        gfx_fill_circle(x+4*r/10,y-2*P,r/3);
        gfx_fill_circle(x+4*r/10,y-3*P/2,r/3);

/* Now on to the pupils */        
     gfx_color(0,0,150);
     switch(orientation){
        case up:
        /*pupils*/      
                gfx_fill_circle(x-4*r/10,y-11*P/4,r/6);
                gfx_fill_circle(x+4*r/10,y-11*P/4,r/6);
                break;
        case down:
                gfx_fill_circle(x-4*r/10,y-3*P/4,r/6);/*pupils are 1/2 radius of the entire eye */
                gfx_fill_circle(x+4*r/10,y-3*P/4,r/6);
                break;
        case left:
                gfx_fill_circle(x-4*r/10-3*P/4,y-7*P/4,r/6);
                gfx_fill_circle(x+4*r/10-3*P/4,y-7*P/4,r/6);
                break;
        case right:
                gfx_fill_circle(x-4*r/10+3*P/4,y-7*P/4,r/6);
                gfx_fill_circle(x+4*r/10+3*P/4,y-7*P/4,r/6);
                break;
      }
  }
}

int movePacman(Location *pacman,Location ghosts[],char movement,int xtopleft,int ytopleft,int boardHeight,int boardWidth,int activeGhosts,int state[],int *score,int frightenLoop[]){
/*returns 1 if pacman moves successfully, 0 if he does not move */
  int i=pacman->y,j=pacman->x,death,k;
  switch(movement){
	case 'Q'://left movement
		if(j!=0){
		  if(board[i][j-1]!=1 && board[i][j-1]!=8){
                    /* Check for the big dot */
                    if(board[i][j-1]==7){
                        state[0]=state[1]=state[2]=state[3]=frighten;   //switch ghosts to frighten state
			for(k=0;k<=3;k++) frightenLoop[k]=0;
			*score+=50;
                    }
		    if(board[i][j-1]==0){*score+=10;}//add 10 points for every small dot
		    board[i][j-1]=2;
		    board[i][j]=9;
		    pacman->prevX=pacman->x;
		    pacman->x--;
		    pacman->orientation=left;
		  }
		} else if (i==7 && j==0){
                  board[i][j]=9;
		  if(board[i][columns-1]==0){*score+=10;}//add 10 points for every small dot
                  board[i][columns-1]=2;
		  pacman->x=columns-1;
		  pacman->orientation=left;
                }
		return 1;
		break;
	case 'T'://down movement
		if(i<(rows-1)){
                  if(board[i+1][j]!=1 && board[i+1][j]!=8){
                    /* Check for the big dot */
                    if(board[i+1][j]==7){
                        state[0]=state[1]=state[2]=state[3]=frighten;   //switch ghosts to frighten state
			for(k=0;k<=3;k++) frightenLoop[k]=0;
			*score+=50;
		    }
		    if(board[i+1][j]==0){*score+=10;}//add 10 points for every small dot
                    board[i+1][j]=2;
                    board[i][j]=9;
                    pacman->prevY=pacman->y;
		    pacman->y++;
		    pacman->orientation=down;
                  }
                }
		return 1;
		break;
	case 'S'://right movement
		if(j<(columns-1)){
                  if(board[i][j+1]!=1 && board[i][j+1]!=8){
                    /* Check for the big dot */
		    if(board[i][j+1]==7){
			state[0]=state[1]=state[2]=state[3]=frighten;	//switch ghosts to frighten state
			for(k=0;k<=3;k++) frightenLoop[k]=0;
			*score+=50;
		    }
		    if(board[i][j+1]==0){*score+=10;}//add 10 points for every small dot
		    board[i][j+1]=2;
                    board[i][j]=9;
                    pacman->prevX=pacman->x;
		    pacman->x++;
		    pacman->orientation=right;
                  }
                } else if (i==7 && j==columns-1){//this is for the spot on the rght to flip sides of the board
		  if(board[i][0]==0){*score+=10;}//add 10 points for every small dot
		  board[i][0]=2;
		  board[i][j]=9;
		  pacman->x=0;
		  pacman->orientation=right;
		}
		return 1;
		break;
	case 'R'://up movement
		if(i!=0){
                  if(board[i-1][j]!=1 && board[i-1][j]!=8){
                    /* Check for the big dot */
                    if(board[i-1][j]==7){
                        state[0]=state[1]=state[2]=state[3]=frighten;	//switch ghosts to frighten state
			for(k=0;k<=3;k++) frightenLoop[k]=0;
			*score+=50;
                    }
		    if(board[i-1][j]==0){*score+=10;}//add 10 points for every small dot
                    board[i-1][j]=2;
                    board[i][j]=9;
                    pacman->prevY=pacman->y;
		    pacman->y--;
		    pacman->orientation=up;
                  }
                }
		return 1;
		break;
	default: break;
  }
  return 0;//pacman did not move
}
int checkDeath(Location *pacman,Location ghosts[],int xtopleft,int ytopleft,int boardHeight,int boardWidth,int height,int width,int lives,int state[],int *score,int newScore[],int frightenLoop[],int loop){
/* Returns 1 if death, returns 0 otherwise */
  int i,j,fright=0;
  for(i=blinky;i<=clyde;i++){
    if(pacman->x==ghosts[i].x && pacman->y==ghosts[i].y && state[i]==frighten){
	printf("Killed a Ghost");
	state[i]=dead;
	frightenLoop[i]=0;
	//Calculate how many ghosts are in the frightened state still to determine number of points
	for(j=0;j<=3;j++){
		if(state[j]==frighten){
			fright++;	
		}
	}
	gfx_color(255,255,255);
	switch(fright){
                case 0:
                        newScore[1]=pacman->x;
                        newScore[2]=pacman->y;
                        newScore[0]=1600;
                        *score+=1600;
                        break;
                case 1:
                        newScore[1]=pacman->x;
                        newScore[2]=pacman->y;
                        newScore[0]=800;
                        *score+=800;
                        break;
                case 2:
                        newScore[1]=pacman->x;
                        newScore[2]=pacman->y;
                        newScore[0]=400;
                        *score+=400;
                        break;
                case 3:
                        newScore[1]=pacman->x;
                        newScore[2]=pacman->y;
                        newScore[0]=200;
                        *score+=200;
                        break;
        }
    } else if(pacman->x==ghosts[i].x && pacman->y==ghosts[i].y && state[i]<frighten){
	printf("\n\nPacman Died\n\n");
	deathAnimation(pacman,ghosts,xtopleft,ytopleft,boardHeight,boardWidth,height,width,lives,state,*score,newScore,frightenLoop,loop); 
	return 1;
    } else { continue; }
  }
  return 0;
}

void deathAnimation(Location *pacman,Location ghosts[],int xtl,int ytl,int boardHeight,int boardWidth,int height,int width,int lives,int state[],int score,int newScore[],int frightenLoop[],int loop){
/* Death animation is in four states, first full circle, then pacman oriented upward, then half circle, then small sliver below, then gone, then explode */
  int r=radius/2-1;
  int timer=1000000;
  int x=xtl+pacman->x*radius+radius/2;   //first location to draw, will increment 5 times
  int y=ytl+pacman->y*radius+radius/2;
  
  animateMotion(xtl,ytl,boardHeight,boardWidth,pacman,ghosts,height,width,lives,state,score,newScore,frightenLoop,loop);
  gfx_wait();

  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,5,score);
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,radius/2-1);
  //gfx_wait();

  gfx_wait();
  //usleep(timer);
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,6,score);
  drawPacman(x,y,up,1);

  gfx_wait();
  //usleep(timer);
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,7,score);
  gfx_color(255,255,0);
  drawPacman(x,y,0,0);
  gfx_color(0,0,0);
  gfx_fill_rectangle(x-radius/2-1,y-radius/2+1,radius,radius/2-1);

  gfx_wait();
  //usleep(timer);  
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,8,score);
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,radius/2-1);
  gfx_color(0,0,0);
  gfx_fill_triangle(x-r,y-r,x+r,y-r,up);
  gfx_fill_triangle(x+r,y-r,x+r,y+r,right);
  gfx_fill_triangle(x-r,y-r,x-r,y+r,left);

  gfx_wait();
  //usleep(timer); 
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,9,score);

  gfx_wait();
  //usleep(timer);
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,10,score);
  gfx_color(255,255,0);

  float angle=2*M_PI/5;       //partial snowflake
  int i,xnew[5],ynew[5];
  for(i=1;i<=5;i++){
    xnew[i-1]=x+r*cos(i*angle+3*M_PI/2);
    ynew[i-1]=y+r*sin(i*angle+3*M_PI/2);
    gfx_line(x,y,xnew[i-1],ynew[i-1]);
  }

  gfx_wait();
  //usleep(timer);
  gfx_clear();
  drawBoard(xtl,ytl,boardHeight,boardWidth,height,width,lives,11,score);
  gfx_color(255,255,0);
  angle=2*M_PI/5;              //snowflake with short lines
  for(i=1;i<=5;i++){
    xnew[i-1]=x+r*cos(i*angle+3*M_PI/2);
    ynew[i-1]=y+r*sin(i*angle+3*M_PI/2);
    gfx_line((x+xnew[i-1])/2,(y+ynew[i-1])/2,xnew[i-1],ynew[i-1]);
  }
  gfx_wait();
  //usleep(timer);
}

void targetGhosts(Location ghosts[],Location *pacman,int xtl,int ytl,int boardHeight,int boardWidth,int active,int state[]){
  int i,j,k=blinky,move,a;
  static int loop=0;
  double dist[4];
/* This loop will update the locations of each individual ghost */
/* Assumptions: ghosts cannot turn around
 * Each ghost tries to get to its specified target that turn */
  int target[2];//target[0]=ytarget target[1]=xtarget
  for(k=blinky;k<=active;k++){
    if(state[k]==frighten && loop%2){//if a ghost is frightened, they slow down to one space every two loops
	target[0]=ghosts[k].y; 
	target[1]=ghosts[k].x;
	continue;
    }
    switch(state[k]){
      case chase: //this is the chase state, all ghosts go for pacman 
	switch(k){
/* Blinky's movement targets pacman directly. His target is pacman's current position */
	    case blinky:
		target[0]=pacman->y;
		target[1]=pacman->x;
		break;
/* Pinky's target is 3 spaces in front of pacman's current position, toward his orientation */
	    case pinky:
		switch(pacman->orientation){
			case left:
				target[0]=pacman->y;
				target[1]=pacman->x - 3;
				break;
			case right:
				target[0]=pacman->y;
                                target[1]=pacman->x + 3;
                                break;
			case up:
				target[0]=pacman->y - 3;
                                target[1]=pacman->x;
                                break;
			case down:
				target[0]=pacman->y + 3;
                                target[1]=pacman->x;
                                break;
		} break;
/* Inky's movement uses blinky's location and pacman's location. His target is blinky's target +2 in direction of pacman's orientation times two */
	    case inky:
		switch(pacman->orientation){
                        case left:
                                target[0]=2*(pacman->y - ghosts[blinky].y)+ghosts[blinky].y;
                                target[1]=2*(pacman->x - 2 - ghosts[blinky].x)+ghosts[blinky].x;
                                break;
                        case right:
                                target[0]=2*(pacman->y - ghosts[blinky].y)+ghosts[blinky].y;
                                target[1]=2*(pacman->x + 2 - ghosts[blinky].x)+ghosts[blinky].x;
                                break;
                        case up:
                                target[0]=2*(pacman->y - 2 - ghosts[blinky].y)+ghosts[blinky].y;
                                target[1]=2*(pacman->x - ghosts[blinky].x)+ghosts[blinky].x;
                                break;
                        case down:
                                target[0]=2*(pacman->y + 2 - ghosts[blinky].y)+ghosts[blinky].y;
                                target[1]=2*(pacman->x - ghosts[blinky].x)+ghosts[blinky].x;
                                break;
                } break;
/* If distance greater than 5, target pacman, otherwise, target random target on board */
	    case clyde:
		if(targetDistance(ghosts[clyde].x,ghosts[clyde].y,pacman->x,pacman->y)>=5){
			target[0]=pacman->y;
                	target[1]=pacman->x;
		} else {
			target[0]=rows-1;
			target[1]=0;
		} break;
      	}
	break;
      case scatter:	//This is when the ghosts go to their respective target corners
	switch(k){	//each has its own designated corner
	    case blinky://top right
		target[0]=0;
		target[1]=columns-1;
		break;
	    case pinky://top left
		target[0]=0;
		target[1]=0;
		break;
	    case inky://bottom right
		target[0]=rows-1;
		target[1]=columns-1;
		break;
	    case clyde://bottom left
		target[0]=rows-1;
		target[1]=0;
		break;
	}
	break;
      case frighten:	//This is randomized chaotic movement, usually slower
	target[0]=rand()%rows;
	target[1]=rand()%columns;
	break;
      case dead:
	target[0]=6;
	target[1]=7;
	break;
    }
/* This next block makes sure the target is on the board */
    if(target[0]>=rows)		target[0]=rows-1;
    else if(target[0]<0)	target[0]=0;
    if(target[1]>=columns)	target[1]=columns-1;
    else if(target[1]<0)	target[1]=0;
/* This block of code decides where to move the ghost in this loop */
    if(state[k]==dead && ghosts[k].x==target[1] && ghosts[k].y==target[0]){
	//This means they are at their designated target
	/* Put them into the ghost house */
	ghosts[k].y++; //at top of house, move into house and change state
	state[k]=housed;
    } else if(state[k]==housed){
	ghosts[k].y--;
	state[k]=chase;	
    } else {
    	moveGhost(&ghosts[k],xtl,ytl,boardHeight,boardWidth,target);
        
    }
  }
  loop++;
}
void moveGhost(Location *ghost,int xtl,int ytl,int boardHeight,int boardWidth,int target[]){
/* Moves all active ghosts */
  int i=ghost->y, j=ghost->x;
  int move[4],movement=0; //array of viable moves (each index applies to different move)
  double distance[4]={1000,1000,1000,1000}; //either set to distance or 1000 (arbitrary high value)
  if(board[i+1][j]!=1 && board[i+1][j]!=8 && (i+1)<rows && (i+1)!=ghost->prevY){  //down
    move[down]=1;
    distance[down]=targetDistance(target[1],target[0],j,i+1);
  } else{ move[down]=0; distance[down]=1000;}
  
  if(board[i-1][j]!=1 && board[i-1][j]!=8 && (i-1)>=0 && (i-1)!=ghost->prevY){  //up
    move[up]=1;
    distance[up]=targetDistance(target[1],target[0],j,i-1);
  } else{ move[up]=0; distance[up]=1000;}

  if(board[i][j+1]!=1 && board[i][j+1]!=8 && (j+1)<columns && (j+1)!=ghost->prevX){  //right
    move[right]=1;
    distance[right]=targetDistance(target[1],target[0],j+1,i);
  } else{ move[right]=0; distance[right]=1000;}
  
  if(board[i][j-1]!=1 && board[i][j-1]!=8 && (j-1)>=0 && (j-1)!=ghost->prevX){  //left
    move[left]=1;
    distance[left]=targetDistance(target[1],target[0],j-1,i);
  } else{ move[left]=0; distance[left]=1000;}

/* Now that we know which moves are viable and the distances from each, 
 * determine which is the closest to target */
  movement=minLocation(distance);
  if (i==7 && j==0 && ghost->prevX==1){//this is for the spot on the left to flip sides of the board
        ghost->prevX=columns;
        ghost->x=columns-1;
        ghost->orientation=left;
  } else if (i==7 && j==columns-1 && ghost->prevX==columns-2){//this is for the spot on the rght to flip sides of the board
        ghost->prevX=-1;
	ghost->x=0;
        ghost->orientation=left;
  } else {
    switch(movement){
      case up:
	ghost->prevY=ghost->y;
	ghost->prevX=ghost->x;
        ghost->y--;                                  //move up
        ghost->orientation=up;                       //set orientation upward
    	break;
      case down:
	ghost->prevY=ghost->y;
	ghost->prevX=ghost->x;
        ghost->y++;                                  //move down
        ghost->orientation=down;                     //set orientation downward
    	break;
      case left:
	ghost->prevX=ghost->x;
	ghost->prevY=ghost->y;
        ghost->x--;                                  //move left
        ghost->orientation=left;                     //set orientation left
	break;
      case right:
	ghost->prevX=ghost->x;
	ghost->prevY=ghost->y;
        ghost->x++;                                  //move right
        ghost->orientation=right;                    //set orientation right
	break;
    }
  }
}
int dotsNumber(void){
  int i,j,dots=0; 
  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
	if(board[i][j]==0 || board[i][j]==7) {dots++;}
    }
  }
  return dots; 
}

double targetDistance(int x1, int y1, int x2,int y2){
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int minLocation(double distances[]){
  int min;
  min=((distances[0] < distances[1])? 0 : 1);//returns the index of the minimum distance
  min=((distances[min] < distances[2])? min : 2);
  min=((distances[min] < distances[3])? min : 3);
  return min;
}

void titleScreen(int height,int width,Location ghosts[],Location *pacman,int state[]){
/*Draws the title screen */
  int i=-1,x,y;
  gfx_clear();
  gfx_color(255,255,255);
  gfx_text(width*.35+10,height*.8,"Programmed By: CORY JBARA");
  gfx_text(width*.7,height*.8-32,"Use the arrow keys to move");
  gfx_text(width*.7,height*.8,"Press any button to start");
  
/* Draw dots and point values */
  gfx_color(255,200,0);
  gfx_fill_rectangle(width*.8-2,height/2-15,4,4);
  gfx_color(255,255,255);
  gfx_text(width*.8+10,height/2-9,"- 10 Points");

  gfx_color(255,200,0);
  gfx_fill_circle(width*.8-4,height/2+15,8);
  gfx_color(255,255,255);
  gfx_text(width*.8+10,height/2+20,"- 50 Points");
  
/* Draws ghosts and their names */  
  x=width/3;
  y=height*.45+i*height/15;
  gfx_text(x,y,"CHARACTER ------------------------ NICKNAME");
  
  for(i=0;i<4;i++){
	state[i]=chase;
	x=width/4;
	y=height*.45+i*height/15;
	drawGhost(x,y,i,right,state,0);
	
	x=width/3;
	switch(i){
	    case blinky:
                gfx_color(255,0,0);
		gfx_text(x,y,"SHADOW  -------------------------- \"BLINKY\"");
                break;
            case pinky:
                gfx_color(255,150,200);
		gfx_text(x,y,"SPEEDY  -------------------------- \"PINKY\"");
                break;
            case inky:
                gfx_color(0,255,255);
		gfx_text(x,y,"BASHFUL -------------------------- \"INKY\"");
                break;
            case clyde:
                gfx_color(255,130,0);
		gfx_text(x,y,"POKEY   -------------------------- \"CLYDE\"");
                break;
     	}
  }
/* Pacman being chased */
  i++;
  y=height*.72;
  for(i=0;i<=3;i++){
	x=width*.4+i*width/20;
        drawGhost(x,y,i,left,state,0);
  }
  drawPacman(width*.4-width/20,y,left,1);

/* P */
  gfx_color(255,255,0);
  gfx_fill_rectangle(10,height/5-50,width/15,150);
  gfx_fill_circle(10+width/15,height/5,50);
  gfx_color(0,0,0);
  gfx_fill_circle(10+width/15,height/5,10);

/* A */
  gfx_color(255,255,0);
  int loop=0;
  int x1=width*.1;
  int x2=width*.27;
  int center=(x1+x2)/2;
  y=height/5+100-1;
  while(x1<x2){//down
        gfx_line(x1,y,x2,y);
    if(loop%2){
        x1++;
        x2--;
    }
        y--;
    	loop++;
  }
  gfx_color(0,0,0);
  gfx_fill_circle(center,height*.24,10);

/* C ::::: Code copied from drawPacman*/
  int rad=200;
  x=width*.24+rad/2;
  y=height/5;
  gfx_color(255,255,0);
  gfx_fill_circle(x,y,rad/2-1);

  gfx_color(0,0,0);
  int r=rad/2-1;
  gfx_fill_triangle(x+r,y-r,x+r,y+r,right);

/* M */
  gfx_color(255,255,0);
  x=width*.47;
  y=height/5-50;
  gfx_fill_rectangle(x,y,width*.15,150);
  gfx_color(0,0,0);
  gfx_fill_triangle(x,y,x+width*.15,y,up);

/* A */
  gfx_color(255,255,0);
  loop=0;
  x1=width*.63;
  x2=width*.17 +x1;
  center=(x1+x2)/2;
  y=height/5+100-1;
  while(x1<x2){//down
        gfx_line(x1,y,x2,y);
    if(loop%2){
        x1++;
        x2--;
    }
        y--;
        loop++;
  }
  gfx_color(0,0,0);
  gfx_fill_circle(center,height*.24,10);

/* N */
  gfx_color(255,255,0);
  x=width*.81;
  y=height/5-50;
  gfx_fill_rectangle(x,y,width*.15,150);
  gfx_color(0,0,0);
  x-=2;
  y-=height/10;
  gfx_fill_triangle(x+width*.1,y,x+width*.1,y+height*.2,right);
}

char *num2str(int n)
{
  static char a[10]; 
  sprintf(a, "%d", n);
  return (char *) a;
}
/*int ghostMoves(Location ghost){
* Checks the number of moves a ghost can make *
  int moves=0;
  int i=ghost.y, j=ghost.x;
  if(board[i+1][j]!=1 && board[i+1][j]!=8 && (i+1)!=ghost.prevY)      moves++;
  if(board[i-1][j]!=1 && board[i-1][j]!=8 && (i-1)!=ghost.prevY)      moves++;
  if(board[i][j+1]!=1 && board[i][j+1]!=8 && (j+1)!=ghost.prevX)      moves++;
  if(board[i][j-1]!=1 && board[i][j-1]!=8 && (j-1)!=ghost.prevX)      moves++;
  return moves;
}*/
//min (a) < (b) ? (a) : (b)
/*  for(k=blinky;k<=blinky;k++){
    i=ghosts[k].y; j=ghosts[k].x;	//set i and j to current coordinates of the ghost
    if(i < pacman->y){					//pacman is below
	if(board[i+1][j]!=1 && board[i+1][j]!=8){	//able to move down
	  //board[i][j]=
	  //board[i+1][j]
	  ghosts[k].prevY=ghosts[k].y;			
	  ghosts[k].y++;				//move down
	} else if(j < pacman->x){				//pacman is to the right
          if(board[i][j+1]!=1 && board[i][j+1]!=8){     	//able to move right
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x++;					//move right
          } else if(board[i-1][j]!=1 && board[i-1][j]!=8){	//able to move up, not right
	    ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y--;					//move up
	  } else if(board[i][j-1]!=1 && board[i][j-1]!=8){	//able to move left not right or up
	    ghosts[k].prevX=ghosts[k].x;			//move left
            ghosts[k].x--;
	  }
	} else if(j > pacman->x){                               //pacman is to the left
          if(board[i][j-1]!=1 && board[i][j-1]!=8){             //able to move left
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x--;                                      //move left
          } else if(board[i-1][j]!=1 && board[i-1][j]!=8){      //able to move up, not left
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y--;                                      //move up
          } else if(board[i][j+1]!=1 && board[i][j+1]!=8){      //able to move right not left or up
            ghosts[k].prevX=ghosts[k].x;                        //move right
            ghosts[k].x++;
          }
	}
    } else if(i > pacman->y){                           //pacman is above
        if(board[i-1][j]!=1 && board[i-1][j]!=8){       //able to move up
          ghosts[k].prevY=ghosts[k].y;                  
          ghosts[k].y--;                                //move up
        } else if(j < pacman->x){                               //pacman is to the right
          if(board[i][j+1]!=1 && board[i][j+1]!=8){             //able to move right
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x++;                                      //move right
          } else if(board[i+1][j]!=1 && board[i+1][j]!=8){      //able to move down, not right
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y++;                                      //move down
          } else if(board[i][j-1]!=1 && board[i][j-1]!=8){    //able to move left not right or down
            ghosts[k].prevX=ghosts[k].x;                        //move left
            ghosts[k].x--;
          }
        } else if(j > pacman->x){                               //pacman is to the left
          if(board[i][j-1]!=1 && board[i][j-1]!=8){             //able to move left
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x--;                                      //move left
          } else if(board[i+1][j]!=1 && board[i+1][j]!=8){      //able to move down, not left
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y++;                                      //move down          
	  } else if(board[i][j+1]!=1 && board[i][j+1]!=8){    //able to move right not left or down
            ghosts[k].prevX=ghosts[k].x;                        //move right
            ghosts[k].x++;
          }
        }
    } else {						//pacman is on same row
	if(j < pacman->x){                               //pacman is to the right
          if(board[i][j+1]!=1 && board[i][j+1]!=8){             //able to move right
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x++;                                      //move right
          } else if(board[i+1][j]!=1 && board[i+1][j]!=8){      //able to move down, not right
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y++;                                      //move down 
	 } else if(board[i-1][j]!=1 && board[i-1][j]!=8){      //able to move up not right or down
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y--;                                      //move up	         
	  } else if(board[i][j-1]!=1 && board[i][j-1]!=8){    //only able to move left
            ghosts[k].prevX=ghosts[k].x;                        //move left
            ghosts[k].x--;
          }
        } else if(j > pacman->x){                               //pacman is to the left
          if(board[i][j-1]!=1 && board[i][j-1]!=8){             //able to move left
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x--;                                      //move right
          } else if(board[i+1][j]!=1 && board[i+1][j]!=8){      //able to move down, not right
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y++;                                      //move down 
          } else if(board[i-1][j]!=1 && board[i-1][j]!=8){      //able to move up not right or down
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y--;                                      //move up                          
	  } else if(board[i][j+1]!=1 && board[i][j+1]!=8){      //only able to move right
            ghosts[k].prevX=ghosts[k].x;                        //move right
            ghosts[k].x++;
          }
	}
    }


if(j > pacman->x){                               //pacman is to the left
          if(board[i][j-1]!=1 && board[i][j-1]!=8){             //able to move left
            ghosts[k].prevX=ghosts[k].x;
            ghosts[k].x--;                                      //move left
          } else if(board[i+1][j]!=1 && board[i+1][j]!=8){      //able to move down, not left
            ghosts[k].prevY=ghosts[k].y;
            ghosts[k].y++;                                      //move down                    } else if(board[i][j+1]!=1 && board[i][j+1]!=8){    //able to move right not left or down
            ghosts[k].prevX=ghosts[k].x;                        //move right
            ghosts[k].x++;
          }
        }
  }printf("\t\t\tBlinky x:%i Blinky y:%i\n",ghosts[blinky].x,ghosts[blinky].y);
}
*/
/* This block checks to see how many possible moves the ghost has */
    /*if(ghostMoves(ghosts[k])==1){	//only one move to make
	printf("YES\n");
	i=ghosts[k].y; j=ghosts[k].x;
	if(i<(rows-1)){
	    if(board[i+1][j]!=1 && board[i+1][j]!=8){		//able to move down
		ghosts[k].prevY=ghosts[k].y;
		ghosts[k].y++;					//move down
		ghosts[k].orientation=down;			//set orientation downward
	    }
	} 
	if(i!=0){
	    if(board[i-1][j]!=1 && board[i-1][j]!=8){	//able to move up
		ghosts[k].prevY=ghosts[k].y;
                ghosts[k].y--;                                  //move upward
                ghosts[k].orientation=up;                       //set orientation upward
  	    }
	} 
	if(j<(columns-1)){
	    if(board[i][j+1]!=1 && board[i][j+1]!=8){	//able to move right
		ghosts[k].prevX=ghosts[k].x;
                ghosts[k].x++;                                  //move right
                ghosts[k].orientation=right;                    //set orientation right
  	    }
	} 
	if(j!=0){
	    if(board[i][j-1]!=1 && board[i][j-1]!=8){	//able to move left
		ghosts[k].prevX=ghosts[k].x;
                ghosts[k].x--;                                  //move left
                ghosts[k].orientation=left;                     //set orientation left
	    }
	} continue;					//go to next ghost in loop
  
Extra code for draw pacman
 * for(i=0;i<2*M_PI;i+=M_PI/60){
    gfx_line(x,y,x+radius/2*cos(i),y+radius/2*sin(i));
  }

*  for(i=0;i<radius/2;i++){
    gfx_line(x+i,y-radius/2+i,x-i,y-radius/2+i);
  }
  for(i=radius/2;i>0;i--){
    gfx_line(x+i,y+radius/2-i,x-i,y+radius/2-i);
  }
*
  for(i=radius/2-1;i>0;i--){
    gfx_circle(x,y,i);
    gfx_circle(x,y+1,i);
  }*/

