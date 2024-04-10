
// VolnovoyPaketDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "VolnovoyPaket.h"
#include "VolnovoyPaketDlg.h"
#include "afxdialogex.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CVolnovoyPaketDlg



CVolnovoyPaketDlg::CVolnovoyPaketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VOLNOVOYPAKET_DIALOG, pParent)
	, sigma(0.1)
	, delta_t(0.01)
	, R0(1)
	, controlPsi(0)
	, num_point(600)
	, a(0.1)
	, num_furie(250)
	, left(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVolnovoyPaketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, sigma);
	DDX_Text(pDX, IDC_EDIT4, delta_t);
	DDX_Text(pDX, IDC_EDIT5, R0);
	DDX_Text(pDX, IDC_EDIT6, controlPsi);
	DDX_Text(pDX, IDC_EDIT7, num_point);
	DDX_Control(pDX, IDC_PAKET, evolution_paket);
	DDX_Control(pDX, IDC_EDIT6, control);
	DDX_Text(pDX, IDC_EDIT1, a);
	DDX_Control(pDX, IDC_EDIT3, num_iter);
	DDX_Text(pDX, IDC_EDIT8, num_furie);
	DDX_Control(pDX, IDC_FURIE, gr_furie);
	DDX_Control(pDX, IDC_SOBFUNC, sobs_func);
	DDX_Control(pDX, IDC_COMBO1, cur_f);
	DDX_Control(pDX, IDC_BUTTON1, but_grap_fur);
	DDX_Control(pDX, IDC_BUTTON4, but_grap_func);
	DDX_Control(pDX, IDC_COMBO2, status_grap);
	DDX_Text(pDX, IDC_EDIT9, left);
}

BEGIN_MESSAGE_MAP(CVolnovoyPaketDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CVolnovoyPaketDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVolnovoyPaketDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CVolnovoyPaketDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CVolnovoyPaketDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CVolnovoyPaketDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// Обработчики сообщений CVolnovoyPaketDlg

BOOL CVolnovoyPaketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	but_grap_fur.EnableWindow(FALSE);
	but_grap_func.EnableWindow(FALSE);
	my_timer = SetTimer(NULL, 10, NULL);

	status_grap.AddString(L"Модуль");
	status_grap.AddString(L"Реальная часть");
	status_grap.AddString(L"Мнимая часть");
	status_grap.SetCurSel(1);

	evolution_paket.nameGraph = L"Эволюция волного пакета";

	InitializeCriticalSection(&cs);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CVolnovoyPaketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CVolnovoyPaketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD __stdcall CVolnovoyPaketDlg::evulate(PVOID p)
{
	CVolnovoyPaketDlg* dlg = (CVolnovoyPaketDlg*)p;
	dlg->but_grap_fur.EnableWindow(FALSE);
	dlg->but_grap_func.EnableWindow(FALSE);
	for (int num_evolut = 1; num_evolut < dlg->my_paket.num_psi; num_evolut++)
	{
		EnterCriticalSection(&dlg->cs);
		dlg->cur_psi = dlg->my_paket.step_evolution(num_evolut);
		LeaveCriticalSection(&dlg->cs);
		dlg->iterator = num_evolut;
		Sleep(5);
		if (dlg->stop) return 0;
	}
	dlg->my_paket.furie_my_psy();
	dlg->but_grap_fur.EnableWindow(TRUE);
	dlg->but_grap_func.EnableWindow(TRUE);
	//dlg->KillTimer(dlg->my_timer);
	return 0;
}

CString str;
void CVolnovoyPaketDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (!evolution_paket.my_func.empty()) evolution_paket.my_func.clear();
	if (!evolution_paket.my_x.empty()) evolution_paket.my_x.clear();
	vector<double> for_func;
	double work = 0.;
	EnterCriticalSection(&cs);
	for (int i = 0; i < cur_psi.size(); i++)
	{
		for_func.push_back(sqrt(cur_psi[i].real() * cur_psi[i].real() + cur_psi[i].imag() * cur_psi[i].imag()));
		work += for_func[i];
	}
	LeaveCriticalSection(&cs);
	str.Format(L"%.2f", work);
	control.SetWindowTextW(str);
	str.Format(L"%d", iterator);
	num_iter.SetWindowTextW(str);
	evolution_paket.my_func.push_back(for_func);
	evolution_paket.my_x.push_back(my_paket.GetPo());
	evolution_paket.Invalidate(FALSE);
	if (work > 1e20)
	{
		stop = true;
	}
}


void CVolnovoyPaketDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	if (!evolution_paket.my_func.empty()) evolution_paket.my_func.clear();
	if (!evolution_paket.my_x.empty()) evolution_paket.my_x.clear();
	my_paket = Evolution(a, sigma, delta_t, R0, num_point, left);
	vector<double> for_func;
	vector<compl> cur_psi = my_paket.my_psi[0];
	for (int i = 0; i < cur_psi.size(); i++) for_func.push_back(cur_psi[i].real() * cur_psi[i].real() + cur_psi[i].imag() * cur_psi[i].imag());
	evolution_paket.my_func.push_back(for_func);
	evolution_paket.my_x.push_back(my_paket.GetPo());
	Invalidate(FALSE);

	stop = false;
	CreateThread(NULL, NULL, evulate, this, NULL, NULL);
	//my_timer = SetTimer(NULL, 10, NULL);
}


void CVolnovoyPaketDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	stop = true;
	//KillTimer(my_timer);
}


void CVolnovoyPaketDlg::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	DeleteCriticalSection(&cs);
	CDialogEx::OnCancel();
}


void CVolnovoyPaketDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	if (!gr_furie.my_func.empty()) gr_furie.my_func.clear();
	if (!gr_furie.my_x.empty()) gr_furie.my_x.clear();
	vector<double> otscet_furie, f;
	double max = 0.;
	int size = my_paket.for_fft[num_furie].size();
	CString variant_grap; 
	status_grap.GetLBText(status_grap.GetCurSel(), variant_grap);
	if (variant_grap == L"Модуль")
	{
		for (int i = 0; i < size; i++)
		{
			otscet_furie.push_back(sqrt(my_paket.for_fft[num_furie][i].real() * my_paket.for_fft[num_furie][i].real() + my_paket.for_fft[num_furie][i].imag() * my_paket.for_fft[num_furie][i].imag()));
			f.push_back(i / (delta_t * (size - 1)));
			if (otscet_furie[i] > max) max = otscet_furie[i];
		}
	}
	else if (variant_grap == L"Реальная часть")
	{
		for (int i = 0; i < size; i++)
		{
			otscet_furie.push_back(my_paket.for_fft[num_furie][i].real());
			f.push_back(i / (delta_t * (size - 1)));
			if (otscet_furie[i] > max) max = otscet_furie[i];
		}
	}
	else if (variant_grap == L"Мнимая часть")
	{
		for (int i = 0; i < size; i++)
		{
			otscet_furie.push_back(my_paket.for_fft[num_furie][i].imag());
			f.push_back(i / (delta_t * (size - 1)));
			if (otscet_furie[i] > max) max = otscet_furie[i];
		}
	}

	int comb_size = cur_f.GetCount();
	if (comb_size != 0) 
		for (int i = 0; i < comb_size; i++)
		{
			cur_f.DeleteString(0);
		}

	CString str;
	if (fabs(otscet_furie[0]) > fabs(otscet_furie[1]))
	{
		str.Format(L"%d", 0);
		cur_f.InsertString(-1, str);
	}
	if (fabs(otscet_furie[size - 1]) > fabs(otscet_furie[size - 2]))
	{
		str.Format(L"%d", size - 1);
		cur_f.InsertString(-1, str);
	}
	for (int i = 1; i < size - 1; i++)
	{
		if (fabs(otscet_furie[i]) > fabs(otscet_furie[i - 1]) && fabs(otscet_furie[i]) > fabs(otscet_furie[i + 1]))
		{
			str.Format(L"%d", i);
			cur_f.InsertString(-1, str);
		}
	}
	gr_furie.my_func.push_back(otscet_furie);
	gr_furie.my_x.push_back(f);
	gr_furie.nameGraph = L"Преобразование Фурье";
	Invalidate(FALSE);
}


void CVolnovoyPaketDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString str;
	if (!sobs_func.my_func.empty()) sobs_func.my_func.clear();
	if (!sobs_func.my_x.empty()) sobs_func.my_x.clear();
	cur_f.GetLBText(cur_f.GetCurSel(), str);
	int my_f = _wtof(str);
	vector<compl> otscet;
	int size = my_paket.for_fft.size();
	for (int i = 0; i < size; i++)
	{
		otscet.push_back(my_paket.for_fft[i][my_f]);
	}
	vector<double> func, f;

	CString variant_grap;
	status_grap.GetLBText(status_grap.GetCurSel(), variant_grap);
	if (variant_grap == L"Модуль")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(sqrt(otscet[i].real() * otscet[i].real() + otscet[i].imag() * otscet[i].imag()));
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	else if (variant_grap == L"Реальная часть")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(otscet[i].real());
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	else if (variant_grap == L"Мнимая часть")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(otscet[i].imag());
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	sobs_func.my_func.push_back(func);
	//sobs_func.my_x.push_back(f);
	sobs_func.my_x.push_back(my_paket.GetPo());
	sobs_func.nameGraph = L"Собственная функция";
	Invalidate(FALSE);
}


void CVolnovoyPaketDlg::OnCbnSelchangeCombo1()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString str;
	if (!sobs_func.my_func.empty()) sobs_func.my_func.clear();
	if (!sobs_func.my_x.empty()) sobs_func.my_x.clear();
	cur_f.GetLBText(cur_f.GetCurSel(), str);
	int my_f = _wtof(str);
	vector<compl> otscet;
	int size = my_paket.for_fft.size();
	for (int i = 0; i < size; i++)
	{
		otscet.push_back(my_paket.for_fft[i][my_f]);
	}
	vector<double> func, f;

	CString variant_grap;
	status_grap.GetLBText(status_grap.GetCurSel(), variant_grap);
	if (variant_grap == L"Модуль")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(sqrt(otscet[i].real() * otscet[i].real() + otscet[i].imag() * otscet[i].imag()));
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	else if (variant_grap == L"Реальная часть")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(otscet[i].real());
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	else if (variant_grap == L"Мнимая часть")
	{
		for (int i = 0; i < size; i++)
		{
			func.push_back(otscet[i].imag());
			f.push_back(i / (delta_t * (size - 1)));
		}
	}
	sobs_func.my_func.push_back(func);
	//sobs_func.my_x.push_back(f);
	sobs_func.my_x.push_back(my_paket.GetPo());
	sobs_func.nameGraph = L"Собственная функция";
	Invalidate(FALSE);
}
