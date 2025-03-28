#include "keyboard.h"
#include "Entity.h" 
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// ncurses data
WINDOW *GameWindow;
int Width, Height;

// enemy/entity data
const int EnemyNum = 5;
Enemy AllEnemies[EnemyNum];
Entity player;

// player and game-state data
int PlayerAlive = 1;
extern int PlayerPoints;
const int UpdateMS = 75;

int main() {
    // score display
    const int score_len = 16; // display length
    char score_str[score_len] = {0}; // points display string

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

        // reset and display point string
        memset(&score_str[0], 0, score_len); // clear point string
        snprintf(&score_str[0], score_len, "Score: %d", PlayerPoints / EnemyNum); // set point string
        mvaddstr(0, 0, &score_str[0]); // add score string
        
        if(k.IsKeyPressed(KEY_Q)) {
            // wait for release and then quit
            while(k.IsKeyPressed(KEY_Q));
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

        usleep(UpdateMS*1000);
    }
    // reset enemies
    for(int i = 0; i < EnemyNum; ++i) {
        AllEnemies[i].spawned = 0;
        AllEnemies[i].p_killed = 0;
    }
    // draw 'You Died!' screen
    clear();
    if(!PlayerAlive) {
        mvaddstr(Height / 2, (Width / 2) - 9, "You Died!");
        mvaddstr((Height / 2)+1, (Width/2)-(strlen(score_str)+1), score_str);
        refresh();
    }

    // await keypress
    while(!PlayerAlive) {
        if(k.IsKeyPressed(KEY_Q)) {
            // wait for release and then quit
            while(k.IsKeyPressed(KEY_Q));
            break;
        }
        if(k.IsKeyPressed(KEY_R)) {
            // when R key released, reset game
            while(k.IsKeyPressed(KEY_R));

            // now reset game
            clear();
            PlayerAlive = 1;
            goto reset_game;
        }
    }
    // end program
    k.StopReading(); // stop input thread
    endwin(); // end ncurses window
    return 0;
}