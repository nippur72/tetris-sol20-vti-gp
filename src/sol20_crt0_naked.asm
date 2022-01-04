EXTERN _main
EXTERN main    

    start:
        ld hl, 0xbfff
        ld sp, hl
        call _main
        ret
        halt

