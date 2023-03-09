//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#ifndef	REF_COUNT_H
#define	REF_COUNT_H

class RefCount
{
public:
	RefCount();
	virtual	~RefCount();
	void	UpCount();
	void	DownCount();
private:
	int		References;
};

inline void RefCount::UpCount()
{
	++References;
}

inline void RefCount::DownCount()
{
	if (--References == 0)
		delete this;
	else;
}

#endif

