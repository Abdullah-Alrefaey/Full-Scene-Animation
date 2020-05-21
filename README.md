# Full-Scene-Animation

| Name                    | Section | Bench Number |
|-------------------------|---------|--------------|
| Ahmad Abdelmageed       | 1       |            8 |
| Ahmad Mahdy             | 1       |            9 |
| Remon Alber             | 1       |           31 |
| Salma Ayman Ahmed       | 1       |           37 |
| Abdullah Mohammed Sabry | 2       |            8 |

# Camera Movement Task

## Table of content
##### 1. Personal Information
##### 2. Task Objectives
##### 3. Usage (Keyboard Shortcuts)
##### 4. Animations
##### 5. Texture Mapping 
##### 6. Objects
##### 7. Lighting
##### 8. Conclusion

## Task Objectives
The main target of this project is to animate the full body robot from the previous tasks using three different animations , load different objects and interact with them by the robot ,aplly differrent texture mapping and lighting.

## Description
This project aims to

## Usage (Keyboard Shortcuts)
Using the keyboard in upper or lower case, you could test the movement of the body.
1. Animations:
    * Walk Forward : `'d'`
    * Walk Backward :`'a'`
    * Jump :`'space'`
    * Jump Over :`'l'`
    * Kick :`'k'`
2. Camera Movements:    
    * Moving Camera Right: '&rarr;'
    * Moving Camera Left: '&larr;'
    * Moving Camera Up: '&uarr;'
    * Moving Camera Down: '&darr;'
    * Moving Forward: `'+'`
    * Moving Backward: `'-'`
3. Body Transformations:    
    * Moving the whole body: `'b'`
    * Moving the left and right arm: `'r'` and `'q'`
    * Moving the left and right elbow: `'f'` and `'a'`
    * Moving the left and right leg: `'e'` and `'w'`
    * Moving the left and right knee: `'d'` and `'s'`
    * Moving the left fingers base, `'t'`, `'u'`, `'o'`, `'g'`, `'j'`.
    * Moving the left fingers up, `'y'`, `'i'`, `'p'`, `'h'`, `'k'`.
    * Moving the right fingers base, `'z'`, `'c'`, `'n'`, `'['`, `';'`.
    * Moving the right fingers up, `'x'`, `'v'`, `'m'`, `']'`, `'/'`.

## Animations
We made three animations using timers and some conditions which are walk (forward , backward),jump (in place,jump over) and kick which interact with a ball object 

### 1. Walk Forward
Three joints rotates to make this animation (hip, knee, shoulder),
Also ,this movement is limited by the size of the ground, so , the robot can't walk outside the floor

![move foreward](images/forward.gif)

### 2. Walk Backward
Same as the forward but in the opposite direction.

![move backeward](images/backward.gif)

### 3.Jump
As shown on the gif three joints rotate while jumping (knee , elbow , shoulder)

![jump](images/jump.gif)

### 4. Jump Over
This animation is mix between the jump and move forward animations 
![Jump Over](images/jumpover.gif)

### 5. Kick
This Animation interact with a ball object 

![Kick ](images/kick.gif)

here the ball object also interact with another object (table)  

![Kick above table ](images/kick1.gif)



## Texture Mapping
We apply three different textures ,and we change between them using
a menu binned to the right mouse button .

![texture](images/texture.gif)

## Objects
As shown from the previous gifs we uploads two different objects.  

## Lighting

## Conclusion
