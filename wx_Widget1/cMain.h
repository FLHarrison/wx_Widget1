#pragma once
#include "wx/wx.h"

class cMain : public wxFrame 
{
public:
	cMain();
	~cMain(); 

public:
	int difficulty = 7;

	int nWidth = 10; 
	int nHeight = 10;
	wxButton **btns; 
	int *nField = nullptr;
	bool bFirstClick = true;

	void NewGame(int sX, int sY);
	void ResetGame();
	void OnButtonClicked(wxCommandEvent& evt);
	int CountBombs(int x, int y);
	bool IsGameWon();
	void RevealNeighbors(int x, int y);

	wxDECLARE_EVENT_TABLE();

};

