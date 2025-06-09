#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

// Global shared data and mutex with priority ceiling
int shared_data = 0;
pthread_mutex_t data_mutex;

void* low_priority_task(void* arg) {
    for (int round = 0; round < 10; round++) {
        pthread_mutex_lock(&data_mutex);
        printf("Low-priority task: locked the data.\n");
        sleep(2);
        printf("Low-priority task: reading data: %d\n", shared_data);
        sleep(3);
        pthread_mutex_unlock(&data_mutex);
        printf("Low-priority task: unlocked the data.\n");
        sleep(1);
    }
    return NULL;
}

void* medium_priority_task(void* arg) {
    for (int i = 0; i < 40; i++) {
        for (volatile int j = 0; j < 200000000; j++) {}
        printf("Medium-priority task: running... (step %d)\n", i + 1);
    }
    return NULL;
}

void* high_priority_task(void* arg) {
    printf("High-priority task: trying to lock the data...\n");
    while (pthread_mutex_trylock(&data_mutex) != 0) {
        printf("High-priority task: cannot run because low-priority task locked the data, will wait...\n");
        for (volatile int k = 0; k < 160000000; k++) {}
    }
    printf("High-priority task: finally locked the data now!!!!!!!!!\n");

    for (int round = 0; round < 10; round++) {
        for (volatile int j = 0; j < 100000000; j++) {}
        printf("High-priority task: working... (round %d)\n", round + 1);
        shared_data += 1;
        printf("High-priority task: updated data to %d\n", shared_data);
    }

    pthread_mutex_unlock(&data_mutex);
    printf("High-priority task: unlocked the data.\n");
    return NULL;
}

int main() {
    pthread_t low, medium, high;
    pthread_attr_t attr;
    struct sched_param param;

    // ====== Setup priority ceiling mutex ======
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_PROTECT);
    pthread_mutexattr_setprioceiling(&mattr, 30); // Ceiling = highest thread priority
    pthread_mutex_init(&data_mutex, &mattr);

    // Set thread scheduling attributes
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    // Create low-priority thread
    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&low, &attr, low_priority_task, NULL);

    // Create medium-priority thread
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&medium, &attr, medium_priority_task, NULL);

    sleep(1); // Allow low/medium to start first

    // Create high-priority thread
    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&high, &attr, high_priority_task, NULL);

    // Wait for all threads
    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    // Cleanup
    pthread_mutex_destroy(&data_mutex);
    pthread_mutexattr_destroy(&mattr);
    return 0;
}
