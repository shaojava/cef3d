//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 1.5.2017
//---------------------------------------------------------------------------

#include "Win32Common.h"

LRESULT CALLBACK Win32Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32Window* Instance = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!Instance)
		return DefWindowProc(hWnd, message, wParam, lParam);

	return Instance->Delegate->WindowProc(hWnd, message, wParam, lParam);
}

HWND Win32Window::CreateNativeWindow(const InitWindowDefinition& def)
{
	WNDCLASSEX wc;
	Instance = def.Instance;
	Width = def.Width;
	Height = def.Height;

	LPCWSTR className = L"Cef3DSampleWindow";

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = def.Instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = className;

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, def.Width, def.Height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	Handle = CreateWindowEx(NULL,
		className,
		L"Cef3D Rendering Window",
		WS_OVERLAPPEDWINDOW,
		def.X,
		def.Y,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		def.Instance,
		NULL);

	ShowWindow(Handle, SW_SHOW);

	Delegate = def.Delegate;

	// Associate |this| with hwnd
	SetLastError(ERROR_SUCCESS);
	LONG_PTR result = ::SetWindowLongPtr(
		Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	if (result != 0)
		return 0;
	//assert(result != 0 || GetLastError() == ERROR_SUCCESS);

	return Handle;
}

void Win32Window::Resize(int newWidth, int newHeight)
{
	Width = newWidth;
	Height = newHeight;
}
