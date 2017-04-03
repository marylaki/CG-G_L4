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
	CRect Sun;		 // ������������� ������
	CRect Earth;	               // ������������� �����
	CRect Moon;		 // ������������� ����
	CRect Mars;
	CRect EarthOrbit;            // �������������, ��������� ������ ������ �����
	CRect MoonOrbit;           // �������������, ��������� ������ ������ ����
	CRect MarsOrbit;

	CMatrix ECoords;            // ������� ���������� ����� � �� ������ ( x,y,1)
	CMatrix MCoords;          // ������� ���������� ���� � �� ����� ( x,y,1)
	CMatrix MCoords1;        // ������� ���������� ���� � �� ������ ( x,y,1)
	CMatrix MrsCoords;
	CMatrix PM;		 // ������� �������� ��� ����
	CMatrix PE;		 // ������� �������� ��� �����
	CMatrix PMrs;
	CRect RW;		 // ������������� � ����
	CRectD RS;		 // ������������� ������� � ���

	double wEarth;	               // ������� �������� ����� ������������ ������, ����./���.
	double wMoon;	 // ������� �������� ���� ������������ �����, ����./���.
	double wMars;	 // ������� �������� ����� ������������ ������, ����./���.

	double fiE;		 // ������� ��������� ����� � ������� �������� ������, ����
	double dfiE;	               // ���� �������� ����� �� ����� dt.
	double fiM;		 // ������� ��������� ���� � ������� �������� �����, ����
	double dfiM;	               // ���� �������� ����� �� ����� dt
	double fiMrs;
	double dfiMrs;

	double dt;		   // �������� �������������, ���.
public:
	CSunSystem();
	void SetDT(double dtx){ dt = dtx; };   // ��������� ��������� �������������
	void SetNewCoords();                     // ��������� ����� ���������� ������
	void GetRS(CRectD& RSX);	   // ���������� ������� ������� � ������� ��
	CRect GetRW(){ return RW; };	   // ���������� ������� ������� � ����	
	void Draw(CDC& dc);		    // ��������� ��� ���������������� ��������� ���������
};
