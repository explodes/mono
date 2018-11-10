#include "webs/statusz/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/sysinfo.h"
#include "sys/types.h"

int parseLine(char* line) {
  // This assumes that a digit will be found and the line ends in " Kb".
  int i = strlen(line);
  const char* p = line;
  while (*p < '0' || *p > '9') p++;
  line[i - 3] = '\0';
  i = atoi(p);
  return i;
}

int getProcessMemoryKB() {  // Note: this value is in KB!
  FILE* file = fopen("/proc/self/status", "r");
  int result = -1;
  char line[128];

  while (fgets(line, 128, file) != NULL) {
    if (strncmp(line, "VmRSS:", 6) == 0) {
      result = parseLine(line);
      break;
    }
  }
  fclose(file);
  return result;
}

MemoryUsage collectMemoryStatistics() {
  MemoryUsage mem;

  struct sysinfo memInfo;
  sysinfo(&memInfo);

  mem.system = memInfo.totalram - memInfo.freeram;
  mem.system *= memInfo.mem_unit;

  mem.total = memInfo.totalram;
  mem.total *= memInfo.mem_unit;

  mem.process = static_cast<long long>(getProcessMemoryKB()) * 1024LL;

  return mem;
}