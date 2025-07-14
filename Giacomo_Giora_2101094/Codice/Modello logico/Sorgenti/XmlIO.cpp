#include "../Headers/XmlIO.h"
#include "../Headers/Media.h"
#include "../Headers/Film.h"
#include "../Headers/Libro.h"
#include "../Headers/Vinile.h"
#include "../Headers/GiocoDaTavolo.h"
#include "../Headers/Rivista.h"
#include "../Headers/Biblioteca.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

QDomElement XmlIO::mediaToXml(const Media* media, QDomDocument& doc) const {
    QDomElement elemento = doc.createElement("media");
    media->toXml(elemento, doc); // Chiamata polimorfica per convertire il media in XML
    return elemento;
}

Media* XmlIO::xmlToMedia(const QDomElement& elemento) const{
    // Attributi comuni a tutti i media
    string id = elemento.attribute("id").toStdString();
    string titolo = elemento.attribute("titolo").toStdString();
    string autore = elemento.attribute("autore").toStdString();
    string genere = elemento.attribute("genere").toStdString();
    int anno = elemento.attribute("anno").toInt();
    string lingua = elemento.attribute("lingua").toStdString();
    string immagine = elemento.attribute("immagine").toStdString();
    bool disponibilita = elemento.attribute("disponibilita").toInt();
    int numero_copie = elemento.attribute("numero_copie").toInt();
    int in_prestito = elemento.attribute("in_prestito").toInt();
    string collocazione = elemento.attribute("collocazione").toStdString();
    double rating = elemento.attribute("rating").toDouble();

    // Creazione del media in base al tipo specificato nel XML
    string tipo = elemento.attribute("tipo").toStdString();

    if (tipo == "film") {
        int durata = elemento.attribute("durata").toInt();
        vector<string> cast;
        QDomNodeList castList = elemento.elementsByTagName("attore");
        for (int i = 0; i < castList.size(); ++i) {
            QDomElement attore = castList.at(i).toElement();
            cast.push_back(attore.text().toStdString());
        }
        return new Film(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, durata, cast, in_prestito, collocazione, rating);

    } else if (tipo == "libro") {
        string isbn = elemento.attribute("isbn").toStdString();
        string editore = elemento.attribute("editore").toStdString();
        int numeroPagine = elemento.attribute("numero_pagine").toInt();
        return new Libro(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, isbn, editore, numeroPagine, in_prestito, collocazione, rating);

    } else if (tipo == "rivista") {
        string editore = elemento.attribute("editore").toStdString();
        int numeroPagine = elemento.attribute("numero_pagine").toInt();
        string dataPubblicazione = elemento.attribute("data_pubblicazione").toStdString();
        string periodicita = elemento.attribute("periodicita").toStdString();

        return new Rivista(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, editore, numeroPagine, dataPubblicazione, periodicita, in_prestito, collocazione, rating);

    } else if (tipo == "gioco"){
        int numeroGiocatori = elemento.attribute("numero_giocatori").toInt();
        int etaMinima = elemento.attribute("eta_minima").toInt();
        int durata = elemento.attribute("durata").toInt();
        string editore = elemento.attribute("editore").toStdString();

        return new GiocoDaTavolo(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, numeroGiocatori, etaMinima, durata, editore, in_prestito, collocazione, rating);

    } else if (tipo == "vinile"){
        int numeroTracce = elemento.attribute("numero_tracce").toInt();
        int durata = elemento.attribute("durata").toInt();

        return new Vinile(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, numeroTracce, durata, in_prestito, collocazione, rating);
    }

    return nullptr;
}


bool XmlIO::salvaSuFile(const Biblioteca& biblio, const string& filePath) const {
    QDomDocument doc;
    QDomElement root = doc.createElement("biblioteca");
    doc.appendChild(root);

    for (const Media* media : biblio.getListaMedia()) {
        QDomElement elemento = mediaToXml(media, doc);
        root.appendChild(elemento);
    }

    QFile file(QString::fromStdString(filePath));
    
    // IMPORTANTE: WriteOnly sovrascrive completamente il file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    return true;
}

bool XmlIO::caricaDaFile(Biblioteca& biblio, const string& filePath) {
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly)) {
        return false; // Errore nell'aprire il file
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return false; // Errore nel parsing del documento XML
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList listaMedia = root.elementsByTagName("media");

    for (int i = 0; i < listaMedia.size(); ++i) {
        QDomElement elemento = listaMedia.at(i).toElement();
        Media* media = xmlToMedia(elemento); // Crea un oggetto Media da XML
        if (media) {
            biblio.aggiungiMedia(media); // Aggiunge il media alla biblioteca
        }
    }

    return true; // Caricamento riuscito
}