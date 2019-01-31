/*  =========================================================================
    version - Report 0MQ versoin in C
    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    version - Report 0MQ version 
@discuss
@end
*/


#include "czguide_classes.h"

int main (int argc, char *argv [])
{
	int major, minor, patch; 
	zmq_version (&major, &minor, &patch);
	printf("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
	return 0;
}