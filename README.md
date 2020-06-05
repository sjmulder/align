align
=====
**align** **-C**|**c**|**R**|**r** [**-w** *width*]

Copies a file from standard input to standard output, indented with
spaces to be horizontally centered in the terminal.

Copies a file from standard input to standard output, indented with
spaces to be centered or right-align, depending on the chosen mode:

 - **-C**: Center as a block
 - **-c**: Center per line
 - **-R**: Right-align as a block
 - **-r**: Right-align per line

Tabs are expanded. The target width can be set with **-w** *width* and
defaults to the COLUMNS environment variable, or 80 if unset.

Examples
--------
Centering a file in block and per-line modes respectively:

    $ align -C <poem.txt

           Into the machine
           The soul descended
           Speaking to its creator
           Greeting the world

    $ align -c <poem.txt

              Into the machine
             The soul descended
           Speaking to its creator
             Greeting the world

Building
--------
Should work on Unix (Linux, BSD, macOS, ...) or on Windows with
minor changes:

    $ make
    $ sudo make install

Author
------
By Sijmen J. Mulder (<ik@sjmulder.nl>), June 2020. See LICENSE.md.
