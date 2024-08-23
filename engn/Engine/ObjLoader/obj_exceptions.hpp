#ifndef OBJ_EXCEPTIONS_HPP
#define OBJ_EXCEPTIONS_HPP

#include <exception>
#include <string>

class ObjBaseException : std::exception {
private:
	const std::string row_message;
	const std::string full_message;
	int row;
	std::string message;
public:
	ObjBaseException(const int row, const std::string &message);
	const char *what() const noexcept override;
};

class ObjFileOpenException final : public ObjBaseException {
private:
	std::string path;
public:
	ObjFileOpenException(const int row, const std::string &path);
	const std::string &getPath() const;
};

class FileNotOpenedException final : public ObjBaseException {
public:
	FileNotOpenedException(const int row);
};

class UnderfinedTagException final : public ObjBaseException {
private:
	std::string tagname;
public:
	UnderfinedTagException(const int row, const std::string &tagname);
	const std::string &getTagName() const;
};

class InvalidFaceElement final : public ObjBaseException {
private:
	int index;
public:
	InvalidFaceElement(const int row, const int index);
	const int &getIndex() const;
};

#endif // !OBJ_EXCEPTIONS_HPP