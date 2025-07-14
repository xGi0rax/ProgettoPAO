#ifndef JSONIO_H
#define JSONIO_H

#include "IOStrategy.h"
#include <QJsonObject>
#include <string>
using std::string;

class Media; // Forward declaration della classe Media

class JsonIO : public IOStrategy {
public:
    JsonIO() = default;
    ~JsonIO() override = default;

    // Metodi per salvare e caricare la biblioteca su e da file JSON
    bool salvaSuFile(const Biblioteca& biblio, const string& filePath) const override;
    bool caricaDaFile(Biblioteca& biblio, const string& filePath) override;

    QJsonObject mediaToJson(const Media* media) const;  // Converte un Media in QJsonObject
    Media* jsonToMedia(const QJsonObject& json) const;  // Crea un Media da QJsonObject
};

#endif // JSONIO_H