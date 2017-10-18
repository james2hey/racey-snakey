Racey Snakey
A two player game based off the classic Nokia mobile phone snake game. However, 
this game has two snakes. It is a race to eat the most food so you grow faster 
than your opponent. Try to make your opponent hit your tail. Beware, you 
can collide with your own tail as well!

Prerequisites
1). It is assumed that you are using a Linux OS. Unexpected results may occur
    otherwise.
2). Ensure you have two functioning ATmega32u2 microcontrollers, along with the
    USB cables for the UCFK4.
3). Refer to http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/UCFK4 for
    installation of the AVR compilation tools etc.

Installing
1). Open a terminal window so you can access the command line.
2). Navigate to your desired installation directory.
3). Clone Stephen Weddell's open source ence260-ucfk4 directory, using
    'git clone https://eng-git.canterbury.ac.nz/steve.weddell/ence260-ucfk4.git'
4). Navigate to the 'ence260-ucfk4' directory.
4). Navigate to the 'assignment' directory.
5). Clone Team 426's project into this directory, using
    'git clone https://eng-git.canterbury.ac.nz/ence260-2017/team426.git'.

Deployment
1). Plug in both of your UCFK4's into your computer, the green LED's should
    light up when they have been turned on.
2). Navigate into the team426 directory that you have now installed.
3). To build the program, use 'make program' for each of the UCFK4's. They
    should both display a message for the game to begin.
4). If any problems arise, ensure that you are in the team426 directory and
    that the makefile in this directory being run.
5). Call 'make clean' to clear all of the newly generated files after use.

Built With
Geany - The IDE used.
ence260-ucfk4 - The modules our created moduals use.

Authors
Gerry Toft & James Toohey, Team 426.
