#include <stdio.h>

void put_headers() {
  puts("Content-Type: application/json;charset=UTF-8");
  puts("Cache-control: max-age=0");
  puts("Pragma: no-cache");
  puts("");
}
