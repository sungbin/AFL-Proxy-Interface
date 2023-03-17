#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

unsigned int
to_target(char *buf, int size);

unsigned int
from_target(char *buf, int size);

FILE * to_target_fd = 0x0;
FILE * from_target_fd = 0x0;

int
main (int argc, char *argv[]) {

    if (argc < 3) {

        printf("./proxy from_target_pipe to_target_pipe\n");
        return 1;
    }
    
    // set up pipes
    char *to_target_str = argv[1];
    char *from_target_str = argv[2];

    fprintf(stderr, "open %s \n", to_target_str);
    to_target_fd = fopen(to_target_str, "w+");

    //fprintf(stderr, "open %s \n", from_target_str);
    //from_target_fd = fopen(from_target_str, "r+");

    fprintf(stderr, "H2 \n");
    // test
    int return_code;
    to_target("HELO", 4);
    //from_target((char*)&return_code, 4);
    printf("return code: %d \n", return_code);
    
    fclose(to_target_fd);
    //fclose(from_target_fd);

    return 0;
}


unsigned int
to_target(char *buf, int size) {

    unsigned int total_written_num = 0;
    
    fprintf(stderr, "totla_written : %d \n", total_written_num);
    fflush(stderr);
    while (size - total_written_num > 0) {

	int written_num = 0;
        if (size > 2048) {
            written_num = fwrite(buf+total_written_num, 1, 2048, to_target_fd);
	} else {
            written_num = fwrite(buf+total_written_num, 1, size, to_target_fd);
	}

	total_written_num += written_num;
	fprintf(stderr, "written : %d \n", written_num);
        fflush(stderr);
    }
    assert(size == total_written_num);    
    //buf[size-1] = 0x0;
    //fprintf(stderr, "LOG: to target \"%s\"", buf);
    return total_written_num;
}

unsigned int
from_target(char *buf, int size) {

    unsigned int total_read_num = 0;
    while (size - total_read_num > 0) {

	int read_num = 0;
        if (size > 2048) {
            read_num = fread(buf+total_read_num, 1, 2048, from_target_fd);
	} else {
            read_num = fread(buf+total_read_num, 1, size, from_target_fd);
	}

	total_read_num += read_num;
    }
    assert(size == total_read_num);    
    //buf[size-1] = 0x0;
    //fprintf(stderr, "LOG: from target \"%s\"", buf);
    return total_read_num;
}
