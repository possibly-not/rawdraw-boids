//Copyright (c) 2011 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./rawdraw/os_generic.h"
#define CNFG_IMPLEMENTATION


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

#define HMX 40
#define HMY 40
short screenx, screeny;



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

	CNFGSetup( "Test Bench", 640, 480 );

	while(1)
	{
		int i, pos;
		float f;
		iframeno++;


		CNFGClearFrame();
		CNFGColor( 0xFFFFFF );


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

		/*SecToWait = .016 - ( ThisTime - LastFrameTime );
		LastFrameTime += .016;
		if( SecToWait > 0 )
			OGUSleep( (int)( SecToWait * 1000000 ) );*/
	}

	return(0);
}

