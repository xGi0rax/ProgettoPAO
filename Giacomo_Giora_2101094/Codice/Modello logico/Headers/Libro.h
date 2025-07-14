#ifndef LIBRO_H
#define LIBRO_H
#include "Media.h"
#include <string>
using std::string;

class Libro : public Media{
private:
    string isbn; // ISBN del libro
    string editore; // Editore del libro
    int npagine; // Numero di pagine del libro

public:
    Libro(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, string isbn,  string editore, int npagine, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    ~Libro() = default;

    // Metodi getter
    string getIsbn() const;
    string getEditore() const;
    int getNPagine() const;

    // Metodi setter
    void setIsbn(const string& isbn);
    void setEditore(const string& editore);
    void setNPagine(const int& npagine);

    // Metodi per convertire l'oggetto in un oggetto JSON e XML
    void toJson(QJsonObject& jsonObj) const override;
    void toXml(QDomElement& elemento, QDomDocument& doc) const override;
};

#endif //LIBRO_H