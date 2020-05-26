<h1 style="text-align: center;"> Full-Scene-Animation Project</h1>
<h3 style="text-align: center;"> Submitted to: Dr. Ayman Eldeib</h3>
<h3 style="text-align: center;"> 2019 - 2020</h3>
<h4 style="text-align: center;"> Group ID: 05</h4>

| Name                    | Section | B.N Number   | E-mail                        |
|-------------------------|---------|--------------|-------------------------------|
| Ahmad Abdelmageed Ahmad | 1       |            8 | ahmedmageed19@gmail.com       |
| Ahmad Mahdy Mohammed    | 1       |            9 | ahmadmahdy3098@gmail.com      |
| Remon Alber Fely        | 1       |           31 | remonalbear522@gmail.com      |
| Salma Ayman Ahmed       | 1       |           37 | soayman9@gmail.com            |
| Abdullah Mohammed Sabry | 2       |            8 | abdullah.m.alrefaey@gmail.com |

<div style="page-break-after: always;"></div>

## Table of content
##### 1. Task Objectives
##### 2. Usage (Keyboard Shortcuts)
##### 3. Animations
##### 4. Texture Mapping 
##### 5. Objects
##### 6. Lighting & Color
##### 7. Issues
##### 8. Conclusion

<div style="page-break-after: always;"></div>

## Task Objectives
The main target of this project is:
* Applying three different types of animations to a full-robotic body.
* Loading different external objects.
* Applying texture mapping to the floor with different options.
* Applying Lighting & Coloring concept
* Make an interaction between the robotic body and an object.

## Description
Design a simulation for a full-scene animation for a robotic body. The scene consists of a robotic body standing on a floor and two different objects; a fixed table and a ball to play with. The target is to animate this body and interact with these objects with some movements to perform an animation process.

There are different types of animation applied in this project, some of them applied on the robot itself, and the other applied with the objects. In the following sections, we will explain each animation movement alone in more details.

## Basic Usage (Keyboard Shortcuts)
Using the keyboard in upper or lower case, you could test the movement of the body and its animation.
##### 1. Animations:
* Walking Forward : `'d'`
* Walking Backward :`'a'`
* Jumping :`'space'`
* Jumping Over :`'l'`
* Kick :`'k'`

##### 2. Body Transformations:
* Rotate the whole body: `'b'`
* Moving the left and right shoulder aside: `'y'` and `'r'`
* Moving the left and right shoulder forward: `'u'` and `'t'`
* Moving the left and right elbow: `'h'` and `'f'`
* Moving the left and right leg: `'z'` and `'g'`
* Moving the left and right knee: `'x'` and `'v'`

<br>

##### 3. Camera Movements:    
* Right: '&rarr;'
* Left: '&larr;'
* Up: '&uarr;'
* Down: '&darr;'
* Zoom In: `'+'`
* Zoom Out: `'-'`

#### Note On Using Keyboard
Performing any animation movement has to be waited till the end of it. If you click on any key that executes any type of movement i.e. if you pressed 'space' which makes the robot jumps, please wait till the robot jump to the air and return back to the ground then click on other button. 

This is just to let the animation be done successfully without any delay or corruption,  so please wait until the animation ends then perform the other movement.

## Animations
We applied three different types of animations to the whole scene. Some of the animations are related to the body without interacting with any external object and the others by interacting with an external objects such as the ball and table.

### The idea of the animation
The basic idea of applying an animation to a robotic body or to any object is to change its state or its position in different steps and swapping the buffer in each frame rapidly. This will make the scene looks like it's animating.

### Types of animations
- Walking forward
- Walking backward
- Jumping
- Jumping Forward
- Kicking a ball on the ground or above a table

Now lets talk about each type and see how it looks in the following GIFs.
### 1. Walking Forward
To apply this animation, three joints is rotated (hip, knee, shoulder) with increasing the coordinates of the body in the same direction he wants to move along. Also this movement is limited by the dimensions of the ground so the robot can't walk outside the floor.

<img src="results/WalkingForward.gif" alt="Walking Forward" width="400" height="300">

### 2. Walking Backward
The same concept as walking forward but the joints rotates in the opposite direction as well as the coordinates of the body decreases in the opposite direction.

<img src="results/WalkingBackward.gif" alt="Walking Backward" width="400" height="300">

### 3. Jumping
As shown on the gif three joints (knee, elbow and shoulder) are rotated while jumping and the coordinates of the body is changing and return to the original state.

<img src="results/Jumping.gif" alt="Jumping" width="400" height="300">

### 4. Jump Forward
This animation is made by mixing between the jumping and moving forward animations.

<img src="results/JumpingOver.gif" alt="Jumping Over" width="400" height="300">

### 5. Kicking a ball
This type of animation has 2 cases, one without interacting with the table and the other by interacting with it.

##### Case #1: Kicking the ball on the ground
This is applied by moving the right leg backward then forward and after it reaches the ball, the ball is kicked as shown in the GIF. As you see the distance between the ball and the table is too large, so the ball will fall on the ground.

<img src="results/KickOnGround.gif" alt="Kick On Ground" width="400" height="300">

##### Case #2: Kicking the ball on the table
This is the same animation as above exactly but here the ball is close to the table, so after kicking it, the ball will fall on the table.

<img src="results/KickOnTable.gif" alt="Kick On Table" width="400" height="300">

## Texture Mapping
We applied three different types of texture with the ability of changing between them using a drop menu pinned to the right mouse button.

Images are saved in a BMP format with dimensions of `128*128` pixels. Then it's loaded and pinned to it's texture ID, which is then rendered to fit the vertices of the floor square. Repeat Mode is activated allowing the compiler to repeat the `128*128` pixels along the whole floor. 

<img src="results/TextureMapping.gif" alt="Texture Mapping" width="400" height="300">

## Loading External Objects
As shown from the previous GIFs we uploaded two different objects. Using glm library allows us to load external object files of extension `.obj` which uses `.mtl` files to color the object.

## Lighting & Color
Two Light sources are applied with a diffuse mode and a white light, turning the scene from complete darkness to light.

<br>

[![Little red ridning hood](http://i.imgur.com/7YTMFQp.png)](https://vimeo.com/3514904 "Little red riding hood - Click to Watch!")

with a **youtube** video embedded

## Issues
| Issue                                                                   | Solution                             |
|-------------------------------------------------------------------------|--------------------------------------|
| Reflection of the view when moving the camera under the floor           | Applying the lighting and coloring   |
| Some textures were not showing as they should be as the real JPEG image | Converting the bit depth of the .bmp image from 32 to 24 |
| Linking the animation of the body with the object                       | Calculation of the difference distances between them and some conditions |

## Conclusion
Briefly, the idea of this project is to apply the animation concept on a full-scene with a robotic body standing on a floor using `C++` with `openGL` API, `GLUT` and `GLM` libraries. We applied some features such as animating the robotic body and interacting with a ball. Also the ball interact with the table object. Three textures were added to the scene with an option to choose between anyone of them.