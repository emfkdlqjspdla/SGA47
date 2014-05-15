#pragma once

#include "Image.h"
#include <list>

class Animation
{
public :
	Animation()
		: update_dt(0), update_delay(100)
		, bDrawImage(true)
		, index(0)
	{}
	~Animation()
	{
		std::list<Image*>::iterator it;
		for (it = imgList.begin(); it != imgList.end(); it++)
		{
			delete (*it);
		}
	}

	void AddImage(Image* pImage)
	{
		imgList.push_back(pImage);
	}

	void Update(DWORD tick)
	{
		if (!bDrawImage) return;

		if (update_dt > update_delay)
		{
			index++;
			if (index == imgList.size())
				bDrawImage = false;

			update_dt -= update_delay;
		}

		update_dt += tick;
	}
	void Draw(HDC hdc)
	{
		if (!bDrawImage) return;

		std::list<Image*>::iterator it = imgList.begin();
		std::advance(it, index);

		(*it)->Draw(hdc);
	}
	void SetDrawRect(const Rect& rc)
	{
		std::list<Image*>::iterator it;
		for (it = imgList.begin(); it != imgList.end(); it++)
		{
			(*it)->SetDrawRect(rc);
		}
	}
	bool IsPlaying() const
	{
		return bDrawImage;
	}

private :
	std::list<Image*> imgList;

	DWORD update_dt;
	DWORD update_delay;

	int index;

	bool bDrawImage;
};