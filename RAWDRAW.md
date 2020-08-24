/home/uhh/Documents/GitHub/rawdraw-boids/rawdraw/CNFGXDriver.c
    void CNFGDrawText( const char * text, short scale )

    void CNFGDrawBox( short x1, short y1, short x2, short y2 )

    void CNFGDrawTextbox( int x, int y, const char * text, int textsize )


    uint32_t CNFGColor( uint32_t RGB )
    void CNFGClearFrame()
    void CNFGTackSegment( short x1, short y1, short x2, short y2 )
    void CNFGTackPixel( short x1, short y1 )
    void CNFGTackPoly( RDPoint * points, int verts )

    void CNFGInternalResize( short x, short y ) { }

    void CNFGSetLineWidth( short width )


/home/uhh/Documents/GitHub/rawdraw-boids/rawdraw/CNFGXDriver.c
    void 	CNFGSetWindowIconData( int w, int h, uint32_t * data )

    void CNFGGetDimensions( short * x, short * y )

    void CNFGTearDown()

    void CNFGUpdateScreenWithBitmap( uint32_t * data, int w, int h )




huh i think i figured out my c workflow, visual studio code with the terminal open inside and use make;./binary 