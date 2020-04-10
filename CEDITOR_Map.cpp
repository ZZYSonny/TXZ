#include "pch.h"
#include "CEDITOR_Map.h"

CEDITOR_Map::CEDITOR_Map(CEDITOR_Display* currentDisplay) :
	CBASE_Map(),
	myDisplay(currentDisplay)
{

}

CEDITOR_Map::~CEDITOR_Map()
{

}

void CEDITOR_Map::NewMap(int n, int m)
{
	myDisplay->Reset(n, m);
	MessageBox(0, _T("..."), _T("From xht"), 0);
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
		{
			m_map[i][j] = PIC_NULL;
			myDisplay->Update(i, j, m_map[i][j]);
		}
}

void CEDITOR_Map::ChangeMap(int r, int c, int delta)
{
	m_map[r][c] = (m_map[r][c] + delta + 5) % 5;
	myDisplay->Update(r, c, m_map[r][c]);
}

void CEDITOR_Map::Change(int r, int c)
{
	ChangeMap(r, c, 1);
	undoHistory.push(1024 * r + c);
}

void CEDITOR_Map::Change(int rc)
{
	ChangeMap(rc / 1024, rc % 1024, 1);
	undoHistory.push(rc);
}

void CEDITOR_Map::Undo()
{
	if (!undoHistory.empty())
	{
		int rc = undoHistory.top();
		undoHistory.pop();
		redoHistory.push(rc);
		ChangeMap(rc / 1024, rc % 1024, -1);
	}
	else
	{
		MessageBox(NULL, _T("没有操作了"), _T("无法Undo"), 0);
	}
}

void CEDITOR_Map::Redo()
{
	if (!redoHistory.empty())
	{
		int rc = redoHistory.top();
		redoHistory.pop();
		Change(rc);
	}
	else
	{
		MessageBox(NULL, _T("没有操作了"), _T("无法Redo"), 0);
	}
}
