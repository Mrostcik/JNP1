#include <utility>
#include <exception>
#include <iostream>
#include <regex>

#include "piece.h"
#include "exceptions.h"
#include "modes.h"

// A non-Playlist Piece will never cause a cycle.
bool Piece::causesCycle() const {
    return false;
}

void Work::checkContent() {
    static const std::regex validChars{"^[[:alnum:]\\s,.!?':;\\-]*$"};

    if (!std::regex_match(content, validChars))
        throw CorruptedContentException();
}

Work::Work(const std::map<std::string, std::string>& data)
try : title(data.at("title")), content(data.at("content")) {
    checkContent();
}
catch (std::out_of_range& e) { // When no 'title' metadata or content were given by the user
    throw CorruptedFileException();
}

Song::Song(const std::map<std::string, std::string>& data)
try : Work(data), artist(data.at("artist")) {}
catch (std::out_of_range& e) { // When no 'artist' metadata was given by the user (or exception from Work)
    throw CorruptedFileException();
}

void Song::play() const {
    std::cout<<"Song ["<<artist<<", "<<title<<"]: "<<content<<std::endl;
}

Movie::Movie(const std::map<std::string, std::string>& data)
try : Work(data), year(data.at("year")) {
    checkYear();
    rot13();
}
catch (std::out_of_range& e) { // When no 'year' metadata was given by the user (or exception from Work)
    throw CorruptedFileException();
}

void Movie::play() const {
    std::cout<<"Movie ["<<title<<", "<<year<<"]: "<<content<<std::endl;
}

void Movie::checkYear() {
    static const std::regex validYear{"^$|^[1-9][0-9]*$"};

    if (!std::regex_match(year, validYear))
        throw CorruptedFileException();
}

void Movie::rot13() {
    for (char & c : content) {
        if(c >= 65 && c <= 90) {
            c = (c + 13) < 91 ? c+13 : (c+13)%91 + 65;
        }
        else if (c >= 97 && c <= 122) {
            c = (c + 13 < 123) ? c+13 : (c + 13)%123 + 97;
        }
    }
}

Playlist::Playlist(std::string name_) : pieces(), mode(createSequenceMode()),
                                        name(std::move(name_)), cycleHelper(false) {}

bool Playlist::causesCycle() const {
    if (cycleHelper)
        return true;

    for (auto & piece : pieces) {
        if (piece->causesCycle())
            return true;
    }

    return false;
}

bool Playlist::checkIfAddedCausesCycle(const std::shared_ptr<Piece>& element) {
    bool causes;

    cycleHelper = true;
    causes = element->causesCycle();
    cycleHelper = false;

    return causes;
}

// Pass the ptr by const reference because it is copied (and thus the ownership
// counter is increased) in push_back anyway.
void Playlist::add(const std::shared_ptr<Piece>& element) {
    // We prohibit the user from adding a playlist that (directly or indirectly)
    // contains the playlist to which it is being added (or is the playlist itself).
    // This is because having such cycle in the playlist would cause it to play infinitely.
    if (checkIfAddedCausesCycle(element)) {
        throw CycleException();
    }

    pieces.push_back(element);
}

void Playlist::add(const std::shared_ptr<Piece>& element, int position) {
    if (checkIfAddedCausesCycle(element)) {
        throw CycleException();
    }

    pieces.insert(pieces.begin() + position, element);
}

void Playlist::remove() {
    pieces.pop_back();
}

void Playlist::remove(int position) {
    pieces.erase(pieces.begin() + position);
}

void Playlist::setMode(std::shared_ptr<Mode> mode_) {
    mode = std::move(mode_);
}

void Playlist::play() const {
    std::cout<<"Playlist ["<<name<<"]"<<std::endl;
    mode->play(pieces);
}
