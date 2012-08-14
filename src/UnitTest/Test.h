#pragma once

class Test
{
public:
	Test(void);
	~Test(void);
};

#define  TEST(x)	if(!(x)) { TRACE(L"[ERROR] %s\n", L#x); ASSERT(0); } else { TRACE(L"[SUCCESS] %s\n", L#x); }
void Test1();
void Test2();
void Test3();