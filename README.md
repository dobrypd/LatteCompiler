Latte language compiler
-----------------------

Author Piotr Dobrowolski

pd291528@students.mimuw.edu.pl

Compilation:
-----------

`$ make`

With debug output:

`$ make debug=1`

Running:
-------------

`$ ./latc_x86 file_name`

Run:

`$ ./latc_x86 -h` for more help.

Structure of directories:
--------------------

 * `/src/` sources,
 * `/src/grammar` bnfc grammar,
 * `/src/kernel/` cpp files,
 * `/src/includes/` headers,
 * `/bin/` built elements,
 * `/scripts/` scripts, ie testing,
 * `/testy/` tests,

 
Latte language characteristic:
---------------

Latte is quite simmilar to Java. Best way to know basic structure is to
read tests (`/tests/lattests`). Otherwise you can compile whole project
go to `/bin/grammar` and `make` to create grammar documentation.

