# rts-filter.awk: Input filter for rts.

# Require that pid, burst be positive, arrival and priority be
# non-negative, and deadline be greater than arrival; ignore I/O.
$1 >  0 &&
$2 >  0 &&
$3 >= 0 &&
$4 >= 0 &&
$5 >  $3 {
    print $1, $2, $3, $4, $5
}
