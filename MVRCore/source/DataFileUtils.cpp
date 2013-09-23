//========================================================================
// MinVR
// Platform:    Any
// API version: 1.0
//------------------------------------------------------------------------
// Copyright (c) 2013 Regents of the University of Minnesota
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
// * Neither the name of the University of Minnesota, nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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