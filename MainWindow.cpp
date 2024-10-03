#include "MainWindow.h"
#include "wx/filedlg.h"  // For wxFileDialog (used in Open, Save, Import operations)
#include <wx/numdlg.h>   // For wxGetNumberFromUser (used for randomization with a seed)
#include "pause.xpm"     // Bitmap for the Pause button
#include "play.xpm"      // Bitmap for the Play button
#include "next.xpm"      // Bitmap for the Next button
#include "trash.xpm"     // Bitmap for the Clear button

// Event table linking menu IDs to event handler functions
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_NEW, MainWindow::OnNew)                        // New game board
EVT_MENU(ID_OPEN, MainWindow::OnOpen)                      // Open a saved game board
EVT_MENU(ID_SAVE, MainWindow::OnSave)                      // Save the current game board
EVT_MENU(ID_SAVE_AS, MainWindow::OnSaveAs)                 // Save the game board as a new file
EVT_MENU(ID_EXIT, MainWindow::OnExit)                      // Exit the application
EVT_MENU(10001, MainWindow::OnPlay)                        // Play button
EVT_MENU(10002, MainWindow::OnPause)                       // Pause button
EVT_MENU(10003, MainWindow::OnNext)                        // Next generation button
EVT_MENU(10004, MainWindow::OnClear)                       // Clear the game board
EVT_MENU(ID_SETTINGS, MainWindow::OnOpenSettings)          // Open settings dialog
EVT_MENU(ID_TOGGLE_NEIGHBOR_COUNT, MainWindow::OnToggleNeighborCount)  // Toggle neighbor count visibility
EVT_MENU(ID_RANDOMIZE, MainWindow::OnRandomize)            // Randomize the grid
EVT_MENU(ID_RANDOMIZE_WITH_SEED, MainWindow::OnRandomizeWithSeed)  // Randomize with a custom seed
EVT_MENU(ID_VIEW_FINITE, MainWindow::OnSetFinite)          // Set universe to finite boundary
EVT_MENU(ID_VIEW_TOROIDAL, MainWindow::OnSetToroidal)      // Set universe to toroidal boundary
EVT_MENU(ID_RESET_SETTINGS, MainWindow::OnResetSettings)   // Reset settings to default
EVT_MENU(ID_IMPORT, MainWindow::ImportGameBoard)           // Import a game board pattern
EVT_MENU(ID_VIEW_SHOW_GRID, MainWindow::OnToggleShowGrid)  // Toggle grid visibility
EVT_MENU(ID_VIEW_SHOW_THICK_GRID, MainWindow::OnToggleShowThickGrid)  // Toggle 10x10 grid lines
EVT_TIMER(20001, MainWindow::OnTimer)                      // Timer for advancing generations
wxEND_EVENT_TABLE()

// Constructor for MainWindow
MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), timer(new wxTimer(this, 20001)) {

    // Load settings from file (e.g., grid size, show grid, etc.)
    settings.Load();

    // Initialize the game board with the grid size from the settings
    gameBoard.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));

    // Create the drawing panel and pass references to the game board and settings
    drawingPanel = new DrawingPanel(this, gameBoard);
    drawingPanel->SetSettings(&settings);  // Pass settings to the drawing panel

    // Create the status bar (bottom bar showing generations and living cell count)
    statusBar = CreateStatusBar();
    UpdateStatusBar();  // Initialize the status bar text

    // Initialize the menu bar (File, View, Options menus)
    InitializeMenuBar();

    // Create toolbar with buttons for Play, Pause, Next, and Clear
    wxToolBar* toolbar = CreateToolBar();

    wxBitmap playIcon(play_xpm);
    toolbar->AddTool(10001, "Play", playIcon);  // Play button
    wxBitmap pauseIcon(pause_xpm);
    toolbar->AddTool(10002, "Pause", pauseIcon);  // Pause button
    wxBitmap nextIcon(next_xpm);
    toolbar->AddTool(10003, "Next", nextIcon);  // Next generation button
    wxBitmap trashIcon(trash_xpm);
    toolbar->AddTool(10004, "Clear", trashIcon);  // Clear board button

    // Finalize and display the toolbar
    toolbar->Realize();

    // Ensure the layout is correct
    this->Layout();
}

// Destructor for MainWindow
MainWindow::~MainWindow() {
    if (timer) {
        delete timer;  // Clean up timer resource
    }
}

// Initialize the menu bar with File, View, and Options menus
void MainWindow::InitializeMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar();

    // File menu: New, Open, Save, Import, Save As, Exit
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_NEW, "&New\tCtrl-N", "Create a new game board");
    fileMenu->Append(ID_OPEN, "&Open\tCtrl-O", "Open an existing game board");
    fileMenu->Append(ID_SAVE, "&Save\tCtrl-S", "Save the current game board");
    fileMenu->Append(ID_IMPORT, "&Import\tCtrl-I", "Import a game board pattern");
    fileMenu->Append(ID_SAVE_AS, "Save &As", "Save the game board to a new file");
    fileMenu->Append(ID_EXIT, "E&xit", "Exit the application");
    menuBar->Append(fileMenu, "&File");

    // View menu: Finite, Toroidal, Show Grid, Show Thick Grid
    wxMenu* viewMenu = new wxMenu();
    wxMenuItem* finiteItem = new wxMenuItem(viewMenu, ID_VIEW_FINITE, "Finite", "", wxITEM_CHECK);
    finiteItem->SetCheckable(true);
    viewMenu->Append(finiteItem);  // Append the item
    finiteItem->Check(!settings.isToroidal);  // Check if current setting is Finite

    wxMenuItem* toroidalItem = new wxMenuItem(viewMenu, ID_VIEW_TOROIDAL, "Toroidal", "", wxITEM_CHECK);
    toroidalItem->SetCheckable(true);
    viewMenu->Append(toroidalItem);  // Append the item
    toroidalItem->Check(settings.isToroidal);  // Check if current setting is Toroidal

    wxMenuItem* showGridItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_GRID, "Show Grid", "", wxITEM_CHECK);
    showGridItem->SetCheckable(true);
    viewMenu->Append(showGridItem);  // Show/hide grid
    showGridItem->Check(settings.showGrid);  // Set default check state from settings

    wxMenuItem* showThickGridItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_THICK_GRID, "Show 10x10 Grid", "", wxITEM_CHECK);
    showThickGridItem->SetCheckable(true);
    viewMenu->Append(showThickGridItem);  // Show/hide thick grid lines
    showThickGridItem->Check(settings.showThickGrid);  // Set default check state from settings

    menuBar->Append(viewMenu, "&View");

    // Options menu: Settings, Randomize, Reset Settings
    wxMenu* optionsMenu = new wxMenu();
    optionsMenu->Append(ID_SETTINGS, "Settings", "Open Settings Dialog");
    optionsMenu->Append(ID_RANDOMIZE, "Randomize Grid", "Randomize the grid with time as a seed");
    optionsMenu->Append(ID_RANDOMIZE_WITH_SEED, "Randomize Grid with Seed", "Randomize the grid with a custom seed");
    optionsMenu->Append(ID_RESET_SETTINGS, "Reset Settings", "Reset all settings to their default state");
    menuBar->Append(optionsMenu, "&Options");

    // Set the created menu bar as the window's menu bar
    SetMenuBar(menuBar);
}

// Event handler for creating a new board
void MainWindow::OnNew(wxCommandEvent& event) {
    ClearBoard();  // Clear the game board
    currentFileName.Clear();  // Remove the file name for Save operations
}

// Event handler for opening an existing game board file
void MainWindow::OnOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open game board"), "", "",
        "Game files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;  // Cancelled by the user

    LoadFromFile(openFileDialog.GetPath());  // Load the file
}

// Event handler for saving the game board
void MainWindow::OnSave(wxCommandEvent& event) {
    if (currentFileName.IsEmpty()) {
        OnSaveAs(event);  // If no file name, fall back to Save As
    }
    else {
        SaveToFile(currentFileName);  // Save using the current file name
    }
}

// Event handler for saving the game board as a new file
void MainWindow::OnSaveAs(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, _("Save game board as"), "", "",
        "Game files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;  // Cancelled by the user

    currentFileName = saveFileDialog.GetPath();  // Save the file path
    SaveToFile(currentFileName);
}

// Event handler for exiting the program
void MainWindow::OnExit(wxCommandEvent& event) {
    Close(true);  // Close the main window and exit the application
}

// Event handler for setting finite universe
void MainWindow::OnSetFinite(wxCommandEvent& event) {
    settings.isToroidal = false;
    wxMenuBar* menuBar = GetMenuBar();
    menuBar->FindItem(ID_VIEW_TOROIDAL)->Check(false);  // Uncheck Toroidal
    menuBar->FindItem(ID_VIEW_FINITE)->Check(true);     // Check Finite
}

// Event handler for setting toroidal universe
void MainWindow::OnSetToroidal(wxCommandEvent& event) {
    settings.isToroidal = true;
    wxMenuBar* menuBar = GetMenuBar();
    menuBar->FindItem(ID_VIEW_TOROIDAL)->Check(true);   // Check Toroidal
    menuBar->FindItem(ID_VIEW_FINITE)->Check(false);    // Uncheck Finite
}

// Event handler for importing a game board
void MainWindow::ImportGameBoard(wxCommandEvent& event) {
    wxFileDialog importFileDialog(this, _("Import game board pattern"), "", "",
        "Game files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (importFileDialog.ShowModal() == wxID_CANCEL)
        return;  // Cancelled by the user

    std::ifstream file(importFileDialog.GetPath().ToStdString());

    if (!file.is_open()) {
        wxMessageBox("Failed to import the game board pattern.", "Error", wxICON_ERROR);
        return;
    }

    // Temporary game board for the pattern we're importing
    std::vector<std::vector<bool>> importedBoard;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '!') continue;  // Skip empty lines and comments

        std::vector<bool> row;
        for (char c : line) {
            row.push_back(c == '*');  // '*' is a live cell, '.' is a dead cell
        }
        importedBoard.push_back(row);
    }

    file.close();

    // Get dimensions of the imported pattern
    int importRows = importedBoard.size();
    int importCols = importedBoard[0].size();

    // Ensure pattern doesn't exceed the current grid size
    if (importRows > settings.gridSize || importCols > settings.gridSize) {
        wxMessageBox("Imported pattern exceeds the grid size. Data may be lost.", "Warning", wxICON_WARNING);
    }

    // Optionally: Center the imported pattern inside the grid
    int startRow = (settings.gridSize - importRows) / 2;
    int startCol = (settings.gridSize - importCols) / 2;

    // Place the pattern on the current game board without resizing the grid
    for (int i = 0; i < importRows; ++i) {
        for (int j = 0; j < importCols; ++j) {
            if (i + startRow < settings.gridSize && j + startCol < settings.gridSize) {
                gameBoard[i + startRow][j + startCol] = importedBoard[i][j];
            }
        }
    }

    drawingPanel->Refresh();  // Redraw the game board to reflect the new pattern
}

// Event handler for resetting settings to default
void MainWindow::OnResetSettings(wxCommandEvent& event) {
    settings.ResetToDefaults();  // Reset all settings to default
    wxMessageBox("Settings have been reset to default.", "Reset Complete", wxOK | wxICON_INFORMATION);  // Notify the user
}

// Event handler for playing the game (starting the simulation)
void MainWindow::OnPlay(wxCommandEvent& event) {
    timer->Start(settings.interval);  // Start the timer with the interval from settings
}

// Event handler for pausing the game (stopping the simulation)
void MainWindow::OnPause(wxCommandEvent& event) {
    timer->Stop();  // Stop the timer
}

// Event handler for moving to the next generation manually
void MainWindow::OnNext(wxCommandEvent& event) {
    NextGeneration();  // Move to the next generation
}

// Event handler for clearing the game board
void MainWindow::OnClear(wxCommandEvent& event) {
    ClearBoard();  // Clear the game board
}

// Event handler for randomizing the game board (using a random seed)
void MainWindow::OnRandomize(wxCommandEvent& event) {
    RandomizeGrid(static_cast<int>(time(nullptr)));  // Randomize using the current time as the seed
}

// Event handler for randomizing the game board with a user-defined seed
void MainWindow::OnRandomizeWithSeed(wxCommandEvent& event) {
    long seed = wxGetNumberFromUser(
        "Enter a seed for randomization",
        "Seed:", "Randomize with Seed", 0, LONG_MIN, LONG_MAX);

    if (seed != -1) {
        RandomizeGrid(static_cast<int>(seed));  // Randomize using the user-provided seed
    }
}

// Timer event handler to advance the game to the next generation
void MainWindow::OnTimer(wxTimerEvent& event) {
    NextGeneration();
}

// Function to advance to the next generation of cells (game logic)
void MainWindow::NextGeneration() {
    std::vector<std::vector<bool>> sandbox(gameBoard.size(), std::vector<bool>(gameBoard[0].size(), false));

    for (int row = 0; row < gameBoard.size(); ++row) {
        for (int col = 0; col < gameBoard[row].size(); ++col) {
            int livingNeighbors = CountLivingNeighbors(row, col);

            if (gameBoard[row][col] && (livingNeighbors == 2 || livingNeighbors == 3)) {
                sandbox[row][col] = true;
            }
            else if (!gameBoard[row][col] && livingNeighbors == 3) {
                sandbox[row][col] = true;
            }
        }
    }

    gameBoard.swap(sandbox);

    generation++;
    UpdateStatusBar();

    drawingPanel->Refresh();  // Redraw the game board with the new generation
}

// Function to count the number of living neighbors around a given cell
int MainWindow::CountLivingNeighbors(int row, int col) {
    int livingNeighbors = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;  // Skip the current cell

            int neighborRow = row + i;
            int neighborCol = col + j;

            if (settings.isToroidal) {
                // Handle wrapping for toroidal universe
                if (neighborRow < 0) neighborRow = settings.gridSize - 1;
                if (neighborRow >= settings.gridSize) neighborRow = 0;
                if (neighborCol < 0) neighborCol = settings.gridSize - 1;
                if (neighborCol >= settings.gridSize) neighborCol = 0;
            }
            else {
                // Skip out-of-bounds indices for finite universe
                if (neighborRow < 0 || neighborRow >= settings.gridSize || neighborCol < 0 || neighborCol >= settings.gridSize) {
                    continue;
                }
            }

            if (gameBoard[neighborRow][neighborCol]) {
                livingNeighbors++;
            }
        }
    }

    return livingNeighbors;
}

// Function to clear the game board (reset all cells to dead)
void MainWindow::ClearBoard() {
    for (auto& row : gameBoard) {
        std::fill(row.begin(), row.end(), false);
    }

    generation = 0;
    livingCells = 0;

    UpdateStatusBar();
    drawingPanel->Refresh();  // Redraw the empty game board
}

// Update the status bar with the current generation and living cells count
void MainWindow::UpdateStatusBar() {
    livingCells = 0;
    for (const auto& row : gameBoard) {
        livingCells += std::count(row.begin(), row.end(), true);
    }

    wxString statusText = wxString::Format("Generations: %d | Living Cells: %d", generation, livingCells);
    statusBar->SetStatusText(statusText);
}

// Save the current game board to a file
void MainWindow::SaveToFile(const wxString& fileName) {
    std::ofstream file(fileName.ToStdString());

    if (!file.is_open()) {
        wxMessageBox("Failed to save the game board.", "Error", wxICON_ERROR);
        return;
    }

    for (const auto& row : gameBoard) {
        for (bool cell : row) {
            file << (cell ? '*' : '.');
        }
        file << '\n';
    }

    file.close();
}

// Load a game board from a file
void MainWindow::LoadFromFile(const wxString& fileName) {
    std::ifstream file(fileName.ToStdString());

    if (!file.is_open()) {
        wxMessageBox("Failed to load the game board.", "Error", wxICON_ERROR);
        return;
    }

    std::vector<std::vector<bool>> newBoard;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '!') continue;

        std::vector<bool> row;
        for (char c : line) {
            row.push_back(c == '*');
        }
        newBoard.push_back(row);
    }

    settings.gridSize = newBoard.size();
    gameBoard = newBoard;

    file.close();

    Refresh();
}

// Toggle showing the grid in the game board
void MainWindow::OnToggleShowGrid(wxCommandEvent& event) {
    settings.showGrid = !settings.showGrid;
    drawingPanel->Refresh();  // Redraw the grid with/without grid lines
    settings.Save();
}

// Toggle showing thicker grid lines every 10 cells
void MainWindow::OnToggleShowThickGrid(wxCommandEvent& event) {
    settings.showThickGrid = !settings.showThickGrid;
    drawingPanel->Refresh();  // Redraw the grid with/without thick grid lines
    settings.Save();
}
void MainWindow::OnOpenSettings(wxCommandEvent& event) {
    Settings tempSettings = settings;  // Make a temporary copy of settings
    SettingsDialog dlg(this, &tempSettings);  // Create a dialog with temporary settings

    if (dlg.ShowModal() == wxID_OK) {  // If the user presses OK
        // Apply the new settings
        settings = tempSettings;
        settings.Save();  // Save settings to a file

        // Reinitialize game board size if grid size has changed
        gameBoard.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
        drawingPanel->SetSettings(&settings);  // Update the drawing panel with the new settings
        drawingPanel->Refresh();  // Redraw the grid
    }
}
void MainWindow::OnToggleNeighborCount(wxCommandEvent& event) {
    settings.showNeighborCount = !settings.showNeighborCount;  // Toggle the neighbor count setting
    drawingPanel->Refresh();  // Redraw the panel with updated neighbor count visibility
    settings.Save();  // Save the updated settings
}
void MainWindow::RandomizeGrid(int seed) {
    srand(seed);  // Seed the random number generator

    for (int row = 0; row < settings.gridSize; ++row) {
        for (int col = 0; col < settings.gridSize; ++col) {
            // Randomly set each cell as alive (45% chance) or dead (55% chance)
            gameBoard[row][col] = (rand() % 100) < 45;
        }
    }

    drawingPanel->Refresh();  // Redraw the grid to show the randomized cells
}
