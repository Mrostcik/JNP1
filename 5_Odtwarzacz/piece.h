#ifndef PIECE_H_
#define PIECE_H_
#include <string>
#include <map>
#include <deque>
#include <memory>
#include <iostream>
#include <regex>

class Piece {
private:
    // We make Playlist a friend of Piece in order to allow it to access the protected
    // causesCycle() method from instances of other Piece subclasses.
    friend class Playlist;
protected:
    virtual bool causesCycle() const;
public:
    virtual ~Piece() = default;
    virtual void play() const = 0;
};

class Mode {
public:
    virtual void play(const std::deque<std::shared_ptr<Piece>>& playlist) const = 0;
    virtual ~Mode() = default;
};

class Playlist : public Piece {
private:
    std::deque<std::shared_ptr<Piece>> pieces;
    std::shared_ptr<Mode> mode;
    std::string name;
    bool cycleHelper;

    bool checkIfAddedCausesCycle(const std::shared_ptr<Piece>& element);
protected:
    bool causesCycle() const override;
public:
    explicit Playlist(std::string name_);
    void add(const std::shared_ptr<Piece>& element);
    void add(const std::shared_ptr<Piece>& element, int position);
    void remove();
    void remove(int position);
    void setMode(std::shared_ptr<Mode> mode);
    void play() const override;
};

class Work : public Piece {
private:
    void checkContent();
protected:
    std::string title;
    std::string content;
public:
    explicit Work(const std::map<std::string, std::string>& data);
};

class Song : public Work {
private:
    std::string artist;
public:
    explicit Song(const std::map<std::string, std::string>& data);
    void play() const override;
};

class Movie : public Work {
private:
    std::string year;

    void checkYear();
    void rot13();
public:
    explicit Movie(const std::map<std::string, std::string>& data);
    void play() const override;
};

#endif /* PIECE_H_ */
