#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./rawdraw/os_generic.h"


#define CNFG_IMPLEMENTATION
#include <time.h>
#include "./rawdraw/CNFG.h"

unsigned frames = 0;
unsigned long iframeno = 0;

void HandleKey( int keycode, int bDown )
{
	if( keycode == 27 ) exit( 0 );
	printf( "Key: %d -> %d\n", keycode, bDown );
}
void HandleButton( int x, int y, int button, int bDown )
{
	printf( "Button: %d,%d (%d) -> %d\n", x, y, button, bDown );
}
void HandleMotion( int x, int y, int mask )
{
//	printf( "Motion: %d,%d (%d)\n", x, y, mask );
}
void HandleDestroy()
{
	printf( "Destroying\n" );
	exit(10);
}
int main()
{
    int i, x, y;
    double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	double LastFrameTime = OGGetAbsoluteTime();
	double SecToWait;
    int linesegs = 0;

    

	CNFGSetup( "Boids Demo", 150, 150 );
    
	while(1)
	{
        iframeno++;

		short w, h;
        if (iframeno%2000==0){
		    CNFGClearFrame();
        }
		CNFGHandleInput();
		CNFGGetDimensions( &w, &h );
		CNFGColor( 0xffffff );
		
        char str[12];
        //sprintf(str, "%f", elapsed_time_milli*1000);

        CNFGPenX = iframeno%1000; CNFGPenY = (short)(50*sin((float)(i+iframeno)*.01) + (i%20)*30);
		CNFGDrawText( "hi" , 4 );

        sprintf(str, "%d", iframeno);
        CNFGPenX = 10; CNFGPenY = 40;
        CNFGDrawText( str , 4 );

        frames++;

		CNFGSwapBuffers();




        ThisTime = OGGetAbsoluteTime();
		if( ThisTime > LastFPSTime + 1 )
		{
			printf( "FPS: %d\n", frames );
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