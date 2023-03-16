#include <stdio.h>


int main (int argc, char *argv[])  { 

    char buf[4096];
    int size = fread(buf, sizeof(char), 4096, stdin);
    if (size < 4)
        return 0;

    if (buf[0] == 'F') {
        if (buf[1] == 'U') {
            if (buf[2] == 'Z') {
                if (buf[3] == 'Z') {
		    fprintf(stderr, "crash! \n");
		    __builtin_trap();
		} else {
		    return 0;
		}
	    } else {
	        return 0;
	    }
	} else {
	    return 0;
	}
    } else {
	return 0;
    }

    return 0;
}
