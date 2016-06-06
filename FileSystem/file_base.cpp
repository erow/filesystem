#include "file_base.h"

#include <iomanip>

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
	char tmp[10];
	sprintf(tmp, "%6s", owner.c_str());
	res += "  " + std::string(tmp);

	//time
	char tmpBuf[200];
	strftime(tmpBuf, 200, "%Y/%m/%d %H:%M", localtime(&time_modified));
	res += "  " + std::string(tmpBuf);

	//
	res += "  " + file_name;
	return res;
}

void file_base::setmod(std::string m)
{
	char own, group, other;
	own = m[0] - '0'; group = m[1] - '0'; other = m[2] - '0';
	mod = { mod.type,own,group,other };
}

ptr_file make_file(std::string name) {

	return std::make_shared<mfile>(name);
}

ptr_dir make_direction(std::string name , ptr_dir parent) {
	return std::make_shared<direction>(name, parent);
}
ptr_dir make_direction(std::string name) {
	return std::make_shared<direction>(name);
}
direction::direction(std::string name) :file_base(name) { mod.type = file_type::folder; }

direction::direction(std::string name, ptr_dir parent) : file_base(name)
{
	mod.type = file_type::folder;
	m_data[".."] = parent; parent->file_name = "..";
}

inline void * direction::get_data() {
	return this;
}

bool direction::create_file(std::string name)
{
	if (m_data.find(name) != m_data.end())
	{
		return false;
	}
	m_data[name] = make_file(name);
	return true;
}

bool direction::create_direction(std::string name)
{
	if (m_data.find(name) != m_data.end())
	{
		return false;
	}
	m_data[name] = make_direction(name,ptr_dir(this, [](direction* p) {}));
	return true;
}

bool direction::remove(std::string name)
{
	if (m_data.find(name) == m_data.end())
	{
		return false;
	}
	m_data.erase(name);
	return true;
}


void direction::each(void(*fun)(ptr_base))
{
	for (auto i : m_data)
		fun(i.second);
}

ptr_file direction::get_file(std::string name)
{
	if (m_data.find(name) == m_data.end())
	{
		return tmp_file(nullptr, [](mfile*) {});
	}
	auto ptr = m_data[name];
	if (ptr->mod.type == file_type::file) {
		return tmp_file( (mfile*)ptr->get_data(), [](mfile*) {}) ;
	}
		
	return tmp_file(nullptr, [](mfile*) {});
}

ptr_dir direction::get_direction(std::string name)
{
	if (m_data.find(name) == m_data.end())
	{
		return tmp_dir(nullptr, [](direction*) {});
	}
	auto ptr = m_data[name];
	if (ptr->mod.type == file_type::folder) {
		return tmp_dir((direction*)ptr->get_data(), [](direction*) {});
	}
	return tmp_dir(nullptr, [](direction*) {});
}

inline mfile::mfile(std::string name) :file_base(name) { mod.type = file_type::file; }

inline void * mfile::get_data() {
	return this;
}
