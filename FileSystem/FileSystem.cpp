// FileSystem.cpp : 定义控制台应用程序的入口点。
//
#include "b_tree.h"
#include "file_base.h"
#include <iostream>
#include <regex>



using namespace std;
string help = "\
echo    string file \n\
mv      string file \n\
mkdir   mod    name\n\
ls      -      - \n\
cd      dir    - \n\
cat     file   - \n\
chmod   mod    file \n\
su      user   - \n\
help    -      - \n\
q\n\
";
extern std::string current_user;
string print_path(vector<string> path)
{
	string str;
	for (auto item : path)
		str += "/" + item;
	return str;
}


void error_info(err_code info)
{
	switch (info)
	{
	case err_code::access:
		cout << "you cann't access this file!" << endl;
		break;
	case err_code::exist:
		cout << "already exist!" << endl;
		break;
	case err_code::not_find:
		cout << "cann't find file" << endl;
		break;
	default:
		break;
	}
	
}
int main()
{
	vector<string> path = {};
	cout << help << endl;
	ptr_dir root, current_dir;
	root = make_direction("");
	current_dir = root;
	while (true)
	{
		cout  << current_user <<"@"<< print_path(path) << "  $";
		string opt, opr1, opr2;
		cin >> opt >> opr1 >> opr2;
		auto p = (direction*)current_dir->open();
		if(opt=="echo")
		{
			if (p->match_mod(ACCESS::write))
			{
				if (p->create_file(opr2))
					p->get_file(opr2)->m_data = opr1;
				else
				{
					error_info(err_code::exist);
				}
			}
			else
			{
				error_info(err_code::access);
			}
		}
		if (opt == "cp")
		{
			if (!p->get_file(opr1)) {
				error_info(err_code::not_find);continue;
			}
			if (p->create_file(opr2))
			{
				p->get_file(opr2)->m_data= p->get_file(opr1)->m_data;
			}
			else {
				cout << "already exist file" << endl;
			}
		}
		if (opt == "help")
		{
			cout << help;
		}
		if (opt == "su")
		{
			current_user = opr1;
		}
		if(opt=="mkdir")
		{
			if (p->match_mod( ACCESS::excute))
			{
				if (p->create_direction(opr2))
				{
					p->get_direction(opr2)->setmod(opr1);
				}
				else
					error_info(err_code::exist);
			}
			else
				error_info(err_code::access);
		}
		if (opt == "cat")
		{
			auto ptr = p->get_file(opr1);
			if (ptr) {
				cout << ptr->read()<<endl;
			}
			else
			{
				error_info(err_code::not_find);
			}
		}
		if (opt == "chmod")
		{
			auto ptr = p->get_file(opr2);
			if (ptr) {
				ptr->setmod(opr1);
			}
			else
			{
				error_info(err_code::not_find);
			}
		}
		if (opt == "ls")
		{
			if (opr1 == "-")
			{
				cout << current_dir->read(); continue;
			}

			auto ptr = p->get_direction(opr1);
			if (ptr) {
				cout << ptr->read();
			}
			else
			{
				error_info(err_code::not_find);
			}
		}
		if (opt == "cd") {
			auto ptr=p->get_direction(opr1);
			if (ptr)
			{
				if (p->match_mod(ACCESS::write))
				{
					current_dir = p->get_direction(opr1);
				}
				else
					error_info(err_code::access);
			}
			else
				error_info(err_code::not_find);
			
		}
		if (opt == "rm")
		{
			if (p->match_mod(ACCESS::write))
			{
				p->remove(opr1);
			}
			else
				error_info(err_code::access);
			
		}
		if (opt == "mv")
		{
			
			if (p->match_mod( ACCESS::write))
			{
				if (!p->get_file(opr2))
				{
					p->m_data[opr2] = p->m_data[opr1];
					p->m_data[opr2]->file_name = opr2;
					p->m_data.erase(opr1);
				}
				else
					error_info(err_code::exist);
			}
			else
				error_info(err_code::access);

		}
		if (opt == "q")
		{
			break;
		}
	}

    return 0;
}

