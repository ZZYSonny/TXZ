﻿// CGAME.cpp: 实现文件
//

#include "pch.h"
#include "TXZ.h"
#include "CGAME.h"
#include "afxdialogex.h"
#include "CGAME_Display.h"

CGAME_Display* myDisplay;

// CGAME 对话框

IMPLEMENT_DYNAMIC(CGAME, CDialogEx)

CGAME::CGAME(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME, pParent),
	m_mapSizeX(0), m_mapSizeY(0),
	m_playerX(0), m_playerY(0),
	m_map(), m_isFinished(FALSE)
{

}

CGAME::~CGAME()
{
}

void CGAME::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGAME, CDialogEx)
	ON_BN_CLICKED(IDC_RETURN, &CGAME::OnBnClickedReturn)
	ON_MESSAGE(WM_STARTGAME, &CGAME::StartGame)
END_MESSAGE_MAP()


// CGAME 消息处理程序


BOOL CGAME::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置窗口

	// 满屏代码
	CRect ClientRect;
	GetParent()->GetClientRect(ClientRect);
	MoveWindow(ClientRect);
	const int DIALOG_HEIGHT = ClientRect.Height();
	const int DIALOG_WIDTH = ClientRect.Width();

	this->SetBackgroundColor(RGB(255, 255, 0), TRUE);
	/*
	CString str;
	str.Format(_T("Height is %d, Weight is %d"), DIALOG_HEIGHT, DIALOG_WIDTH);
	MessageBox(str, _T("xht"));
	*/

	// 设置控件大小、位置
	const int BUTTON_HEIGHT = (int)(DIALOG_HEIGHT * 0.1);
	const int BUTTON_WIDTH = BUTTON_HEIGHT * 2;
	GetDlgItem(IDC_RETURN)->MoveWindow(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

	myDisplay = new CGAME_Display;
	myDisplay->Create(IDD_DISPLAY, this);
	myDisplay->MoveWindow(0, (int)(DIALOG_HEIGHT * 0.1), DIALOG_WIDTH, (int)(DIALOG_HEIGHT * 0.9));
	myDisplay->ShowWindow(SW_SHOW);


	// 设置窗口按钮
	/*
	字体、文字大小、文字内容
	按钮大小、按钮位置
	焦点位置
	*/

	// 字体、文字大小、文字内容
	m_font.CreateFont(
		(int)(BUTTON_HEIGHT * 0.6), // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		//FW_BOLD, // nWeight 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		FALSE, // cStrikeOut 
		//ANSI_CHARSET, // nCharSet 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("隶书")
	); // lpszFac 
	/*
	字符集：
	中文用GB2312_CHARSET，或谨慎地使用DEFAULT_CHARSET
	建议字体：
	中文用黑体、隶书、华文琥珀、华文行楷
	*/
	GetDlgItem(IDC_RETURN)->SetFont(&m_font);
	GetDlgItem(IDC_RETURN)->SetWindowText(_T("返回"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGAME::OnBnClickedReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	myDisplay->Clear();
	GetParent()->PostMessage(WM_TOSELECTION);
}

BOOL CGAME::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
#ifdef MYDEBUG
		case VK_F2:
		{
			MessageBox(_T("w"), _T("From xht"));
			CStatic* pic2 = new CStatic();
			pic2->Create(NULL, SS_BITMAP, CRect(0, 100, 200, 300), this);

			HBITMAP hbmp;
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_P_MAP0);
			hbmp = (HBITMAP)bitmap.GetSafeHandle();
			pic2->SetBitmap(hbmp);
			pic2->MoveWindow(0, 0, 700, 700);
			pic2->ShowWindow(SW_SHOW);
			UpdateWindow();
			MessageBox(_T("ww"), _T("From xht"));
			delete pic2;
			//
			break;
		}
#endif // MYDEBUG
		case 'W':
		case VK_UP:
#ifdef MYDEBUG
			MessageBox(_T("UP"), _T("From xht"));
#endif // MYDEBUG
			MovePlayer(DIR_UP);
			break;
		case 'S':
		case VK_DOWN:
#ifdef MYDEBUG
			MessageBox(_T("DOWN"), _T("From xht"));
#endif // MYDEBUG
			MovePlayer(DIR_DOWN);
			break;
		case 'A':
		case VK_LEFT:
#ifdef MYDEBUG
			MessageBox(_T("LEFT"), _T("From xht"));
#endif // MYDEBUG
			MovePlayer(DIR_LEFT);
			break;
		case 'D':
		case VK_RIGHT:
#ifdef MYDEBUG
			MessageBox(_T("RIGHT"), _T("From xht"));
#endif // MYDEBUG
			MovePlayer(DIR_RIGHT);
			break;
		case VK_ESCAPE:
#ifdef MYDEBUG
				MessageBox(_T("ESC"), _T("From xht"));
#endif // MYDEBUG
			OnBnClickedReturn();
			break;
		case VK_RETURN:
#ifdef MYDEBUG
			MessageBox(_T("RETURN"), _T("From xht"));
#endif // MYDEBUG
		default:
			return CDialogEx::PreTranslateMessage(pMsg);
		}
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
LRESULT CGAME::StartGame(WPARAM wParam, LPARAM lParam)
{
	// 根据指令开始游戏
	// 释放wParam的内存
	BSTR b = (BSTR)wParam;
	CString s(b);
	SysFreeString(b);
	return SetMap(s);
}

BOOL CGAME::SetMap(CString path)
{
	// 根据路径载入地图
#ifdef MYDEBUG
	MessageBox(path, _T("From xht"));
#endif // MYDEBUG
	
	if (ReadMap(path))
	{
#ifdef MYDEBUG
		MessageBox(_T("读取地图成功"), _T("From xht"));
#endif // MYDEBUG

		// 开始绘制
		myDisplay->Reset(m_mapSizeX, m_mapSizeY);

		for(int i = 1; i <= m_mapSizeX; i++)
			for (int j = 1; j <= m_mapSizeY; j++)
			{
				myDisplay->Update(i, j, m_map[i][j]);
			}

		myDisplay->Update(m_playerX, m_playerY, PIC_PLAYER);
		
		UpdateWindow();

		// 完成绘制
		return TRUE;
	}
	else return FALSE;
}

// 自定义的读取非负整数函数
const int MAXINPUTBUFFERSIZE = 100000;
char myInputBuffer[MAXINPUTBUFFERSIZE], *icp = myInputBuffer;
BOOL getNoneNegativeInteger(int &x, const int LOWERBOUND, const int UPPERBOUND)
{
	long long tempValue = 0LL;
	while (!isdigit(static_cast<unsigned char>(*icp)))
	{
		if (*icp == '\n' || *icp == '\r' || *icp == ' ') icp++;
		else return FALSE;
	}
	while (isdigit(static_cast<unsigned char>(*icp)))
	{
		tempValue = tempValue * 10 + *icp - '0';
		if (tempValue <= UPPERBOUND) icp++;
		else return FALSE;
	}
	if (LOWERBOUND <= tempValue && tempValue <= UPPERBOUND)
	{
		x = (int)tempValue;
		return TRUE;
	}
	else return FALSE;
}

BOOL CGAME::ReadMap(CString path)
{
	if (!PathFileExists(path))
	{
		MessageBox(_T("ERROR：地图文件不存在"), _T("From xht"));
		return FALSE;
	}
	USES_CONVERSION; // 为了使用T2A/W2A将Unicode下的CString转为char*，占用栈空间，不宜在循环、递归中使用，以免栈溢出
	char* pFileName = T2A(path); // 有时间可以把T2A手动实现来替换掉

	FILE* pFile = NULL;
	if (pFileName != NULL)
	{
		fopen_s(&pFile, pFileName, "r");
		if (pFile != NULL)
		{
			int len = (int)fread(myInputBuffer, 1, MAXINPUTBUFFERSIZE, pFile);
			fclose(pFile);
			icp = myInputBuffer;
			if (len == MAXINPUTBUFFERSIZE)
			{
				MessageBox(_T("ERROR：地图文件过大"), _T("From xht"));
				return FALSE;
			}
			myInputBuffer[len] = '\0';
			// 开始读入地图
			if (!getNoneNegativeInteger(m_mapSizeX, 1, MAXMAPSIZE))
			{
				MessageBox(_T("ERROR：地图尺寸越界或缺失"), _T("From xht"));
				return FALSE;
			}
			if (!getNoneNegativeInteger(m_mapSizeY, 1, MAXMAPSIZE))
			{
				MessageBox(_T("ERROR：地图尺寸越界或缺失"), _T("From xht"));
				return FALSE;
			}
			for(int i = 1; i <= m_mapSizeX; i++)
				for (int j = 1; j <= m_mapSizeY; j++)
				{
					if (!getNoneNegativeInteger(m_map[i][j], MP_FIRST, MP_LAST))
					{
						MessageBox(_T("ERROR：地图元素越界或缺失"), _T("From xht"));
						return FALSE;
					}
				}
			if (!getNoneNegativeInteger(m_playerX, 1, MAXMAPSIZE))
			{
				MessageBox(_T("ERROR：人物初始位置越界或缺失"), _T("From xht"));
				return FALSE;
			}
			if (!getNoneNegativeInteger(m_playerY, 1, MAXMAPSIZE))
			{
				MessageBox(_T("ERROR：人物初始位置越界或缺失"), _T("From xht"));
				return FALSE;
			}
			// 完成读入地图

			// 开始补充隐藏边界
			for (int i = 0; i <= m_mapSizeX + 1; i++)
				m_map[i][0] = m_map[i][m_mapSizeY + 1] = 4;
			for (int j = 1; j <= m_mapSizeY; j++)
				m_map[0][j] = m_map[m_mapSizeX + 1][j] = 4;
			// 完成补充隐藏边界

			// 开始检查地图
			if (!CanMoveOn(m_playerX, m_playerY))
			{
				MessageBox(_T("ERROR：人物初始位置在箱子或障碍上"), _T("From xht"));
				return FALSE;
			}
			{
				int tempCount = 0;
				for (int i = 1; i <= m_mapSizeX; i++)
					for (int j = 1; j <= m_mapSizeY; j++)
						if (m_map[i][j] == MP_BOX) tempCount++;
						else if (m_map[i][j] == MP_GOAL) tempCount--;
				if (tempCount != 0)
				{
					MessageBox(_T("ERROR：箱子与目标数量不一致"), _T("From xht"));
					return FALSE;
				}
			}
			
			m_isFinished = FALSE;
			if (IsFinished())
			{
				MessageBox(_T("ERROR：地图初始状态已完成"), _T("From xht"));
				return FALSE;
			}
			// 完成检查地图
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CGAME::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	myDisplay->DestroyWindow();
	delete myDisplay;
	return CDialogEx::DestroyWindow();
}

BOOL CGAME::CanMoveOn(int x, int y)
{
	return ((1 <= x && x <= m_mapSizeX && 1 <= y && y <= m_mapSizeY) && !(m_map[x][y] & (MP_BOX | MP_WALL)));
}
BOOL CGAME::MovePlayer(UINT dir)
{
	if (m_isFinished)
	{
		return FALSE;
	}
	if (DIR_FIRST <= dir && dir <= DIR_LAST)
	{
		int x = m_playerX, y = m_playerY;
		int xx = x + dx[dir], yy = y + dy[dir];
		int xxx = xx + dx[dir], yyy = yy + dy[dir];
		switch (m_map[xx][yy])
		{
		case MP_NULL:
		case MP_GOAL:
			m_playerX = xx; m_playerY = yy;
			myDisplay->Update(x, y, m_map[x][y]);
			myDisplay->Update(xx, yy, PIC_PLAYER);
			UpdateWindow();
			return TRUE;
			break;
		case MP_BOX:
		case MP_FINISH:
			if (CanMoveOn(xxx, yyy))
			{
				m_playerX = xx; m_playerY = yy;
				m_map[xx][yy] &= ~MP_BOX;
				m_map[xxx][yyy] |= MP_BOX;
				myDisplay->Update(x, y, m_map[x][y]);
				myDisplay->Update(xx, yy, PIC_PLAYER);
				myDisplay->Update(xxx, yyy, m_map[xxx][yyy]);
				UpdateWindow();
				CheckFinished();
				return TRUE;
			}
			else
			{
				return FALSE;
			}
			break;
		case MP_WALL:
		default:
			break;
		}
		return FALSE;
	}
	else
	{
		MessageBox(_T("ERROR：人物移动方向非法"), _T("From xht"));
		return FALSE;
	}
	return FALSE;
}

BOOL CGAME::IsFinished()
{
	for(int i = 1; i <= m_mapSizeX; i++)
		for (int j = 1; j <= m_mapSizeY; j++)
			if (m_map[i][j] == MP_BOX || m_map[i][j] == MP_GOAL)
			{
				return m_isFinished = FALSE;
			}
	return m_isFinished = TRUE;
}

BOOL CGAME::CheckFinished()
{
	if (IsFinished())
	{
		MessageBox(_T("恭喜闯关成功！"), _T("From xht"));
		return TRUE;
	}
	else return FALSE;
}