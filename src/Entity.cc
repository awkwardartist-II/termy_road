#include "Entity.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

extern WINDOW *GameWindow;
extern Entity player;

int PlayerPoints = 0;

// spawn in and initialize enemy
Enemy::Enemy() {
    // initialize random spawn position
    this->RandomPos();
    this->spawned = TRUE; // set as spawned enemy (draw allowed)
}

void Enemy::RandomPos() {
    is_vertical = rand() % 2; // true/false is vertical
    if(is_vertical) {
        // random position on bottom of screen
        self.X = rand() % getmaxx(GameWindow);
        self.Y = 1;
    } else {
        // random position on right of screen
        self.Y = rand() % getmaxy(GameWindow);
        self.X = 1;
    }
}

// update and draw enemy
void Enemy::UpdatePos() {
    if(!spawned) {
        RandomPos(); // reset position & continue
        spawned = 1;

        PlayerPoints++; // add one to point counter
        return;
    }

    // update position
    if(!is_vertical) {
        self.X++;
    } else self.Y++;

    // check collisions
    if(self.X >= getmaxx(GameWindow)-1 || self.Y >= getmaxy(GameWindow)-1) {
        spawned = 0; // de-spawn to reposition later
    }

    // check 4 collisions
    if(self.X == player.X && self.Y == player.Y) {
        // holy shit you just died
        p_killed = 1;
    }
    else if(self.X+1 == player.X && self.Y == player.Y) {
        // holy shit you just died
        p_killed = 1;
    }
    else if(self.X == player.X && self.Y+1 == player.Y) {
        // holy shit you just died
        p_killed = 1;
    }
    else if(self.X+1 == player.X && self.Y+1 == player.Y) {
        // holy shit you just died
        p_killed = 1;
    }

    // now draw cars
    mvaddch(self.Y, self.X, '0');
    mvaddch(self.Y+1, self.X, '0');
    mvaddch(self.Y, self.X+1, '0');
    mvaddch(self.Y+1, self.X+1, '0');
}