#ifndef FILM_H
#define FILM_H
#include "Media.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Film: public Media{ 
private:
    int durata; // Durata del film in minuti
    vector<string> cast; // Vettore contenente nomi e cognomi degli attori principali del film

public:
    Film(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, int durata, vector<string> cast, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    ~Film() =default; 

    // Metodi getter
    int getDurata() const;
    vector<string> getCast() const;

    // Metodi setter
    void setDurata(const int& durata);
    void setCast(const vector<string>& cast);

    // Metodi per convertire l'oggetto in un oggetto JSON e XML
    void toJson(QJsonObject& jsonObj) const override;
    void toXml(QDomElement& elemento, QDomDocument& doc) const override;
};
    
#endif //FILM_H