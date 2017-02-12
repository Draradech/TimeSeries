#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>

extern "C" {
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}

#define LIMIT(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define MIN(x, b) ((x) > (b) ? (b) : (x))
#define MAX(x, a) ((x) < (a) ? (a) : (x))

#define METHOD_AVG 1
#define METHOD_MIN 2
#define METHOD_MAX 3

