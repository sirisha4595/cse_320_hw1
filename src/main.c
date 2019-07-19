#include <stdlib.h>

#include "hw1.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif


int main(int argc, char **argv)
{
    if(!validargs(argc, argv)){
        USAGE(*argv, EXIT_FAILURE);
    }
    if(global_options & (0x1L << 63)) {
        USAGE(*argv, EXIT_SUCCESS);
    }
    else if(global_options & U_OPTION ||global_options & D_OPTION ||global_options & C_OPTION ){//-u -d -c options need to be handled
        //calling recode()
        if(recode(argv)==0)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
