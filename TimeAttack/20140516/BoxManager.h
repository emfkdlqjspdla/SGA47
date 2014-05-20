#pragma once

#include <list>
#include "Box.h"

class BoxWrapper : public Box
{
public :
	BoxWrapper();
	~BoxWrapper();

	const int grade() const;
	const int order() const;
	const int zorder() const;

	void grade(const int& in);
	void order(const int& in);
	void zorder(const int& in);

private :
	int m_grade;
	int m_order;
	int m_zorder;
};

class BoxManager
{
	typedef BoxWrapper element;
	typedef std::list<element*> SubList;
	typedef std::list<SubList> GradeList;
	typedef std::list<GradeList> List;
public :
	BoxManager();
	~BoxManager();

	void Attach(HWND);
	void Init();

	void Input(DWORD);
	void Update(DWORD);
	void Draw(HDC);

private :
	void AddGrade(const int& Where, element* pElement);
	void AddSub(const int& Where, const int& Order, element* pElement);
	void DrawReverse(HDC, SubList& sl);
private :
	HWND hOwner;

	List ArmyList;
};