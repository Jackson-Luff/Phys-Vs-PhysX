Phys vs PhysX (The difference is an X ;D )
================================================================
RESOURCES:

Built and compiled in Visual Studio 2013
Redistributional: 
http://www.microsoft.com/en-au/download/details.aspx?id=40784

Additional Resources needed for compile:
 - AntTweakBar 	: version 116 	    | x86 vc12 compatible | GUI Elements 			(http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:download)
 - FBXLoader 	: version x         | x86 vc12 compatible | AIE's custom FBXLoader 		()
 - FMod		: version 1.06.07   | x86 vc12 compatible | FMOD Low Level Programmer API 	(http://www.fmod.org/download/)
 - GL_core	: version 4.4 	    | x86 vc12 compatible | The core essentials of GLUT 	(https://www.opengl.org/registry/doc/glspec44.core.pdf)
 - GLFW		: version 3.1.1     | x86 vc12 compatible | For all rendering purposes 		(http://www.glfw.org/download.html)
 - GLM		: version 0.9.6.3   | x86 vc12 compatible | C++ Math library 			(http://sourceforge.net/projects/ogl-math/)
 - PhysX	: version 9.15.0428 | x86 vc12 compatible | NVIDIA PhysX engine/API		(http://www.nvidia.com/object/physx-9.15.0428-driver.html)
 - STB		: version x	    | x86 vc12 compatible | Loads in image data			(https://github.com/nothings/stb)'

================================================================
INSTALLATION (Release):

Within Release Build (I supply this but for later builds)
DLL's needed WITHIN directory of project .exe
 - AntTweakBar.dll
 - fmod.dll
 - glfw.dll
 - nvToolsExt32_1.dll
 - PhysX3_86.dll
 - PhysX3CharacterKinematic_x86.dll
 - PhysX3Common_x86.dll
 - PhysX3Cooking_x86.dll
 - PhysX3Gpu_x86.dll

NOTE: Something to note that the 'Data' file should be in
      the same directory as the project .exe

INSTALLATION (For project):

In the same directory as the GenGein Solution, Create a folder named
'Dependencies'within this with the following folders for resources:
 'AntTweak'
 'FBXLoader'
 'FMOD'
 'gizmos'
 'gl_core'
 'glew'
 'glfw'
 'glm'
 'PhysX'
 'stb-master'

Once you've created these files, within them an
'include' and 'lib' folder. Of course this is where you put
 the given resources.

IMPORTANT NOTE:
for GML there is a folder called 'glm' instead of the include
(This was for development purposes to clean my #includes)

for GLFW the lib folder's name is 'lib13'. This was to differ from
other GLFW visual studio versions.

================================================================

CONTROLS:

- Within Phys' environment
 W/A/S/D 		  - Move around
 Right Click / Mouse Move - Rotate camera
 Left Click		  - Spawn spheres

- Within PhysX's environment
 W - Move forward at 4x    speed
 S - Move forward at 0.25x speed
 Right Click / Mouse Move - Rotate camera
 Left Click		  - Spawn spheres

- Everything else
 W/A/S/D 		  - Move around
 Right Click / Mouse Move - Rotate camera

================================================================