#include "uuid7.h"

#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

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
  uint64_t ms, subsec, ns;
  static uint64_t ns_prev = 0;
  static unsigned char jitter = 0;

  if (getentropy(out + 9, 7) || clock_gettime(CLOCK_REALTIME, &tv))
    err(EXIT_FAILURE, NULL);
  ns = tv.tv_sec * 1000000000ULL + tv.tv_nsec;
  if (ns <= ns_prev) {
    ns = ns_prev + 1;
  } else if (ns - ns_prev > 1000000) {
    jitter = out[9] >> 7;
  }
  ns_prev = ns;
  ms = ns / 1000000ULL;
  subsec = (ns % 1000000ULL) * 1048576 / 1000000 + jitter;

  out[0] = ms >> 40;
  out[1] = ms >> 32;
  out[2] = ms >> 24;
  out[3] = ms >> 16;
  out[4] = ms >> 8;
  out[5] = ms >> 0;
  out[6] = subsec >> 16 | 0x70; // version
  out[7] = subsec >> 8;
  out[8] = (subsec >> 2 & 0x3F) | 0x80; // variant
  out[9] = (out[9] & 0x3F) | subsec << 6;
}
