#ifndef VINILE_H
#define VINILE_H
#include "Media.h"
#include <string>
using std::string;

class Vinile: public Media{ 
private:
    int ntracce; // Numero di tracce del vinile
    int durata; // Durata totale del vinile in minuti

public:
    Vinile(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, int ntracce, int durata, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    ~Vinile() = default;

    // Metodi getter
    int getNTracce() const;
    int getDurata() const;

    // Metodi setter
    void setNTracce(const int& ntracce);
    void setDurata(const int& durata);

    // Metodi per convertire l'oggetto in un oggetto JSON e XML
    void toJson(QJsonObject& jsonObj) const override;
    void toXml(QDomElement& elemento, QDomDocument& doc) const override;
};

#endif //VINILE_H