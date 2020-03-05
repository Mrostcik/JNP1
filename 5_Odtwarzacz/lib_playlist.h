#ifndef PLAYER_H_
#define PLAYER_H_

#include "piece.h"
#include "file.h"
#include "exceptions.h"
#include "modes.h"

class Player{
public:
    static std::shared_ptr<Piece> openFile(File file);
    static std::shared_ptr<Playlist> createPlaylist(const std::string& name);
};

#endif /* PLAYER_H_ */
