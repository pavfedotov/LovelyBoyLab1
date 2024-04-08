
//------------------------------------------------------- 
//      GL_2D.cpp
//       =========
//  ��������� ������� �����  � OpenGL
//--------------------------------------------------------
#define STRICT

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gl\gl.h>
#include <gl\\glu.h>
// #include <gl\\glaux.h>
#pragma comment (linker, "/defaultlib:opengl32.lib")
#pragma comment (linker, "/defaultlib:glu32.lib")


#define MIN(a,b) ((a)>=(b) ? (b) : (a))
#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MsgBox(str) MessageBox(hwndMain, str, aczAppName, 0L)

char aczAppName[512] = "GlBaseSamp";
LPCSTR pczMainWndClass = "WC__FrameWnd";

HINSTANCE hiApp;
HWND hwndMain;
HDC hdcMain;
HGLRC hGlRc;
// ������� ������ � ������� �����������
RECT rcViewport;

//  ������ ������� ������ � ��������� (���������� ���)
# define DIM_SCENE 2.0 


void InitOpenGL();
void UninitOpenGL();
void InitViewport(const int cx, const int cy);
void Draw();
void DrawModel(int numVertices, int);
LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
void MainOnPaint(HWND);
int RegisterWndClasses(void);

struct Vect {
	double x, y;
};

// ������������� ��������� ��������������� OpenGL
void InitOpenGL()
{
	hdcMain = GetDC(hwndMain);
	int iPixelFormat;
	// ������������� ��������� ������ ��������
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���������
		1,                              // ����� ������ OpenGL
		PFD_DRAW_TO_WINDOW |          // ��������� ������ � ����
		PFD_SUPPORT_OPENGL |          // ��������� OpenGL
		PFD_DOUBLEBUFFER,		// ������� �����������
		PFD_TYPE_RGBA,          // ����� � ������ RGBA
		24,                     // 24 ������� �� ����
		0, 0, 0, 0, 0, 0,       // ���� ����� ������������
		0,                      // �� ������������ �����-��������
		0,                      // �������� ������ ������������
		0,                      // ����� ������������ �� ������������
		0, 0, 0, 0,             // ���� ������������ ������������
		32,                     // 32-��������� ����� �������
		0,                      // ����� ��������� �� ������������
		0,                      // ��������������� ����� �� ������������
		PFD_MAIN_PLANE,         // �������� ����
		0,                      // ��������������
		0, 0, 0                 // ����� ���� ������������
	};
	// ������ pix-�������
	iPixelFormat = ChoosePixelFormat(hdcMain, &pfd);
	if (!iPixelFormat)
		MsgBox("�������� ������ ��������");
	// ��������� pix-�������
	if (!SetPixelFormat(hdcMain, iPixelFormat, &pfd))
		MsgBox("������ �������� �� ����������");
	// �������� OpenGL-���������
	hGlRc = wglCreateContext(hdcMain);
	if (!hGlRc)
		MsgBox("�������� OpenGL �� ������");
	// ��������� �������� OpenGL-���������
	if (!wglMakeCurrent(hdcMain, hGlRc))
		MsgBox("�������� OpenGL �� ����������");
}  // func InitOpenGL

 // ������������ ��������� ���������������
void UninitOpenGL()
{
	// �������� ������� �������� ��������������� OpenGL
	wglMakeCurrent(NULL, NULL);
	// ��������� ��������� ��������������� OpenGL
	if (hGlRc)
		wglDeleteContext(hGlRc);
	// ������������ ��������� ������� �������
	ReleaseDC(hwndMain, hdcMain);
}  //func UninitOpenGL

 // ������������� ������� ������
void InitViewport(const int cx,
	const int cy)
{
	// ������ ������� ������ OpenGL �� ����������� ������� ����
	rcViewport.left = 10;
	rcViewport.top = 10;
	int viewSize = MIN(cx, cy) - 20;
	rcViewport.right = rcViewport.left + viewSize;
	rcViewport.bottom = rcViewport.top + viewSize;
	glMatrixMode(GL_PROJECTION);
	glViewport(rcViewport.left,
		cy - rcViewport.bottom,
		viewSize, viewSize);
	glLoadIdentity();
	glOrtho(-DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE, -DIM_SCENE, DIM_SCENE);
	glMatrixMode(GL_MODELVIEW);
}  //func InitViewport

 // ����� ������
void DrawModel(int numVertices, int num)
{
	// ���������� ������ ���� ����
	glClearColor(1, 1, 1, 1.0);

	// ���������� �����.
	double k_gab = 0.95;
	

	// �������� ����� �����
	glClear(GL_COLOR_BUFFER_BIT);



   // ���������� ������� �������  ������
	glColor3d(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glVertex2d(DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, DIM_SCENE * k_gab);
	glVertex2d(-DIM_SCENE * k_gab, -DIM_SCENE * k_gab);
	glEnd();




	double incr = DIM_SCENE * k_gab;
	double x = 0 - incr;
	double y;
	double y_max = 0.2;
	double x_max = (DIM_SCENE * k_gab) *2/ num;


	//������������
	for (int j = 0; j < num; j++) {



		glBegin(GL_TRIANGLES);
		glColor3d(0, 1, 0);

		for (int i = 1; i < numVertices + 1; ++i) {

			y = 0;

			glVertex2d(x, y - incr);

			x += x_max / numVertices;

			glVertex2d(x, y - incr);

			y = y_max * i / numVertices;

			glVertex2d(x, y - incr);



		}
		glEnd();
		incr -= y_max;
	}

	//�����
	glBegin(GL_LINES);
	glColor3d(0, 0, 0);


	glVertex2d(0 - DIM_SCENE * k_gab, 0 - DIM_SCENE * k_gab);
	glVertex2d(x, y - incr -y_max);

	glEnd();


	//�����)
	 incr = DIM_SCENE * k_gab;
	 x = 0 - incr;

	for (int j = 0; j < num; j++) {



		glBegin(GL_LINE_LOOP);
		glColor3d(0, 0, 0);

		for (int i = 1; i < numVertices + 1; ++i) {

			y = 0;

			Vect vertex1 = { x ,y - incr };

			glVertex2d(vertex1.x, vertex1.y);

			x += x_max / numVertices;

			Vect vertex2 = { x ,y - incr };

			glVertex2d(vertex2.x, vertex2.y);

			y = y_max * i / numVertices;

			Vect vertex3 = { x ,y - incr };

			glVertex2d(vertex3.x, vertex3.y);
			  
			glVertex2d(vertex3.x, vertex3.y);
			glVertex2d(vertex1.x, vertex1.y);
			glVertex2d(vertex2.x, vertex2.y);
			glVertex2d(vertex3.x, vertex3.y);
		}
		glEnd();
		incr -= y_max;
	}

	//�������)) ��� point))

	incr = DIM_SCENE * k_gab;
	x = 0 - incr;

	for (int j = 0; j < num; j++) {


		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glColor3d(1, 0, 0);

		for (int i = 1; i < numVertices + 1; ++i) {
			

			y = 0;

			glVertex2d(x, y - incr);

			x += x_max / numVertices;

			glVertex2d(x, y - incr);

			y = y_max * i / numVertices;

			glVertex2d(x, y - incr);



		}
		glEnd();
		incr -= y_max;
	}

	glFinish();
	SwapBuffers(hdcMain);
} 

// ������� ��� ��������� n ��������������� � �������� radius � ������� � ����� (x, y)
void DrawRectangles(double x, double y, double radius, int n)
{
	// �������� ��������� �����
	glBegin(GL_LINE_LOOP);

	// ��������� ��������� �����
	glEnd();
}
// ��������� �������� ����
LRESULT WINAPI MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		InitViewport(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		MainOnPaint(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0L;
}  // func MainWndProc

 // ���������� WM_PAINT
void MainOnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	DrawModel(6,10);
	// ������ ��������������


	EndPaint(hwnd, &ps);
}  //func MainOnPaint

 // ����������� ������� ����
int RegisterWndClasses()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hInstance = hiApp;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = pczMainWndClass;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}  // func RegisterWndClasses

 // ������������� ����������
void InitializeApp()
{
	// ���������������� ����� � ������� ������� ����
	RegisterWndClasses();
	hwndMain = CreateWindow(pczMainWndClass, NULL,
		WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME |
		WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 800, 600, NULL,
		NULL, hiApp, NULL);
	if (!hwndMain)
		return;
	// ���������������� OpenGL
	InitOpenGL();
	// ������� ����� � ���� ��� ������� ������� ������
	SelectObject(hdcMain, GetStockObject(NULL_BRUSH));
	SelectObject(hdcMain, GetStockObject(WHITE_PEN));
	//
	RECT r;
	GetClientRect(hwndMain, &r);
	InitViewport(r.right, r.bottom);
}  //func InitializeApp

 // ������������ �������� ����������
void UninitializeApp()
{
	UnregisterClass(pczMainWndClass, hiApp);
}  //func UninitializeApp

 // ����� ����� ����������
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE hi_void, LPSTR pc_cl, int wsm)
{
	hiApp = hi;
	InitializeApp();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);
	UninitializeApp();
	return 0;
}  //func WinMain


 // --- main.cpp ---
