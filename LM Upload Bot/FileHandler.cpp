#include "FileHandler.h"

FileHandler::FileHandler(path takeFrom, path putTo)
{
	m_source = takeFrom;
	m_title = m_source.stem().string();

	m_destinationForUploads = putTo;
	m_destinationDir = m_destinationForUploads.string() + "\\" + m_source.filename().string();

	m_files = all_files_from_dir( m_source );
	if(m_files.size() <= 0) throw std::exception ("No target files could be found.");

	// find .nfo file
}
 
void FileHandler::transfer() {

	if(fs::is_directory(m_source)) {
		fs::create_directories(m_destinationDir);
	
		FileList::iterator file;
		string tmp, newDestination;
		// copy everything to destination dir
		// except extract .rar files with parts e.g. .r01, .r02 etc
		std::cout << m_source << std::endl;
		for (  file = m_files.begin(); file != m_files.end(); file++ ) {
			tmp = file->string();
			tmp.replace(0,m_source.string().length(),"");
			newDestination = m_destinationDir.string() + tmp;
			fs::copy_file(file->string(), newDestination, fs::copy_option::overwrite_if_exists);
		}
	} else {
		fs::copy_file(m_source, m_destinationDir, fs::copy_option::overwrite_if_exists);
	}

/*

	// extracts .rar files with parts e.g. .r01, .r02 etc
	/*if(name.matches(".*((.part([2-9]|[0-9]{2,}).rar)|.r[0-9]{2})")) {
    	echo("Found rar file "+name+" but it's a part; ignoring");
    } else if(name.matches(".*((.part1.rar)|(.rar))")) {
    	echo("Found rar file "+name);
    	extract(file, destinationDir);
    } else {
    	echo("Copying a file "+name);
    	copy(file, new File(destinationDir, name));
    }
*/
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