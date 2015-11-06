# procs.awk - Common logic to all the filters for `procs`

# Begin by throwing away the first line of input, which contains
# column headers.
BEGIN {
    getline
}

# Use a tab as the field separator.
BEGIN {
    FS="\t"
}
