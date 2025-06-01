# Parc
## Purpose (why?)
This is a **simple** library. Moreover, it was (initially) made just because I enjoy exploring parsing, and is therefore largely a personal project. However, I don't mind expanding it's features or its set of applications.

## Examples
Use `make` to compile and run examples:

``` sh
$ make examples
$ ./build/variables "let x = 42;"
```

## Tests
To run tests use the `examples-test` target:

``` sh
$ make examples-test
$ ./build/variables.test
```

NOTE: At the moment, the tests are just a bunch of asserts, hopefully we implement a more elaborate testing system in the future.
