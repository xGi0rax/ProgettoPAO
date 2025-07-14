#ifndef XMLIO_H
#define XMLIO_H

#include "IOStrategy.h"
#include "Biblioteca.h"
#include <QDomDocument>
#include <QDomElement>
#include <string>
using std::string;

class Media;

class XmlIO : public IOStrategy {
public:
    XmlIO() = default;
    ~XmlIO() override = default;

    // Metodi per salvare e caricare la biblioteca su e da file XML
    bool salvaSuFile(const Biblioteca& biblio, const string& filePath) const override;
    bool caricaDaFile(Biblioteca& biblio, const string& filePath) override;

    QDomElement mediaToXml(const Media* media, QDomDocument& doc) const;  // Converte un Media in QDomElement
    Media* xmlToMedia(const QDomElement& elemento) const;  // Crea un Media da QDomElement
};

#endif // XMLIO_H