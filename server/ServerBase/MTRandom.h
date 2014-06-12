#ifndef _MT_RANDOM_H_
#define _MT_RANDOM_H_

#include "mtwist.h"

class CMTRandom
{
public:
	CMTRandom() : m_UseCount(0) {}
	CMTRandom(int seed) : m_UseCount(0)
	{
		mts_seed32( &m_RandState, (uint32_t)seed );
	}
	void SetSeed(int seed)
	{
		mts_seed32( &m_RandState, (uint32_t)seed );
		m_UseCount = 0;
	}
	unsigned int NextUInt()
	{
		++m_UseCount;
		return (unsigned int)mts_lrand( &m_RandState );
	}
	unsigned int NextUInt(unsigned int unMax)
	{
		return ( NextUInt() % unMax );
	}

private:
	mt_state m_RandState;
	unsigned int m_UseCount;
};

#endif
