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

#ifndef DATAFILEUTILS_H
#define DATAFILEUTILS_H

#include <string>
#include <vector>
#include <iostream>
#include "MVRCore/StringUtils.H"
#include <boost/filesystem.hpp>

namespace MinVR
{

class DataFileUtils
{
public:

	/// Given a filename, this method will search _dataFilePaths to locate it.
	static std::string findDataFile(const std::string &filename);

	/// Adds the path to _dataFilePaths. The special sequence $(NAME) gets replaced by the decygified value of the environment variable NAME
	static void addFileSearchPath(const std::string &path);

	static DataFileUtils& instance();
	static void cleanup();

private:
	/** Don't allow public construction. */
    DataFileUtils();
	~DataFileUtils() {}
    static void init();

	std::string _findDataFile(const std::string &filename);
	void _addFileSearchPath(const std::string &path);

	std::vector<std::string> _dataFilePaths;
};

}
#endif
