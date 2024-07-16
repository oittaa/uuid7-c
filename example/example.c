#include <stdio.h>
#include <stdlib.h>

#include "uuid7.h"

int main(void) {
  uuid_t uuid;
  char buf[UUID_STR_LEN];

  uuid_generate_v7(uuid);
  uuid_to_string(uuid, buf);
  printf("%s\n", buf);

  return EXIT_SUCCESS;
}
