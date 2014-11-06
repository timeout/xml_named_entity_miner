#include "dir.hpp"
#include "bilanz.hpp"

#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cerrno>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <regex>

static auto getWorkingPath( std::string &path ) -> bool;
static auto changeDirectory( const std::string &path ) -> void;

Dir::Dir( ) : pathname_{}, entries_{}, dirty_{false}, read_{false} {
    std::string path;
    if ( !getWorkingPath( path ) ) {
        throw std::runtime_error( "Could not get current directory" );
    }
    pathname_ = path;
}

auto Dir::exists( const Pathname &pathname ) -> bool {
    struct stat sb;
    if ( fstatat( AT_FDCWD, pathname.toString( ).c_str( ), &sb, 0 ) == 0 ) { // success
        if ( S_ISDIR( sb.st_mode ) )
            return true;
    }
    return false;
}

auto Dir::mk( const Pathname &path ) -> void {}

auto Dir::rm( const Pathname &path ) -> void {}

auto Dir::pwd( ) -> Pathname { return pathname_; }

auto Dir::chdir( const Pathname &pathname ) -> Dir & {
    changeDirectory( pathname.toString( ).c_str( ) );
    std::string path;
    if ( !getWorkingPath( path ) ) {
        throw std::runtime_error( "Could not get current directory" );
    }
    pathname_ = path;
    dirty_ = true;
    read_ = false;
    return *this;
}

auto Dir::read( ) -> Dir & {
    if ( dirty_ ) {
        entries_.clear( );
    }
    if ( !read_ ) {
        DIR *dir;
        dir = opendir( pathname_.toString( ).c_str( ) );
        if ( dir == nullptr ) {
            // TODO: errno
            throw std::runtime_error( "Could not open directory" );
        }
        class dirent *entry;
        class stat sb;
        while ( ( entry = readdir( dir ) ) != nullptr ) {
            if ( ( std::strcmp( entry->d_name, "." ) != 0 ) &&
                 ( std::strcmp( entry->d_name, ".." ) != 0 ) &&
                 ( lstat( entry->d_name, &sb ) == 0 ) && ( !S_ISDIR( sb.st_mode ) ) ) {
                entries_.push_back( pathname_ + Pathname{entry->d_name} ); //
            }
        }
        closedir( dir );
        read_ = true;
    }
    return *this;
}

auto Dir::entries( ) const -> const std::vector<Pathname> & { return entries_; }

auto Dir::filter( const std::regex &glob ) const -> std::vector<Pathname> {
    std::vector<Pathname> result;
    std::copy_if( std::begin( entries_ ), std::end( entries_ ),
                  std::back_inserter( result ), [&]( const Pathname &pathname ) {
        return std::regex_match( pathname.toString( ), glob );
    } );
    return result;
}

static auto getWorkingPath( std::string &path ) -> bool {
    typedef std::pair<dev_t, ino_t> file_id; // (device number, inode number)
    bool success = false;
    int start_fd = ( open( ".", O_RDONLY ) ); // start directory
    if ( start_fd != -1 ) {                   // success
        struct stat sb;
        if ( fstat( start_fd, &sb ) == 0 ) { // success
            file_id current_id{sb.st_dev, sb.st_ino};
            // root directory (destination), success
            if ( stat( "/", &sb ) == 0 ) {
                std::vector<std::string> path_components;
                file_id root_id{sb.st_dev, sb.st_ino};

                while ( current_id != root_id ) {
                    bool pushed = false;
                    if ( chdir( ".." ) == 0 ) { // success
                        DIR *dir = opendir( "." );
                        if ( dir != nullptr ) { // success
                            dirent *entry;
                            while ( ( entry = readdir( dir ) ) != nullptr ) { // success
                                // not a "." or a ".."
                                if ( ( std::strcmp( entry->d_name, "." ) != 0 ) &&
                                     ( std::strcmp( entry->d_name, ".." ) != 0 ) &&
                                     ( lstat( entry->d_name, &sb ) == 0 ) ) {
                                    file_id child_id{sb.st_dev, sb.st_ino};
                                    // we found where we came from
                                    if ( child_id == current_id ) {
                                        path_components.push_back( entry->d_name );
                                        pushed = true;
                                        break;
                                    }
                                }
                            } // break
                            closedir( dir );

                            if ( pushed && ( stat( ".", &sb ) == 0 ) ) {
                                current_id = file_id{sb.st_dev, sb.st_ino};
                            }
                        } // can't read information at this level
                    }
                    if ( !pushed ) {
                        break;
                    }
                }
                if ( current_id == root_id ) { // success
                    // build the path
                    path = "/";
                    for ( std::vector<std::string>::reverse_iterator i =
                              path_components.rbegin( );
                          i != path_components.rend( ); ++i ) {
                        path += *i + "/";
                    }
                    success = true;
                }
                fchdir( start_fd );
            }
        }
        close( start_fd );
    }
    return success;
}

static auto changeDirectory( const std::string &path ) -> void {

    if ( chdir( path.c_str( ) ) == 0 ) { // success
        return;
    }

    int error = errno;
    switch ( error ) {
    case ENOENT:
        throw std::runtime_error( "No such file or directory" );
    case EACCES:
        throw std::runtime_error( "Access denied" );
    case ENOMEM:
        throw std::runtime_error( "Insufficient storage" );
    case EFAULT:
        throw std::runtime_error( "Bad address" );
    case EIO:
        throw std::runtime_error( "I/O error" );
    case ELOOP:
        throw std::runtime_error( "Too many symbolic links encountered" );
    case ENOTDIR:
        throw std::runtime_error( "Not a directory" );
    case ENAMETOOLONG:
        throw std::runtime_error( "File name too long" );
    default:
        std::ostringstream str;
        str << "Unrecognised error " << error;
        throw std::runtime_error( str.str( ) );
    }
}
