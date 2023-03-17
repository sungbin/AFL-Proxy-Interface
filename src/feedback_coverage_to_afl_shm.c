#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM_ENV_VAR    "__AFL_SHM_ID"
#define COVERAGE_MAP_SIZE  (1 << 16)

unsigned int hash(unsigned int edge);

int
main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("USAGE: ./feedback_coverage_to_afl_shm COVERAGE_FILE RETURN_CODE");
	exit(1);
    }

    // attach shared memory of AFL to a variable
    char * shm_str = getenv(SHM_ENV_VAR);
    if (shm_str == NULL) {
        fprintf(stderr, "turn on AFL-fuzz \n");
	exit(1);
    }
    int shm_id = atoi(shm_str);
    unsigned char* trace_bits = shmat(shm_id, NULL, 0);

    // open coverage file
    char *coverage_file = argv[1];
    FILE *coverage_fp = fopen(coverage_file, "rb");

    // coverage_map will be sent to shared memory
    unsigned int coverage_map[COVERAGE_MAP_SIZE] = {0, };

    // process coverage data file
    unsigned int buf[512];
    while (! feof(coverage_fp)) {
        int read_num = fread(buf, 4, 512, coverage_fp);
        for (int i = 0; i < read_num; i++) {
		unsigned int edge = buf[i];
		edge = hash(edge);
		coverage_map[edge]++;
        }
    }
    fclose(coverage_fp);


    // update coverage on shared memory
    coverage_map[0] = coverage_map[0] == 0 ? 1 : coverage_map[0]; // set at most one byte for AFL
    int return_code = atoi(argv[2]);

    //XXX: write coverage bytes on shared memory in "LITTLE ENDIAN"
    memcpy(trace_bits, &return_code, 4);
    memcpy(trace_bits+4, coverage_map, COVERAGE_MAP_SIZE);

    return 0;
}

unsigned int hash(unsigned int edge) {

     int bound = COVERAGE_MAP_SIZE-1;
     unsigned int res = (unsigned int) ((edge*2654435761) % bound);
     if (res < 0)
         res += bound;

     return res;
}
