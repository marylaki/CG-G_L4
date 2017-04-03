#include "CSunSystem.h"
#include <math.h>

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
void CRectD::setRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);
	cz.cy = fabs(top - bottom);

	return cz;
}

CMatrix SpaceToWindow(CRectD& rs, CRect& rw)
{
	CMatrix M(3, 3);
	CSize sz = rw.Size();
	int dwx = sz.cx;
	int dwy = sz.cy;
	CSizeD szd = rs.SizeD();

	double dsx = szd.cx;
	double dsy = szd.cy;
	double kx = (double)dwx / dsx;
	double ky = (double)dwy / dsy;

	M(0, 0) = kx;
	M(0, 1) = 0;
	M(0, 2) = (double)rw.left - kx*rs.left;
	M(1, 0) = 0;
	M(1, 1) = -ky;
	M(1, 2) = (double)rw.bottom + ky*rs.bottom;
	M(2, 0) = 0;
	M(2, 1) = 0;
	M(2, 2) = 1;

	return M;
}
void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
{
	int dsx = RS.right - RS.left;
	int dsy = RS.top - RS.bottom;
	int xsL = RS.left;
	int ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt(dsx, dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg(xsL, ysL);
	dc.SetViewportOrg(xwL, ywH);
}

CMatrix CreateRotate2D(double fi)
{
	double fg = fmod(fi, 360.0);
	double ff = (fg / 180.0)*pi;

	CMatrix RM(3, 3);

	RM(0, 0) = cos(ff);
	RM(0, 1) = -sin(ff);
	RM(1, 0) = sin(ff);
	RM(1, 1) = cos(ff);
	RM(2, 2) = 1;

	return RM;
}
CMatrix CreateTranslate2D(double dx, double dy)
{
	CMatrix TM(3, 3);

	TM(0, 0) = 1;
	TM(0, 2) = dx;
	TM(1, 1) = 1;
	TM(1, 2) = dy;
	TM(2, 2) = 1;

	return TM;
}

CSunSystem::CSunSystem()
{
	double rS = 30, rE = 20, rM = 10, rMrs=17;   
	double RoE = 10 * rS, RoM = 5 * rE, RoMrs=5*rS;
	double d = RoE + RoM + rM;

	this->RS.setRectD(-d, d, d, -d);
	this->RW.SetRect(0, 0, 600, 600);

	this->Sun.SetRect(-rS, rS, rS, -rS);
	this->Earth.SetRect(-rE, rE, rE, -rE);
	this->Moon.SetRect(-rM, rM, rM, -rM);
	this->Mars.SetRect(-rMrs, rMrs, rMrs, -rMrs);

	this->EarthOrbit.SetRect(-RoE, RoE, RoE, -RoE);
	this->MoonOrbit.SetRect(-RoM, RoM, RoM, -RoM);
	this->MarsOrbit.SetRect(-RoMrs, RoMrs, RoMrs, -RoMrs);

	this->fiE = 0;
	this->fiM = 0;
	this->fiMrs = 0;

	this->wEarth = 5;
	this->wMoon = 50;
	this->wMars = -8;

	this->dt = 0.2;

	this->dfiM = this->wMoon*this->dt;
	this->dfiE = this->wEarth*this->dt;
	this->dfiMrs = this->wMars*this->dt;

	this->MCoords.RedimMatrix(3);
	this->MCoords1.RedimMatrix(3);
	this->ECoords.RedimMatrix(3);
	this->MrsCoords.RedimMatrix(3);

	this->PE.RedimMatrix(3, 3);
	this->PM.RedimMatrix(3, 3);
	this->PMrs.RedimMatrix(3, 3);

	RoM = (this->MoonOrbit.right - this->MoonOrbit.left) / 2;
	RoE = (this->EarthOrbit.right - this->EarthOrbit.left) / 2;
	RoMrs = (this->MarsOrbit.right - this->MarsOrbit.left) / 2;

	double ff = (this->fiM / 180.0)*pi;
	double x = RoM * cos(ff);
	double y = RoM * sin(ff);
	this->MCoords(0) = x;
	this->MCoords(1) = y;
	this->MCoords(2) = 1;

	ff = (this->fiE / 180.0)*pi;
	x = RoE * cos(ff);
	y = RoE * sin(ff);
	this->ECoords(0) = x;
	this->ECoords(1) = y;
	this->ECoords(2) = 1;

	ff = (this->fiMrs / 180.0)*pi;
	x = RoMrs * cos(ff);
	y = RoMrs * sin(ff);
	this->MrsCoords(0) = x;
	this->MrsCoords(1) = y;
	this->MrsCoords(2) = 1;

	this->PE = CreateRotate2D(this->dfiE);
	this->PM = CreateRotate2D(this->dfiM);
	this->PMrs = CreateRotate2D(this->dfiMrs);
}
void CSunSystem::SetNewCoords()                     // Вычисляет новые координаты планет
{
	this->MCoords = this->PM*this->MCoords;
	double x = this->ECoords(0);
	double y = this->ECoords(1);
	CMatrix p = CreateTranslate2D(x, y);
	this->MCoords1 = p*this->MCoords;
	this->MCoords1 = this->PE*this->MCoords1;

	this->ECoords = this->PE*this->ECoords;
	this->MrsCoords = this->PMrs*this->MrsCoords;
}
void CSunSystem::GetRS(CRectD& RSX)	   // Возвращает область графика в мировой СК
{
	RSX.left = RS.left;
	RSX.right = RS.right;
	RSX.top = RS.top;
	RSX.bottom = RS.bottom;
}
void CSunSystem::Draw(CDC& dc)		    // Рисование без самостоятельного пересчета координат
{
	CBrush SBrush, EBrush, MBrush, MrsBrush, *pOldBrush;
	CRect R;
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	SBrush.CreateSolidBrush(RGB(255, 0, 0));
	EBrush.CreateSolidBrush(RGB(0, 0, 255));
	MBrush.CreateSolidBrush(RGB(0, 255, 0));
	MrsBrush.CreateSolidBrush(RGB(128, 0, 128));
	//Орбиты
	dc.SelectStockObject(NULL_BRUSH);
	dc.Ellipse(this->EarthOrbit);
	dc.Ellipse(this->MarsOrbit);
	int d = this->MoonOrbit.right;
	R.SetRect
		(this->ECoords(0) - d,
		this->ECoords(1) + d,
		this->ECoords(0) + d,
		this->ECoords(1) - d);
	dc.Ellipse(R);
	//Sun
	pOldBrush = dc.SelectObject(&SBrush);
	dc.Ellipse(this->Sun);
	//Earth
	d = this->Earth.right;
	R.SetRect
		(this->ECoords(0) - d,
		this->ECoords(1) + d,
		this->ECoords(0) + d,
		this->ECoords(1) - d);
	dc.SelectObject(&EBrush);
	dc.Ellipse(R);
	//Moon
	d = this->Moon.right;
	R.SetRect
		(this->MCoords1(0) - d,
		this->MCoords1(1) + d,
		this->MCoords1(0) + d,
		this->MCoords1(1) - d);
	dc.SelectObject(&MBrush);
	dc.Ellipse(R);
	//Mars
	d = this->Mars.right;
	R.SetRect
		(this->MrsCoords(0) - d,
		this->MrsCoords(1) + d,
		this->MrsCoords(0) + d,
		this->MrsCoords(1) - d);
	dc.SelectObject(&MrsBrush);
	dc.Ellipse(R);

	dc.SelectObject(pOldBrush);
}
