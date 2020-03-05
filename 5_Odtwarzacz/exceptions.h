#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <exception>

class PlayerException : public std::exception {
public:
    const char * what() const noexcept override {
        return "PlayerException";
    }
};

class UnsupportedTypeException : public PlayerException {
public:
    const char * what() const noexcept override {
        return "unsupported type";
    }
};

class CorruptedFileException : public PlayerException {
public:
    const char * what() const noexcept override {
        return "corrupt file";
    }
};

class CorruptedContentException : public PlayerException {
public:
    const char * what() const noexcept override {
        return "corrupt content";
    }
};

class CycleException : public PlayerException {
public:
    const char * what() const noexcept override {
        return "cycled playlist";
    }
};

#endif /* EXCEPTIONS_H_ */
