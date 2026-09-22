#ifdef _WIN32
#include <windows.h>
typedef HANDLE sem_t;
#define sem_init(s, p, v) ((void)((*(s) = CreateSemaphore(NULL, v, 0x7FFFFFFF, NULL)), 1))
#define sem_wait(s) ((void)WaitForSingleObject(*(s), INFINITE))
#define sem_post(s) ((void)ReleaseSemaphore(*(s), 1, NULL))
#define sem_destroy(s) ((void)CloseHandle(*(s)))
typedef HANDLE pthread_t;
#define pthread_create(t, a, f, arg) ((void)((*(t) = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(f), (arg), 0, NULL)), 1))
#define pthread_join(t, r) ((void)WaitForSingleObject(t, INFINITE))
#else
#include <pthread.h>
#include <semaphore.h>
#endif
#include <stdio.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0, items;
sem_t empty, full, mutex;

void *producer(void *arg) {
    for (int i = 0; i < items; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in] = i;
        printf("Produced: %d at slot %d\n", i, in);
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < items; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = buffer[out];
        printf("Consumed: %d from slot %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main() {
    printf("Enter number of items to produce: ");
    scanf("%d", &items);

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t pid, cid;
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    return 0;
}