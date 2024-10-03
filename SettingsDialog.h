#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "wx/wx.h"
#include "wx/spinctrl.h"  // For wxSpinCtrl
#include "wx/clrpicker.h" // For wxColourPickerCtrl
#include "Settings.h"     // Include the settings object

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent, Settings* settings);

    // Event handlers for buttons
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

private:
    Settings* settings;  // Pointer to the settings object

    // Controls
    wxSpinCtrl* gridSizeCtrl;
    wxSpinCtrl* intervalCtrl;
    wxColourPickerCtrl* livingCellColorPicker;
    wxColourPickerCtrl* deadCellColorPicker;

    // Method to initialize controls and set default values
    void InitializeControls();

    wxDECLARE_EVENT_TABLE();
};

#endif // SETTINGSDIALOG_H
