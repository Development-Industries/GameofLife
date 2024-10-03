#ifndef APP_H
#define APP_H

#include "wx/wx.h"
#include "MainWindow.h"  // Ensure this includes your main window class

class App : public wxApp {
public:
    virtual bool OnInit();

private:
    MainWindow* mainWindow = nullptr;  // Ensure correct initialization
};

#endif // APP_H
