# uuid7
A C library for the new time-based UUID suited for a database key.

[![codecov](https://codecov.io/github/oittaa/uuid7-c/graph/badge.svg?token=XXQO298RMF)](https://codecov.io/github/oittaa/uuid7-c)

### Usage
```c
uuid_t uuid;
char buf[UUID_STR_LEN];

uuid_generate_v7(uuid);
uuid_to_string(uuid, buf);
printf("%s\n", buf);
```

`uuid_generate_v7()` is thread-safe.