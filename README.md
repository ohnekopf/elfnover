elfnover: strip symbol versioning
=================================

This little script removes dynamic symbol version info sections from
an ELF executable, in order to make it load if you dont have the
right library version.
Of course, this might result in a buggy execution.


Building
--------
Just clone this repo and `make` 

    git clone ${THIS_REPO}
    cd elfnover
    make

Usage
-----

Provide a path to an executable and the
pogrram will create a copy appending `_nover` to it

	./elfnover bash
	# bash_nover should now exist in the current directory


