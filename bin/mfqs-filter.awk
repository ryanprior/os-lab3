# mfqs-filter.awk: Input filter for mfqs.

# Require that pid, burst be positive, arrival and priority be
# non-negative; ignore deadline and I/O.
$1 >  0 &&
$2 >  0 &&
$3 >= 0 &&
$4 >= 0 {
    print $1, $2, $3, $4
}
