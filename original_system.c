#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global shared data and mutex for synchronization
int shared_data = 0;
pthread_mutex_t data_mutex;

// Low-priority task thread function
void* low_priority_task(void* arg) {
    // Lock the mutex to access shared data
    pthread_mutex_lock(&data_mutex);
    printf("Low-priority task: locked the data.\n");
    sleep(2); // Simulate resource usage (holding the lock for a while)

    // Read the shared data while holding the lock
    printf("Low-priority task: reading data: %d\n", shared_data);
    sleep(3); // Intentionally delay releasing the lock to simulate priority inversion

    // Unlock the mutex after finishing work
    pthread_mutex_unlock(&data_mutex);
    printf("Low-priority task: unlocked the data.\n");
    return NULL;
}

// Medium-priority task thread function
void* medium_priority_task(void* arg) {
    // This task runs independently and does not access shared data
    // It simulates CPU usage that could preempt the high-priority task
    for (int i = 0; i < 5; i++) {
        printf("Medium-priority task: running...\n");
        sleep(1);
    }
    return NULL;
}

// High-priority task thread function
void* high_priority_task(void* arg) {
    // Sleep to ensure the low-priority task starts first and locks the mutex
    sleep(1);
    printf("High-priority task: trying to lock the data...\n");

    // Attempt to lock the mutex (may be blocked if low-priority task holds it)
    pthread_mutex_lock(&data_mutex);

    // Update the shared data after acquiring the lock
    shared_data += 1;
    printf("High-priority task: updated data to %d\n", shared_data);

    // Unlock the mutex after updating
    pthread_mutex_unlock(&data_mutex);
    printf("High-priority task: unlocked the data.\n");
    return NULL;
}

int main() {
    pthread_t low, medium, high;

    // Initialize the mutex before creating threads
    pthread_mutex_init(&data_mutex, NULL);

    // Create the low-priority thread first to ensure it locks the mutex
    pthread_create(&low, NULL, low_priority_task, NULL);
    sleep(1); // Ensure the low-priority task runs first and locks the mutex

    // Create the medium-priority and high-priority threads
    pthread_create(&medium, NULL, medium_priority_task, NULL);
    pthread_create(&high, NULL, high_priority_task, NULL);

    // Wait for all threads to finish
    pthread_join(low, NULL);
    pthread_join(medium, NULL);
    pthread_join(high, NULL);

    // Destroy the mutex after all threads have finished
    pthread_mutex_destroy(&data_mutex);
    return 0;
}
