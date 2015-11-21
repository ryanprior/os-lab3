# whs-filter.awk: Input filter for whs.

# Require that pid, burst be positive; arrival I/O be non-negative;
# priority be in [0,99]; ignore deadline.
$1 >  0  &&
$2 >  0  &&
$3 >= 0  &&
$4 >= 0  &&
$4 <= 99 &&
$6 >= 0 {
    print
}
