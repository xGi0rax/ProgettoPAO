#include "../Headers/GiocoDaTavolo.h"
#include "../Headers/Media.h"
#include <string>
#include <QJsonObject>
#include <QString>
#include <QDomElement>
#include <QDomDocument>

GiocoDaTavolo::GiocoDaTavolo(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie,  int ngiocatori, int durata, int etaMinima, string editore, int in_prestito, string collocazione, double rating) : Media(titolo, autore, genere, anno, lingua, immagine, disponibilita, numero_copie, in_prestito, collocazione, rating), ngiocatori(ngiocatori), durata(durata), etaMinima(etaMinima), editore(editore) {}

void GiocoDaTavolo::toJson(QJsonObject& jsonObj) const {
    Media::toJson(jsonObj);
    jsonObj["tipo"] = "gioco";
    jsonObj["numero_giocatori"] = ngiocatori;
    jsonObj["durata"] = durata;
    jsonObj["eta_minima"] = etaMinima;
    jsonObj["editore"] = QString::fromStdString(editore);
}

void GiocoDaTavolo::toXml(QDomElement& elemento, QDomDocument& doc) const {
    Q_UNUSED(doc); // Silenzia il warning per in non uso di doc (che viene usato solo nella classe Film)
    Media::toXml(elemento, doc);
    elemento.setAttribute("tipo", "gioco");
    elemento.setAttribute("numero_giocatori", ngiocatori);
    elemento.setAttribute("durata", durata);
    elemento.setAttribute("eta_minima", etaMinima);
    elemento.setAttribute("editore", QString::fromStdString(editore));
}

// Getter
int GiocoDaTavolo::getNGiocatori() const {
    return ngiocatori;
}

int GiocoDaTavolo::getDurata() const {
    return durata;
}

int GiocoDaTavolo::getEtaMinima() const {
    return etaMinima;
}

string GiocoDaTavolo::getEditore() const {
    return editore;
}

// Setter
void GiocoDaTavolo::setNGiocatori(const int& ngiocatori) {
    this->ngiocatori = ngiocatori;
}

void GiocoDaTavolo::setDurata(const int& durata) {
    this->durata = durata;
}

void GiocoDaTavolo::setEtaMinima(const int& etaMinima) {
    this->etaMinima = etaMinima;
}

void GiocoDaTavolo::setEditore(const string& editore) {
    this->editore = editore;
}