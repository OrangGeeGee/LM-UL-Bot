#ifndef __FileHandler__
#define __FileHandler__

#include <iostream>
#include <set>
#include <string>
#include <exception>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>

using std::string;
using std::cout;
using std::endl;
namespace xpress = boost::xpressive;
namespace fs = boost::filesystem;
typedef fs::path path;
typedef std::set<fs::path> FileList;

class FileHandler
{
private:
    FileList m_files;
	string m_title;
    string m_unrar;
	path m_source;
	path m_destinationForUploads;
	path m_destinationDir;
    path m_nfo;

	FileList all_files_from_dir(path dir);

public:
    FileHandler(path takeFrom, path putTo, string unrar);
 
	int get_file_count() { return m_files.size(); }
    FileList get_files() { return m_files; }
	string get_title() { return m_title; }
	string get_destination_dir() { return m_destinationDir.string(); }
    string get_nfo()  { return m_nfo.string(); }
	bool has_nfo() { return m_nfo.string().size() > 0 ? true : false; }

	void transfer();
};
 
#endif 