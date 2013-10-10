#include <math.h>
#include <sstream>
#include <windows.h>
#include <deque>

class Error {
private:
	static std::deque <std::string> *errors;
	static Error *_self;

	Error() {};
	~Error() {};

public:
	static void throwError(std::string error) {
		if (_self == NULL) 
			_self = new Error();
		errors->push_back(error);
	}
	static void clearErrors() {
		errors->clear();
	}
	static void showErrors() {
		std::string err;
		std::deque <std::string> ::iterator p_str = errors->begin();
		int i = 0;
		while (p_str != errors->end()) {
			err+=Convert(i);
			err+=". ";
			err+=*p_str;
			err+="\n";
			p_str++;
			i++;
		}
		if (err == "") 
			err = "Empty";
		MessageBox(NULL, err.c_str(), "Errors", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}
};
Error *Error::_self = NULL;
std::deque <std::string> *Error::errors = new std::deque <std::string>();

