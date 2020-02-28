# Particles

Simple particle system written in C99 & OpenGL 4.5. All the particle data is located only on GPU memory and particles are updated using compute pipeline. Rendering is done with a single instanced draw call.

Particle parameters include start and end color, lifetime, start speed, acceleration, start size, scaling rate, emission rate, gravity, direction and position.

## Build

Clone with submodules

`git clone --recurse-submodules https://github.com/jparimaa/particles.git`

Configure and build, for example

`cmake . && make`

Run!

## Tools

As submodules
- cglm
- glfw
- stb

Other
- glad

## Screenshot

![image](image.png?raw=true "image")
