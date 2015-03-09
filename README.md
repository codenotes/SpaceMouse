# SpaceMouse
SpaceMouse is cool, but its software and drivers make life difficult. Their stuff tries to do too much, and "helps" in ways you'd prefer it not.  For example, automatically directing the mouse input only to the application that presently has focus.  This is a major issue if you need the spacemouse to be dedicated always to one particular task (like controlling a robot) regardless of the application you are using at the time. 

So have written a win32 layer that talks to mouse directly, and will feed forward to a vJoy virutal joystick so as to be able to use 3d spacemouse in anything.  The vJoy joystick is a generic proxy we can then use in any application, without having to write a specific per-applicaiton interface to each App.
