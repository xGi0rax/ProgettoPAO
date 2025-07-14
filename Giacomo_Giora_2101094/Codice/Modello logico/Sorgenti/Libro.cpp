#include "../Headers/Libro.h"
#include "../Headers/Media.h"
#include <string>
#include <QJsonObject>
#include <QString>
#include <QDomElement>
#include <QDomDocument>

Libro::Libro(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, 
    int numero_copie, string isbn, string editore, int npagine,int in_prestito, string collocazione, double rating) : Media(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, in_prestito, collocazione, rating), 
    isbn(isbn), editore(editore), npagine(npagine) {}

void Libro::toJson(QJsonObject& jsonObj) const {
    Media::toJson(jsonObj);
    jsonObj["tipo"] = "libro";
    jsonObj["isbn"] = QString::fromStdString(isbn);
    jsonObj["editore"] = QString::fromStdString(editore);
    jsonObj["numero_pagine"] = npagine;
}

void Libro::toXml(QDomElement& elemento, QDomDocument& doc) const {
    Q_UNUSED(doc); // Silenzia il warning per in non uso di doc (che viene usato solo nella classe Film)
    Media::toXml(elemento, doc);
    elemento.setAttribute("tipo", "libro");
    elemento.setAttribute("isbn", QString::fromStdString(isbn));
    elemento.setAttribute("editore", QString::fromStdString(editore));
    elemento.setAttribute("numero_pagine", npagine);
}

// Getter
string Libro::getIsbn() const {
    return isbn;
}

string Libro::getEditore() const {
    return editore;
}

int Libro::getNPagine() const {
    return npagine;
}

// Setter

void Libro::setIsbn(const string& isbn) {
    this->isbn = isbn;
}

void Libro::setEditore(const string& editore) {
    this->editore = editore;
}

void Libro::setNPagine(const int& npagine) {
    this->npagine = npagine;
}
