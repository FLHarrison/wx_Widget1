#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "MineSweeper - Widgets!", wxPoint(30, 30), wxSize(800, 600))
{
	btns = new wxButton * [nWidth * nHeight];
	wxGridSizer* grid = new wxGridSizer(nWidth, nHeight, 0, 0);

	nField = new int[nWidth * nWidth];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nHeight; y++)
		{
			btns[y * nWidth + x] = new wxButton(this, 10000 + (y * nWidth + x));
			btns[y * nHeight + x]->SetFont(font);
			grid->Add(btns[y * nWidth + x], 1, wxEXPAND | wxALL);

			btns[y * nWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			nField[y * nWidth + x] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete[]btns;
	delete[]nField;
}

void cMain::NewGame(int sX, int sY)
{
	int mines = difficulty;
	while (mines > 0) {
		int rx = rand() % nWidth;
		int ry = rand() % nHeight;
		if (nField[ry * nWidth + rx] == 0 && rx != sX && ry != sY) {
			nField[ry * nWidth + rx] = -1;
			mines--;
		}
	}
	bFirstClick = false;
}

void cMain::ResetGame()
{
	bFirstClick = true;
	for (int x = 0; x < nWidth; x++) {
		for (int y = 0; y < nHeight; y++) {
			nField[y * nWidth + x] = 0;
			btns[y * nWidth + x]->SetLabel("");
			btns[y * nWidth + x]->Enable(true);
		}
	}
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	// Get coordinate of button i field array
	int x = (evt.GetId() - 10000) % nWidth;
	int y = (evt.GetId() - 10000) / nWidth;
	if (bFirstClick == true) {
		NewGame(x, y);
	}
	btns[y * nWidth + x]->Enable(false);
	if (nField[y * nWidth + x] == -1) {
		wxMessageBox("Game over!");
		ResetGame();
	}
	else {
		int bombs = CountBombs(x, y);
		btns[y * nWidth + x]->SetLabel(std::to_string(bombs));
		if (bombs == 0) {
			RevealNeighbors(x, y);
		}
		if (IsGameWon()) {
			wxMessageBox("You win!");
			ResetGame();
		}
	}
	evt.Skip();
}

int cMain::CountBombs(int x, int y)
{
	int count = 0;
	for (int i = -1; i < 2; i++) 
	{
		for (int j = -1; j < 2; j++) 
		{
			int nx = x + i;
			int ny = y + j;
			// if negative position, invalid
			if (nx < 0 || ny < 0) { continue; }
			// if outside the field, invalid
			if (nx >= nWidth) { continue; }
			if (ny >= nHeight) { continue; }
			count += (nField[ny * nWidth + nx] * -1);
		}
	}
	return count;
}

bool cMain::IsGameWon()
{
	int count = 0;
	int total = nWidth * nHeight;
	for (int i = 0; i < total; i++) {
		if (btns[i]->IsEnabled() == true) {
			count++;
		}
	}
	return count == difficulty;
}

void cMain::RevealNeighbors(int x, int y)
{
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			int nx = x + i;
			int ny = y + j;
			// if negative position, invalid
			if (nx < 0 || ny < 0) { continue; }
			// if outside the field, invalid
			if (nx >= nWidth) { continue; }
			if (ny >= nHeight) { continue; }
			if (btns[ny * nWidth + nx]->IsEnabled() == false) { continue; }
			int bombs = CountBombs(nx, ny);
			btns[ny * nWidth + nx]->Enable(false);
			btns[ny * nWidth + nx]->SetLabel(std::to_string(bombs));
			if (bombs == 0) 
			{
				RevealNeighbors(nx, ny);

			}
		}
	}
}
