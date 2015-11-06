# utilities

## procs

`procs` reads a stream of processes on `stdin`, sanitizes them
according to the parameters of a given scheduler, sorts them, and
prints them on `stdout`. It expects the same format as the `test/100k`
file provided by Dr. Tan. (Consequently, it ignores and throws out the
first line of input.)

examples:

    # sanitize and sort 100k processes for MFQS
    bin/procs mfqs <test/100k
    
    # just 500 processes for RTS
    head -n 500 test/100k | bin/procs rts
    
    # sanitize and sort all 100k for whs, but only print first 20
    # processes to be run
    bin/procs whs <test/100k | head -n 20

### sanitization parameters

These are specified in `doc/lab3.pdf` and implemented in
`bin/*-filter.awk`.

### sort order

1. arrival time (low->high)
2. priority (high->low)
3. pid (low->high)
