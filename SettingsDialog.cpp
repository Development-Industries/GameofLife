#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settingsPtr)
    : wxDialog(parent, wxID_ANY, "Configure Settings", wxDefaultPosition, wxDefaultSize),
    settings(settingsPtr) {

    // Main vertical box sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Grid Size (using wxSpinCtrl)
    wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size: ");
    gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 50, settings->gridSize);
    gridSizeSizer->Add(gridSizeLabel, 0, wxALL, 5);
    gridSizeSizer->Add(gridSizeCtrl, 0, wxALL, 5);
    mainSizer->Add(gridSizeSizer, 0, wxEXPAND);

    // Interval (using wxSpinCtrl)
    wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* intervalLabel = new wxStaticText(this, wxID_ANY, "Interval (ms): ");
    intervalCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, settings->interval);
    intervalSizer->Add(intervalLabel, 0, wxALL, 5);
    intervalSizer->Add(intervalCtrl, 0, wxALL, 5);
    mainSizer->Add(intervalSizer, 0, wxEXPAND);

    // Living Cell Color (using wxColourPickerCtrl)
    wxBoxSizer* livingCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* livingCellLabel = new wxStaticText(this, wxID_ANY, "Living Cell Color: ");
    livingCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetLivingCellColor());
    livingCellColorSizer->Add(livingCellLabel, 0, wxALL, 5);
    livingCellColorSizer->Add(livingCellColorPicker, 0, wxALL, 5);
    mainSizer->Add(livingCellColorSizer, 0, wxEXPAND);

    // Dead Cell Color (using wxColourPickerCtrl)
    wxBoxSizer* deadCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* deadCellLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color: ");
    deadCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetDeadCellColor());
    deadCellColorSizer->Add(deadCellLabel, 0, wxALL, 5);
    deadCellColorSizer->Add(deadCellColorPicker, 0, wxALL, 5);
    mainSizer->Add(deadCellColorSizer, 0, wxEXPAND);

    // OK and Cancel buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // Set the sizer for the dialog
    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

// Event handler for the OK button
void SettingsDialog::OnOk(wxCommandEvent& event) {
    // Apply the changes to the settings object
    settings->gridSize = gridSizeCtrl->GetValue();
    settings->interval = intervalCtrl->GetValue();
    settings->SetLivingCellColor(livingCellColorPicker->GetColour());
    settings->SetDeadCellColor(deadCellColorPicker->GetColour());

    // Close the dialog
    EndModal(wxID_OK);
}

// Event handler for the Cancel button
void SettingsDialog::OnCancel(wxCommandEvent& event) {
    // Do not apply any changes
    EndModal(wxID_CANCEL);
}
