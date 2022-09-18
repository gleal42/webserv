/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:27:09 by gleal             #+#    #+#             */
/*   Updated: 2022/09/17 02:05:28 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.hpp"
#include <sys/mman.h>

namespace file
{
	typedef std::set<std::string>	Extensions;
	typedef std::map<std::string, std::string> MimeTypes;

	std::string const content_type( const std::string & path )
	{
		std::string extension = get_extension(path);

		// Turn this into a separate mime_type function with access to static map
		MimeTypes mime_types;

		mime_types[".css"] = "text/css";
		mime_types[".csv"] = "text/csv";
		mime_types[".gif"] = "image/gif";
		mime_types[".htm"] = "text/html";
		mime_types[".html"] = "text/html";
		mime_types[".ico"] = "image/x-icon";
		mime_types[".jpeg"] = "image/jpeg";
		mime_types[".jpg"] = "image/jpeg";
		mime_types[".js"] = "application/javascript";
		mime_types[".json"] = "application/json";
		mime_types[".png"] = "image/png";
		mime_types[".pdf"] = "application/pdf";
		mime_types[".svg"] = "image/svg+xml";
		mime_types[".txt"] = "text/plain";
		mime_types[".php"] = "application/x-httpd-php";

		if (extension.size() && mime_types.find(extension) != mime_types.end()) {
			return mime_types[extension];
		}
		return "application/octet-stream";
	}

	std::streampos	size( std::string &full_path )
	{
		std::streampos	fsize = 0;
		std::ifstream	file( full_path.c_str(), std::ios::binary );

		fsize = file.tellg();
		file.seekg( 0, std::ios::end );
		fsize = file.tellg() - fsize;
		file.close();

		return fsize;
	}

	long	size( FILE *open_file )
	{
		fseek(open_file, 0L, SEEK_END);
		long end_file = ftell(open_file);
		rewind(open_file);
		return (end_file - ftell(open_file));
	}

	void	save( const std::string &file_body, const std::string & filename )
	{
		std::ofstream	outfile;
		std::string		tmp = "public/uploads/" + filename;

		if (is_file(tmp))
		{
			std::string	extension = get_extension(filename);
			if (extension.empty() == false)
				tmp = tmp.substr(0, (tmp.size() - extension.size()));
			int nbr = 0;
			while (is_file(tmp + to_string(++nbr) + extension))
				;
			tmp = tmp + to_string(nbr) + extension;
		}
		outfile.open(tmp.c_str() , std::ios::binary);
		if ( (outfile.rdstate() & std::ifstream::failbit ) != 0) {
			throw std::runtime_error("Couldn't open new file");
		}
		outfile.write(file_body.data(), file_body.size());
		if ( (outfile.rdstate() & std::ifstream::failbit ) != 0
			|| (outfile.rdstate() & std::ifstream::badbit ) != 0) {
			throw std::runtime_error("Couldn't write to file");
		}
		outfile.close();
	}

	// Added a protection to prevent us from deleting a repository code or other testing data

	void	remove( const std::string & filename )
	{
		if (filename.substr(0, 15) != "public/uploads/")
			throw HTTPStatus<403>();
		std::cout << "Filename is [" << filename << "]" << std::endl;
		if (std::remove(filename.c_str()) != 0)
			throw HTTPStatus<404>();
	}

	std::string	get_string(FILE *file_ptr, int file_fd )
	{
		long sz = file::size(file_ptr);
		std::cout << "file has size " << sz << std::endl;
		char *file_contents = (char *) mmap(0, sz, PROT_READ, MAP_PRIVATE, file_fd, 0);
		if (file_contents == (void *)-1) {
			throw HTTPStatus<500>();
		}
		std::string	str(file_contents, sz);
		str.push_back('\0');
		munmap(file_contents, sz);
		return (str);
	}

	Indexes_cit find_valid_index(const std::string &root, const StringVector &indexes )
	{
		for (Indexes_cit index = indexes.begin();
			index != indexes.end();
			index++)
		{
			if (is_file(root + (*index)))
				return (index);
		}
		return (indexes.end());
	}
}
