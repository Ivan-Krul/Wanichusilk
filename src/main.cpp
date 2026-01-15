#include "Application.h"

int main(int args, char** argv) {
    Application App;
    App.OnInit();

    while (App.IsRunning()) {
        App.OnLoop();
    }

    return App.OnExit();
}
