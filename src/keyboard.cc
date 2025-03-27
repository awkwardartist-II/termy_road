#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "keyboard.h"

// keyboard event types
enum {
    KEYB_RELEASED,
    KEYB_PRESSED,
    KEYB_REPEATED,
};

// the thread function responsible for detecting
// key press events and setting the keymap
void *InputThread(void *KeyInfo) {
    // ensure thread can be killed
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    InputInfo *Info = (InputInfo*)KeyInfo; // keyboard input information
    int *KeyMap = &(Info->AllKeys[0]); // map of all keys

    static const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return (void*)EXIT_FAILURE; // return error
    }
    Info->IO_fd = fd; // set file descriptor for later

    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
        // now we have the key, do something with it
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
            int val = 0;
            if(ev.value == KEYB_PRESSED || ev.value == KEYB_REPEATED)
                val = 1; // set as pressed key
            if(ev.code > 0xFF) 
                continue; // invalid key press
            KeyMap[ev.code] = val; // set key press
        }
    }
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return (void*)EXIT_FAILURE;
}

// read all available keys in input buffer
void Keyboard::StartReading(void) {
    // initialize input_thread
    memset(Info.AllKeys, 0, KEY_MAX*sizeof(int)); // clear keymap
    pthread_create(&this->Thread, NULL, InputThread, (void*)&(this->Info)); // create thread for reading input   
}

void Keyboard::StopReading(void) {
    pthread_cancel(Thread); // kill thread
}

int Keyboard::IsKeyPressed(int Key) {
    // invalid key press
    if(Key > KEY_MAX) 
        return 0;
    return this->Info.AllKeys[Key];
}