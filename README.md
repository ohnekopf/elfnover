elfnover: strip symbol versioning
=================================

This little script removes dynamic symbol version info sections from
an ELF executable, in order to make it load if you dont have the
right library version.
Of course, this might result in a buggy execution.


Building
--------
Just clone this repo and `make` 

    $ git clone ${THIS_REPO}
    $ cd elfnover
    $ make

Usage
-----

Provide a path to an executable and the
pogrram will create a copy appending `_nover` to it

	$ ./elfnover bash
	# bash_nover should now exist in the current directory


Its possible you'll need to use `LD_PRELOAD` to preload some
dynamic libraries if they cant be found when you try to run the program,
this happens sometimes, for example with older libpthread in glibc.

for example:

	$ ./elfnover foo
	$ ./foo_nover

	# ./foo: symbol lookup error: ./foo_nover: undefined symbol: pthread_setaffinity_np

	$ LD_PRELOAD=libpthread-2.33.so:libdl-2.33.so:libutil-2.33.so ./foo_nover

	# program runs 
