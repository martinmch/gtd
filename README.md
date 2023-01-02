# gtd

> A simple c program for effective time management.

This is a remake of [gtd](https://github.com/connermcd/gtd/) by connermcd.

## Flags

- `-b` : start on a break
- `-c` : custom command (defaults to "clear")
- `-m` : toggle MPD on change
- `-n` : notify on change
- `-s` : speak command
- `-t` : show time in tmux status bar
- `-T` : update time in /tmp file
- `-e` : specify time to end at (e.g. 2:45pm today) **(Missing)**
- `-f` : specify a configuration file **(Missing)**

## Usage

    gtd [ -bcmnstTef ] [ work length ] [ break length ]

The program loops between two states, work and break. Supplied with two
arguments, `gtd 25 5`, the program will run with 25 minutes of work seperated
by 5 minute breaks.
