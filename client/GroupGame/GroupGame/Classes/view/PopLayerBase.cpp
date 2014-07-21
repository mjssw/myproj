#include "PopLayerBase.h"
using namespace cocos2d;

CPopLayerBase::CPopLayerBase() : m_parent(NULL)
{
}

CPopLayerBase::~CPopLayerBase()
{
}

void CPopLayerBase::onEnter()
{
	Layer::onEnter();
	m_parent = getParent();
	_PauseParentEventListener();
}

void CPopLayerBase::onExit()
{
	Layer::onExit();
	_ResumeParentEventListener();
	m_parent = NULL;
}

void CPopLayerBase::_PauseParentEventListener()
{
	if( !m_parent )
	{
		CCLog( "[CPopLayerBase::_PauseParentEventListener] getParent NULL" );
		return;
	}
	EventDispatcher *dispatcher = m_parent->getEventDispatcher();
	if( !dispatcher )
	{
		CCLog( "[CPopLayerBase::_PauseParentEventListener] getEventDispatcher NULL" );
		return;
	}
	dispatcher->pauseEventListenersForTarget( m_parent, true );
}

void CPopLayerBase::_ResumeParentEventListener()
{
	if( !m_parent )
	{
		CCLog( "[CPopLayerBase::_ResumeParentEventListener] getParent NULL" );
		return;
	}
	EventDispatcher *dispatcher = m_parent->getEventDispatcher();
	if( !dispatcher )
	{
		CCLog( "[CPopLayerBase::_ResumeParentEventListener] getEventDispatcher NULL" );
		return;
	}
	dispatcher->resumeEventListenersForTarget( m_parent, true );
}
