//========================================================================
// MinVR
// Platform:    Any
// API version: 1.0
//------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2013 Regents of the University of Minnesota
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//========================================================================

#include "MVRCore/DataFileUtils.H"

using namespace G3DLite;

namespace MinVR
{

static DataFileUtils* common = nullptr;

DataFileUtils& DataFileUtils::instance()
{
	init();
	return *common;
}

void DataFileUtils::init()
{
	if (common == nullptr)
	{
		common = new DataFileUtils();
	}
}

void DataFileUtils::cleanup()
{
    if (common != nullptr) {
        delete common;
        common = nullptr;
    }
}

std::string DataFileUtils::findDataFile(const std::string &filename)
{
	return instance()._findDataFile(filename);
}

void DataFileUtils::addFileSearchPath(const std::string &path)
{
	instance()._addFileSearchPath(path);
}

DataFileUtils::DataFileUtils()
{
	_dataFilePaths.append("");
	_dataFilePaths.append("share/");
	_dataFilePaths.append("share/vrsetup/");
	_dataFilePaths.append("share/shaders/");
	_dataFilePaths.append("../../share/");
	_dataFilePaths.append("../../share/vrsetup/");
	_dataFilePaths.append("../../share/shaders/");
	_dataFilePaths.append(INSTALLPATH);
	_dataFilePaths.append(FilePath::concat(INSTALLPATH, "share/"));
	_dataFilePaths.append(FilePath::concat(INSTALLPATH, "share/vrsetup"));
	_dataFilePaths.append(FilePath::concat(INSTALLPATH, "share/shaders"));
}

std::string DataFileUtils::_findDataFile(const std::string &filename)
{
	for (int i = 0; i < _dataFilePaths.size(); i++)	{ 
		std::string fname = FilePath::concat(_dataFilePaths[i], filename);
		if (FileSystem::exists(fname)) {
			return fname;
		}
	}

	std::cout << "Could not find data file as either:" << std::endl;

	for (int i = 0; i < _dataFilePaths.size(); i++) {  
		std::string fname = FilePath::concat(_dataFilePaths[i], filename);
		std::cout << i << ". " << fname << std::endl;
	}

	return "";
}

void DataFileUtils::_addFileSearchPath(const std::string &path)
{
	// Add to the front so that user added paths get searched first
	_dataFilePaths.insert(0, replaceEnvVars(path));
}

} // end namespace