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


#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <G3DLite/G3DLite.h>

// Can be used to get a quoted version of the value of a particular #define
#define DEFINE_TO_QUOTED_STR( d ) #d

namespace MinVR {


G3DLite::Array<std::string> splitStringIntoArray(const std::string &in);

/** 
 This function changes occurances of /cygdrive/c/folder1/folder2/etc..  
 to c:/folder1/folder2/etc.. when running in Windows.  Paths coming
 from program arguments or environment variables will tend to take
 the /cygdrive/<drive-letter>/ form when running under cygwin, but
 file commands compiled with the MS Visual C++ compiler can't deal
 with paths of this form.
 */
std::string decygifyPath(const std::string &in);


/**
 Given an input string, replaces all occurances of $(NAME) with the
 decygified (see above) value of the environment variable NAME.  If
 NAME is not defined, then a blank string is returned.
 */
std::string replaceEnvVars(const std::string &in);


void growAABox(G3DLite::AABox &box, const G3DLite::Vector3 &point);
void growAABox(G3DLite::AABox &box, const G3DLite::AABox &box2);

G3DLite::Color3 Color3FromUints(G3DLite::uint8 r, G3DLite::uint8 g, G3DLite::uint8 b);

unsigned int hashCode(const double d);

// Vector2
std::ostream & operator<< ( std::ostream &os, const G3DLite::Vector2 &vec2);
std::istream & operator>> ( std::istream &is, G3DLite::Vector2 &vec2);
// Vector3
//std::ostream & operator<< ( std::ostream &os, const Vector3 &vec3);
std::istream & operator>> ( std::istream &is, G3DLite::Vector3 &vec3);
// Matrix3
std::ostream & operator<< ( std::ostream &os, const G3DLite::Matrix3 &m);
std::istream & operator>> ( std::istream &is, G3DLite::Matrix3 &m);
// Matrix4
std::ostream & operator<< ( std::ostream &os, const G3DLite::Matrix4 &m);
std::istream & operator>> ( std::istream &is, G3DLite::Matrix4 &m);
// CoordinateFrame 
// << is already defined somewhere in G3D..
//std::ostream & operator<< ( std::ostream &os, const CoordinateFrame &m);
std::istream & operator>> ( std::istream &is, G3DLite::CoordinateFrame &m);
// Color3
std::ostream & operator<< ( std::ostream &os, const G3DLite::Color3 &c);
std::istream & operator>> ( std::istream &is, G3DLite::Color3 &c);
// Color4
std::ostream & operator<< ( std::ostream &os, const G3DLite::Color4 &c);
std::istream & operator>> ( std::istream &is, G3DLite::Color4 &c);

// etc.. add on as necessary..


std::string matrix4ToString(G3DLite::Matrix4 m);
std::string coordinateFrameToString(G3DLite::CoordinateFrame cf);

std::string  intToString(int i);
int          stringToInt(const std::string &in);

std::string  realToString(double r);
double       stringToReal(const std::string &in);



/** Pops off the next token in the string where a token is separated
   by a space, a tab, a newline, a comma, or a semicolon.  if the last
   parameter is set then this returns false if it is the last item in
   the string OR if there is a semicolon immediately following the
   token, otherwise it returns false when there are no more tokens in
   the string.
**/
bool  popNextToken(std::string &in, std::string &token, 
		   bool returnFalseOnSemiColon = false);


/// pops off the everything in the string until a semicolon is reached.
/// after the call, in is what's left of the string.  returns true if
/// a semicolon was found, false otherwise
bool  popUntilSemicolon(std::string &in, std::string &popped);

/** Reads comma, tab, etc.. separated data into a 2D array of strings.
  csvString should is normally read from a file.  Each entry of a line
  of the file should be separated by the delimiter string.  If
  removeQuotes is true than quotes around entries are removed before
  appending them to the 2D array that is returned.
*/
G3DLite::Array< G3DLite::Array< std::string > >  readDelimitedData(const std::string &csvString,
                                                 const std::string &delimiter,
                                                 bool removeQuotes=true);

/// returns a string of that is the concatanation of all elements in the array
/// separated by the specified delimiter
std::string   joinIntoString(const G3DLite::Array<std::string>& in,
                             const std::string& delimiter=std::string(" ")); 

/// Converts DOS and UNIX style newlines and tab characters to spaces
std::string  convertNewlinesAndTabsToSpaces(std::string input);

/// returns the minimum of i1 and i2 that is also >=0.  if both are 
/// negative, returns a large positive number.
int  iMinNonNeg(int i1, int i2);


/** This function is used to convert from a string type to something
 else.  The new type is inferred using a template and it must define
 the input stream operator >> for this function to work properly.
**/
template <class T>
static inline bool retypeString(const std::string &str, T &val) {
  std::istringstream is(str.c_str());
  is >> val;
  if (!is) return false;
  else return true;
}


/// Returns a string containing num spaces in it.
std::string spacesString(int num);

/// Returns the number of occurances of substr in str.
int numSubstringOccurances(const std::string &str, const std::string &substr);

/// Like std::string.find() but returns the n-th occurance of substr
/// rather than the first one.
int findNth(const std::string &str, const std::string &substr, const int n);

/// This isn't so much a string utility as an Array utility.  You can use
/// this with an Array of strings, or any other Array.
template <class T>
G3DLite::Array<T> insertIntoArray(G3DLite::Array<T> inputArray, T newElement, 
			 int insertBeforeElementNum) 
{
  debugAssert(insertBeforeElementNum >= 0);
  debugAssert(insertBeforeElementNum <= inputArray.size());

  G3DLite::Array<T> anew;
  
  for (int i=0;i<insertBeforeElementNum;i++)
    anew.append(inputArray[i]);
  
  anew.append(newElement);

  for (int i=insertBeforeElementNum;i<inputArray.size();i++)
    anew.append(inputArray[i]);
  
  debugAssert(anew.size() == (inputArray.size() + 1));
  return anew;
}





/////////////  XML PARSING AND WRITING UTILS BELOW  ////////////////

/**  Here's my interpretation of XML for the mini parser and generator
     stuff below..
  <fieldname1 property1="value1" property2="value2">fielddata1, fielddata2
  </fieldname1>
  <fieldname2> <subfield1>blah</subfield1> </fieldname2>
  etc.. 

     getXMLField returns datastructures based on this description.
     Returns false if the fieldName is not found in input.  If there
     is more than one occurance of a field named fieldName in input,
     this returns data from the first one.  leftOverInput is
     everything in input that comes after the end of the field.
*/
bool  
getXMLField(const std::string &input, const std::string &fieldName,
	    G3DLite::Table<std::string, std::string> &propertiesAndValues,
	    std::string &fieldData,
	    std::string &leftoverInput);

/// fielddata can include a call to writeXMLField to make a subfield, this
/// gets formatted as described in the getXMLField() function description.
std::string  
writeXMLField(const std::string &fieldName,
	      const G3DLite::Table<std::string, std::string> &propertiesAndValues,
	      const std::string &fieldData);


char**
stringArrayTo2DCharArray(const G3DLite::Array<std::string> &a);

void
delete2DCharArray(char **ptr, int size);

// int numStringInstancesBeforeOtherString( const std::string& inputString, 
// 					 const std::string& searchString,
//  					 const std::string& stopString );

} // end namespace
#endif
