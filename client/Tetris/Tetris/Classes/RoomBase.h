#ifndef _ROOM_BASE_H_
#define _ROOM_BASE_H_

class CRoomBase
{
public:
	CRoomBase(int id) : m_Id(id){}
	virtual ~CRoomBase(){}

	int Id(){ return m_Id; }

protected:
	int m_Id;
};

#endif
