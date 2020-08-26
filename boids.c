// this is the first proper C project i've done, 
// so messy code, bodgeness and general spaghetti is to be expected
// i just hope it runs!
//#define boid birb
//#define birb bird etc ;^)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./rawdraw/os_generic.h"

#define CNFG_IMPLEMENTATION
#include <time.h>
#include "./rawdraw/CNFG.h"

#define PI 3.14159265
#define windowW 640
#define windowH 360
#define numberofBoids 2

unsigned frames = 0;
unsigned long iframeno = 0;

int mouseX = 0; 
int mouseY = 0;
int mouseMask = 0;
int precision = 1;

struct Boids{ 
    short size; // size mulitplyer for drawing
    short x;
    short y;
    double rotation; // 0 to 1 works for me
    double velocity; // probably will be unused as every step they'll pick a direction

};

double birbLayout[5][2] = {{0,-1},{-0.75,1},{0,0.25},{0.75,1},{0,-1}};

struct Boids BoidStruct[numberofBoids];

void HandleKey( int keycode, int bDown ){ // keyboard press (goes a few times)
	if( keycode == 27 ) exit( 0 );
	printf( "Key: %d -> %d\n", keycode, bDown );
}
void HandleButton( int x, int y, int button, int bDown ){ // mouse button
    precision++;
	printf( "Button: %d,%d (%d) -> %d\n", x, y, button, bDown );
    
}
void HandleMotion( int x, int y, int mask ){ // mouse position
    mouseX = x;
    mouseY = y;
    mouseMask = mask;
    // on click die
    if(mask){
        HandleDestroy();
    }
}
void HandleDestroy(){ // call to end program 

	printf( "Destroying\n" );
	exit(10);
}

// this is for things like that are centred around 0,0 and transformed there
void xyTransform(int * x, int * y, int sizeMultiplyer, float rotation){
    
    *x *= sizeMultiplyer;
    *y *= sizeMultiplyer;

    if(rotation){
        float currentRotation = (float)atan(*x / *y);
        currentRotation += rotation;
        *x = sin(currentRotation);
        *y = cos(currentRotation);

    }
}

// only draws one birb in order to draw singular birds down the line maybe
void drawBirb(int birdNo){ // eventually make it so it can draw multiple points onto one thing
    
    int firstX, firstY, lastX, lastY;

    firstX = (BoidStruct[birdNo].x * BoidStruct[birdNo].size) + BoidStruct[birdNo].x;
    firstY = (BoidStruct[birdNo].y * BoidStruct[birdNo].size) + BoidStruct[birdNo].y;

    for(int i = 1; i <= 4; i +=1){
        lastX = (BoidStruct[i].x * BoidStruct[i].size) + BoidStruct[i].x;
        lastX = (BoidStruct[i].y * BoidStruct[i].size) + BoidStruct[i].y;
        CNFGTackSegment(firstX,firstY,lastX,lastY);
        firstX = lastX;
        firstY = lastY;
    }

}

// warms up the birds
void initBirbs(){
    for(int i = 0; i < numberofBoids; i++){
        printf("init birb %d\n", i);
        BoidStruct[i].size = 15;
        BoidStruct[i].x = 150;
        BoidStruct[i].y = 150;
        BoidStruct[i].rotation = 0.0;
        
    }

}

void birbStep(){ // one step forwards (should be called ~60 times a second)




}
void drawRegPoly( int x, int y, int sides, int radius){

    double seperation = (1/(double)sides)*(PI/0.5f);
    double currentAngle = 0.0;
    float currX;
    float currY;
    float prevX = 0.0f;
    float prevY = 1.0f*(float)radius;

    for(int i = 0; i <= sides; i++){
        currX = sin(currentAngle)*radius;
        currY = cos(currentAngle)*radius;;
        CNFGTackSegment(x+prevX, y+prevY, x+currX, y+currY);
        prevX = currX;
        prevY = currY;

        currentAngle += seperation;
    }
}
int main()
{
    int i;
    double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	double LastFrameTime = OGGetAbsoluteTime();
	double SecToWait;
    int paddingAmount = 10;
    int LastFPS; // updates every second 
    int linesegs = 0;

    char str[12];
    char str2[100];

    initBirbs();
	//CNFGSetup( "Boids Demo", windowW, windowH );
    // fullscreen because I dont want you to change the window size
    CNFGSetupFullscreen("Boids Rawdraw Demo", 0);
	while(1)
	{
        iframeno++; // woo new frame!

		short w, h; // window size

        

		CNFGHandleInput();
        CNFGClearFrame(); // cleaning up last frame
        

		CNFGGetDimensions( &w, &h ); // updates window size
		CNFGColor( 0xffffff ); // sets "pen" colour that CNFG functions use
		
        // outer border
        CNFGDrawBox(paddingAmount,paddingAmount, w - paddingAmount , h - paddingAmount );
        
        
        // grid
        CNFGColor( 0x333333 );
        int wLine = 0;
        int hLine = 0;
        int squareAmount = 100;
        do { // |||||||||||
            CNFGTackSegment(wLine, 0, wLine, h);
            wLine += squareAmount;
        } while(wLine <= w);

        do { // ==
            CNFGTackSegment(0, hLine, w, hLine);
            hLine += squareAmount;
        } while(hLine <= h);


        CNFGColor( 0xffffff ); // resets the colour back

        
        //this is for the moving hi
        int stuffX = iframeno % w;
        int stuffY = (short)(75*sin((float)(i+iframeno)*.01)) + h/2;

        // misc
        //CNFGTackSegment(mouseX, mouseY, stuffX, stuffY);
        //CNFGTackSegment(rand()%w, rand()%h, rand()%w, rand()%h);
        //CNFGTackSegment(rand()%w, rand()%h, rand()%w, rand()%h);
        //CNFGTackSegment(rand()%w, rand()%h, rand()%w, rand()%h);
        

        int tempX;
        int tempY;
        //xyTransform(&tempX, &tempY, 1, 0.1);
        //CNFGTackSegment(150, 150, tempX, tempY);
        CNFGDrawTextbox(paddingAmount +10 ,20,"oh hello there", 4);

        // draw hi
        CNFGPenX = stuffX; CNFGPenY = stuffY; 
        CNFGDrawBox( paddingAmount, paddingAmount, 0 + paddingAmount, 0 + paddingAmount );
        CNFGColor( 0x000aff );
        drawRegPoly(stuffX, stuffY, round(w/100), 75);
        CNFGColor( rand() );
        drawBirb(0);
        CNFGColor( 0xffffff );

        // fps display
        sprintf(str, "%d", LastFPS);   //b g r
        if(LastFPS < 60) CNFGColor(0x0000ff); // if fps falls below 60

        CNFGPenX = 20; CNFGPenY = h -30;
        CNFGDrawText( str , 4 );
        CNFGColor(0xffffff);

        // window size display
        sprintf(str, "%d", w); // wxh
        strcpy(str2, str);
        strcat(str2, "x");
        sprintf(str, "%d", h);
        strcat(str2, str);
        CNFGPenX = 100; CNFGPenY = h -30;
        CNFGDrawText( str2 , 4 );


        // end of drawing 
        frames++;
		CNFGSwapBuffers();




        ThisTime = OGGetAbsoluteTime();
		if( ThisTime > LastFPSTime + 1 )
		{
			printf( "FPS: %d Mouse: %d,%d\n", frames, mouseX, mouseY);
            //printf("bird %d at %d-%d\n",0,BoidStruct[0].x,BoidStruct[0].y);

            LastFPS = frames; // for display
			frames = 0;
			linesegs = 0;
			LastFPSTime+=1;
		}

        SecToWait = .016 - ( ThisTime - LastFrameTime );
		LastFrameTime += .016;
		if( SecToWait > 0 )
			OGUSleep( (int)( SecToWait * 1000000 ) );
	}
}