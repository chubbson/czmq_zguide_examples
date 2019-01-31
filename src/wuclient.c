/*  =========================================================================
    wuserver - Weather update client in C

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    wuclient - Weather update client
    Connects SUB socket to tcp://localhost:5556
    Collects weather update and finds avg temp in zipcode
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
        zsys_info ("wuclient - Weather update server ");

    // Socket to talk to server
    printf ("Collecting updates from weather server...\n");

    zsock_t *subscriber = zsock_new (ZMQ_SUB);
    int rc = zsock_bind (subscriber, "%s", "tcp://*:5556");
    assert(rc == 0);

    // Subscribe to zipcode, default in NYC, 10001
    char *filter = (argc > 1)? argv [1]: "10001";
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
    assert (rc == 0);

    // Process 100 updates
    int update_nbr;
    long total_temp = 0; 
    for (update_nbr = 0; update_nbr < 100; update_nbr++) {
        char *string = zstr_recv(subscriber);

        int zipcode, temperature, relhumidity;
        sscanf (string, "%d %d %d",
            &zipcode, &temperature, &relhumidity);
        total_temp += temperature;
        zstr_free (&string);
    }
    printf ("Average temperature for zipcode '%s' was %dF\n", 
    	filter, (int) (total_temp / update_nbr));

    zsock_destroy(&subscriber);
    return 0; 
}