#include <stdio.h>

#include "series.h"

struct series {
    int* sub_series;
    size_t len;
};


series_t *series_create(int length, int source[], int offset)
{  
    series_t *series = calloc(1, sizeof(series_t));
    series->sub_series = calloc(length + 1, sizeof(int));
    series->len = length;

    for (size_t i = 0; i < length; i++) {
        if (source[i] != '\0') {
            series->sub_series[i] = source[i + offset];
        }
    }
    series->sub_series[length] = -1;

    return series;
}


void series_destroy(series_t *s)
{
    free(s->sub_series);
    free(s);
  
}


int series_get(series_t *s, int i)
{
    return s->sub_series[i];
}


size_t series_length(series_t *s)
{
    return s->len;
}

series_t *series_next_wave(int source[], int *offset, int length)
{
    // meningen här för mig var att lista ut hur långt det ökar från och med offset 
    // och så sätt använda series_create för att skapa serien
    // vi vill alltid ta första elementet och sedan jämföra om nästa är större eller inte 
    int initial_offset = *offset;
    int temp_len = 0;
    if (initial_offset < length) {
        temp_len++;
        for (int i = 0; i < length; i++) {
            if (initial_offset + i + 1 != length && initial_offset + i != length ) {
                if (source[initial_offset + i] <= source[initial_offset + i + 1]) {
                    temp_len++;
                } 
                if (source[initial_offset + i] > source[initial_offset + i + 1]) {
                        break;
                }                
            }
        }
    }

    *offset += temp_len;
    return series_create(temp_len, source, initial_offset);
}

#ifndef TEST
int main(void)
{
  puts("If you want to you can write test code here and run it using `make run`");
  return 0;
}
#endif