#include <iostream>

#include "gameoflife.h"

// ok
using namespace std;

int main()
{

    Gameoflife *game = new Gameoflife(60); // grid size

    game->run();

    return 0;
}
