#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global shared data and mutex for synchronization
int shared_data = 0;
pthread_mutex_t data_mutex;

// Low-priority task thread function
void* low_priority_task(void* arg) {
    for (int round = 0; round < 10; round++) { // Run more times to make the task last longer
        pthread_mutex_lock(&data_mutex);
        printf("Low-priority task: locked the data.\n");
        sleep(2);

        printf("Low-priority task: reading data: %d\n", shared_data);
        sleep(3);

        pthread_mutex_unlock(&data_mutex);
        printf("Low-priority task: unlocked the data.\n");
        sleep(1); // Optional: avoid immediate re-locking
    }
    return NULL;
}

// Medium-priority task thread function
void* medium_priority_task(void* arg) {
    for (int i = 0; i < 40; i++) { // Control the number of printouts
        // A longer waiting period is carried out before each print operation.
        for (volatile int j = 0; j < 200000000; j++) {
            // Busy wait, do nothing, just occupy CPU
        }
        printf("Medium-priority task: running... (step %d)\n", i + 1);
    }
    return NULL;
}

// High-priority task thread function
void* high_priority_task(void* arg) {
    printf("High-priority task: trying to lock the data...\n");
    // Keep trying to lock the mutex, print message if failed
    while (pthread_mutex_trylock(&data_mutex) != 0) {
        printf("High-priority task: cannot run because low-priority task locked the data, will wait...\n");
        // Busy wait a little to reduce CPU preemption frequency
        for (volatile int k = 0; k < 160000000; k++) {
            // Short busy wait
        }
    }
    printf("High-priority task: finally locked the data now!!!!!!!!!\n");

    printf("High-priority task: locked the data (hold for all rounds).\n");
    for (int round = 0; round < 10; round++) {
        // Busy work, do not sleep
        for (volatile int j = 0; j < 100000000; j++) {
            // Busy wait
        }
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

    // Initialize the mutex before creating threads
    pthread_mutex_init(&data_mutex, NULL);

    // Set thread attributes and scheduling parameters before creating threads
    pthread_attr_t attr;
    struct sched_param param;

    // Set the scheduling policy to real-time FIFO
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    // Set low priority for the low-priority task
    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&low, &attr, low_priority_task, NULL);

    // Set medium priority for the medium-priority task
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&medium, &attr, medium_priority_task, NULL);

    // Set high priority for the high-priority task
    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&high, &attr, high_priority_task, NULL);

    // Wait for all threads to finish
    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    // Destroy the mutex after all threads have finished
    pthread_mutex_destroy(&data_mutex);
    return 0;
}
