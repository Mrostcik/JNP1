#include <algorithm>
#include <random>
#include <iostream>
#include "modes.h"
#include "exceptions.h"

void SequenceMode::play(const std::deque<std::shared_ptr<Piece>>& playlist) const {
    for (auto & piece : playlist)
        piece->play();
}

ShuffleMode::ShuffleMode(int seed_) : seed(seed_) {}

void ShuffleMode::play(const std::deque<std::shared_ptr<Piece>>& playlist) const {
    std::deque<std::shared_ptr<Piece>> playlistCopy(playlist);
    auto rng = std::default_random_engine(seed);
    std::shuffle(playlistCopy.begin(), playlistCopy.end(), rng);

    for (auto & piece : playlistCopy)
        piece->play();
}

void OddEvenMode::play(const std::deque<std::shared_ptr<Piece>>& playlist) const {
    for (size_t i = 1; i < playlist.size(); i+=2)
        playlist[i]->play();

    for (size_t i = 0; i < playlist.size(); i+=2)
        playlist[i]->play();
}

std::shared_ptr<Mode> createSequenceMode() {
    return std::make_shared<SequenceMode>();
}

std::shared_ptr<Mode> createShuffleMode(int seed) {
    return std::make_shared<ShuffleMode>(seed);
}

std::shared_ptr<Mode> createOddEvenMode() {
    return std::make_shared<OddEvenMode>();
}
