start   add 0 1 2       # Add values in registers 1 and 2, store result in register 0
        beq 1 2 loop    # If registers 1 and 2 are equal, jump to label 'loop'
        lw  3 0 data    # Load the value stored at 'data' into register 3
        halt            # Stop the program

loop    add 2 2 2       # Double the value in register 2
        halt            # Stop the program

data    .fill 12        # Data stored in memory with label 'data'
