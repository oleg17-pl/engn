#include <string>

#include "obj_exceptions.hpp"

ObjBaseException::ObjBaseException(const int row, const std::string &message)
	: row_message{ "Obj error in line: " },
	message{ message },
	row{ row },
	full_message{ row_message + std::to_string(row) + "\n" + message }
{}

const char *ObjBaseException::what() const noexcept {
	return full_message.c_str();
}

ObjFileOpenException::ObjFileOpenException(const int row, const std::string &path)
	: path{ path },
	ObjBaseException{ row, "Failed to open obj file: '" + path + "'" }
{}

const std::string &ObjFileOpenException::getPath() const {
	return path;
}

FileNotOpenedException::FileNotOpenedException(const int row)
	: ObjBaseException{ row, "File not opened!" } 
{}

UnderfinedTagException::UnderfinedTagException(const int row, const std::string &tagname)
	: tagname{ tagname },
	ObjBaseException{ row, "Underfined tag: " + tagname }
{}

const std::string &UnderfinedTagException::getTagName() const {
	return tagname;
}

InvalidFaceElement::InvalidFaceElement(const int row, const int index)
	: index{ index },
	ObjBaseException{ row, "Invalid face element by index: " + std::to_string(index) }
{}

const int &InvalidFaceElement::getIndex() const {
	return index;
}
