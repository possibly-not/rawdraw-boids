# rawdraw-boids

this is an quick and dirty implementation of the boids algorithm using cnlohr's rawdraw

there are 3 main rule that guides every boid

> seperation  
> alignment  
> cohesion

eventually goal seeking with be added for your mouse cursor but lets not get ahead of ourselves  
test test test

whats working now
>basic window with fps counter, window size and   
>can draw regular polygons   
>can draw a bird-ish shape   
>bird struct   

CNFGDrawRegPoly(short sides, short radius) is now a thing inside rawdraw, but is never used
next
>rotation of bird shape  DONE
>make birds start with nice spacing  ALT (place random, then place all evenly on the left side)
>point towards mouse (in a function, not used/wonky)
>smoothly point towards mouse (not needed)
>get bird "moving" towards mouse > they move
>get birds moving alone, randomly in the void > DONE
>avoid crashing into edge  
>bird sensing other birds  
>bird sense with cone of vision (display this)   
>bird collision  
>add sliders  
>seperation  
>alignment  
>cohesion  


Other todos  
>limit window size after open/start
>clean up variable definitions  
>reoder functions    
>remove ununsed code  

maybe  
>make engine steps not tied to frame rate  

> basic window with fps counter, window size and  
> can draw regular polygons  
> can draw a bird-ish shape  
> bird struct

CNFGDrawRegPoly\(short sides, short radius\) is now a thing inside rawdraw, but is never used next

> rotation of bird shape  
> make birds start with nice spacing  
> get bird "moving"  
> avoid crashing into edge  
> bird sensing other birds  
> bird sense with cone of vision \(display this\)  
> bird collision  
> add sliders  
> seperation  
> alignment  
> cohesion

Other todos

> clean up variable definitions  
> reoder functions  
> remove ununsed code
> rawdraw webasm
> add to gitpage

maybe

> make engine steps not tied to frame rate



>no of boids based on resolution

>gol