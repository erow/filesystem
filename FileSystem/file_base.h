#pragma once
#include <string>
#include <time.h>
enum  file_type
{
	file,
	folder
};
struct file_mod
{
	char type : 3, 
		owner : 3, group : 3, other : 3;

};
extern std::string current_user;
class file_base
{
public:
	std::string file_name;
	std::string owner;
	file_mod mod;
	time_t time_modified;


	file_base(std::string name);
	virtual ~file_base();

	std::string get_info();
};

