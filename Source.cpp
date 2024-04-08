
//------------------------------------------------------- 
//      GL_2D.cpp
//       =========
//  Рисование плоских фигур  в OpenGL
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
// область вывода в оконных координатах
RECT rcViewport;

//  Размер области вывода в модельных (габаритный куб)
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

// инициализация контекста воспроизведения OpenGL
void InitOpenGL()
{
	hdcMain = GetDC(hwndMain);
	int iPixelFormat;
	// инициализация структуры формат пикселей
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // размер структуры
		1,                              // номер версии OpenGL
		PFD_DRAW_TO_WINDOW |          // поддержка вывода в окно
		PFD_SUPPORT_OPENGL |          // поддержка OpenGL
		PFD_DOUBLEBUFFER,		// двойная буферизация
		PFD_TYPE_RGBA,          // цвета в режиме RGBA
		24,                     // 24 разряда на цвет
		0, 0, 0, 0, 0, 0,       // биты цвета игнорируются
		0,                      // не используется альфа-параметр
		0,                      // смещение цветов игнорируются
		0,                      // буфер аккумулятора не используется
		0, 0, 0, 0,             // биты аккумулятора игнорируются
		32,                     // 32-разрядный буфер глубины
		0,                      // буфер трафарета не используется
		0,                      // вспомогательный буфер не используется
		PFD_MAIN_PLANE,         // основной слой
		0,                      // зарезервирован
		0, 0, 0                 // маски слоя игнорируются
	};
	// подбор pix-формата
	iPixelFormat = ChoosePixelFormat(hdcMain, &pfd);
	if (!iPixelFormat)
		MsgBox("Неверный формат пикселей");
	// установка pix-формата
	if (!SetPixelFormat(hdcMain, iPixelFormat, &pfd))
		MsgBox("Формат пикселей не установлен");
	// создание OpenGL-контекста
	hGlRc = wglCreateContext(hdcMain);
	if (!hGlRc)
		MsgBox("Контекст OpenGL не создан");
	// установка текущего OpenGL-контекста
	if (!wglMakeCurrent(hdcMain, hGlRc))
		MsgBox("Контекст OpenGL не установлен");
}  // func InitOpenGL

 // освобождение контекста воспроизведения
void UninitOpenGL()
{
	// сбросить текущий контекст воспроизведения OpenGL
	wglMakeCurrent(NULL, NULL);
	// удаленить контекста воспроизведения OpenGL
	if (hGlRc)
		wglDeleteContext(hGlRc);
	// освобождение контекста рабочей области
	ReleaseDC(hwndMain, hdcMain);
}  //func UninitOpenGL

 // инициализация области вывода
void InitViewport(const int cx,
	const int cy)
{
	// задать область вывода OpenGL по наименьшему размеру окна
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

 // вывод модели
void DrawModel(int numVertices, int num)
{
	// установить чёрный цвет фона
	glClearColor(1, 1, 1, 1.0);

	// габаритный коэфф.
	double k_gab = 0.95;
	

	// очистить буфер цвета
	glClear(GL_COLOR_BUFFER_BIT);



   // нарисовать габарит области  вывода
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


	//треугольники
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

	//линия
	glBegin(GL_LINES);
	glColor3d(0, 0, 0);


	glVertex2d(0 - DIM_SCENE * k_gab, 0 - DIM_SCENE * k_gab);
	glVertex2d(x, y - incr -y_max);

	glEnd();


	//ребра)
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

	//точечки)) ака point))

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

// Функция для отрисовки n прямоугольников с радиусом radius и центром в точке (x, y)
void DrawRectangles(double x, double y, double radius, int n)
{
	// Начинаем рисование линий
	glBegin(GL_LINE_LOOP);

	// Завершаем рисование линий
	glEnd();
}
// процедура главного окна
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

 // обработчик WM_PAINT
void MainOnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	DrawModel(6,10);
	// Рисуем прямоугольники


	EndPaint(hwnd, &ps);
}  //func MainOnPaint

 // регистрация классов окон
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

 // инициализация приложения
void InitializeApp()
{
	// зарегистрировать класс и создать главное окно
	RegisterWndClasses();
	hwndMain = CreateWindow(pczMainWndClass, NULL,
		WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME |
		WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 800, 600, NULL,
		NULL, hiApp, NULL);
	if (!hwndMain)
		return;
	// инициализировать OpenGL
	InitOpenGL();
	// выбрать кисть и перо для обводки области вывода
	SelectObject(hdcMain, GetStockObject(NULL_BRUSH));
	SelectObject(hdcMain, GetStockObject(WHITE_PEN));
	//
	RECT r;
	GetClientRect(hwndMain, &r);
	InitViewport(r.right, r.bottom);
}  //func InitializeApp

 // освобождение ресурсов приложения
void UninitializeApp()
{
	UnregisterClass(pczMainWndClass, hiApp);
}  //func UninitializeApp

 // точка входа приложения
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
