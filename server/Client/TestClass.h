#ifndef _TEST_T_CLASS_H_
#define _TEST_T_CLASS_H_

#include <stdio.h>
#include "TestClass2.h"

template <typename T>
class TClass1
{
};

class TemplateClassTest
{
public:
	template <typename T>
	void Test()
	{
		printf("Test\n");
	}

	~TemplateClassTest(){}
	static TemplateClassTest& Instance()
	{
		if( m_pInstance == 0 )
		{
			m_pInstance = new TemplateClassTest();
		}
		return *m_pInstance;
	}

	static TemplateClassTest *m_pInstance;
};

#endif
