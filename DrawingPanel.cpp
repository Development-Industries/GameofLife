#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"  // Include to access MainWindow and its methods

// Event table for handling events in DrawingPanel
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)      // Paint event
EVT_LEFT_UP(DrawingPanel::OnMouseUp)  // Mouse click event
EVT_SIZE(DrawingPanel::OnResize)      // Window resize event
wxEND_EVENT_TABLE()

// Constructor for DrawingPanel, linking it with the game board and settings
DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef)
    : wxPanel(parent, wxID_ANY), gameBoard(gameBoardRef), settings(nullptr) {
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);  // Set background style to avoid flickering
}

// Destructor for DrawingPanel (currently nothing to clean up)
DrawingPanel::~DrawingPanel() {}

// Set the settings object to allow DrawingPanel to access application settings
void DrawingPanel::SetSettings(Settings* settingsPtr) {
    settings = settingsPtr;
}

// Paint event handler: responsible for drawing the grid, cells, and HUD
void DrawingPanel::OnPaint(wxPaintEvent& evt) {
    wxAutoBufferedPaintDC dc(this);  // Use double-buffering to avoid flickering during drawing
    dc.Clear();  // Clear the previous drawings

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);  // Create a graphics context for drawing
    if (!context) return;  // Exit if the context couldn't be created

    wxSize panelSize = GetClientSize();  // Get the size of the drawing panel

    // Calculate the width and height of each cell based on the grid size and panel size
    int cellWidth = panelSize.GetWidth() / settings->gridSize;
    int cellHeight = panelSize.GetHeight() / settings->gridSize;

    // Set the pen color for grid lines (default is black)
    context->SetPen(*wxBLACK);

    // Loop through the game board and draw cells (alive/dead) and grid lines
    for (int row = 0; row < settings->gridSize; ++row) {
        for (int col = 0; col < settings->gridSize; ++col) {
            int x = col * cellWidth;
            int y = row * cellHeight;

            // Set the color of the brush based on whether the cell is alive or dead
            if (gameBoard[row][col]) {
                context->SetBrush(wxBrush(settings->GetLivingCellColor()));
            }
            else {
                context->SetBrush(wxBrush(settings->GetDeadCellColor()));
            }

            // Draw each cell as a rectangle
            context->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    // HUD Drawing: Only draw if HUD is enabled in the settings
    if (settings->showHUD) {
        wxFont font(wxFontInfo(12).Bold());  // Set the font for the HUD text
        context->SetFont(font, *wxRED);      // Set red color for the HUD text

        // Access MainWindow to retrieve generationCount and livingCellsCount
        MainWindow* parent = static_cast<MainWindow*>(GetParent());

        wxString hudText = wxString::Format(
            "Generations: %d\nLiving Cells: %d\nBoundary: %s\nGrid Size: %d x %d",
            parent->GetGenerationCount(), parent->GetLivingCellsCount(),
            settings->isToroidal ? "Toroidal" : "Finite", settings->gridSize, settings->gridSize
        );

        double textWidth, textHeight;
        context->GetTextExtent(hudText, &textWidth, &textHeight);  // Get the size of the text

        // Draw the HUD text in the bottom left corner of the panel
        context->DrawText(hudText, 10, panelSize.GetHeight() - textHeight - 10);
    }

    delete context;  // Clean up the graphics context after use
}

// Resize event handler: refresh the panel when resized
void DrawingPanel::OnResize(wxSizeEvent& event) {
    Refresh();  // Redraw the panel to fit the new size
    event.Skip();  // Allow default handling of the resize event
}

// Mouse click event handler: toggles the state of a clicked cell (alive/dead)
void DrawingPanel::OnMouseUp(wxMouseEvent& event) {
    int mouseX = event.GetX();  // Get X-coordinate of the mouse click
    int mouseY = event.GetY();  // Get Y-coordinate of the mouse click

    wxSize panelSize = GetClientSize();  // Get the size of the panel

    // Calculate which cell was clicked based on the mouse position
    int cellWidth = panelSize.GetWidth() / settings->gridSize;
    int cellHeight = panelSize.GetHeight() / settings->gridSize;

    // Ensure valid cell size to prevent division by zero errors
    if (cellWidth == 0 || cellHeight == 0) return;

    // Calculate the row and column of the clicked cell
    int col = mouseX / cellWidth;
    int row = mouseY / cellHeight;

    // Ensure the clicked cell is within the grid bounds
    if (row >= 0 && row < settings->gridSize && col >= 0 && col < settings->gridSize) {
        gameBoard[row][col] = !gameBoard[row][col];  // Toggle the cell's state (alive or dead)
    }

    Refresh();  // Redraw the panel to reflect the updated cell state
}
