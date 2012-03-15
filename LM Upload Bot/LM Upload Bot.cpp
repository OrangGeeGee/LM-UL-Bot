// LM Upload Boat.cpp : Defines the entry point for the console application.
//
//#define BOOST_PROGRAM_OPTIONS_NO_LIB ON
#define CONFIG_FILENAME "config.ini"
#define D_SCL_SECURE_NO_WARNINGS ON

#include <iostream>
#include <string>
#include <set>
#include <exception>

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "FileHandler.h"

using namespace std; 
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;
namespace ptime = boost::posix_time;

int main(int argc, char* argv[])
{	
	string logFile = "";
	if(argc < 2) {
		cout << "Instructions" << endl;
		cout << "To use this bot launch it with the path as a parameter." << endl;
		cout << "The path parameter provided can be either a directory or a single file." << endl << endl;
		cout << "For example in uTorrent \"Run this program when download completes\" field must be filled with this line:" << endl;
		cout << "<PATH_TO_EXE>\\program.exe %D" << endl;
		cout << "where <PATH_TO_EXE> is path to this executable file." << endl << endl;
	} else {
		try {
			fs::path target (argv[1]);
			cout << "Starting work on " << target << endl;

			// config.ini file parse
			string configFilePath (fs::current_path().string() + "\\" + CONFIG_FILENAME);
			pt::ptree pt;
			pt::ini_parser::read_ini(configFilePath, pt);
			fs::path destinationDir (pt.get<string>("Paths.destinationDir"));
			fs::path torrentWatchDir (pt.get<string>("Paths.torrentWatchDir"));
			string unpacker = pt.get<string>("Paths.unpacker");
			string lmLoginCookie = pt.get<string>("LinkoManija.userLoginCookie");
			logFile = pt.get<string>("MISC.logFile");
			// some basic dir checks
			if(!fs::is_directory(destinationDir)) throw exception("Destination directory incorrectly specified in configuration file.");
			if(!fs::is_directory(torrentWatchDir)) throw exception("Torrent Watch directory incorrectly specified in configuration file.");
			cout << "Config file read with great success" << endl;

			// init object responsible for file operations
			FileHandler fh(target, destinationDir, unpacker);
			// copy/extract everything
			fh.transfer();
			cout << "Total " << fh.get_file_count() << " files in " << fh.get_destination_dir() << endl;

			// TODO make a new torrent file
			
			// TODO make description for lm
			
			// TODO determine lm category 
			
			// TODO upload torrent to lm
			
			// TODO download new torrent file and save it to output dir
		} catch(exception e) {
			cout << "Exception caught: " << e.what() << endl;
			
			if(logFile.length() > 0) {

				cout << "Writing error to " << logFile << endl;
				ofstream myfile (logFile, ios::app); //"example.txt" );
				if (myfile.is_open()) {
					ptime::ptime now = ptime::second_clock::local_time();
					myfile << ptime::to_simple_string(now) << " Error " << e.what() << "\n";
					myfile << argv[0] << " " << argv[1] << "\n";
					myfile << "\n";
					myfile.close();
				}
				else cout << "Unable to open logFile" << endl;
			}
		}
		

	}
	system("pause");
	
	return 0;
}