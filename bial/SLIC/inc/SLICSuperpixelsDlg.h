
// SLICSuperpixelsDlg.h : header file
//

#include <vector>
#include <string>
using namespace std;

#pragma once


// CSLICSuperpixelsDlg dialog
class CSLICSuperpixelsDlg : public CDialog
{
// Construction
public:
	CSLICSuperpixelsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SLICSUPERPIXELS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCreatesuperpixels();


private:

	bool BrowseForFolder(string& folderpath);
	void GetPictures(vector<string>& picvec);
	void DoSupervoxelVideoSegmentation();
public:
	afx_msg void OnEnChangeEditSpcount();
	afx_msg void OnEnChangeEditCompactness();
	int m_spcount;
	double m_compactness;
};
