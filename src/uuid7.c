#include "uuid7.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/random.h>
#include <time.h>
#include <sys/types.h>

static void __uuid_set_variant_and_version(uuid_t uuid, int version) {
  uuid[6] = (uuid[6] & 0xf) | version << 4;
  uuid[8] = (uuid[8] & 0x3F) | 0x80;
}

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

  clock_gettime(CLOCK_REALTIME, &tv);
  ns = tv.tv_sec * 1000000000ULL + tv.tv_nsec;
  if (ns <= ns_prev)
    ns = ns_prev + 1;
  ns_prev = ns;
  ms = ns / 1000000ULL;
  subsec = (ns % 1000000ULL) * 1048576 / 1000000;

  out[0] = ms >> 40;
  out[1] = ms >> 32;
  out[2] = ms >> 24;
  out[3] = ms >> 16;
  out[4] = ms >> 8;
  out[5] = ms >> 0;
  out[6] = subsec >> 16;
  out[7] = subsec >> 8;
  out[8] = subsec >> 2;
  getentropy(out + 9, 7);
  out[9] = (out[9] & 0x3F) | subsec << 6;
  __uuid_set_variant_and_version(out, 7);
}
