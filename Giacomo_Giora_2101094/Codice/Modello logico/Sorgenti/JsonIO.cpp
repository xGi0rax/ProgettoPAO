#include "../Headers/JsonIO.h"
#include "../Headers/Biblioteca.h"
#include "../Headers/Media.h"
#include "../Headers/Film.h"
#include "../Headers/Libro.h"
#include "../Headers/Vinile.h"
#include "../Headers/GiocoDaTavolo.h"
#include "../Headers/Rivista.h"
#include <QDomElement>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QJsonObject>
using std::vector;

QJsonObject JsonIO::mediaToJson(const Media* media) const {
    QJsonObject elemento;
    media->toJson(elemento); // Chiamata polimorfica per convertire il media in JSON
    return elemento;
}

Media* JsonIO::jsonToMedia(const QJsonObject& elemento) const{
    // Attributi comuni a tutti i media
    string id = elemento["id"].toString().toStdString();
    string titolo = elemento["titolo"].toString().toStdString();
    string autore = elemento["autore"].toString().toStdString();
    string genere = elemento["genere"].toString().toStdString();
    int anno = elemento["anno"].toInt();
    string lingua = elemento["lingua"].toString().toStdString();
    string immagine = elemento["immagine"].toString().toStdString();
    bool disponibilita = elemento["disponibilita"].toBool();
    int numero_copie = elemento["numero_copie"].toInt();
    int in_prestito = elemento["in_prestito"].toInt();
    string collocazione = elemento["collocazione"].toString().toStdString();
    double rating = elemento["rating"].toDouble();

    // Creazione del media in base al tipo specificato nel JSON
    string tipo = elemento["tipo"].toString().toStdString();
    
    if (tipo == "film") {
        int durata = elemento["durata"].toInt();
        vector<string> cast;
        QJsonArray jCast = elemento["cast"].toArray();
        for (const QJsonValue& elemento : jCast) {
            cast.push_back(elemento.toString().toStdString());
        }
        return new Film(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, durata, cast, in_prestito, collocazione, rating);

    } else if (tipo == "libro") {
        string isbn = elemento["isbn"].toString().toStdString();
        string editore = elemento["editore"].toString().toStdString();
        int numeroPagine = elemento["numero_pagine"].toInt();

        return new Libro(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, isbn, editore, numeroPagine, in_prestito, collocazione, rating);

    } else if (tipo == "rivista") {
        string editore = elemento["editore"].toString().toStdString();
        int numeroPagine = elemento["numero_pagine"].toInt();
        string dataPubblicazione = elemento["data_pubblicazione"].toString().toStdString();
        string periodicita = elemento["periodicita"].toString().toStdString();

        return new Rivista(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, editore, numeroPagine, dataPubblicazione, periodicita, in_prestito, collocazione, rating);

    } else if (tipo == "gioco") {
        int numeroGiocatori = elemento["numero_giocatori"].toInt();
        int etaMinima = elemento["eta_minima"].toInt();
        int durata = elemento["durata"].toInt();
        string editore = elemento["editore"].toString().toStdString();

        return new GiocoDaTavolo(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, numeroGiocatori, etaMinima, durata, editore, in_prestito, collocazione, rating);

    } else if (tipo == "vinile") {
        int numeroTracce = elemento["numero_tracce"].toInt();
        int durata = elemento["durata"].toInt();

        return new Vinile(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, numeroTracce, durata, in_prestito, collocazione, rating);
    }

    return nullptr;
}


bool JsonIO::salvaSuFile(const Biblioteca& biblio, const string& filePath) const {
    QFile file(QString::fromStdString(filePath));
    
    // IMPORTANTE: WriteOnly sovrascrive completamente il file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QJsonArray jArray;
    for (const Media* media : biblio.getListaMedia()) {
        jArray.append(mediaToJson(media));
    }

    QJsonDocument jDoc(jArray);
    file.write(jDoc.toJson());
    file.close();
    return true;
}


bool JsonIO::caricaDaFile(Biblioteca& biblio, const string& filePath) {
    QFile file(QString::fromStdString(filePath));
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false; // File non valido o inaccessibile
    }
    
    QByteArray dati = file.readAll();
    file.close();

    QJsonDocument jdoc = QJsonDocument::fromJson(dati);
    if(!jdoc.isArray()) {
        return false; // JSON malformato
    }

    QJsonArray jArray = jdoc.array();

    // Caricamento dei nuovi dati
    for (const QJsonValue& elemento : jArray) {
        QJsonObject jObj = elemento.toObject();
        Media* media = jsonToMedia(jObj); // Crea un oggetto Media da QJsonObject
        if (media) {
            biblio.aggiungiMedia(media); // Aggiunge il media alla biblioteca
        }
    }

    return true; // Caricamento riuscito
}
