# How to create your own tests for HW05


## Overview

The following files are supplied with the assignment template:

* `dt-tests-cli`: Test runner. **DO NOT EDIT!**
* `test-utils.sh`: Utilities for this homework. **DO NOT EDIT!**
* `tests-basic.sh`: Basic tests.

To add new tests, either edit `tests-basic.sh` or create a new file with the
name `tests-SUITE.sh` and re-run `cmake`.

The following sections provide a tutorial on writing tests for this tool.

You can also optionally use Unit Tests in CUT for testing your own components
as usual.


## 1. Tree structure

Reuse an existing tree structure (`create_SCENARIO()` functions) or design
your own by creating a new `create_SCENARIO()` function.

The following functions are available for creating tree structures:

* `create_path PATH/…/…`

  Creates a directory, included nested paths.


* `create_file PATH/…/NAME`

  Creates an empty file (0 bytes, 0 blocks)


* `create_file PATH/…/NAME SIZE`

  Creates a file filled with empty blocks (SIZE bytes, 0 blocks)


**AFTER** creating the tree structure, you may modify files and directories
using other shell tools. Most likely you will need `chmod MODE FILE`.


## 2. Create a test function

Write a `test_NAME()` function for your test.

* First, call your chosen `create_SCENARIO()` function.

* Optionally modify the tree structure.

* Finally, execute your binary. The path to this binary is available in `$EXE`
  variable, for example

      du -s basic_tree

  The `du` is not actually your program, but a wrapper that also calls
  valgrind. Thus, you do not need to do anything else.
  Do **not** redirect standard outputs; the test harness already does this.


## 3. Provide expected output and error output

* Place expected standard output into `tests/data/test_NAME.out`.
* Place expected error output into `tests/data/test_NAME.err`.

Re-run `cmake`.

**NOTE**: These files do not need to exist; in that case, empty output is
implied.


## 4. Register the test

Immediately after `}` of your test function, call `run` as follows:

    run "Test description" test_NAME

* The first argument should be a short description of your test.
* The second argument is the name of your test **without** `()`.

If you expect your program to exit with a non-zero code, add `-e CODE` after
`run`. The following modes are recognized:

* To exit with any non-zero code, use any negative value:

      run -e -1 "This test should fail" test_NAME

* Set to a non-negative value to exit with this precise status:

      run -e 1 "Must exit with 1" test_NAME
      run -e 6 "Must exit with 6" test_NAME

That's it!
