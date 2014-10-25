Development
===========

Fixes/Minor Improvements
------------------------

Standardise on use of styx::list, std::list or std::vector.  It would be nice
to eliminate the dependency on styx as very little of it is used.

Major Improvements
------------------

Add a hades::rename class that tuples can derive from.  This would rename a
database attribute to a tuple attribute (for example, rename 'album.caption' to
'album&#95;caption' to avoid clashes with 'photograph.caption').

