#pragma once
#include "CMatrix.h"
#include <afxwin.h>

const double pi = 3.14159;

struct CSizeD
{
	double cx;
	double cy;
};
struct CRectD
{
	double left;
	double top;
	double right;
	double bottom;
	CRectD()
	{
		left = top = right = bottom = 0;
	}
	CRectD(double l, double t, double r, double b);
	void setRectD(double l, double t, double r, double b);
	CSizeD SizeD();
};

CMatrix CreateRotate2D(double fi);
CMatrix CreateTranslate2D(double dx, double dy);

CMatrix SpaceToWindow(CRectD& rs, CRect& rw);
void SetMyMode(CDC& dc, CRectD& RS, CRect& RW);

class CSunSystem
{
	CRect Sun;		 // Прямоугольник Солнца
	CRect Earth;	               // Прямоугольник Земли
	CRect Moon;		 // Прямоугольник Луны
	CRect Mars;
	CRect EarthOrbit;            // Прямоугольник, описанный вокруг орбиты Земли
	CRect MoonOrbit;           // Прямоугольник, описанный вокруг орбиты Луны
	CRect MarsOrbit;

	CMatrix ECoords;            // Текущие координаты Земли в СК Солнца ( x,y,1)
	CMatrix MCoords;          // Текущие координаты Луны в СК Земли ( x,y,1)
	CMatrix MCoords1;        // Текущие координаты Луны в СК Солнца ( x,y,1)
	CMatrix MrsCoords;
	CMatrix PM;		 // Матрица поворота для луны
	CMatrix PE;		 // Матрица поворота для Земли
	CMatrix PMrs;
	CRect RW;		 // Прямоугольник в окне
	CRectD RS;		 // Прямоугольник области в МСК

	double wEarth;	               // Угловая скорость Земли относительно Солнца, град./сек.
	double wMoon;	 // Угловая скорость Луны относительно Земли, град./сек.
	double wMars;	 // Угловая скорость Марса относительно Солнца, град./сек.

	double fiE;		 // Угловое положение Земли в системе кординат Солнца, град
	double dfiE;	               // Угол поворота Земли за время dt.
	double fiM;		 // Угловое положение Луны в системе кординат Земли, град
	double dfiM;	               // Угол поворота Земли за время dt
	double fiMrs;
	double dfiMrs;

	double dt;		   // Интервал дискретизации, сек.
public:
	CSunSystem();
	void SetDT(double dtx){ dt = dtx; };   // Установка интервала дискретизации
	void SetNewCoords();                     // Вычисляет новые координаты планет
	void GetRS(CRectD& RSX);	   // Возвращает область графика в мировой СК
	CRect GetRW(){ return RW; };	   // Возвращает область графика в окне	
	void Draw(CDC& dc);		    // Рисование без самостоятельного пересчета координат
};
