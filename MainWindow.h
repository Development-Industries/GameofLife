#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "wx/wx.h"               // wxWidgets library for GUI elements
#include "wx/timer.h"            // wxTimer for handling time-based events
#include "DrawingPanel.h"        // Custom class that handles rendering the game board
#include "Settings.h"            // Custom class that holds the application's settings
#include "SettingsDialog.h"      // Custom dialog for modifying settings
#include <vector>                // STL vector for handling game board data

class MainWindow : public wxFrame {
public:
    // Constructor: Initializes the main window with title, position, and size
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    // Destructor: Cleans up resources when MainWindow is destroyed
    ~MainWindow();

    // Event Handlers for buttons and menu items
    void OnPlay(wxCommandEvent& event);               // Start game timer
    void OnPause(wxCommandEvent& event);              // Stop game timer
    void OnNext(wxCommandEvent& event);               // Advance one generation
    void OnClear(wxCommandEvent& event);              // Clear the game board
    void OnOpenSettings(wxCommandEvent& event);       // Open settings dialog
    void OnTimer(wxTimerEvent& event);                // Timer event to trigger generation updates
    void OnToggleNeighborCount(wxCommandEvent& event);// Toggle the display of neighbor counts
    void OnRandomize(wxCommandEvent& event);          // Randomly populate the grid
    void OnRandomizeWithSeed(wxCommandEvent& event);  // Randomly populate the grid with a user-provided seed
    void OnResetSettings(wxCommandEvent& event);      // Reset settings to default
    void ImportGameBoard(wxCommandEvent& event);      // Import a game board from a file

    // Event handlers for toggling view options
    void OnToggleShowGrid(wxCommandEvent& event);     // Show/hide the grid
    void OnToggleShowThickGrid(wxCommandEvent& event);// Show/hide thick 10x10 grid lines

    // Event handlers for setting universe boundary types
    void OnSetFinite(wxCommandEvent& event);          // Set universe to finite
    void OnSetToroidal(wxCommandEvent& event);        // Set universe to toroidal

    // Event handlers for file operations
    void OnNew(wxCommandEvent& event);                // Create a new game board
    void OnOpen(wxCommandEvent& event);               // Open a saved game board
    void OnSave(wxCommandEvent& event);               // Save the current game board
    void OnSaveAs(wxCommandEvent& event);             // Save the current game board as a new file
    void OnExit(wxCommandEvent& event);               // Exit the application

    // Game logic methods
    void NextGeneration();                            // Calculate and advance to the next generation
    void ClearBoard();                                // Clear the game board, resetting all cells
    void RandomizeGrid(int seed);                     // Populate the game board with random cells
    int CountLivingNeighbors(int row, int col);       // Count the number of living neighbors for a specific cell
    void UpdateStatusBar();                           // Update the status bar with generation and living cell count

    // File I/O methods
    void SaveToFile(const wxString& fileName);        // Save the current game board to a file
    void LoadFromFile(const wxString& fileName);      // Load a game board from a file


    int GetGenerationCount() const { return generation; }  // Getter for generation count
    int GetLivingCellsCount() const { return livingCells; }  // Getter for living cells count

private:
    DrawingPanel* drawingPanel;                       // Panel for drawing the game board
    std::vector<std::vector<bool>> gameBoard;         // The game board: a 2D vector of cells (true = alive, false = dead)
    int generation = 0;                               // Current generation count
    int livingCells = 0;                              // Number of living cells on the board
    wxStatusBar* statusBar;                           // Status bar to display generation and living cell count
    wxTimer* timer;                                   // Timer to advance generations automatically

    Settings settings;                                // Application settings (grid size, colors, etc.)
    wxString currentFileName;                         // Name of the current file (for Save/Save As operations)

    // Enum for menu item IDs to avoid conflicts with built-in wxWidgets IDs
    enum {
        ID_SETTINGS = wxID_HIGHEST + 1,               // Menu ID for opening the settings dialog
        ID_TOGGLE_NEIGHBOR_COUNT,                     // Menu ID for toggling neighbor counts
        ID_RANDOMIZE,                                 // Menu ID for randomizing the grid
        ID_RANDOMIZE_WITH_SEED,                       // Menu ID for randomizing with a custom seed
        ID_NEW,                                       // Menu ID for creating a new game board
        ID_OPEN,                                      // Menu ID for opening a saved game board
        ID_SAVE,                                      // Menu ID for saving the current game board
        ID_SAVE_AS,                                   // Menu ID for saving the game board as a new file
        ID_EXIT,                                      // Menu ID for exiting the application
        ID_VIEW_FINITE,                               // Menu ID for setting finite universe boundary
        ID_VIEW_TOROIDAL,                             // Menu ID for setting toroidal universe boundary
        ID_RESET_SETTINGS,                            // Menu ID for resetting settings to default
        ID_IMPORT,                                    // Menu ID for importing a game board pattern
        ID_VIEW_SHOW_GRID,                            // Menu ID for toggling grid visibility
        ID_VIEW_SHOW_THICK_GRID                       // Menu ID for toggling thick 10x10 grid lines
    };

    // Helper method to initialize the menu bar with all the options
    void InitializeMenuBar();

    // Event table declaration for wxWidgets event handling
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINWINDOW_H
