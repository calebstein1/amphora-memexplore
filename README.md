# Amphora Memory Explorer

This is a basic memory explorer for the [Amphora Game Engine](https://github.com/calebstein1/amphora).
It allows read-only access to a running Amphora game's heap.

This program is currently only supported on MacOS and Linux, but Windows support is in development.
It also requires the latest development branch of Amphora which includes its custom allocator, this will not work with Amphora's current main branch.

## Usage

- `memexplore -d <block>`: dumps the entire contents of a memory block to the console
- `memexplore -p <block,index>`: shows the value of the byte stored at block, index
- `memexplore -l`: list all blocks with their associated memory caategories
