#include "file_base.h"


std::string print_access(char mod)
{
	std::string res;
	if (mod & (1 << 2))
		res += 'r';
	else
		res += '-';
	if (mod & (1 << 1))
		res += 'w';
	else
		res += '-';
	if (mod & (1))
		res += 'x';
	else
		res += '-';
	return res;
}
std::string current_user="erow";
file_base::file_base(std::string name)
{
	file_name = name;
	time(&time_modified);
	owner = current_user;
	mod = { file_type::file,7,4,4 };
}

file_base::~file_base()
{
}

std::string file_base::get_info()
{
	std::string res;
	if (mod.type == file_type::file)
		res += '-';
	if (mod.type == file_type::folder)
		res += 'd';
	//access
	res += print_access(mod.owner) + print_access(mod.group) + print_access(mod.other);

	//owner
	res += "  " + owner;

	//time
	char tmpBuf[200];
	strftime(tmpBuf, 200, "%Y/%m/%d %H:%M", localtime(&time_modified));
	res += "  " + std::string(tmpBuf);

	//
	res += "  " + file_name;
	return res;
}
