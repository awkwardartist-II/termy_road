#include "keyboard.h"
#include "Entity.h" 
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

WINDOW *GameWindow;
int Width, Height;

const int EnemyNum = 5;
Enemy AllEnemies[EnemyNum];
Entity player;

int PlayerAlive = 1;

int main() {
    // initialize RNG
    srand(time(NULL));

    Keyboard k;
    k.StartReading(); // initialize keyboard for reading    
    
    // now initialize ncurses
    GameWindow = initscr(); // initialize screen
    getmaxyx(GameWindow, Height, Width); // get screen width/height
    noecho(); // disable key echo
    curs_set(0); // disable cursor
reset_game:
    // position and reset player
    player.X = Width/2;
    player.Y = Height/2;
    mvaddstr(player.Y-2, player.X-(23/2), "Press any key to start.");
    mvaddch(player.Y, player.X, 'x');
    refresh();

    // initialize enemies
    for(int i = 0; i < EnemyNum; ++i) {
        AllEnemies[i] = Enemy(); // initialize enemy
    }
    getch();

    // player control loop
    while(PlayerAlive) {
        clear();
        if(k.IsKeyPressed(KEY_Q)) {
            // end main game loop
            break; 
        }

        // player control thread
        if(k.IsKeyPressed(KEY_W) && player.Y > 0)
            --player.Y; 
        else if(k.IsKeyPressed(KEY_S) && player.Y < Height-1)
            ++player.Y;
        if(k.IsKeyPressed(KEY_A) && player.X > 0)
            --player.X;
        else if(k.IsKeyPressed(KEY_D) && player.X < Width-1)
            ++player.X;
        // update each enemy
        for(int i = 0; i < EnemyNum; ++i) {
            AllEnemies[i].UpdatePos();
            if(AllEnemies[i].p_killed) {
                // this car killed the player
                PlayerAlive = 0;
            }
        }
        // now draw player
        if(PlayerAlive)
            mvaddch(player.Y, player.X, 'x');
        refresh();

        usleep(100*1000);
    }
    // reset enemies
    for(int i = 0; i < EnemyNum; ++i) {
        AllEnemies[i].spawned = 0;
        AllEnemies[i].p_killed = 0;
    }
    clear();
    mvaddstr(Height / 2, (Width / 2) - 9, "You Died!");
    refresh();

    // await keypress
    while(1) {
        if(k.IsKeyPressed(KEY_Q))
            break;
        if(k.IsKeyPressed(KEY_R)){
            clear();
            PlayerAlive = 1;
            
            // when R key released, reset game
            while(k.IsKeyPressed(KEY_R));
            goto reset_game;
        }
    }
    k.StopReading(); // stop input thread
    endwin(); // end ncurses window
    return 0;
}