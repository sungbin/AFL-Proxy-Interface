#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *coverage_fp = 0x0;

uint32_t *coverage_set = 0x0;
uint32_t coverage_cnt = 0;
uint32_t alloc_num = 2048;

void print_coverage_set();
void update_coverage(uint32_t edge);

extern "C" void add_coverage_set(uint32_t edge) {

    if (coverage_set == 0x0)
        coverage_set = (uint32_t*)malloc(sizeof(uint32_t)*alloc_num);
    if (coverage_cnt >= alloc_num) {
        alloc_num *= 2;
	coverage_set = (uint32_t*)realloc(coverage_set, alloc_num);
    }

    int duplicated = 0;
    for (uint32_t i = 0; i < coverage_cnt; i++) {
        if (coverage_set[i] == edge) {
	    duplicated = 1;
	    break;
        }
    }
    if (! duplicated) {
        coverage_set[coverage_cnt] = edge;
        coverage_cnt++;
	update_coverage(edge);
	//print_coverage_set();
    }
}

void print_coverage_set() {
    fprintf(stderr, "%d coverd: [", coverage_cnt);
    for (uint32_t i = 0; i < coverage_cnt; i++) {
        fprintf(stderr, "%d, ", coverage_set[i]);
    }
    fprintf(stderr, "\b\b] \n");
}

void update_coverage(uint32_t edge) {

    if (coverage_fp == 0x0)
        coverage_fp = fopen(".coverage", "ab");

    int written = fwrite(&edge, sizeof(uint32_t), 1, coverage_fp);
    if (written != 1)
        exit(1);

    fflush(coverage_fp);
}
