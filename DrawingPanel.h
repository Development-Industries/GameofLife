#ifndef DRAWINGPANEL_H  // Start the include guard
#define DRAWINGPANEL_H

#include "wx/wx.h"
#include "Settings.h"  // Make sure Settings.h is included
#include <vector>

class DrawingPanel : public wxPanel {
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef);
    ~DrawingPanel();

    void SetSettings(Settings* settingsPtr);  // Setter for settings pointer
    void OnPaint(wxPaintEvent& evt);
    void OnResize(wxSizeEvent& event);
    void OnMouseUp(wxMouseEvent& event);

private:
    std::vector<std::vector<bool>>& gameBoard;  // Reference to the game board
    Settings* settings = nullptr;  // Pointer to the Settings object

    wxDECLARE_EVENT_TABLE();  // Declare the event table for DrawingPanel
};

#endif  // End of DRAWINGPANEL_H include guard
