#include "canvas.h"
#include "game.h"

int main(int argc, char **argv) {
    Game *billiards = new Game();

    Canvas *canvas = new Canvas();
    canvas->initialize(billiards);
    return 0;
}