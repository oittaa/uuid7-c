#include "uuid7.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <time.h>

#define N_SAMPLES 100000
static char samples[N_SAMPLES][40];

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
  setup();

  test_format();
  fprintf(stderr, "  %s: ok\n", "test_format");
  test_order();
  fprintf(stderr, "  %s: ok\n", "test_order");

  return EXIT_SUCCESS;
}
