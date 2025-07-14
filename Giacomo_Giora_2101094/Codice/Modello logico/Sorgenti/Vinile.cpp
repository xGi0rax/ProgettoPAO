#include "../Headers/Vinile.h"
#include "../Headers/Media.h"
#include <string>
#include <QJsonObject>
#include <QString>
#include <QDomElement>
#include <QDomDocument>

Vinile::Vinile(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, int ntracce, int durata, int in_prestito, string collocazione, double rating) : Media(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, in_prestito, collocazione, rating), ntracce(ntracce), durata(durata) {}

void Vinile::toJson(QJsonObject& jsonObj) const {
    Media::toJson(jsonObj);
    jsonObj["tipo"] = "vinile";
    jsonObj["numero_tracce"] = ntracce;
    jsonObj["durata"] = durata;
}

void Vinile::toXml(QDomElement& elemento, QDomDocument& doc) const {
    Q_UNUSED(doc); // Silenzia il warning per in non uso di doc (che viene usato solo nella classe Film)
    Media::toXml(elemento, doc);
    elemento.setAttribute("tipo", "vinile");
    elemento.setAttribute("numero_tracce", ntracce);
    elemento.setAttribute("durata", durata);
}

// Getter
int Vinile::getNTracce() const {
    return ntracce;
}

int Vinile::getDurata() const {
    return durata;
}

// Setter
void Vinile::setNTracce(const int& ntracce) {
    this->ntracce = ntracce;
}

void Vinile::setDurata(const int& durata) {
    this->durata = durata;
}