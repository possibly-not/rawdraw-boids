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
#define windowW 1920    //1024
#define windowH 1008    //576
#define numberofBoids 100
unsigned frames = 0;
unsigned long iframeno = 0;

int mouseX = 0; 
int mouseY = 0;
int mouseMask = 0;
int precision = 1; // i forgot what this is used for in handlebutton()

// this really should be integers? i.e times alll by 100 and call it a day
// okay but 0.75 and 0.25 can be stored perfectly as floats so *shrug*
double birbLayout[5][2] = {{0,-1},{-0.75,1},{0,0.25},{0.75,1},{0,-1}}; 
//double birbLayout[5][2] = {{0,1},{-0.75,-1},{0,-0.25},{0.75,-1},{0,1}};


struct Boids{ 
    short size; // size mulitplyer for drawing
    int x;
    int y;
    int edgeScared;
    double rotation; // radians 0 to 2*PI
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
    
    // dont deal with wrap arround
    if(rotation > 0){
        //newX = (*x * cos(rotation)) - (*y * sin(rotation));
        //newY = (*y * cos(rotation)) + (*x * sin(rotation));
        newX *= sizeMultiplyer;
        newY *= sizeMultiplyer;
    } else {
        newX = *x * sizeMultiplyer;
        newY = *y * sizeMultiplyer;
    }
    *x = newX;
    *y = newY;

}

// calculates true bearing of two points
// radians
double returnRotate(short x1, short y1, short x2, short y2){
    x2 = x2 - x1;
    y2 = y2 - y1;
    
    return atan2(x2, -y2);
}

// return distance between two points
double returnDistance(short x1, short y1, short x2, short y2){
    double distance = 0; 
    // Pythagorean theorem 
    // works out hyptotenous
    distance = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
    return distance;
}

// rotation to the opposite way
// radians
int returntoComfort(short x1, short y1, short x2, short y2){
    // if uncomfortable
    return returnRotate(0, 0, -(x2-x1), -(y2-y1)); 

}

// returns if it is too close to the edges based on comfortsize
int returnEdgeComfort(short x1, short y1, short w, short h, int comfortSize){
    // if uncomfortable
    if(x1-comfortSize < 0 || y1-comfortSize < 0 || x1+comfortSize > w || y1+comfortSize > h){
        return 1;
    }
    return 0;
}

double mixRotation(double currentRotation, double rotateCohesion, double rotateAlignment, double rotateSeperation){

    // currently "disabled"
    return (double)(currentRotation );
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

// draws one boid
// takes the number of the boid in the struct,(double) transforms the shape to its place
// and then draws each segment (into int)
void drawBirb(short birdNo){ // eventually make it so it can draw multiple points onto one thing
    double firstX, firstY, lastX, lastY;
    

    // setting up first point
    firstX = (birbLayout[0][0]);
    firstY = (birbLayout[0][1]);
    // transform round 0,0
    xyTransform(&firstX, &firstY, BoidStruct[birdNo].size, -BoidStruct[birdNo].rotation);
    // translate to right space
    firstX += (double)BoidStruct[birdNo].x;
    firstY += (double)BoidStruct[birdNo].y;
    
    CNFGColor( BoidStruct[birdNo].boidColour );
    for(int i = 1; i <= 4; i +=1){
        
        // set second point
        lastX = (birbLayout[i][0]);
        lastY = (birbLayout[i][1]);

        // transform it the same
        xyTransform(&lastX, &lastY, BoidStruct[birdNo].size, -BoidStruct[birdNo].rotation);
        // translate to the right space
        lastX += BoidStruct[birdNo].x;
        lastY += BoidStruct[birdNo].y;
        
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
    double birbSpacingH = (h - (2*gridPadding))/numberofBoids;


    // places boids randomly
    for(int i = 0; i < numberofBoids; i++){
        
        // place boids left side + nicely spaced
        //BoidStruct[i].x = gridPadding;    //(rand() % (w - gridPadding*2)) + gridPadding;
        //BoidStruct[i].y = birbSpacingH * i + gridPadding;//(rand() % (h - gridPadding*2)) + gridPadding;

        // place boids randomly 
        BoidStruct[i].x = (rand() % (w - gridPadding*2)) + gridPadding;
        BoidStruct[i].y = (rand() % (h - gridPadding*2)) + gridPadding;

        BoidStruct[i].size = 15;
        BoidStruct[i].rotation = 0.0;

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

// one step forwards (should be called ~60 times a second)
// this does a lot of things and is messy + unoptimised bc of all the loops, increasing number of boids will greatly decrease speed 
void birbStep(short w, short h){ 
    int boidSpeed = 1;

    // must be < number of boids
    int considerationSize = 150; // how many of the nearest boids it considers to be worthwhile to calculate
    // used for updating x, y
    double newX = 0;
    double newY = 0;

    //rules 
    for(int i = 0; i < numberofBoids; i++){ 
        
        //init 
        int totalX = 0 ;
        int totalY = 0 ;
        int totalLocal = 0;
        
        // init final values that will make its way into the rotation
        double rotateCohesion = 0.0;
        double rotateAlignment = 0.0;
        double rotateSeperation = 0.0;

        int edgeComfortSize = 40; // space for the walls
        int boidComfortSize = 40; // space between boids
        int boidComfortCount = 0; // used to average out the emptiest direction
        double totalBoidComfort = 0; // add it all here, average out into rotateSeperate
        double totalRotation = 0.0;

        double initialRotation = BoidStruct[i].rotation;

        //do stuff 
        CNFGColor( 0x777777);
        drawRegPoly(BoidStruct[i].x,BoidStruct[i].y, 35, considerationSize);
        // gather numbers for rules in local area 
        for(int z = 0; z < numberofBoids; z++){
            // if not current boid
            if(z != i){
                // if in local space
                if(returnDistance(BoidStruct[i].x, BoidStruct[i].y, BoidStruct[z].x, BoidStruct[z].y) < considerationSize){ 
                    // draw line to a boid (shows what a boid is considering)
                    CNFGColor( 0xffaaaa);
                    CNFGTackSegment(BoidStruct[i].x, BoidStruct[i].y, BoidStruct[z].x, BoidStruct[z].y);

                    // adding to local x/y average
                    totalX += BoidStruct[z].x;
                    totalY += BoidStruct[z].y;
                    totalLocal++;
                    // adding to local rotation average
                    totalRotation += BoidStruct[z].rotation;

                    // SEPERATION
                    if(returnDistance(BoidStruct[i].x, BoidStruct[i].y, BoidStruct[z].x, BoidStruct[z].y) < boidComfortSize){   
                        // show boundary has been crossed
                        CNFGColor( 0xaaaaff);
                        drawRegPoly(BoidStruct[z].x,BoidStruct[z].y, 15, boidComfortSize);

                        // add to total
                        totalBoidComfort += returntoComfort(BoidStruct[i].x, BoidStruct[i].y, BoidStruct[z].x, BoidStruct[z].y);
                        
                        // ++ to count
                        boidComfortCount++;
                    
                    }
                }
            }
        }

        //////// Cohesion
        rotateCohesion = returnRotate(BoidStruct[i].x, BoidStruct[i].y, totalX/(numberofBoids - 1), totalY/(numberofBoids - 1)); // minus one as we ignore the current boid

        //////// Alignment
        rotateAlignment = totalRotation / (numberofBoids - 1);

        //////// Separation
        // if colliding with others
        if(totalBoidComfort) rotateSeperation = totalBoidComfort / (boidComfortCount); 

        /*/ if near the edge (padding amount)
        if(returnEdgeComfort(BoidStruct[i].x, BoidStruct[i].y, w, h, edgeComfortSize) || BoidStruct[i].edgeScared){
            // rotate to the centre instead of using rules
            if(BoidStruct[i].edgeScared <= 0) BoidStruct[i].edgeScared = 60;

            BoidStruct[i].rotation = (BoidStruct[i].rotation*2 + returnRotate(BoidStruct[i].x, BoidStruct[i].y, w/2,h/2)/3);
            BoidStruct[i].edgeScared--;
        } else; {
            // use rule rotation
            
        }*/
        
        // keep rotation within number bounds
        //BoidStruct[i].rotation += 0.01;

        if(totalBoidComfort){
            BoidStruct[i].rotation = (rotateSeperation);

        }else if (totalLocal > 4){

            BoidStruct[i].rotation = (rotateAlignment);
        }else{
            BoidStruct[i].rotation = (rotateCohesion);
        }

        if(BoidStruct[i].rotation > 2*PI) BoidStruct[i].rotation = 0;


        // move in direction 
        newX = sin(BoidStruct[i].rotation*2*PI)*boidSpeed;
        newY = cos(BoidStruct[i].rotation*2*PI)*boidSpeed;

        BoidStruct[i].x += newX;
        BoidStruct[i].y += newY;

        char str[30];
        sprintf(str, "%f", BoidStruct[i].rotation);
        CNFGPenX=BoidStruct[i].x; CNFGPenY=BoidStruct[i].y;
        CNFGDrawText(str, 3);

        //BoidStruct[i].rotation += 0.001;
        // walls are ENFORCED
        if(1){
            if(BoidStruct[i].x < 0) BoidStruct[i].x = 0;
            if(BoidStruct[i].x > w) BoidStruct[i].x = w;

            if(BoidStruct[i].y < 0) BoidStruct[i].y = 0;
            if(BoidStruct[i].y > h) BoidStruct[i].y = h;

        }
        // looping walls (unused)
        if(0){
            if(BoidStruct[i].x < 0) BoidStruct[i].x = w;
            if(BoidStruct[i].x > w) BoidStruct[i].x = 0;

            if(BoidStruct[i].y < 0) BoidStruct[i].y = h;
            if(BoidStruct[i].y > h) BoidStruct[i].y = 0;
        }


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

    // make random more random
    
    srand(time(NULL));
	CNFGSetup( "Boids Rawdraw Demo", windowW, windowH );

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

        CNFGColor( rand() );

        // okay lets do some birbs 
        birbStep(w, h);
        // and draw the birbs
        for(int i = 0; i < numberofBoids; i++) drawBirb(i);
        

        // all base stuff finished, drawing the rest of the ui elements

        // top left message 
        CNFGColor( 0xffffff );
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
