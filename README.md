# What Is This?
* This is just an assignment as a part of OSN at IIITH
* This assignment's goal was to make us use `system calls`

### To compile all files at once:   
```
$ make
```

## File structure:
```
2021101034
├── Makefile
├── q1.c
├── q2.c
├── q3.c
├── README.md
└── README.pdf
```

# Q1:
* Assumed `Input File Name` to be at max `10^6` characters
* If the Assignment Directory Already exists printed error to console on purpose.
* Compile:
    ```sh
    $ gcc q1.c -o q1 
    or
    $ make q1
    ```
* Run:
    ```sh
    $ ./q1
    ```

# Q2:
* breakpoints provided are assumed to be integers
* if the break points are invalid the program throws error to console and returns `-1`
* throws errors if the directory already exists (`Not a bug`), `kept intentionally`
* Compile:
    ```sh
    $ gcc q2.c -o q2
    or
    $ make q2 
    ```
* Run:
    ```sh
    $ ./q2
    ```

# Q3:
* If the Directory did not exist , did not print the permission of directory.
* If old file / new file did not exist , program stops and returns `-1`
* output structure:
    ```txt
    is Directory created
    is new file the reverse of old file
    permission list of new file
    permission list of old file
    permission list of directory
    ```
* Compile:
    ```sh
    $ gcc q3.c -o q3
    or
    $ make q3 
    ```
* Run:
    ```sh
    $ ./q3
    ```