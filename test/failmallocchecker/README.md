This is a small program that first counts all possible calls to
`malloc` of a particular function. After the count is done, then
the library makes every single each call to `malloc` fail while
looping over a function. It is meant to be used to test every possible
allocation failure.


