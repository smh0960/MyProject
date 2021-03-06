//MyProject.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MyProject.h"
#include <winsock.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib");

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i=0;

	static RECT  rect;
	HDC          hdc;
	PAINTSTRUCT  ps;
    switch (message)
    {
	case WM_CREATE:
		CreateWindow(TEXT("button"), TEXT("连接"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,0,0,100,100,hWnd, (HMENU)1001, ((LPCREATESTRUCT)lParam)->hInstance,NULL);
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case 1001:
			{
				hdc = GetDC(hWnd);

				//GetClientRect(hWnd, &rect);
				//DrawText(hdc, L"hellooooooooo", 10, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				TextOut(hdc, 10, 200, TEXT("连接中\r\n"), 3);
				char te[1024] = "lllss\r\n vvv";
				TextOutA(hdc, 10, 180, te, 100);
				WSADATA WSAData;
				int iError;
				SOCKET sock;
				SOCKADDR_IN serverAddr;
				int port = 110;
				if (iError = WSAStartup(MAKEWORD(2, 0), &WSAData)) {
					TextOut(hdc, 500, 500, TEXT("error"), 5);
				}
				sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				/*char sockstr[10];
				sprintf_s(sockstr, "%d", sock);
				MessageBoxA(NULL, sockstr, "HelloMsg", 0);*/
				if (sock == INVALID_SOCKET)
				{
					MessageBoxA(NULL, "Socket creation error", "HelloMsg", 0);
					WSACleanup();
					return TRUE;
				}
				serverAddr.sin_family = AF_INET;
				serverAddr.sin_port = htons(port);
				serverAddr.sin_addr.s_addr = inet_addr("220.181.12.110");
				connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
				char buf[1024],buf2[10240];			 
				recv(sock, buf, 1024, 0);		//接收响应数据
				TextOutA(hdc, 10, 220, buf, 100);
				char bufferHello[] = "user farewell86@163.com\r\n";
				int len;
				len=send(sock, bufferHello, strlen(bufferHello), 0);
				//send(sock, "helo a\r\n", 6, 0);
				recv(sock, buf2, 1024, 0);
				TextOutA(hdc, 10, 240, buf2, 100);
				char pass[] = "pass shen132168\r\n";
				len = send(sock, pass, strlen(pass), 0); 
				recv(sock, buf2, 1024, 0);
				TextOutA(hdc, 10, 260, buf2, 100);


				char retr[] = "retr 161\r\n";
				len = send(sock, retr, strlen(retr), 0);
				recv(sock, buf2, 10240, 0);
				MessageBoxA(NULL, buf2, "dd", 0);
				TextOutA(hdc, 10, 280, buf2, 10240);

				ReleaseDC(hWnd, hdc);
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            HDC hdc = BeginPaint(hWnd, &ps);
			TCHAR a[4] = L"sbs";
			TextOut(hdc, 522, 5, a, 2);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
