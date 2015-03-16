#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShellAPI.h> // ShellExecuteEx()
#include <strsafe.h>
#pragma warning(disable: 4995)
#include <cstdlib>    // for std::atexit to handle process close
#include <vector>
#include <string>

// Headers for XML-RPC
//
#include <libtransport/rpc_session.hpp>
typedef instek::RPCSession<instek::SSL_SOCK> NowRPCSession;

#define QVB_PATH       L"QtVideoBrowser\\"
#define MAX_LOADSTRING 100

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#endif

ATOM             MyRegisterClass(HINSTANCE);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void             AtExit();


//  Global Variables
// 
TCHAR     g_szTitle[MAX_LOADSTRING];
TCHAR     g_szWindowClass[MAX_LOADSTRING];
HWND      g_hMainWnd  = NULL;
HINSTANCE g_hMainInst = NULL;
HANDLE    g_hProcess  = NULL; // handle of outer process
HWND      g_hSubWnd   = NULL; // window handle of sub-process

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 		
    StringCchPrintf(g_szTitle, MAX_LOADSTRING, L"Test");
    StringCchPrintf(g_szWindowClass, MAX_LOADSTRING, L"TestWindow");
	MyRegisterClass(hInstance);

	if(InitInstance(hInstance, nCmdShow) == FALSE) {	
		return FALSE;
	}
    
    // Launch outer process
    // Parameters for process: 
    //     - window handle, 
    //     - user name
    //     - password
    //     - the folder where QVideoBrowser.exe locates
    //
    // NOTE: if not use wide char, be sure that using UTF8 string for user name and password
    WCHAR wzParams[50];
    StringCchPrintfW(wzParams, 50, L"%d %s %s %s", g_hMainWnd, L"admin", L"id", QVB_PATH);

    SHELLEXECUTEINFO s_info = {0};
    s_info.cbSize       = sizeof(SHELLEXECUTEINFO);
	s_info.fMask        = SEE_MASK_NOCLOSEPROCESS;
	s_info.hwnd         = NULL;
	s_info.lpVerb       = L"open";
	s_info.lpFile       = QVB_PATH L"QVideoBrowser.exe";
	s_info.lpParameters = wzParams;
	s_info.nShow        = SW_SHOWNORMAL;

    if(::ShellExecuteEx(&s_info) == FALSE)
    {
        OutputDebugStringW(L"Execute outer process failed.\n");
        assert(false);
    }
    else
    {
        g_hProcess = s_info.hProcess;        
        ::WaitForInputIdle(g_hProcess, INFINITE);

        // Initialize XML-RPC
        std::string    address     = "127.0.0.1";
        int            port        = 45678;
        NowRPCSession* pRpcSession = new NowRPCSession(address, port, false);
        
        instek::MethodCallPtr pfuncCall(new instek::MethodCall);
	    pfuncCall->SetMethodName("QtVideoBrowser.QueryHWND");
	    pRpcSession->SetTimeoutValue(3000);
        // Blocking call
	    instek::MethodResponsePtr resp = pRpcSession->Call(pfuncCall);
	    if(resp.get() != NULL)
	    {
		    if(resp->IsOK())
		    {
                int nRet = (int)resp->GetResult();
                g_hSubWnd = (HWND)nRet;
            }
		    else
		    {
                char szErrorMsg[100];
                StringCchPrintfA(szErrorMsg, 100, "Error - code: %d, msg: %s", resp->GetErrValue(), resp->GetErrMsg().c_str());
                OutputDebugStringA(szErrorMsg);
		    }
	    }
	    else
	    {
		    OutputDebugStringW(L"Response is NULL.\n");
	    }
        SAFE_DELETE(pRpcSession);
    }    

    MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
    std::atexit(::AtExit);
	return (int)msg.wParam;
}

void AtExit()
{
    OutputDebugStringW(L"AtExit() - Terminate outer process.\n");

    if(g_hProcess != NULL)
    {        
		DWORD exitcode = 0;
		::GetExitCodeProcess(g_hProcess, &exitcode);

		if(exitcode == STILL_ACTIVE)
			TerminateProcess(g_hProcess, 0); 
    }
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = ::WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = NULL;
	wcex.hCursor       = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = g_szWindowClass;
	wcex.hIconSm       = NULL;
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{   
   g_hMainInst = hInstance;
   g_hMainWnd  = CreateWindow(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
                              100, 0, 1600, 1000, 
                              NULL, NULL, hInstance, NULL);
   if(g_hMainWnd == NULL)
   {
       OutputDebugStringW(L"InitInstance() - Create window failed.\n");
       return FALSE;
   }

   ShowWindow(g_hMainWnd, nCmdShow);
   UpdateWindow(g_hMainWnd);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
    
    case WM_SIZE:
        {
            if(g_hSubWnd != NULL) 
            {
                // Change window size of sub-process's top window
                ::SetWindowPos(g_hSubWnd, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOMOVE | SWP_ASYNCWINDOWPOS);                
            }                
            break;
        }

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}