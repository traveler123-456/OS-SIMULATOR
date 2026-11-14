#include "cleanup.h"
#include "ProcessManagement.h"
#include "MemoryManagement.h"
#include <iostream>

using namespace std;

static int
FreeTree (Directory *node)
{
  if (node == nullptr)
    return 0;

  int count = 1;
  count += FreeTree (node->child);  
  count += FreeTree (node->sibling); 
  delete node;
  return count;
}

void
Cleanup (FileSystem *fs)
{
  cout << "\nCleaning up...\n";

  int procs = 0;
  while (FRONT != nullptr)
    {
      process *next = FRONT->right;
      delete FRONT;
      FRONT = next;
      procs++;
    }
  REAR = nullptr;
  cout << " [1/3] processes freed      : " << procs << "\n";

  cout << " [2/3] memory blocks freed  : " << MemoryCleanup () << "\n";

  int nodes = FreeTree (fs->root) - 1;
  fs->root = nullptr;
  fs->current = nullptr;
  cout << " [3/3] files/folders freed  : " << nodes << "\n";
}