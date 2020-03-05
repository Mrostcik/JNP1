#include "lib_playlist.h"

std::shared_ptr<Piece> Player::openFile(File file) {
    std::vector<std::string> data = file.getData();
    std::map<std::string, std::string> map;
    std::string type = "";
    for (unsigned i = 0; i < data.size(); i++) {
        if (i == 0)
            type = data[i];
        else if (i == data.size() - 1) {
            map["content"] = data[i];
        }
        else {
            std::string delimiter = ":";

            size_t pos = 0;
            std::string token;
            pos = data[i].find(delimiter);
            token = data[i].substr(0, pos);
            data[i].erase(0, pos + delimiter.length());
            map[token] = data[i];
        }
    }

    if (type == "audio") {
        return std::make_shared<Song>(Song(map));
    }
    else if (type == "video") {
        return std::make_shared<Movie>(Movie(map));
    }
    else if (map.count("content") > 0) {
        throw UnsupportedTypeException();
    }
    else {
        // Based on the attached usage example, no matter if the type is unsupported,
        // we want to treat it as a corrupt file if it has no content.
        throw CorruptedFileException();
    }
}

std::shared_ptr<Playlist> Player::createPlaylist(const std::string& name) {
    return std::make_shared<Playlist>(name);
}
