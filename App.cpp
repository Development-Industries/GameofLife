#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    // Initialize the main window (make sure the constructor parameters are correct)
    mainWindow = new MainWindow("Game of Life", wxPoint(0, 0), wxSize(200, 200));

    mainWindow->Show(true);
    return true;
}
