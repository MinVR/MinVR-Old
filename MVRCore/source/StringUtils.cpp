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

#include "MVRCore/StringUtils.H"

#include <sstream>

using namespace std;

namespace MinVR {

std::ostream & operator<< ( std::ostream &os, const glm::vec2 &vec2) {
	// format:  (x, y)
	return os << "("<<vec2.x<<", "<<vec2.y<<")";
}

std::istream & operator>> ( std::istream &is, glm::vec2 &vec2) {
	// format:  (x, y)
	char dummy;
	return is >> dummy >> vec2.x >> dummy >> vec2.y >> dummy;
}

std::ostream & operator<< ( std::ostream &os, const glm::vec3 &vec3) {
	// format:  (x, y, z)
	return os << "("<<vec3.x<<", "<<vec3.y<<", "<<vec3.z<<")";
}


std::istream & operator>> ( std::istream &is, glm::vec3 &vec3) {
	// format:  (x, y, z)
	char dummy;
	return is >> dummy >> vec3.x >> dummy >> vec3.y >> dummy >> vec3.z >> dummy;
}

std::ostream & operator<< ( std::ostream &os, const glm::vec4 &vec4) {
	// format:  (x, y, z, w)
	return os << "("<<vec4.x<<", "<<vec4.y<<", "<<vec4.z<<", "<<vec4.w<<")";
}


std::istream & operator>> ( std::istream &is, glm::vec4 &vec4) {
	// format:  (x, y, z, w)
	char dummy;
	return is >> dummy >> vec4.x >> dummy >> vec4.y >> dummy >> vec4.z >> dummy >> vec4.w >> dummy;
}

std::ostream & operator<< ( std::ostream &os, const glm::mat3 &m) {
	// format:  ((r1c1, r1c2, r1c3), (r2c1, r2c2, r2c3), (r3c1, r3c2, r3c3))
	// glm is column major
	return os << "((" << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << "), "
		<< "(" << m[0][1] << ", " << m[1][1] << ", " << m[2][1] << "), "
		<< "(" << m[0][2] << ", " << m[1][2] << ", " << m[2][2] << "))";
}

std::istream & operator>> ( std::istream &is, glm::mat3&m) {
	// format:  ((r1c1, r1c2, r1c3), (r2c1, r2c2, r2c3), (r3c1, r3c2, r3c3))
	// glm is column major
	char c;
	return is >> c >> c >> m[0][0] >> c >> m[1][0] >> c >> m[2][0] >> c >> c
		>> c >> m[0][1] >> c >> m[1][1] >> c >> m[2][1] >> c >> c
		>> c >> m[0][2] >> c >> m[1][2] >> c >> m[2][2] >> c >> c;
}

std::ostream & operator<< ( std::ostream &os, const glm::mat4 &m) {
	// format:  ((r1c1, r1c2, r1c3, r1c4), (r2c1, r2c2, r2c3, r2c4), etc.. )
	// glm is column major
	return os << "((" << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "), "
		<< "(" << m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "), "
		<< "(" << m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "), "
		<< "(" << m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "))";
}

std::istream & operator>> ( std::istream &is, glm::mat4 &m) {
	// format:  ((r1c1, r1c2, r1c3, r1c4), (r2c1, r2c2, r2c3, r2c4), etc.. ) 
	char c;
	return is >> c >> c >> m[0][0] >> c >> m[1][0] >> c >> m[2][0] >> c >> m[3][0] >> c >> c
		>> c >> m[0][1] >> c >> m[1][1] >> c >> m[2][1] >> c >> m[3][1] >> c >> c
		>> c >> m[0][2] >> c >> m[1][2] >> c >> m[2][2] >> c >> m[3][2] >> c >> c
		>> c >> m[0][3] >> c >> m[1][3] >> c >> m[2][3] >> c >> m[3][3] >> c >> c;
}

int iMinNonNeg(int i1, int i2) 
{
	if (i1 < 0)
		i1 = std::numeric_limits<int>::max();
	if (i2 < 0)
		i2 = std::numeric_limits<int>::max();

	if (i1 < i2)
		return i1;
	else
		return i2;
}

std::string trimWhitespace(const std::string& s)
{
	if (s.length() == 0) {
        return s;
    }
    size_t left = 0;
    
    // Trim from left
    while ((left < s.length()) && iswspace(s[left])) {
        ++left;
    }

    size_t right = s.length() - 1;
    // Trim from right
    while ((right > left) && iswspace(s[right])) {
        --right;
    }
    
    return s.substr(left, right - left + 1);
}

bool popNextToken(std::string &in, std::string &token, bool returnFalseOnSemiColon)
{
	in = trimWhitespace(in);

	// if no more tokens, return false
	if (in.size() == 0) {
		return false;
	}
	else if ((in[0] == ';') && (returnFalseOnSemiColon)) {
		in = in.substr(1);
		return false;
	}

	int end = in.size();
	end = iMinNonNeg(end, in.find(" "));
	end = iMinNonNeg(end, in.find('\t'));
	end = iMinNonNeg(end, in.find(";"));
	end = iMinNonNeg(end, in.find(","));
	end = iMinNonNeg(end, in.find('\n'));
	end = iMinNonNeg(end, in.find('\r'));

	token = in.substr(0,end);
	in = in.substr(end);
	return (token.size() > 0);
}

std::vector<std::string> splitStringIntoArray(const std::string &in)
{
	std::vector<std::string> a;
	std::string s = in;
	std::string token;
	while (popNextToken(s, token, false)) {
		a.push_back(token);
	}
	return a;
}

std::string decygifyPath(const std::string &in)
{
#ifdef WIN32
	std::string input = in;
	size_t startofcyg = input.find("/cygdrive/");
	if (startofcyg != std::string::npos) {
		std::string drive = input.substr(startofcyg + 10, 1);
		std::string newpath = input.substr(0,startofcyg) + drive + std::string(":") + input.substr(startofcyg + 11);
		// recursive call
		return decygifyPath(newpath);
	}
	else {
		return input;
	}
#else
	return in;
#endif
}


std::string	replaceEnvVars(const std::string &in)
{
	// the special sequence $(NAME) gets replaced by the decygified value 
	// of the environment variable NAME

	std::string instr = in;
#ifdef LINUX
	int evstart = instr.find("$(");
#else
	int evstart = instr.find("\$(");
#endif
	while (evstart >= 0) {
		std::string evandrest = instr.substr(evstart+2);
		int evend = evandrest.find(")");
		std::string ev = evandrest.substr(0,evend);
		std::string evval = getenv( ev.c_str() );
		evval = decygifyPath(evval);
		instr = instr.substr(0,evstart) + evval + evandrest.substr(evend+1);
#ifdef LINUX
		evstart = instr.find("$(");
#else
		evstart = instr.find("\$(");
#endif
	}
	return instr;
}

std::string	intToString(int i)
{
	std::ostringstream ostr;
	// if using strstream rather than stringstream, the following call
	// requires a   << "\0"   at the end.
	ostr << i;  
	return std::string(ostr.str());
}

int	stringToInt(const std::string &in)
{
	int i;
	std::istringstream istr(in.c_str());
	istr >> i;
	return i;
}

std::string	realToString(double r)
{
	std::ostringstream ostr;
	// if using strstream rather than stringstream, the following call
	// requires a   << "\0"   at the end.
	ostr << r;  
	return std::string(ostr.str());
}

double stringToReal(const std::string &in)
{
	double r;
	std::istringstream istr(in.c_str());
	istr >> r;
	return r;
}

std::string	joinIntoString( const std::vector<std::string>& in, const std::string& delimiter )
{
	std::string s;
	if ( in.size() ) {
		s = in[0];
		for ( int i = 1; i < in.size(); i++ )
			s = s + delimiter + in[i];
	}
	return s;
}



std::vector< std::vector< std::string > > readDelimitedData(const std::string &csvString, const std::string &delimiter,	bool removeQuotes)
{
	std::vector< std::vector< std::string> > data;
	std::string csv;

	// first remove '\r' characters to make DOS/Windows style returns
	// look like unix
	for (int i=0;i<csvString.length();i++) {
		if (csvString[i] != '\r')
			csv = csv + csvString[i];
	}

	bool done=false;
	while ((!done) && (csv.length())) {
		int nl = csv.find("\n");
		int l = csv.length();
		int endofline = iMinNonNeg(nl, l);
		if (endofline > 0) {
			std::string line = csv.substr(0,endofline);
			std::vector<std::string> dataLine;

			while (line.length() > 0) {
				int end = iMinNonNeg(line.find(delimiter), line.length());
				std::string item = line.substr(0,end);
				if ((removeQuotes) && (item[0] == '"') && (item[item.length()-1] == '"'))
					item = item.substr(1,item.length()-2);
				dataLine.push_back(item);
				if (end+1 >= line.length()) {
					if (line[line.length()-1] == ',')
						dataLine.push_back("");
					line = "";
				}
				else
					line = line.substr(end+1);
			}

			if (dataLine.size())
				data.push_back(dataLine);

			csv = csv.substr(endofline+1);
		}
		else
			done = true;
	}

	return data;
}


bool popUntilSemicolon(std::string &in, std::string &popped)
{
	int semi = in.find(";");
	if (semi == in.npos)
		return false;
	else {
		popped = in.substr(0,semi);
		in = in.substr(semi+1);
		return true;
	}
}

std::string	convertNewlinesAndTabsToSpaces(std::string input) 
{
	int i = 0;
	while (i < (int)input.size()) {
		if ((input[i] == '\r') || (input[i] == '\n') || (input[i] == '\t'))
			input[i] = ' ';
		i++;
	}
	return input;
}


std::string spacesString(int num)
{
	std::string s;
	for (int i=0;i<num;i++)
		s = s + " ";
	return s;
}


int	numSubstringOccurances(const std::string &str, const std::string &substr)
{
	int n = 0;
	std::string s = str;
	while (s.find(substr) != s.npos) {
		int start = s.find(substr);
		s = s.substr(start+1);
		n++;
	}
	return n;
}


int	findNth(const std::string &str, const std::string &substr, const int n)
{
	std::string s = str;
	int nthIndex = 0;
	for (int i=0;i<n;i++) {
		int index = s.find(substr);
		if (index == s.npos)
			return s.npos;
		s = s.substr(index+1);
		if (i == 0)
			nthIndex = index;
		else
			nthIndex += index+1;
	}
	return nthIndex;
}

std::string readWholeFile(const std::string& filename)
{
	string instr;
	if (boost::filesystem::exists(filename))
	{
		boost::filesystem::ifstream fIn;
		fIn.open(filename.c_str(),std::ios::in);

		if (!fIn) {
			BOOST_ASSERT_MSG(false, "Error: Unable to load file");
		}
		std::stringstream ss;
		ss << fIn.rdbuf();
    
		instr =  ss.str();
	}
	else
	{  
		std::stringstream ss;
		ss << "Cannot locate file " << filename;
		BOOST_ASSERT_MSG(false, ss.str().c_str());
	}
	return instr;
}

/*
bool getXMLField(const std::string &input, const std::string &fieldName,
	Table<std::string, std::string> &propertiesAndValues,
	std::string &fieldData,
	std::string &leftoverInput)
{
	// Clear out the table of values 
	propertiesAndValues.clear();

	// Look for the beginning of the expected field
	int startfield = input.find("<" + fieldName);
	if (startfield == input.npos) {
		leftoverInput = input;
		return false;
	}

	// Get the substring that contains all the attributes of the field
	std::string propplus = input.substr(startfield + fieldName.length() + 1);
	int endofnameandproperties = propplus.find(">");
	std::string properties = propplus.substr(0,endofnameandproperties);


	while ((properties.size()) && (properties.find("=") != properties.npos)) {
		properties = trimWhitespace(properties);
		int equals = properties.find("=");
		std::string name = properties.substr(0,equals);
		bool quoted = false;
		if (properties[equals+1] == '"') {
			quoted = true;
			equals++;
		}
		std::string valplus = properties.substr(equals+1);
		int end = 0;
		if (quoted)
			end = valplus.find("\"");
		else
			end = valplus.find(" ");
		std::string value = valplus.substr(0,end);
		propertiesAndValues.set(name,value);
		properties = valplus.substr(end+1);
	}

	// If there is no data, then some XML writers will end the field
	// with a /> rather than the conventional way
	if ((endofnameandproperties-1 >= 0) && (propplus[endofnameandproperties-1] == '/')) {
		leftoverInput = propplus.substr(endofnameandproperties + 1);
	}
	else {

		// Get all the xml text after the end of the field begin carrot
		// symbols everything from here to the matching </myXmlField> is
		// stored in the field data.  This is a little tricky because there
		// may be a subfield with the same fieldName *inside* the field
		// data, so we need to make sure we have the right number of xml
		// field start tokens and field end tokens.
		std::string dataplus = propplus.substr(endofnameandproperties + 1);

		int numstart = 1;
		int numend = 0;
		int endofdata;
		while (numend != numstart) {
			endofdata = findNth(dataplus, "</" + fieldName + ">", numend+1);
			if (endofdata == dataplus.npos) {
				std::cout << "getXMLField error: Can't find the " << numend+1 <<
					"-th occurance of </" << fieldName << "> in " << dataplus << std::endl;
				return false;
			}
			//debugAssert(endofdata != dataplus.npos);
			numend++;
			fieldData = dataplus.substr(0, endofdata);
			numstart = numSubstringOccurances(fieldData, "<" + fieldName + ">") +
				numSubstringOccurances(fieldData, "<" + fieldName + " ") + 1;
		}

		leftoverInput = dataplus.substr(endofdata+1);
	}

	return true;
}



std::string  
	writeXMLField(const std::string &fieldName,
	const Table<std::string, std::string> &propertiesAndValues,
	const std::string &fieldData)
{
	std::string s;
	s = "<" + fieldName;
	Array<std::string> keys = propertiesAndValues.getKeys();
	for (int i=0;i<keys.size();i++) {
		s = s + " " + keys[i] + "=\"" + propertiesAndValues[keys[i]] + "\"";
	}
	s = s + ">" + fieldData + "</" + fieldName + ">\n";
	return s;
}


char**
	stringArrayTo2DCharArray(const Array<std::string> &a)
{
	int n = a.size();

	if (n == 0)
		return NULL;

	std::cout << "n=" << n << std::endl;

	char **ptr;
	ptr = new char * [n];
	for (int i=0;i<n;i++) {
		//ptr[i] = new char[strlen(a[i].c_str())+1];
		ptr[i] = new char[1024];
		strcpy(ptr[i], a[i].c_str());
		std::cout << i << " " << ptr[i] << std::endl;
	}
	return ptr;
}


void delete2DCharArray(char **ptr, int size)
{
	for (int i=0;i<size;i++)
		delete [] ptr[i];
	delete [] ptr;
}

*/

} // end namespace

