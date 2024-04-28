#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
using namespace std;

mutex mtx;
sem_t sem;

void runMeFirst()
{
    cout << "Run me first\n";
    sem_post(&sem);
}
void runMeSecond()
{
    sem_wait(&sem);
    cout << "Run me second\n";
}
void runMeThird()
{
    sem_wait(&sem);
    cout << "Run me third\n";
}
int main()
{
    const int num_threads = 3;
    // initialize the semaphore with the number of threads
    sem_init(&sem, 0, 2); // 0: thread, 1; process
    thread threads[num_threads];
    threads[0] = thread(runMeFirst);
    threads[1] = thread(runMeSecond);
    threads[2] = thread(runMeThird);

    for (int i = 0; i < num_threads; ++i)
    {
        threads[i].join();
    }
    sem_destroy(&sem); // destroy the semaphore
    return 0;
}