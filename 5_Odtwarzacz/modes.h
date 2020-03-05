#include "piece.h"
#ifndef MODES_H_
#define MODES_H_
#include <deque>

class SequenceMode : public Mode {
public:
    void play(const std::deque<std::shared_ptr<Piece>>& playlist) const override;
};

class ShuffleMode : public Mode {
    int seed;
public:
    explicit ShuffleMode(int seed_);
    void play(const std::deque<std::shared_ptr<Piece>>& playlist) const override;
};

class OddEvenMode : public Mode {
public:
    void play(const std::deque<std::shared_ptr<Piece>>& playlist) const override;
};

std::shared_ptr<Mode> createSequenceMode();

std::shared_ptr<Mode> createShuffleMode(int seed);

std::shared_ptr<Mode> createOddEvenMode();

#endif /* MODES_H_ */
