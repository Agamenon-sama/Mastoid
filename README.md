# Mastoid

Mastoid is a file based music player.

There is no accounts, no subscriptions, no need for internet access, no complex music folders... Just organize your files in a decent hierarchical layout and enjoy.

## To do

- music player
    - implement playlists
    - make it possible to change the seek time of the player
    - change the look of the progression bar and volume bar
    - re-arrange the layout
    - get better icons
- file browser
    - fix the title when the back button is pressed
    - maybe filtering non-relevant files
    - better ui
- image view
    - make it possible to accept jpg covers and set the default cover if no pic is provided (requires C++ so I'll leave it for later)
- settings menu
    - I should start by thinking how to integrate it in the ui
- spectrum viewer ?
- keyboard shortcuts

The ui in general needs to lot of work still, but I'll focus on the features first.

## Dependencies

This application is built with Qt6 and QML.

## Build

Preferably, install Qt Creator and open the `CMakeLists.txt` and build it from there. That's the easiest way.
