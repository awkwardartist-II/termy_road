
// contains a single character/entity
class Entity {
public:
    int X, Y; // current position
};

class Enemy {
public:
    Entity self; // instance of self entity
    int spawned = 0; // set if object on screen
    int is_vertical;
    int p_killed = 0;
    
    Enemy(); // on initialization
    void UpdatePos(); // update and draw enemy
private:
    void RandomPos(); // sets pos to random spawn pos
};