#include "clock.h"
:userdoc.
:title."Clock" Help
:body.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='clock.bmp' align=center.
:p.
This program is a substantial revision of Charles Petzold's Clock program,
which can be found in his book, "Programming the OS/2 Presentation Manager".
It displays an analog clock, updated once per second.
:p.
For Frank's Nursery, this program has been modified to monitor system task
count or processing load and to change its border color according to the
current level.
:p.
The help facility is active, as you've already seen, and those program
commands that exist may be accessed via the window's system menu.  The
following commands are available&colon.
:sl compact.
:li.:hpt.Save Defaults:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.Reset Defaults:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Hide Controls:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configure...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.About...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Close:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.In addition to those features already described, this program accepts
commands from the OS/2 2.0 Font and Color Palette programs.

:h1 res=IDM_SAVE_APPLICATION.Save Defaults (Menu Option)
:i1.Save Defaults (Menu Option)
When you select this menu option, the program saves its current position
and the status of the Hide Controls option.  The next time the program
is started, it will be started with that position and with the controls
hidden (or not) according to the saved state.
:note.The short-cut key for this command is F2.

:h1 res=IDM_RESET_DEFAULTS.Reset Defaults (Menu Option)
:i1.Reset Defaults (Menu Option)
Selecting this menu option will reset the program's font and color
attributes to their default values.

:h1 res=IDM_HIDE_CONTROLS.Hide Controls (Menu Option)
:i1.Hide Controls (Menu Option)
This menu option, when selected, will cause the program's frame controls
(the system menu, the titlebar and the minimize button) to be hidden.
This option can be toggled with a double-click on either mouse button.
Also, since I saw it as very useful to be able to move the window while
the controls were hidden, the window has been set up so that you can
drag it with either mouse button.
:note.The short-cut key for this command is Alt+H.

:h1 res=IDM_CONFIGURE.Configure (Menu Option)
:i1.Configure (Menu Option)
This menu option, when selected, will cause the program's configuration
dialog to be displayed.
For more information, see :hpt.Configure (Dialog):ehpt.:hdref res=IDD_CONFIGURE..
:note.The short-cut key to this option is Alt+C.

:h2 res=IDD_CONFIGURE.Configure (Dialog)
This dialog allows you to change all configurable aspects of this
program's operation.  To escape without effecting any changes, press
the ESCAPE key or click on the Escape pushbutton.
To save the changes you've made, press the ENTER key
or click on the OK pushbutton.
.br
The configurable items include the following:
:p.
:hp2.Clock Type:ehp2.
The clock face can be either analog (with hands) or digital.
When the clock is iconized, the face is always analog.
:p.
:hp2.24-Hour Clock:ehp2.
The digital clock can display time either as a 24-hour military-style
time, from 00&colon.00 to 23&colon.59, or as a 12-hour civilian time,
with 'am' and 'pm' appended to the numeric time.
:p.
:hp2.Hide Controls:ehp2.
When the clock is not iconized, it can be show with or without the usual
title bar, system menu and minimize/maximize buttons.  If you chose to
hide these controls, the sizing border remains.  The window may be
positioned by dragging its center area.  It may be resized by dragging
any of the four borders or the corners.
:p.
:hp2.Chime:ehp2.
This button activates an hourly chime function.  When this button has
been check-marked, the clock will beep twice at the top of every hour.
:p.
:hp2.Float to Top:ehp2.
This button activates an window's float-to-top function.
When this button has been check-marked, the clock will float to the top
of the window stack, making itself visible all the time.
:p.
:hp2.Animate:ehp2.
This button activates the window open &colon. close animation feature.
When this is active, the program window will open and close in the
same way as the folder objects do: with sound and motion.
As with folders, animation will not occur unless it has been
enabled at the system level.
:p.
:hp2.Display Seconds:ehp2.
This button causes the seconds to be displayed in the clock
when the clock is in digital form.  The second hand is always
displayed when the clock is in analog form and over a certain
size anyway.
:p.
:hp2.Alert Type:ehp2.
The clock's border changes color according to system load or task count.
You may select which item it will monitor.
:p.
:hp2.Alert Levels:ehp2.
You may adjust the task counts or CPU load levels at which the clock's
border will change from green to yellow and from yellow to red.  Use
the up or down arrows to adjust the values, or use the mouse to press
the up or down buttons to the right of the values.

:h1 res=IDM_ABOUT.About (Menu Option)
:i1.About (Menu Option)
This menu option, when selected, will cause the program's About dialog
to be displayed.
For more information, see :hpt.About (Dialog):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.About (Dialog)
This dialog box displays the program name, icon and copyright information.
To exit the dialog, press the ENTER key, the SPACE bar or the ESCAPE key.

:h1 res=IDM_EXIT.Close (Menu Option)
:i1.Close (Menu Option)
This menu option, when selected, will cause the program to terminate.

:h1 res=IDM_KEYS_HELP.Keys Help
:i1.Keys Help
The following function keys have been defined for this program&colon.
:sl compact.
:li.F1 - Help
:li.F2 - Save Defaults
:li.F3 - Exit
:li.Alt+H - Hide Controls
:li.Alt+C - Configure
:esl.

:h1 res=IDD_PROFILE_PATH.Set Profile Path
:i1.Set Profile Path
This dialog is displayed when the program cannot find its profile (INI) file,
and asks you where the file is or where it is to be created.
:p.
Only a valid existing directory name will be accepted.
Once you have entered the name, press the ENTER key or click on the OK
button for the program to continue.
:p.
If you wish to abort the program's initialization sequence, press
the ESC key or click on the escape button.

:euserdoc.
