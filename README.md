# KBC
This a simple quiz game I wrote in C++. It has a sample quiz file with 6 questions. You can add more questions or even create your own quiz file.

## Format of quiz file
The quiz file must strictly be in the following format:
1st question
Option A
Option B
Option C
Option D
Correct option
2nd question
Option A
....

## Building the project
Code::Blocks + MinGW project files are provided within the repository. It's easy to port on other build systems/IDEs.
This project is WINDOWS-DEPENDENT.

Dependencies:
1. [FreeGLUT] (https://freeglut.sourceforge.net) (link freeglut_static.lib or freeglut_static.a)
2. GDI+
3. OpenGL 1.1 (Needed for FreeGLUT only)
4. GLU32

Windows specific library dependencies include:
1. gdi32
2. user32
3. kernel32
4. pthread
5. winmm
6. comctl32
7. comdlg32
8. msimg32
