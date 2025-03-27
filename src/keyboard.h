#pragma once
#include <pthread.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

// information associated with
// input thread
typedef struct {
    int IO_fd;
    int AllKeys[KEY_MAX]; // map of pressed keys
} InputInfo;

// keyboard class for multithreaded 
// input events
class Keyboard {
public:
    // start or stop input thread
    void StartReading();
    void StopReading();

    // checks if key currently down
    int IsKeyPressed(int Key);
private:
    pthread_t Thread; // thread used for reading IO
    InputInfo Info; // information associated with thread
};