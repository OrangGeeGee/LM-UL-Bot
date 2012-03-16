#include "FileHandler.h"

FileHandler::FileHandler(path takeFrom, path putTo, string unrar)
{
	m_source = takeFrom;
	m_title = m_source.stem().string();

	m_destinationForUploads = putTo;
	m_destinationDir = m_destinationForUploads.string() + "\\" + m_source.filename().string();

	m_unrar = unrar;

	m_files = all_files_from_dir( m_source );
	if(m_files.size() <= 0) throw std::exception ("No target files could be found.");

	// find .nfo file
	xpress::sregex nfoRegex = xpress::sregex::compile(".nfo$");

	FileList::iterator file;
	for ( file = m_files.begin(); file != m_files.end(); file++ )
		if(xpress::regex_search(file->string(), nfoRegex)) {
			m_nfo = *file;
			cout << ">> .nfo file found" << endl;
		}
	
}
 
void FileHandler::transfer() {

	if(fs::is_directory(m_source)) {
		fs::create_directories(m_destinationDir);
	
		FileList::iterator file;
		string tmp, newDestination;
		string unrar;
		int returncode;
		xpress::sregex partRegex = xpress::sregex::compile(".*((.part([2-9]|[0-9]{2,}).rar)|.r[0-9]{2}|.[0-9]{3})");
		xpress::sregex rarRegex = xpress::sregex::compile(".*((.part1.rar)|(.rar)|(.001))");
		
		// copy everything to destination dir
		// except extract .rar files with parts e.g. .r01, .r02 etc
		for (  file = m_files.begin(); file != m_files.end(); file++ ) {
			tmp = file->string();
			cout << ">> " << tmp << " ";
			if (xpress::regex_search(tmp, rarRegex)) {
				// found rar file - unrar
				cout << "extract" << endl;
				unrar = m_unrar+ " \"" +tmp+ "\" \"" +m_destinationDir.string()+ "\"";
				returncode = system(unrar.c_str());
				//cout << "unrar.exe returned " << returncode;
				if(returncode!=0 && returncode!=10) {
					string s ( "unrar.exe returned code " + boost::lexical_cast<string>( returncode ) );
					throw std::exception( s.c_str() );
				}
					
			} else if(xpress::regex_search(tmp, partRegex)) {
				// found part file
				cout << "ignore (part file)" << endl;
			} else {
				// copy any other file
				cout << "copy" << endl;
				tmp.replace(0,m_source.string().length(),"");
				newDestination = m_destinationDir.string() + tmp;
				fs::copy_file(file->string(), newDestination, fs::copy_option::overwrite_if_exists);
			}
		}
	} else {
		cout << ">> " << m_source << " copy" << endl;
		fs::copy_file(m_source, m_destinationDir, fs::copy_option::overwrite_if_exists);
	}

	m_files = all_files_from_dir(m_destinationDir);
}

FileList FileHandler::all_files_from_dir(path dir) {
	FileList files, tmpFiles;
	if ( fs::exists( dir ) ) {
		fs::directory_iterator end_itr; // default construction yields past-the-end
		for ( fs::directory_iterator itr( dir ); itr != end_itr; ++itr ) {
			if ( fs::is_directory(itr->status()) ) {
				// recurse into deeper dir
				tmpFiles = all_files_from_dir( itr->path() );
				for(FileList::iterator f = tmpFiles.begin(); f != tmpFiles.end(); ++f) {
					files.insert( *f );
				}
			} else {
				// single file
				files.insert( itr->path() );
			}
		}
	}
	return files;
}