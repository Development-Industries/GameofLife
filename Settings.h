#ifndef SETTINGS_H
#define SETTINGS_H

#include "wx/wx.h"
#include <fstream>

// Structure to store the settings of the game
struct Settings {
    int gridSize = 15;  // Default grid size (number of cells)
    int interval = 50;  // Timer interval in milliseconds for updating the game
    bool showNeighborCount = false;  // Option to show/hide neighbor count
    bool showGrid = true;  // Option to show/hide the grid lines
    bool showThickGrid = false;  // Option to show/hide thicker 10x10 grid lines
    bool showHUD = true;  // Option to show/hide the HUD display (Heads-Up Display)
    bool isToroidal = false;  // Boundary type (finite or toroidal)

    // RGBA values for living and dead cells
    unsigned int livingCellRed = 128;
    unsigned int livingCellGreen = 128;
    unsigned int livingCellBlue = 128;
    unsigned int livingCellAlpha = 255;

    unsigned int deadCellRed = 255;
    unsigned int deadCellGreen = 255;
    unsigned int deadCellBlue = 255;
    unsigned int deadCellAlpha = 255;

    // Method to get wxColour for living cells
    wxColour GetLivingCellColor() const {
        return wxColour(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
    }

    // Method to get wxColour for dead cells
    wxColour GetDeadCellColor() const {
        return wxColour(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
    }

    // Setters for cell colors
    void SetLivingCellColor(const wxColour& color) {
        livingCellRed = color.Red();
        livingCellGreen = color.Green();
        livingCellBlue = color.Blue();
        livingCellAlpha = color.Alpha();
    }

    void SetDeadCellColor(const wxColour& color) {
        deadCellRed = color.Red();
        deadCellGreen = color.Green();
        deadCellBlue = color.Blue();
        deadCellAlpha = color.Alpha();
    }

    // Save settings to a file (binary format)
    void Save() const {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(this), sizeof(Settings));
            file.close();
        }
    }

    // Load settings from a file (binary format)
    void Load() {
        std::ifstream file("settings.bin", std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read(reinterpret_cast<char*>(this), sizeof(Settings));
            file.close();
        }
    }

    // Method to reset all settings to default values
    void ResetToDefaults() {
        *this = Settings();  // Reassign to default-constructed Settings object
        Save();  // Save the default settings immediately
    }
};

#endif // SETTINGS_H
