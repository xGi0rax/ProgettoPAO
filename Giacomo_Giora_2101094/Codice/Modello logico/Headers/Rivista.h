#ifndef RIVISTA_H
#define RIVISTA_H
#include "Media.h"
#include <string>
using std::string;

class Rivista: public Media{
private:
    string editore; // Editore della rivista
    int n_pagine; // Numero di pagine
    string data_pubb; // Data di pubblicazione (formato: "DD-MM-YYYY")
    string periodicita; // Mensile, settimanale, annuale, ecc.

public:
    Rivista(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, string editore, int n_pagine, string data_pubb, string periodicita, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    ~Rivista() = default;

    // Metodi getter
    string getEditore() const;
    int getNPagine() const;
    string getDataPubb() const;
    string getPeriodicita() const;

    // Metodi setter
    void setEditore(const string& editore);
    void setNPagine(const int& n_pagine);
    void setDataPubb(const string& mese_pubb);
    void setPeriodicita(const string& periodicita);

    // Metodi per convertire l'oggetto in un oggetto JSON e XML
    void toJson(QJsonObject& jsonObj) const override;
    void toXml(QDomElement& elemento, QDomDocument& doc) const override;
};

#endif //RIVISTA_H