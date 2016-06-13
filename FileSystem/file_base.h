#pragma once
#include <string>
#include <time.h>
#include <memory>
#include <map>

enum file_type
{
	file,
	folder
};
struct file_mod
{
	char type : 3, 
		owner : 3, group : 3, other : 3;

};

enum  ACCESS
{
	read = 0,
	write = 1,
	excute = 2
};
enum class err_code {
	exist,
	access,
	not_find
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
	void setmod(std::string m);
	bool match_mod( int w);
	virtual std::string read()=0;
	virtual void * open()=0;
};
class mfile :public file_base
{
public:
	mfile(std::string name);
	~mfile() = default;
	void* open() final;
	std::string read() final;
	std::string m_data;
};
class direction;
using ptr_base = std::shared_ptr<file_base>;
using ptr_file = std::shared_ptr<mfile>;
using ptr_dir = std::shared_ptr<direction>;

using tmp_file = std::unique_ptr<mfile,void (*)(mfile*)>;
using tmp_dir = std::unique_ptr<direction, void(*)(direction*)>;

class direction :public file_base
{
public:
	std::map<std::string, ptr_base> m_data;
	direction(std::string name);
	direction(std::string name,ptr_dir parent);
	~direction() = default;
	void* open();
	std::string read() final;
	bool create_file(std::string name);
	bool create_direction(std::string name);
	bool remove(std::string name);
	
	void each(void (*fun)(ptr_base));
	ptr_file get_file(std::string name);
	ptr_dir  get_direction(std::string name);
	
};

ptr_file make_file(std::string name);
ptr_dir make_direction(std::string name,ptr_dir parent);
ptr_dir make_direction(std::string name);

