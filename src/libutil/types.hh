#pragma once

#include "config.h"

#include <string>
#include <list>
#include <set>

#include <boost/format.hpp>


namespace nix {


/* Inherit some names from other namespaces for convenience. */
using std::string;
using std::list;
using std::set;
using std::vector;
using boost::format;


struct FormatOrString
{
    string s;
    FormatOrString(const string & s) : s(s) { };
    FormatOrString(const format & f) : s(f.str()) { };
    FormatOrString(const char * s) : s(s) { };
};


/* BaseError should generally not be caught, as it has Interrupted as
   a subclass. Catch Error instead. */
class BaseError : public std::exception
{
protected:
    string prefix_; // used for location traces etc.
    string err;
public:
    unsigned int status; // exit status
    BaseError(const FormatOrString & fs, unsigned int status = 1);
    ~BaseError() throw () { };
    const char * what() const throw () { return err.c_str(); }
    const string & msg() const throw () { return err; }
    const string & prefix() const throw () { return prefix_; }
    BaseError & addPrefix(const FormatOrString & fs);
};

#define MakeError(newClass, superClass) \
    class newClass : public superClass                  \
    {                                                   \
    public:                                             \
        newClass(const FormatOrString & fs, unsigned int status = 1) : superClass(fs, status) { }; \
    };

MakeError(Error, BaseError)

class SysError : public Error
{
public:
    int errNo;
    SysError(const FormatOrString & fs);
};


typedef list<string> Strings;
typedef set<string> StringSet;


/* Paths are just strings. */
typedef string Path;
typedef list<Path> Paths;
typedef set<Path> PathSet;


typedef enum {
    lvlError = 0,
    lvlInfo,
    lvlTalkative,
    lvlChatty,
    lvlDebug,
    lvlVomit
} Verbosity;


}
