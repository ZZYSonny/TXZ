#include "pch.h"
#include "CRESOURCE.h"

const CString CRESOURCE::m_idToFilename[] = {
	_T("P_null.bmp"),
	_T("P_box.bmp"),
	_T("P_goal.bmp"),
	_T("P_finish.bmp"),
	_T("P_wall.bmp"),
	_T("P_player.bmp"),
	_T("P_preview_fail.bmp")
};

BOOL CRESOURCE::CheckDir(CString dir)
{
	for (int i = 0; i < m_picCount; i++)
	{
		CString picDir = dir + m_idToFilename[i];

		if (!PathFileExists(picDir))
		{
			MessageBox(0, _T("缺少") + m_idToFilename[i], _T("资源错误"), 0);
			return FALSE;
		}

		HBITMAP tmpBMP = (HBITMAP)LoadImage(NULL, picDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (tmpBMP == NULL)
		{
			MessageBox(0, m_idToFilename[i] + _T("不是一个有效的bmp"), _T("资源错误"), 0);
			return FALSE;
		}
	}
	return TRUE;
}

CRESOURCE::CRESOURCE()
{
	if (CheckDir(m_dir))
	{
		for (int i = 0; i < m_picCount; i++)
			m_bitmap[i] = (HBITMAP) LoadImage(NULL, m_dir+m_idToFilename[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
}

CRESOURCE::~CRESOURCE()
{

}

HBITMAP CRESOURCE::GetPic(int picTag)
{
	return m_bitmap[picTag];
}