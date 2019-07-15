#include <stdio.h>
#include <uv.h>

uv_barrier_t blocker;
uv_rwlock_t numlock;
int shared_num;

void reader(void *n) {
  int num = *(int *)n;
  int i;
  for (i = 0; i < 20; i++) {
    uv_rwlock_rdlock(&numlock);
    printf("Reader %d: acquired lock\n", num);
    printf("Reader %d: shared_num = %d\n", num, shared_num);
    uv_rwlock_rdunlock(&numlock);
    printf("Reader %d: release lock\n", num);
  }
  uv_barrier_wait(&blocker);
  printf("reader uv_barrier_wait\n");
}

void writer(void *n) {
  int num = *(int *)n;
  int i;
  for (i = 0; i < 20; i++) {
    uv_rwlock_wrlock(&numlock);
    printf("Writer %d: acquired lock\n", num);
    shared_num++;
    printf("Writer %d: incremented shared_num = %d\n", num, shared_num);
    uv_rwlock_wrunlock(&numlock);
    printf("Writer %d: release lock\n", num);
  }
  uv_barrier_wait(&blocker);
  printf("writer uv_barrier_wait\n");
}

int main() {
  uv_barrier_init(&blocker, 4);

  shared_num = 0;
  uv_rwlock_init(&numlock);

  uv_thread_t threads[3];

  int thread_nums[] = {100,1001,2000};

  uv_thread_create(&threads[0], reader, &thread_nums[0]);
  uv_thread_create(&threads[1], reader, &thread_nums[1]);
  uv_thread_create(&threads[2], writer, &thread_nums[2]);

  
  int count = uv_barrier_wait(&blocker);
  uv_barrier_destroy(&blocker);

  uv_rwlock_destroy(&numlock);
  return 0;
}
