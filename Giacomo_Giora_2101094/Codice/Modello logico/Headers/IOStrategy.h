#ifndef IOSTRATEGY_H
#define IOSTRATEGY_H

#include <string>
using std::string;

class Biblioteca; // Forward declaration della classe Biblioteca

class IOStrategy {
public:
    virtual ~IOStrategy() = default;
    virtual bool salvaSuFile(const Biblioteca& biblio, const string& filePath) const = 0;
    virtual bool caricaDaFile(Biblioteca& biblio, const string& filePath) = 0;
};

#endif // IOSTRATEGY_H