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

//#include "boids.h"

#define PI 3.14159265
#define windowW 640
#define windowH 360
#define numberofBoids 300
unsigned frames = 0;
unsigned long iframeno = 0;

int mouseX = 0; 
int mouseY = 0;
int mouseMask = 0;
int precision = 1;
//double birbLayout[5][2] = {{0,-1},{-0.75,1},{0,0.25},{0.75,1},{0,-1}};
double birbLayout[5][2] = {{0,1},{-0.75,-1},{0,-0.25},{0.75,-1},{0,1}};


struct Boids{ 
    short size; // size mulitplyer for drawing
    int x;
    int y;
    double rotation; // 0 to 1 works for me
    double velocity; // probably will be unused as every step they'll pick a direction
    uint32_t boidColour;
};


struct Boids BoidStruct[numberofBoids];

void HandleKey( int keycode, int bDown ){ // keyboard press
	if( keycode == 27 ) exit( 0 );
	printf( "Key: %d -> %d\n", keycode, bDown );
}
void HandleButton( int x, int y, int button, int bDown ){ // mouse buttons
    precision++;
	printf( "Button: %d,%d (%d) -> %d\n", x, y, button, bDown );
    
}
void HandleMotion( int x, int y, int mask ){ // mouse position
    mouseX = x;
    mouseY = y;
    mouseMask = mask;
    // on click end the program
    if(mask){
        HandleDestroy();
    }
}
void HandleDestroy(){ // call to end the program 

	printf( "Destroying\n" );
	exit(10);
}

// this is for things like that are centred around 0,0 and transformed there
// could remove sizeMulipler? but always should be 1
// usually we pass in firstX/Y lastX/Y so pointers make sense

void xyTransform(double * x, double * y, int sizeMultiplyer, double rotation){
    
    double newX, newY; // thank you cnlohr 

    if(rotation){
        newX = (*x * cos(rotation)) - (*y * sin(rotation));
        newY = (*y * cos(rotation)) + (*x * sin(rotation));
    } 
    newX *= sizeMultiplyer;
    newY *= sizeMultiplyer;

    *x = newX;
    *y = newY;

}
// calculates true bearing
double returnRotate(short x1, short y1, short x2, short y2){
    x2 = x2 - x1;
    y2 = y2 - y1;
    
    return atan2(x2, -y2);
    


}
// draws one boid
// takes the number of the boid in the struct,(double) transforms the shape to its place
// and then draws each segment (into int)
void drawBirb(short birdNo){ // eventually make it so it can draw multiple points onto one thing
    double firstX, firstY, lastX, lastY;
    
    firstX = (birbLayout[0][0]);
    firstY = (birbLayout[0][1]);
    // transform round 0,0
    xyTransform(&firstX, &firstY, BoidStruct[birdNo].size, -BoidStruct[birdNo].rotation*2*PI);
    // translate to right space
    firstX += BoidStruct[birdNo].x;
    firstY += BoidStruct[birdNo].y;
    
    CNFGColor( BoidStruct[birdNo].boidColour );
    for(int i = 1; i <= 4; i +=1){
        
        // set second point
        lastX = (birbLayout[i][0]);
        lastY = (birbLayout[i][1]);
        //printf("%f by %f\n", lastX, lastY);
        // transform it the same
        xyTransform(&lastX, &lastY, BoidStruct[birdNo].size, -BoidStruct[birdNo].rotation*2*PI);
        // translate to the right space
        lastX += BoidStruct[birdNo].x;
        lastY += BoidStruct[birdNo].y;
        //printf("%d by %d\n", lastX, lastY);
        CNFGTackSegment(firstX,firstY,lastX,lastY);

        // prepare for next loop
        firstX = lastX;
        firstY = lastY;
    }
    CNFGColor( 0xffffff );

}

// warms up the birds
void initBirbs(int w, int h){
    int boidCounter = 0; 
    int gridMultiplier = 100;
    int gridPadding = 75;


    // could be compressed into one loop but thats for later
    for(int i = 0; i < numberofBoids; i++){
        //printf("init birb %d\n", i);
        BoidStruct[i].x = (rand() % (w - gridPadding*2)) + gridPadding;
        BoidStruct[i].y = (rand() % (h - gridPadding*2)) + gridPadding;

        BoidStruct[i].size = 15;
        BoidStruct[i].rotation = 0;//rand() / (double)(RAND_MAX)*2*PI;

        BoidStruct[i].boidColour = rand();
    }
    // i wanted to evenly space out the boids in a grid but i gave up...
    /*
    for(double ybirb = 0; ybirb < (double)realH; ybirb += boidGridSideH){ 
        for(double xbirb = 0; xbirb < (double)realW; xbirb += boidGridSideW){ 
            // if it is in birb boudns set x/y
            printf("place birb %d \n", boidCounter);
            if(boidCounter < numberofBoids){
                
                printf("birb %fx%f\n", xbirb,ybirb);
                BoidStruct[boidCounter].x = round(xbirb)+ gridPadding;
                BoidStruct[boidCounter].y = round(ybirb)+ gridPadding;
                
                boidCounter++;
            } else {
                break;
            }
            // if not break

        if(!(boidCounter < numberofBoids))break;
        }
    }*/

}

void birbStep(short w, short h){ // one step forwards (should be called ~60 times a second)
    int boidSpeed = 10;

    double wantToPoint = 0.0;
    double rotateLimiter = 0.05; // ratio between current rotation and where it wants to point (maybe eventually can mix with collision)
    double newX = 0;
    double newY = 0;
    for(int i = 0; i < numberofBoids; i++){ 
        
        //init 

        //do stuff 
        
        // 3 steps
        // collision 

        // where do i want to point
        // calculate new direction 
        BoidStruct[i].rotation = (BoidStruct[i].rotation + 0.01);


        // move in direction 
        newX = BoidStruct[i].x + sin(BoidStruct[i].rotation*(PI/0.5))*boidSpeed;
        newY = BoidStruct[i].y + cos(BoidStruct[i].rotation*(PI/0.5))*boidSpeed;

        BoidStruct[i].x = newX;
        BoidStruct[i].y = newY;

        // keep it in bounds
        if(BoidStruct[i].x < 0) BoidStruct[i].x = w;
        if(BoidStruct[i].x > w) BoidStruct[i].x = 0;

        if(BoidStruct[i].y < 0) BoidStruct[i].y = h;
        if(BoidStruct[i].y > h) BoidStruct[i].y = 0;
        
        // draw (we can do this after we step the bird forward as we wont be moving it again this frame)
        drawBirb(i);

    }
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

    
	CNFGSetup( "Boids Rawdraw Demo", 1920, 1008 );
    // fullscreen because I dont want you to change the window size
    //CNFGSetupFullscreen("Boids Rawdraw Demo", );
    short w, h; // window size
    CNFGGetDimensions( &w, &h ); // updates the window size variables
    initBirbs(w,h);
    CNFGClearFrame();
	while(1)
	{
        iframeno++; // woo new frame! up the total frame number

		

		CNFGHandleInput();
        CNFGClearFrame(); // cleaning up last frame
        

		CNFGGetDimensions( &w, &h ); // updates the window size variables
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


        // draw hi
        CNFGPenX = stuffX; CNFGPenY = stuffY; 
        //CNFGDrawBox( paddingAmount, paddingAmount, 0 + paddingAmount, 0 + paddingAmount );
        CNFGColor( 0x000aff );
        //drawRegPoly(stuffX, stuffY, round(w/100), 75);
        CNFGColor( rand() );

        // okay lets do some birbs 
        birbStep(w, h);
        
        CNFGColor( 0xffffff );

        // all base stuff finished, drawing the rest of the ui elements

        // top left message 
        CNFGDrawTextbox(paddingAmount +10 ,20,"oh hello there", 4);


        // fps display
        sprintf(str, "%d", LastFPS);   //b g r
        if(LastFPS < 60) CNFGColor(0x0000ff); // if fps falls below 60

        CNFGPenX = 20.9; CNFGPenY = h -30;
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
        // /*
        SecToWait = .016 - ( ThisTime - LastFrameTime );
		LastFrameTime += .016;
		if( SecToWait > 0 )
			OGUSleep( (int)( SecToWait * 1000000 ) );
        // */
	}
}
