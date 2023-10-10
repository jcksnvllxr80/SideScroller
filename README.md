# SideScroller Demo in Unreal Engine 5.2

Just messing around with unreal engine and c++ to create a mario like game.

![Game Image](Content/Assets/Images/SideScroller.png)

---

## TODO:

#### Bugs:

- bouncing when leaving a ladder out the top
- fireball hit knocks player the wrong direction
- fireballs go through walls
- sometimes fireballs just move the player and no hurt
- game crashes if player dies (error with the pointsInterface)

#### Features:

- enemy projects spawn points
    - rotate with their direction
    - outside of their bodies
- kill z
- start menu
    - multiplayer
        - 2, 3, & 4 player options (make player selection set players in lobby before start)
        - add player selection (select the color)
    - settings
        - resolution
- new levels
- add lives
- add 1-ups (extra life)
- make AI patrol instead of always following player
- add points system for killing enemies etc.
- add midpoint saves of levels
- make slippery and bouncy surfaces with different materials
  https://www.youtube.com/watch?v=2rB8LpxI9Ww
- game saves
- slide down hills when moving and crouching on a hill
- esc key calls in game menu and set time dilation to 0
- add item feedback animation to pickups
- implement the frog jump and fall flipbook on the frog and get AI to make him jump

#### Done:

~~- add other color fox animations to blueprints. one BP for each color so they can have unique properties (fast, light, acceleration, etc)~~  
~~- add separate animations for the players jump when going up and one when down~~  
