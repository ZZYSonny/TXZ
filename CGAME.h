﻿#pragma once

/*
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_FIRST 0
#define DIR_LAST 3

const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, -1, 1 };
*/

// CGAME 对话框

class CGAME : public CDialogEx
{
	DECLARE_DYNAMIC(CGAME)

public:
	CGAME(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGAME();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedRestartbutton();
	afx_msg void OnBnClickedReturnbutton();
	afx_msg LRESULT StartGame(WPARAM wParam, LPARAM lParam);
protected:

private:
	CFont m_font;
};
