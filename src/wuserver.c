/*  =========================================================================
    wuserver - Weather update server in C

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    wuserver - Weather update server in C
    Binds PUB socket to tcp://\*:5556
    Publishes ranom weather updates 
@discuss
@end
*/

#include "czguide_classes.h"

int main (int argc, char *argv [])
{
    bool verbose = false;
    int argn;
    for (argn = 1; argn < argc; argn++) {
        if (streq (argv [argn], "--help")
        ||  streq (argv [argn], "-h")) {
            puts ("wuserver [options] ...");
            puts ("  --verbose / -v         verbose test output");
            puts ("  --help / -h            this information");
            return 0;
        }
        else
        if (streq (argv [argn], "--verbose")
        ||  streq (argv [argn], "-v"))
            verbose = true;
        else {
            printf ("Unknown option: %s\n", argv [argn]);
            return 1;
        }
    }
    //  Insert main code here
    if (verbose)
        zsys_info ("wuserver - Weather update server ");

    zsock_t *publisher = zsock_new (ZMQ_PUB);
    int rc = zsock_bind (publisher, "%s", "tcp://*:5556");
    assert(rc == 0);

    // Initialize random number generator
    srandom ((unsigned) time (NULL));

    while(1){
        // Get values that will fool the boss
        int zipcode, temperature, relhumidity;
        zipcode     = randof (10000);
        temperature = randof (215) - 80;
        relhumidity = randof (20) + 10;

        // Send message to all subscribers
        char update [20];
        sprintf(update, "%05d %d %d", zipcode, temperature, relhumidity);
        zstr_send(publisher, update);
    }

    zsock_destroy(&publisher);
    return 0; 
}
