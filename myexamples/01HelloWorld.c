//Make it so we don't need to include any other C files in our build.
#define CNFG_IMPLEMENTATION

//this should point to the right place
#include "../rawdraw/CNFG.h" 

void HandleKey( int keycode, int bDown ) { }
void HandleButton( int x, int y, int button, int bDown ) { }
void HandleMotion( int x, int y, int mask ) { }
void HandleDestroy() { }

int main()
{   
	CNFGSetup( "Example App", 1024, 768 );
	while(1)
	{
		short w, h;
		CNFGClearFrame();
		CNFGHandleInput();
		CNFGGetDimensions( &w, &h );
		CNFGColor( 0xffffff );
		CNFGPenX = 10; CNFGPenY = 10;
		CNFGDrawText( "Hello, World", 2 );
		CNFGSwapBuffers();
	}
}