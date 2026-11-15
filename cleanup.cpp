#include "cleanup.h"
#include "ProcessManagement.h"
#include "MemoryManagement.h"
#include <iostream>
#include "Boot.h"

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
  cout << "\nShutting down...\n";

  int procs = 0;
  while (FRONT != nullptr)
    {
      process *next = FRONT->right;
      delete FRONT;
      FRONT = next;
      procs++;
    }
  REAR = nullptr;
  Step ("Stopped processes (" + to_string (procs) + " removed)");

  Step ("Released memory (" + to_string (MemoryCleanup ()) + " blocks)");

  int nodes = FreeTree (fs->root) - 1;
  fs->root = nullptr;
  fs->current = nullptr;
  Step ("Unmounted file system (" + to_string (nodes) + " files/folders)");

  Step ("System halted");
}