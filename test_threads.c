#include "uuid7.h"

#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BILLION 1000000000LLU
#define N_SAMPLES 100000
#define N_THREADS 8
static char samples[N_THREADS][N_SAMPLES][40];
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

void *setup(void *id) {
  uuid_t uuid;
  int n = (intptr_t)id;
  for (int i = 0; i < N_SAMPLES; i++) {
    uuid_generate_v7(uuid);
    uuid_to_string(uuid, samples[n][i]);
  }
  return NULL;
}

void test_format(void) {
  for (int n = 0; n < N_THREADS; n++) {
    for (int i = 0; i < N_SAMPLES; i++) {
      assert(strlen(samples[n][i]) == 36);
      for (int j = 0; j < 36; j++) {
        char c = samples[n][i][j];
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
}

void test_order(void) {
  for (int n = 0; n < N_THREADS; n++) {
    for (int i = 1; i < N_SAMPLES; i++)
      assert(strcmp(samples[n][i - 1], samples[n][i]) < 0);
  }
}

int main(void) {
  pthread_t tid[N_THREADS];
  int err, i;

  printf("### TEST_THREADS\n");
  toggle_timer();
  for (i = 0; i < N_THREADS; i++) {
    err = pthread_create(&tid[i], NULL, &setup, (void *)(intptr_t)i);
    if (err != 0)
      printf("\ncan't create thread :[%s]", strerror(err));
  }
  for (i = 0; i < N_THREADS; i++)
    pthread_join(tid[i], NULL);
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
