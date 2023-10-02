#include "OpenBrowser.h"

#include <tchar.h>
//CSVのブラウザ
void OpenBrowser::OpenCsvFile(CString filename)
{
	CString default = _T("C:\\K020G1233\\SoulHunterGit\\Resources\\");

	ShellExecute(nullptr, _T("open"), default + filename, nullptr, nullptr, SW_SHOW);
}
//Youtubeを開く(エディタのため)
void OpenBrowser::OpenWebPage()
{
	ShellExecute(nullptr, _T("open"), _T("https://www.youtube.com/watch?v=2tZxBXLhc24"), nullptr, nullptr, SW_SHOW);
}
