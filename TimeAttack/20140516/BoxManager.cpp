#include "BoxManager.h"

BoxWrapper::BoxWrapper()
: m_grade(0)
, m_order(0)
{}
BoxWrapper::~BoxWrapper()
{}
const int BoxWrapper::grade() const
{
	return m_grade;
}
const int BoxWrapper::order() const
{
	return m_order;
}
const int BoxWrapper::zorder() const
{
	return m_zorder;
}
void BoxWrapper::grade(const int& in)
{
	m_grade = in;
}
void BoxWrapper::order(const int& in)
{
	m_order = in;
}
void BoxWrapper::zorder(const int& in)
{
	m_zorder = in;
}

BoxManager::BoxManager()
: hOwner(NULL)
{}
BoxManager::~BoxManager()
{}

void BoxManager::Attach(HWND hWnd)
{
	hOwner = hWnd;
}
void BoxManager::Init(void)
{
	Rect rcBox(0,0,100,50);
	rcBox = rcBox >> Size(50,50);

	BoxWrapper* pGrade = new BoxWrapper;
	pGrade->Attach(hOwner);
	pGrade->SetDrawRect(rcBox);
	pGrade->SetColor(RGB(rand()%150+50,rand()%150+50,rand()%150+50));
	pGrade->grade(0);
	pGrade->order(0);
	pGrade->zorder(0);

	AddGrade(0, pGrade);
}
void BoxManager::Input(DWORD tick)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	List::iterator it;
	for (it = ArmyList.begin(); it != ArmyList.end(); it++)
	{
		GradeList& rGList = (*it);
		GradeList::iterator jt;
		for (jt = rGList.begin(); jt != rGList.end(); jt++)
		{
			SubList& rSList = (*jt);
			SubList::iterator kt;
			for (kt = rSList.begin(); kt != rSList.end(); kt++)
			{
				Rect tmp;
				if (::IntersectRect(&tmp, &rc, &((*kt)->GetDrawRect())))
				{
					(*kt)->Input(tick);
				}
			}
		}
	}
}
void BoxManager::Update(DWORD tick)
{
	List::iterator it;
	for (it = ArmyList.begin(); it != ArmyList.end(); it++)
	{
		GradeList& rGList = (*it);
		GradeList::iterator jt;
		for (jt = rGList.begin(); jt != rGList.end(); jt++)
		{
			SubList& rSList = (*jt);
			SubList::iterator kt;
			for (kt = rSList.begin(); kt != rSList.end(); kt++)
			{
				(*kt)->Update(tick);

				if ((*kt)->IsSelect())
				{
					Size Move(0, 100);
					Rect rcTmp = (*kt)->GetDrawRect();

					BoxWrapper* pGrade = new BoxWrapper;
					pGrade->Attach(hOwner);
					pGrade->SetDrawRect(rcTmp>>Move);
					pGrade->SetColor(RGB(rand()%150+50,rand()%150+50,rand()%150+50));

					if ((*kt)->order() == 0)
					{
						List::iterator itnext = it;
						itnext++;
						int order = 0;
						if (itnext == ArmyList.end())
							order = 0;
						else
							order = (*itnext).size();

						pGrade->grade((*kt)->grade() + 1);
						pGrade->order(order);
						pGrade->zorder(0);
						pGrade->SetDrawRect(pGrade->GetDrawRect()>>Size(150*order,0));

						AddGrade((*kt)->grade()+1, pGrade);
					}
					else
					{
						pGrade->grade((*kt)->grade());
						pGrade->order((*kt)->order());
						pGrade->zorder((*kt)->zorder() + 1);
						pGrade->SetDrawRect((*kt)->GetDrawRect());
						AddSub((*kt)->grade(), (*kt)->order(), pGrade);
					}

					break;
				}
			}
		}
	}
}
void BoxManager::Draw(HDC hdc)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	List::iterator it;
	for (it = ArmyList.begin(); it != ArmyList.end(); it++)
	{
		GradeList& rGList = (*it);
		GradeList::iterator jt;
		for (jt = rGList.begin(); jt != rGList.end(); jt++)
		{
			SubList& rSList = (*jt);
			SubList::iterator kt;
			for (kt = rSList.begin(); kt != rSList.end(); kt++)
			{
				Rect tmp;
				if (::IntersectRect(&tmp, &rc, &((*kt)->GetDrawRect())))
				{
					(*kt)->Draw(hdc);
				}
			}
		}
	}
}

void BoxManager::AddGrade(const int& Where, element* pElement)
{
	List::iterator it = ArmyList.begin();
	std::advance(it, Where);
	
	if (it == ArmyList.end())
	{
		SubList sList;
		sList.push_back(pElement);

		GradeList gList;
		gList.push_back(sList);

		ArmyList.push_back(gList);
	}
	else
	{
		SubList sList;
		sList.push_back(pElement);

		(*it).push_back(sList);
	}
}
void BoxManager::AddSub(const int& Where, const int& Order, element* pElement)
{
	List::iterator it = ArmyList.begin();
	std::advance(it, Where);

	GradeList& rGList = (*it);
	GradeList::iterator jt = rGList.begin();
	std::advance(jt, Order);
	
	SubList& rSList = (*jt);

	unsigned int size = rSList.size();
	pElement->SetDrawRect(pElement->GetDrawRect()>>Size(5*size,5*size));

	rSList.push_back(pElement);
}