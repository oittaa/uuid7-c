#define UUID_STR_LEN 37

typedef unsigned char uuid_t[16];

extern void uuid_generate_v7(uuid_t out);
extern void uuid_to_string(const uuid_t uuid, char *out);
