#include "uuid7.h"

#include <err.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void uuid_to_string(const uuid_t uuid, char *out) {
  static char const fmt[16] = "0123456789abcdef";
  char *p = out;
  int i;

  for (i = 0; i < 16; i++) {
    if (i == 4 || i == 6 || i == 8 || i == 10) {
      *p++ = '-';
    }
    size_t tmp = uuid[i];
    *p++ = fmt[tmp >> 4];
    *p++ = fmt[tmp & 15];
  }
  *p = '\0';
}

void uuid_generate_v7(uuid_t out) {
  struct timespec tv;
  uint64_t subsec, ts;
  static uint64_t ts_prev = 0;
  static unsigned char jitter = 0;

  if (getentropy(out + 9, 7) || clock_gettime(CLOCK_REALTIME, &tv))
    err(EXIT_FAILURE, NULL);
  ts = tv.tv_sec * 1000000000ULL + tv.tv_nsec;
  pthread_mutex_lock(&lock);
  if (ts <= ts_prev) {
    ts = ts_prev + 1;
  } else if (ts - ts_prev > 1000000) {
    jitter = out[9] >> 7;
  }
  ts_prev = ts;
  subsec = (ts % 1000000ULL << 20) / 1000000 + jitter;
  pthread_mutex_unlock(&lock);
  ts /= 1000000ULL; // unix_ts_ms

  out[0] = ts >> 40;
  out[1] = ts >> 32;
  out[2] = ts >> 24;
  out[3] = ts >> 16;
  out[4] = ts >> 8;
  out[5] = ts >> 0;
  out[6] = subsec >> 16 | 0x70; // version
  out[7] = subsec >> 8;
  out[8] = (subsec >> 2 & 0x3F) | 0x80; // variant
  out[9] = (out[9] & 0x3F) | subsec << 6;
}
