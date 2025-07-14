#ifndef MEDIA_H
#define MEDIA_H
#include <string>
#include <iostream>
#include <QString>

class QJsonObject;
class QDomElement;
class QDomDocument;

using std::string;

class Media {
private:
    string id = ""; // Nella forma "X" dove X è un numero progressivo univoco per il media all'interno della biblioteca
    string titolo; // Titolo del media
    string autore; // Autore del media
    string genere; // Genere del media (es. "Fantascienza", "Commedia", etc.)
    int anno; // Anno di pubblicazione
    string lingua; // Lingua del media (es. "Italiano", "Inglese", etc.)
    string immagine;  // Percorso dell'immagine del media
    bool disponibilita; // Disponibilità del media (true se disponibile, false se tutte le copie in prestito)
    int numero_copie; // Numero totale di copie del media
    int in_prestito; // Numero di copie attualmente in prestito
    string collocazione; // Collocazione del media nella biblioteca (es. scaffale "A1", "B2", etc.)
    double rating; // Valutazione del media (da 1 a 5 stelle)
    
public:
    Media(string titolo, string autore, string genere, int anno, string lingua, string immagine, bool disponibilita, int numero_copie, int in_prestito = 0, string collocazione = "", double rating = 0.0);
    virtual ~Media() =default;

    // Metodi getter
    string getId() const;
    string getTitolo() const;
    string getAutore() const;
    string getGenere() const;
    int getAnno() const;
    string getLingua() const;
    string getImmagine() const;
    bool getDisponibilita() const;
    int getNumeroCopie() const;
    int getInPrestito() const;
    string getCollocazione() const;
    double getRating() const;

    // Metodi setter
    void setId(const string& id);
    void setTitolo(const string& titolo);
    void setAutore(const string& autore);
    void setGenere(const string& genere);
    void setAnno(const int& anno);
    void setLingua(const string& lingua);
    void setImmagine(const string& imm);
    void setDisponibilita(bool dispon); 
    void setNumeroCopie(const int& n_copie);
    void setInPrestito(const int& in_prestito);
    void setCollocazione(const string& coll);
    void setRating(const double& rating);

    // Metodo per convertire l'oggetto in un oggetto JSON
    virtual void toJson(QJsonObject& jsonObj) const;

    // Metodo per convertire l'oggetto in un oggetto XML
    virtual void toXml(QDomElement& elemento, QDomDocument& doc) const;

    // Metodo virtuale per ottenere informazione del media in formato QString, non è ridefinito nelle sottoclassi perche' le informazioni da resituire sono comuni a tutti i media
    virtual QString mediaInfo() const; 
};

#endif // MEDIA_H