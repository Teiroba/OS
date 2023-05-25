void* sbrk(int incr) {
  extern char _end;		/* Defined by the linker */
  static char *heap_end;
  char *prev_heap_end;
 
  if (heap_end == 0) {
    heap_end = &_end + 1000000;
  }
  prev_heap_end = heap_end;

  heap_end += incr;
  return (void*) prev_heap_end;
}