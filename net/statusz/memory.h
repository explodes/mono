#ifndef NET_STATUSZ_MEMORY_H
#define NET_STATUSZ_MEMORY_H

struct MemoryUsage {
  // The total memory used by the system (bytes)
  long long system;
  // The total memory used by the process (bytes)
  long long process;
  // The total memory available on the system (bytes)
  long long total;
};

MemoryUsage collectMemoryStatistics();

#endif