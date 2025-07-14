#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include "Media.h"
#include <string>
#include <vector>
#include <QString>

using std::vector;
using std::string;

class Biblioteca {
private:
    vector<Media*> listaMedia;  // vettore di puntatori a Media
    int nextIdmedia;            // contatore per l'auto-incremento degli Id dei media

public:
    Biblioteca();
    ~Biblioteca();
    
    // Gestione media
    void aggiungiMedia(Media* media);
    bool rimuoviMedia(Media* media);
    bool modificaMedia(const string& id, Media* newMedia);
    
    // Ricerca media
    Media* cercaMediaDaID(const string& id) const;
    Media* cercaMediaDaT_A_A(const string& titolo, const string& autore, int anno) const;
    bool esisteMedia(const string& titolo, const string& autore, int anno) const;
    
    // Filtri
    vector<Media*> filtra(const string& titolo = "",
                         const string& tipoMedia = "",
                         const string& genere = "",
                         double ratingMin = 0.0,
                         double ratingMax = 5.0,
                         bool disponibilita = true,
                         const string& lingua = "",
                         int annoMin = 1200,
                         int annoMax = 2050) const;
    
    // Gestione prestiti
    bool prendiInPrestito(const Media* media);
    bool restituisci(const Media* media);
    
    // Getter
    vector<Media*> getListaMedia() const;
};

#endif // BIBLIOTECA_H