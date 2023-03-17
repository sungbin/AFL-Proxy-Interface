#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *coverage_fp = 0x0;

uint32_t *coverd_edges = 0x0;
uint32_t coverage_cnt = 0;
uint32_t alloc_num = 2048;

void update_coverage(uint32_t edge);

extern "C" void add_coverd_edges(uint32_t edge) {

    if (coverd_edges == 0x0)
        coverd_edges = (uint32_t*)malloc(sizeof(uint32_t)*alloc_num);
    if (coverage_cnt >= alloc_num) {
        alloc_num *= 2;
	coverd_edges = (uint32_t*)realloc(coverd_edges, alloc_num);
    }

    coverd_edges[coverage_cnt] = edge;
    coverage_cnt++;
    update_coverage(edge);
}

void update_coverage(uint32_t edge) {

    if (coverage_fp == 0x0)
        coverage_fp = fopen(".coverage", "ab");

    int written = fwrite(&edge, sizeof(uint32_t), 1, coverage_fp);
    if (written != 1)
        exit(1);

    fflush(coverage_fp);
}
