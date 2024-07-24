#include "uuid7.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BILLION 1000000000LLU
#define N_SAMPLES 100000
static char samples[N_SAMPLES][40];
static uint64_t elapsed;

static void toggle_timer() {
  static size_t started = 0;
  static struct timespec start, end;
  if (started) {
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed =
        BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    started = 0;
  } else {
    clock_gettime(CLOCK_MONOTONIC, &start);
    started = 1;
  }
}

void setup(void) {
  uuid_t uuid;
  for (int i = 0; i < N_SAMPLES; i++) {
    uuid_generate_v7(uuid);
    uuid_to_string(uuid, samples[i]);
  }
}

void test_format(void) {
  for (int i = 0; i < N_SAMPLES; i++) {
    assert(strlen(samples[i]) == 36);
    for (int j = 0; j < 36; j++) {
      char c = samples[i][j];
      if (j == 8 || j == 13 || j == 18 || j == 23)
        assert(c == '-');
      else if (j == 14)
        assert(c == '7');
      else if (j == 19)
        assert(c == '8' || c == '9' || c == 'a' || c == 'b');
      else
        assert(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
               c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
               c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' ||
               c == 'f');
    }
  }
}

void test_order(void) {
  for (int i = 1; i < N_SAMPLES; i++)
    assert(strcmp(samples[i - 1], samples[i]) < 0);
}

int main(void) {
  fprintf(stderr, "### TEST_UUID7\n");
  toggle_timer();
  setup();
  toggle_timer();
  fprintf(stderr, "  %s: ok  %llu.%09llus\n", "setup", elapsed / BILLION,
          elapsed % BILLION);

  toggle_timer();
  test_format();
  toggle_timer();
  fprintf(stderr, "  %s: ok  %llu.%09llus\n", "test_format", elapsed / BILLION,
          elapsed % BILLION);

  toggle_timer();
  test_order();
  toggle_timer();
  fprintf(stderr, "  %s: ok  %llu.%09llus\n", "test_order", elapsed / BILLION,
          elapsed % BILLION);

  return EXIT_SUCCESS;
}
