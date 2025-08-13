# Amphora Memory Explorer

This is a basic memory explorer for the [Amphora Game Engine](https://github.com/calebstein1/amphora).
It allows read-only access to a running Amphora game's heap.

This program is currently only supported on MacOS and Linux, but Windows support is in development.
It also requires the latest development branch of Amphora which includes its custom allocator, this will not work with Amphora's current main branch.

## Usage

When launched, the program will automatically connect to a running Amphora game.
This requires that an Amphora game be running and that it did not have to fall back to a private anonymous heap for whatever reason.

Once connected, the following commands are supported:

- `dump [block]`: Dumps the entire contents of the specified memory block
- `peek [block] [index]`: Displays the value of the byte stored at the specified block, index pair
- `list_categories`: Shows a list of all memory blocks and their associated categories
- `clear`: Clears the screen
- `exit`: Quits the program

