
// MFC_ImageFuseDlg.h : ͷ�ļ�
//
#include"CvvImage.h"
#include"cv.h"
#pragma once

#define _O_TEXT         0x4000 
// CMFC_ImageFuseDlg �Ի���
class CMFC_ImageFuseDlg : public CDialogEx
{
// ����
public:
	CMFC_ImageFuseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_IMAGEFUSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_level;
	CComboBox m_combo;			//������
	cv::Mat_<float> fir;
	cv::Mat_<float> fvs;
	cv::Mat fuse;
public:
	void  DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedMfcbuttonSelect1();
	CString m_filepath1;
	CString m_filepath2;
	afx_msg void OnBnClickedMfcbuttonSelect2();
	afx_msg void OnBnClickedMfcbuttonFuse();
	afx_msg void OnBnClickedExit();
};
