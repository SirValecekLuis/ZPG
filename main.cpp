#include "application.h"


int main() {
    auto *app = new Application();

    app->init();
    app->start();

    delete app;
}
