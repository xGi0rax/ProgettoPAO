#ifndef DETAILSPAGE_H
#define DETAILSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include "Widgets/MediaWidget.h"
#include "Widgets/FilmWidget.h"
#include "Widgets/LibroWidget.h"
#include "Widgets/VinileWidget.h"
#include "Widgets/RivistaWidget.h"
#include "Widgets/GiocoWidget.h"

class DetailsPage : public QWidget {
    Q_OBJECT

public:
    explicit DetailsPage(QWidget *parent = nullptr);
    void setMedia(Media* media);

signals:
    void goBackToMainPage(); // Segnale per tornare alla pagina principale
    void mediaBorrowed(Media* media); // Segnale per il prestito del media
    void mediaReturned(Media* media); // Segnale per la restituzione del media
    // void mediaRequestedFromAffiliate(Media* media); // Segnale per la richiesta da biblioteca affiliata

private slots:
    void onBackButtonClicked(); // Slot per il pulsante indietro
    void onBorrowButtonClicked(); // Slot per il prestito del media
    void onReturnButtonClicked(); // Slot per la restituzione del media
    void onRequestFromAffiliateClicked(); // Slot per la richiesta da biblioteca affiliata

private:
    // Puntatore al media corrente
    Media* currentMedia;

    // Immagine del media
    QLabel* imageLabel;

    QLabel* detailsLabel;
    QLabel* availabilityLabel;

    // Pulsanti indietro, prendi in prestito e restituisci
    QPushButton* backButton;
    QPushButton* borrowButton;
    QPushButton* returnButton;
    QPushButton* requestButton;

    QLabel* copiesLabel;
    
    QVBoxLayout* specificDetailsLayout;
    MediaWidget* currentWidget;

    // Metodi di configurazione UI
    void setupUI();
    void updateUI();
    void clearSpecificDetails();
    void setupSpecificDetails(Media* media);
    MediaWidget* createViewWidgetForMedia(Media* media);
};

#endif // DETAILSPAGE_H