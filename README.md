center
======
**center** [**-w** *width*]

Copies a file from standard input to standard output, indented with
spaces to be horizontally centered in the terminal.

The target width can be set with **-w** *width* and defaults to the
`COLUMNS` environment variable, or 80 if unset. Tabs are converted to
spaces. If the input stream does not support seeking it is first copied
to an unnamed temporary file.

Example
-------
    $ center -w 2 <hello.txt
                                 Hello, World!

Building
--------
Should work on Unix (Linux, BSD, macOS, ...) or on Windows with
minor changes:

    $ make
    $ sudo make install

Author
------
By Sijmen J. Mulder (<ik@sjmulder.nl>), June 2020. See LICENSE.md.
