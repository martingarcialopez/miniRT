# miniRT

School project for 42 Paris. The subject -> [miniRT_subject](https://cdn.intra.42.fr/pdf/pdf/10458/en.subject.pdf)

A basic ray tracer using the CPU, featuring antialiasing via adaptative supersampling and a multithreaded render.
The supported primitives are spheres, planes, cylinders, squares, triangles, cubes and pyramids.
There aren't any acceleration structures like BVH, and although it supports triangles, a proper KD-tree for polygon meshes
acceleration were not included.

### Installation

> `git clone https://github.com/martingarcialopez/miniRT.git && cd miniRT && make`

The Makefile will compile the project properly on Linux and macOS machines.
It will detect the number of cores of your computer and will define the number of threads used to render accordingly.

### Usage

+ `./miniRT <scene.rt>` To render the .rt scene passed as a parameter

+ `./miniRT <scene.rt> --save` To save the .rt scene passed as a parameter to a .bmp file

### Example

`./miniRT scenes/cornellbox.rt`


======================&nbsp;&nbsp;&nbsp;**Some samples** (click on the images for larger size)&nbsp;&nbsp;&nbsp;======================

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/cornellbox.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/reflection.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/seanight.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/wolfside.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/compound_figures.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/columns.jpg?raw=true)

![alt text](https://github.com/martingarcialopez/miniRT/blob/master/jpg/colored_light.jpg?raw=true)
