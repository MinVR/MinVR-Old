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
	_dataFilePaths.push_back("");
	_dataFilePaths.push_back("share/");
	_dataFilePaths.push_back("share/vrsetup/");
	_dataFilePaths.push_back("share/shaders/");
	_dataFilePaths.push_back("../../share/");
	_dataFilePaths.push_back("../../share/vrsetup/");
	_dataFilePaths.push_back("../../share/shaders/");
	_dataFilePaths.push_back(INSTALLPATH);
	_dataFilePaths.push_back((boost::filesystem::path(INSTALLPATH) / boost::filesystem::path("share/")).string());
	_dataFilePaths.push_back((boost::filesystem::path(INSTALLPATH) / boost::filesystem::path("share/vrsetup")).string());
	_dataFilePaths.push_back((boost::filesystem::path(INSTALLPATH) / boost::filesystem::path("share/shaders")).string());
}

std::string DataFileUtils::_findDataFile(const std::string &filename)
{
	for (int i = 0; i < _dataFilePaths.size(); i++)	{ 
		std::string fname = (boost::filesystem::path(_dataFilePaths[i]) / boost::filesystem::path(filename)).string();
		if (boost::filesystem::exists(fname)) {
			return fname;
		}
	}

	std::cout << "Could not find data file as either:" << std::endl;

	for (int i = 0; i < _dataFilePaths.size(); i++) {  
		std::string fname = (boost::filesystem::path(_dataFilePaths[i]) / boost::filesystem::path(filename)).string();
		std::cout << i << ". " << fname << std::endl;
	}

	return "";
}

void DataFileUtils::_addFileSearchPath(const std::string &path)
{
	// Add to the front so that user added paths get searched first
	_dataFilePaths.insert(_dataFilePaths.begin(), replaceEnvVars(path));
}

} // end namespace