﻿
// VolnovoyPaket.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CVolnovoyPaketApp:
// Сведения о реализации этого класса: VolnovoyPaket.cpp
//

class CVolnovoyPaketApp : public CWinApp
{
public:
	CVolnovoyPaketApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CVolnovoyPaketApp theApp;