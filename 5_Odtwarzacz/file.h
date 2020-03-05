#ifndef FILE_H_
#define FILE_H_

#include<vector>

class File {
    std::vector<std::string> data;
public:
    explicit File(std::string file);
    std::vector<std::string>& getData();
};

#endif /* FILE_H_ */
