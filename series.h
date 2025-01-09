typedef struct series series_t;

/**
* @brief Create a new series
*
* @param length the number of values in the series
* @param source an array containing at least `length` elements starting from index `offset`
* @param offset the starting index to collect values from `source`
* @return a new series containing the first `length` values from `source` starting at index `offset`
* @note the series should be terminated by the value -1
*/
series_t *series_create(int length, int source[], int offset);

/**
* @brief Destroy a series, returning its memory
*
* @param s the series to destroy
*/
void series_destroy(series_t *s);

/**
* @brief Look up a value in a series
*
* @param s the series to lookup in
* @param i the index to look up
* @return the `i`th value of `s`
* @note `i` can be assumed to be within the bounds of `s`
*/
int series_get(series_t *s, int i);

/**
* @brief Get the number of values in a series
*
* @param s a series
* @return the number of values in `s`
*/
size_t series_length(series_t *s);

/**
* @brief Get the next wave from an array of values
*
* @param source an array containing at least `length` integers
* @param offset the starting index of `source`
* @param length the length of `source`
* @return a series containing the longest sequence of non-decreasing values from `source`, starting from `offset`
*/
series_t *series_next_wave(int source[], int *offset, int length);
