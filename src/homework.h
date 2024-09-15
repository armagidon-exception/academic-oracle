#ifndef HOMEWORK_H
#define HOMEWORK_H
#include <time.h>
typedef struct homework_s {
  char* description;
  unsigned long description_length;
} homework_s;

int homework_init_db();

homework_s* homework_get_at(time_t date);

void homework_destroy(homework_s* hm);

void homework_shutdown();

#endif /* end of include guard: HOMEWORK_H */
