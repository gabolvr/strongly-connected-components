EXTIN=".dgraph"
EXTOUT=".rgraph"
EXTSCC=".scc"

make verify_complete
./test_read_write $1
./seq_scc < $1$EXTOUT > $1$EXTSCC
./verify_complete < $1$EXTSCC
