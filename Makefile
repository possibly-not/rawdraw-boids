all : boids 

#for X11 consider:             xorg-dev
#for X11, you will need:       libx-dev
#for full screen you'll need:  libxinerama-dev libxext-dev
#for OGL You'll need:          mesa-common-dev libglu1-mesa-dev

#-DCNFGRASTERIZER
#  and
#-CNFGOGL
#  are incompatible.


MINGW32:=/usr/bin/i686-w64-mingw32-

boids : boids.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g -DCNFGRASTERIZER


clean : 
	rm -rf *.o *~  boids 

 
