// FileSystem.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "b_tree.h"
#include "file_base.h"
#include <iostream>
using namespace std;
int main()
{
	b_tree<int> t;
	file_base file("t.txt");

	cout << file.get_info();
    return 0;
}

