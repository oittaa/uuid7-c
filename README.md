# uuid7
A C library for the new time-based UUID suited for a database key.

### Usage
```c
uuid_t uuid;
char buf[UUID_STR_LEN];

uuid_generate_v7(uuid);
uuid_to_string(uuid, buf);
printf("%s\n", buf);
```

`uuid_generate_v7()` is not thread-safe.