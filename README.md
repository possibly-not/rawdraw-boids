# rawdraw-boids
 this is an quick and dirty implementation of the boids algorithm as first developed by Craig Reynolds

there are 3 main rule that guides every boid
>seperation
>alignment
>cohesion

eventually goal seeking with be adde for your mouse cursor


right now
>engine to time step one 
>one boid has, a 2d vector and an x/y
            ^direction+magnitude



do
>open window
>triangle (cone of vision too)
>time step
>moves one step on vector based on magnitude (should just go up)
>multiple boids (?)
>search for each other (can be optimised via squares but whatever)
>turning reduces magnitude
>