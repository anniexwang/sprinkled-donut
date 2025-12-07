# Showcase Extra Credit

## Title 

**Name of Piece:** [Sprinkled Donut](/images/piece.mov)

## Personal Information

**Full Name:** Annie Wang

**UtorID:** wanga102

**Student Number:** 1008845369

**Assignment Number Augmented:** A5

## Instructions

To run the program and reproduce the output file, run the following commands:

```
mkdir build-release
cd build-release
cmake ../ -DCMAKE_BUILD_TYPE=Release
make
./sprinkled_donut
```

## Description

**Features:** 
- Donut shape:

    - File(s): `src/donut.cpp`, `src/donut.h`
    - Details: The donut shape was created using sphere.cpp as a base, adding values R and r for the outer and inner radius of the shape. x, y and z calculations incorportated R and r and the normal of the donut was calculated since it is not the same as the spherical coordinates. 

- Sprinkles:
    
    - File(s): `sprinkled_donut.cpp`
    - Details: Each sprinkle is a sphere, with `num_sprinkles` number of sprinkles on the donut. The sprinkles are only on the top of the donut. The colour and position of each sprinkle is chosen at random.

- Sprinkling:

    - File(s): `sprinkled_donut.cpp`
    - Details: This is the physics animation of the sprinkles getting "sprinkled" onto the donut. It falls at a velocity based on gravity and stops when it hits the surface on top of the donut.

## Acknowledgements

/data/glazed_donut.png - https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSnXDGy2nwBLL2Is7rIqVDQkR6chGN1zNbDkg&s

## Compilation Verification
[Compilation Verification](/images/Compilation_Validation.mov)