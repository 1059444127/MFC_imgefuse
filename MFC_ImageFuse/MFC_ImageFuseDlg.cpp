
// MFC_ImageFuseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_ImageFuse.h"
#include "MFC_ImageFuseDlg.h"
#include "afxdialogex.h"
#include<io.h>

#include"imagefuse.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//static ImageFuse   imagefuse;				//ʵ����һ������
//��ʾ����̨
void InitConsole()
{
	int nRet = 0;
	FILE* fp;
	AllocConsole();
	system("color b0");
	system("title  ͼ���ں�ϵͳ�������");
	nRet = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nRet, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}
string wstring2string(const wstring &wstr)
{
	string result;

	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.size()), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];

	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.size()), buffer, len, NULL, NULL);
	buffer[len] = '\0';

	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;

	return result;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_ImageFuseDlg �Ի���



CMFC_ImageFuseDlg::CMFC_ImageFuseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_ImageFuseDlg::IDD, pParent)
	, m_level(4)
	, m_filepath1(_T(""))
	, m_filepath2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_ImageFuseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_level);
	DDV_MinMaxInt(pDX, m_level, 1, 5);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_combo);
	DDX_Text(pDX, IDC_EDIT_PATH1, m_filepath1);
	DDX_Text(pDX, IDC_EDIT_PATH2, m_filepath2);
}

BEGIN_MESSAGE_MAP(CMFC_ImageFuseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_SELECT1, &CMFC_ImageFuseDlg::OnBnClickedMfcbuttonSelect1)
	ON_BN_CLICKED(IDC_MFCBUTTON_SELECT2, &CMFC_ImageFuseDlg::OnBnClickedMfcbuttonSelect2)
	ON_BN_CLICKED(IDC_MFCBUTTON_FUSE, &CMFC_ImageFuseDlg::OnBnClickedMfcbuttonFuse)
	ON_BN_CLICKED(ID_EXIT, &CMFC_ImageFuseDlg::OnBnClickedExit)
END_MESSAGE_MAP()

//��ͼ��pic�ؼ�
void CMFC_ImageFuseDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ  
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������  
	ReleaseDC(pDC);
}

// CMFC_ImageFuseDlg ��Ϣ�������

BOOL CMFC_ImageFuseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	InitConsole();
	// Ϊ ��Ͽ�ؼ����б������б���:  
	m_combo.AddString(_T("������˹lap"));
	m_combo.AddString(_T("�Աȶ�"));
	m_combo.AddString(_T("����"));
	m_combo.AddString(_T("С��sidwt"));
	m_combo.AddString(_T("С��"));
	m_combo.AddString(_T("fsd������"));
	m_combo.AddString(_T("gra�ݶ�"));
	m_combo.AddString(_T("pca���ɷ�"));
	m_combo.AddString(_T("aveƽ��"));
	m_combo.SetCurSel(0);			//Ĭ��ѡ��ԭͼ

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC_ImageFuseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_ImageFuseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_ImageFuseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFC_ImageFuseDlg::OnBnClickedMfcbuttonSelect1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR defaultpath[] = TEXT(".\\test_image\\");
	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp|(All files)|*.*"));
	dlg.m_ofn.lpstrTitle = _T("�����ͼ��");
	dlg.m_ofn.lpstrInitialDir = defaultpath;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_filepath1 = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT_PATH1, m_filepath1);
	IplImage *image1 = cvLoadImage(wstring2string(m_filepath1.GetBuffer(0)).c_str(), 0); //��ʾ�Ҷ�ͼƬ  
	Mat infr = Mat(image1, 1);	//1��ʾ�������ݣ�0��ʾ����������
	infr.convertTo(fir, CV_32FC1, 1 / 255.0);// 
	DrawPicToHDC(image1, IDC_STATIC_IR);
}


void CMFC_ImageFuseDlg::OnBnClickedMfcbuttonSelect2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR defaultpath[] = TEXT(".\\test_image\\");
	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||"));
	dlg.m_ofn.lpstrTitle = _T("��ɼ���ͼ��");
	dlg.m_ofn.lpstrInitialDir = defaultpath;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_filepath2 = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT_PATH2, m_filepath2);
	IplImage *image2 = cvLoadImage(wstring2string(m_filepath2.GetBuffer(0)).c_str(), 0); //��ʾ�Ҷ�ͼƬ 
	Mat visi = Mat(image2, 1);
	visi.convertTo(fvs, CV_32FC1, 1 / 255.0);
	DrawPicToHDC(image2, IDC_STATIC_VS);
}


void CMFC_ImageFuseDlg::OnBnClickedMfcbuttonFuse()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	ImageFuse imagefuse(fir, fvs, m_level, 3);
	int64 tin = cv::getTickCount();
	switch (m_combo.GetCurSel())
	{
	case 0:
		fuse=imagefuse.lapFuse();
		break;
	case 1:
		fuse = imagefuse.conFuse();
		break;
	case 2:
		fuse = imagefuse.ratFuse();
		break;
	case 3:
		fuse = imagefuse.fuse_SIDWT();
		break;
	case 4:
		fuse = imagefuse.fuse_DWT();
		break;
	case 5:
		fuse = imagefuse.fuse_fsd();
		break;
	case 6:
		fuse = imagefuse.fuse_gra();
		break;
	case 7:
		fuse = imagefuse.fuse_pca();
		break;
	case 8:
		fuse = imagefuse.fuse_ave();
		break;
	default:
		break;
	}
	std::cout << "��ʱ��" << 1000.0*(getTickCount()-tin)/getTickFrequency() << endl;
	IplImage *image = &(IplImage)fuse;
	DrawPicToHDC(image, IDC_STATIC_FUSE);
}




void CMFC_ImageFuseDlg::OnBnClickedExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnDestroy();
	FreeConsole();
	CDialogEx::OnOK();
}
