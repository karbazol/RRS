#include    "filesystem.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void FileSystem::setRouteRootDir(const std::string &path)
{
    routeRootDir = getNativePath(path);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void FileSystem::setConfigDir(const std::string &path)
{
    configDir = getNativePath(path);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void FileSystem::setLogsDir(const std::string &path)
{
    logsDir = getNativePath(path);
}

void FileSystem::setLibraryDir(const std::string &path)
{
    libraryDir = getNativePath(path);
}

void FileSystem::setTrainsDir(const std::string &path)
{
    trainsDir = getNativePath(path);
}

void FileSystem::setModulesDir(const std::string &path)
{
    modulesDir = getNativePath(path);
}

void FileSystem::setVehiclesDir(const std::string &path)
{
    vehiclesDir = getNativePath(path);
}

void FileSystem::setCouplingsDir(const std::string &path)
{
    couplingsDir = getNativePath(path);
}

void FileSystem::setBinaryDir(const std::string &path)
{
    binDir = getNativePath(path);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getRouteRootDir() const
{
    return routeRootDir;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getConfigDir() const
{
    return configDir;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getLogsDir() const
{
    return logsDir;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getLibraryDir() const
{
    return libraryDir;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getTrainsDir() const
{
    return trainsDir;
}

std::string FileSystem::getModulesDir() const
{
    return modulesDir;
}

std::string FileSystem::getVehiclesDir() const
{
    return vehiclesDir;
}

std::string FileSystem::getCouplingsDir() const
{
    return couplingsDir;
}

std::string FileSystem::getBinaryDir() const
{
    return binDir;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::combinePath(const std::string &path1, const std::string &path2)
{
    if (*(path1.end() - 1) != separator())
        return getNativePath(path1 + separator() + path2);
    else
        return getNativePath(path1 + path2);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getNativePath(const std::string &path)
{
    return osgDB::convertFileNameToNativeStyle(path);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
char FileSystem::separator() const
{
    return osgDB::getNativePathSeparator();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
std::string FileSystem::getLevelUpDirectory(std::string path, int num_levels)
{
    std::vector<std::string> path_elems;
    osgDB::getPathElements(path, path_elems);

 #if __unix__
    std::string tmp = "/";
#else
    std::string tmp = "";
#endif

    for (auto it = path_elems.begin(); it != path_elems.end() - num_levels; ++it)
    {
        tmp += (*it) + separator();
    }

    return tmp;
}
