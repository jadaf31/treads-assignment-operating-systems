#include <iostream>
#include <thread>
#include <dispatch/dispatch.h>
#include <string>
#include <queue>
#include <chrono>
#include <random>
using namespace std;

const int BUFFER_SIZE = 10;
const int MAX_ITERATIONS = 20;
struct requestStructure
{
    int request_id;
    string ip_address;
    string page_requested;
};

queue<requestStructure> msg_queue;
dispatch_semaphore_t empty1;
dispatch_semaphore_t full1;
dispatch_semaphore_t mutex1;

string webPages[10] = {"google.com", "yahoo.com", "loyno.edu", "canvas.org", "github.com",
                       "wikipedia.org", "amazon.com", "ebay.com", "cnn.com", "youtube.com"};

int counter = 0;

void producer()
{
    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        requestStructure request;
        request.request_id = ++counter;
        request.ip_address = "";
        request.page_requested = webPages[rand() % 10];

        dispatch_semaphore_wait(empty1, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(mutex1, DISPATCH_TIME_FOREVER);

        msg_queue.push(request);

        dispatch_semaphore_signal(mutex1);
        dispatch_semaphore_signal(full1);
    }
}

void consumer()
{
    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        dispatch_semaphore_wait(full1, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(mutex1, DISPATCH_TIME_FOREVER);

        requestStructure request = msg_queue.front();
        msg_queue.pop();

        dispatch_semaphore_signal(mutex1);
        dispatch_semaphore_signal(empty1);

        // Display message
        cout << "Completed request " << request.request_id
             << " requesting webpage " << request.page_requested << endl;
    }
}

int main()
{
    empty1 = dispatch_semaphore_create(BUFFER_SIZE);
    full1 = dispatch_semaphore_create(0);
    mutex1 = dispatch_semaphore_create(1);

    thread prod_thread(producer);
    thread cons_thread(consumer);

    prod_thread.join();
    cons_thread.join();

    return 0;
}