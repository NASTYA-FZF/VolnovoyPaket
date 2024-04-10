
// VolnovoyPaketDlg.h: файл заголовка
//

#pragma once
#include "MyGraph.h"
#include "Evolution.h"

// Диалоговое окно CVolnovoyPaketDlg
class CVolnovoyPaketDlg : public CDialogEx
{
// Создание
public:
	CVolnovoyPaketDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VOLNOVOYPAKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double sigma;
	double delta_t;
	double R0;
	double controlPsi;
	int num_point;
	Evolution my_paket;
	std::vector<compl> cur_psi;
	MyGraph evolution_paket;
	static DWORD WINAPI evulate(PVOID p);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	bool stop;
	afx_msg void OnBnClickedButton3();
	CEdit control;
	UINT_PTR my_timer;
	double a;
	int iterator;
	CEdit num_iter;
	CRITICAL_SECTION cs;
	virtual void OnCancel();
	int num_furie;
	MyGraph gr_furie;
	afx_msg void OnBnClickedButton1();
	MyGraph sobs_func;
	CComboBox cur_f;
	afx_msg void OnBnClickedButton4();
	CButton but_grap_fur;
	CButton but_grap_func;
	CComboBox status_grap;
	double left;
	afx_msg void OnCbnSelchangeCombo1();
};
