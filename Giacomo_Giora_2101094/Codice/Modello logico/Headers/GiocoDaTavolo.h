#ifndef GIOCODATAVOLO_H
#define GIOCODATAVOLO_H
#include "Media.h"
#include <string>
using std::string;

class GiocoDaTavolo: public Media{
private: 
    int ngiocatori; // Numero di giocatori massimi per giocare
    int durata;  // Stima durata in minuti
    int etaMinima; // Età minima consigliata per giocare
    string editore; // Editore del gioco da tavolo

public:
    GiocoDaTavolo(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, int ngiocatori, int durata, int etaMinima, string editore, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    ~GiocoDaTavolo() = default;

    // Metodi getter
    int getNGiocatori() const;
    int getDurata() const;
    int getEtaMinima() const;
    string getEditore() const;

    // Metodi setter
    void setNGiocatori(const int& ngiocatori);
    void setDurata(const int& durata);
    void setEtaMinima(const int& etaMinima);
    void setEditore(const string& editore);

    // Metodi per convertire l'oggetto in un oggetto JSON e XML
    void toJson(QJsonObject& jsonObj) const override;
    void toXml(QDomElement& elemento, QDomDocument& doc) const override;
};
    
#endif //GIOCODATAVOLO_H