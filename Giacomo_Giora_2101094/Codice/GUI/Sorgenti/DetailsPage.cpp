#include "../Headers/DetailsPage.h"
#include "../../Modello logico/Headers/Film.h"
#include "../../Modello logico/Headers/Libro.h"
#include "../../Modello logico/Headers/Vinile.h"
#include "../../Modello logico/Headers/Rivista.h"
#include "../../Modello logico/Headers/GiocoDaTavolo.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFile>
#include <QTimer>
#include <QDir>

// ============================
// COSTRUTTORE E INIZIALIZZAZIONE
// ============================

// Costruttore
DetailsPage::DetailsPage(QWidget *parent) : QWidget(parent), currentMedia(nullptr) {
    setupUI();
}

// Crea e configura tutti gli elementi dell'interfaccia grafica
void DetailsPage::setupUI() {
    // Layout principale
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 10);
    mainLayout->setSpacing(8);
    
    backButton = new QPushButton("Indietro");
    backButton->setFixedSize(120, 30);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(0, 104, 201);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 4px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color:rgb(11, 82, 189);"
        "}"
    );
    connect(backButton, &QPushButton::clicked, this, &DetailsPage::onBackButtonClicked);
    
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 5);
    headerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    mainLayout->addLayout(headerLayout);
    
    // Pannello principale con immagine e dettagli
    QWidget *contentWidget = new QWidget();
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(20); 
    
    // Area per visualizzare l'immagine del media
    imageLabel = new QLabel();
    imageLabel->setFixedSize(280, 400);
    imageLabel->setScaledContents(true);
    imageLabel->setStyleSheet(
        "border: 2px solid rgb(60, 58, 58);"
        "background-color: rgb(243, 238, 238);"
    );
    
    // Pannello destro con informazioni e pulsanti
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);
    
    // Area per mostrare i dettagli del media in formato HTML
    QLabel *detailsLabel = new QLabel();
    detailsLabel->setObjectName("detailsLabel");
    detailsLabel->setStyleSheet(
        "font-size: 15px;"
        "line-height: 140%;"
        "background-color: rgb(243, 238, 238);"
        "border: 1px solid rgb(200, 200, 200);"
        "border-radius: 6px;"
        "padding: 15px;"
    );
    detailsLabel->setTextFormat(Qt::RichText);
    detailsLabel->setWordWrap(true);
    detailsLabel->setAlignment(Qt::AlignTop);
    
    // Etichetta per mostrare lo stato di disponibilità
    availabilityLabel = new QLabel();
    availabilityLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 8px;"
        "border-radius: 6px;"
        "background-color: rgba(255, 255, 255, 0.8);"
    );
    availabilityLabel->setAlignment(Qt::AlignCenter);
    
    // Container per i pulsanti di azione
    QWidget *buttonContainer = new QWidget();
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonContainer);
    buttonsLayout->setContentsMargins(0, 10, 0, 0);
    buttonsLayout->setSpacing(15); 
    
    // Pulsanti per prestito, restituzione e richiesta da biblioteca affiliata
    borrowButton = new QPushButton("Prendi in prestito");
    returnButton = new QPushButton("Restituisci");
    requestButton = new QPushButton("🏛️ Richiedi da affiliata"); 
    
    borrowButton->setFixedSize(150, 40);
    returnButton->setFixedSize(130, 40);
    requestButton->setFixedSize(220, 40); 
    
    // Stili per i pulsanti (omessi per brevità)
    borrowButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #28a745;"
        "   color: white;"
        "   padding: 8px 10px;"
        "   font-size: 14px;" 
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #218838;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #6c757d;"
        "}"
    );
    
    returnButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #dc3545;"
        "   color: white;"
        "   padding: 8px 10px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c82333;"
        "}"
    );
    
    requestButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #17a2b8;"
        "   color: white;"
        "   padding: 8px 10px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #138496;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #6c757d;"
        "   color: #adb5bd;"
        "}"
    );
    
    // Collegamento dei pulsanti ai rispettivi slot
    connect(borrowButton, &QPushButton::clicked, this, &DetailsPage::onBorrowButtonClicked);
    connect(returnButton, &QPushButton::clicked, this, &DetailsPage::onReturnButtonClicked);
    connect(requestButton, &QPushButton::clicked, this, &DetailsPage::onRequestFromAffiliateClicked);
    
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(borrowButton);
    buttonsLayout->addWidget(returnButton);
    buttonsLayout->addWidget(requestButton);
    buttonsLayout->addStretch(1);

    rightLayout->addWidget(detailsLabel, 1);
    rightLayout->addWidget(availabilityLabel, 0);
    rightLayout->addWidget(buttonContainer, 0); 

    contentLayout->addWidget(imageLabel, 0); 
    contentLayout->addWidget(rightPanel, 1); 
    
    mainLayout->addWidget(contentWidget, 1);
}

// ============================
// GESTIONE DATI E AGGIORNAMENTO UI
// ============================

// Imposta il media da visualizzare e aggiorna l'interfaccia
void DetailsPage::setMedia(Media* media) {
    currentMedia = media;
    updateUI();
}

// Aggiorna tutti gli elementi dell'interfaccia con i dati del media corrente
void DetailsPage::updateUI() {
    if (!currentMedia) {
        return;
    }
    
    // Calcola le copie disponibili
    int totalCopies = currentMedia->getNumeroCopie();
    int loanedCopies = currentMedia->getInPrestito();
    int availableCopies = totalCopies - loanedCopies;
    
    QString htmlDetails = "<h2 style='margin:0 0 15px 0; padding:0; color:#2c3e50; border-bottom: 2px solid #3498db;'>" + 
                         QString::fromStdString(currentMedia->getTitolo()) + "</h2>";
    
    htmlDetails += "<table style='width:100%; border-collapse: separate; border-spacing: 0 3px;'>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; width:140px; vertical-align:top;'>Autore:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(currentMedia->getAutore()) + "</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Genere:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(currentMedia->getGenere()) + "</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Anno:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(currentMedia->getAnno()) + "</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Lingua:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(currentMedia->getLingua()) + "</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Rating:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(currentMedia->getRating(), 'f', 1) + "/5.0</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Collocazione:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(currentMedia->getCollocazione()) + "</td></tr>";
    htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Copie:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(availableCopies) + 
                   " disponibili su " + QString::number(totalCopies) + " totali</td></tr>";
    
    // Aggiunge informazioni specifiche in base al tipo di media
    if (Film* film = dynamic_cast<Film*>(currentMedia)) {
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Tipo:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>🎬 Film</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Durata:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(film->getDurata()) + " minuti</td></tr>";
        
        const vector<string>& cast = film->getCast();
        if (!cast.empty()) {
            htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Cast:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>";
            for (size_t i = 0; i < cast.size(); ++i) {
                htmlDetails += QString::fromStdString(cast[i]);
                if (i < cast.size() - 1) htmlDetails += ", ";
            }
            htmlDetails += "</td></tr>";
        }
    } 
    else if (Libro* libro = dynamic_cast<Libro*>(currentMedia)) {
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Tipo:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>📚 Libro</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>ISBN:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(libro->getIsbn()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Editore:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(libro->getEditore()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Pagine:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(libro->getNPagine()) + "</td></tr>";
    }
    else if (Vinile* vinile = dynamic_cast<Vinile*>(currentMedia)) {
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Tipo:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>🎵 Vinile</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Numero tracce:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(vinile->getNTracce()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Durata:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(vinile->getDurata()) + " minuti</td></tr>";
    }
    else if (Rivista* rivista = dynamic_cast<Rivista*>(currentMedia)) {
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Tipo:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>📰 Rivista</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Editore:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(rivista->getEditore()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Pagine:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(rivista->getNPagine()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Data pubblicazione:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(rivista->getDataPubb()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Periodicità:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(rivista->getPeriodicita()) + "</td></tr>";
    }
    else if (GiocoDaTavolo* gioco = dynamic_cast<GiocoDaTavolo*>(currentMedia)) {
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Tipo:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>🎲 Gioco da Tavolo</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Editore:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::fromStdString(gioco->getEditore()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Max giocatori:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(gioco->getNGiocatori()) + "</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Durata gioco:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(gioco->getDurata()) + " minuti</td></tr>";
        htmlDetails += "<tr><td style='padding:4px 15px 4px 0; font-weight:bold; color:#34495e; vertical-align:top;'>Età minima:</td><td style='padding:4px 0; color:#2c3e50; line-height:1.2;'>" + QString::number(gioco->getEtaMinima()) + " anni</td></tr>";
    }
    
    htmlDetails += "</table>";
    
    // Aggiorna il contenuto della label dei dettagli
    QLabel* detailsLabel = findChild<QLabel*>("detailsLabel");
    if (detailsLabel) {
        detailsLabel->setText(htmlDetails);
    }
    
    // Carica e visualizza l'immagine del media
    QString imagePath = QString::fromStdString(currentMedia->getImmagine());

    QString resourcePath = ":/Immagini/" + imagePath;
    QPixmap pixmap(resourcePath);
    
    // Cerca l'immagine in diverse posizioni se non trovata nelle risorse
    if (pixmap.isNull()) {
        if (QFile::exists(imagePath)) {
            pixmap.load(imagePath);
        }
        
        if (pixmap.isNull()) {
            QDir currentDir = QDir::current();
            
            QStringList possiblePaths = {
                currentDir.absoluteFilePath("Immagini/" + imagePath),          
                currentDir.absoluteFilePath("../Immagini/" + imagePath),      
                currentDir.absoluteFilePath("../../Immagini/" + imagePath),    
                currentDir.absoluteFilePath("GUI/Immagini/" + imagePath),       
                currentDir.absoluteFilePath("../GUI/Immagini/" + imagePath),  
                QDir::home().absoluteFilePath("Documents/BananoTECH/Immagini/" + imagePath)
            };
            
            for (const QString& path : possiblePaths) {
                if (QFile::exists(path)) {
                    pixmap.load(path);
                    if (!pixmap.isNull()) {
                        break;
                    }
                }
            }
        }
    }
    
    // Imposta l'immagine o un placeholder se non trovata
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap);
    } else {
        QPixmap placeholder(280, 400);
        placeholder.fill(QColor(240, 240, 240));
        imageLabel->setPixmap(placeholder);
    }
    
    // Aggiorna lo stato di disponibilità e abilita/disabilita i pulsanti
    if (currentMedia->getDisponibilita() && availableCopies > 0) {
        availabilityLabel->setText("Disponibile per il prestito");
        availabilityLabel->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #28a745;"
            "padding: 6px; border: 2px solid #28a745; border-radius: 6px;"
            "background-color: rgba(40, 167, 69, 0.1);"
        );
        borrowButton->setEnabled(true);
        
        requestButton->setVisible(true);
        requestButton->setEnabled(false);
        requestButton->setText("🏛️ Richiedi da biblioteca affiliata"); 
        requestButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #6c757d;"
            "   color: #adb5bd;"
            "   padding: 8px 10px;"
            "   font-size: 13px;"
            "   font-weight: bold;"
            "   border: none;"
            "   border-radius: 6px;"
            "}"
        );
    } else {
        availabilityLabel->setText("Non disponibile");
        availabilityLabel->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #dc3545;"
            "padding: 6px; border: 2px solid #dc3545; border-radius: 6px;"
            "background-color: rgba(220, 53, 69, 0.1);"
        );
        borrowButton->setEnabled(false);
        
        requestButton->setVisible(true);
        requestButton->setEnabled(true);
        requestButton->setText("🏛️ Richiedi da biblioteca affiliata");
        requestButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #17a2b8;"
            "   color: white;"
            "   padding: 8px 10px;"
            "   font-size: 13px;"
            "   font-weight: bold;"
            "   border: none;"
            "   border-radius: 6px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #138496;"
            "}"
        );
    }
    
    returnButton->setEnabled(loanedCopies > 0);
    borrowButton->setVisible(true);
    returnButton->setVisible(true);
}

// ============================
// GESTIONE WIDGET SPECIFICI
// ============================

// Rimuove i widget specifici dal layout dei dettagli
void DetailsPage::clearSpecificDetails() {
    QLayoutItem *item;
    while ((item = specificDetailsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    
    currentWidget = nullptr;
}

// Crea widget specifici per visualizzare dettagli in base al tipo di media
void DetailsPage::setupSpecificDetails(Media* media) {
    if (!media) return;
    
    // Crea un widget appropriato in base al tipo di media
    currentWidget = createViewWidgetForMedia(media);
    
    if (currentWidget) {
        currentWidget->setProperty("viewOnly", true);
        specificDetailsLayout->addWidget(currentWidget);
    }
}

// Factory method: crea il widget appropriato per il tipo di media specifico
MediaWidget* DetailsPage::createViewWidgetForMedia(Media* media) {
    MediaWidget* widget = nullptr;
    
    if (Film* film = dynamic_cast<Film*>(media)) {
        FilmWidget* filmWidget = new FilmWidget();
        filmWidget->setCurrentMedia(film);
        filmWidget->setReadOnly(true);  
        widget = filmWidget;
    } 
    else if (Libro* libro = dynamic_cast<Libro*>(media)) {
        LibroWidget* libroWidget = new LibroWidget();
        libroWidget->setCurrentMedia(libro);
        libroWidget->setReadOnly(true);
        widget = libroWidget;
    } 
    else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
        VinileWidget* vinileWidget = new VinileWidget();
        vinileWidget->setCurrentMedia(vinile);
        vinileWidget->setReadOnly(true);
        widget = vinileWidget;
    } 
    else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
        RivistaWidget* rivistaWidget = new RivistaWidget();
        rivistaWidget->setCurrentMedia(rivista);
        rivistaWidget->setReadOnly(true);
        widget = rivistaWidget;
    } 
    else if (GiocoDaTavolo* gioco = dynamic_cast<GiocoDaTavolo*>(media)) {
        GiocoWidget* giocoWidget = new GiocoWidget();
        giocoWidget->setCurrentMedia(gioco);
        giocoWidget->setReadOnly(true);
        widget = giocoWidget;
    }
    
    return widget;
}

// ============================
// SLOT E GESTIONE EVENTI
// ============================

// Slot: gestisce il click del pulsante "Indietro"
void DetailsPage::onBackButtonClicked() {
    emit goBackToMainPage();
}

// Slot: gestisce il prestito del media con verifica disponibilità
void DetailsPage::onBorrowButtonClicked() {  
    if (!currentMedia) return;
    
    // Verifica disponibilità
    int availableCopies = currentMedia->getNumeroCopie() - currentMedia->getInPrestito();
    
    if (currentMedia->getDisponibilita() && availableCopies > 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
            "Conferma prestito", 
            "Confermi di voler prendere in prestito questo media?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {           
            emit mediaBorrowed(currentMedia);
            
            updateUI();
        }
    } else {
        QMessageBox::warning(this, "Prestito non disponibile", 
            "Questo media non è attualmente disponibile per il prestito.");
    }
}

// Slot: gestisce la restituzione del media con conferma utente
void DetailsPage::onReturnButtonClicked() {
    if (!currentMedia) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Conferma restituzione", 
        "Confermi di voler restituire questo media?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        emit mediaReturned(currentMedia);

        updateUI();
    }
}

// Slot: gestisce la richiesta di media da biblioteche affiliate
void DetailsPage::onRequestFromAffiliateClicked() {
    if (!currentMedia) return;
    
    // Mostra dialog di conferma per la richiesta
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Richiesta a biblioteca affiliata", 
        QString("Vuoi richiedere una copia di \"%1\" da una biblioteca affiliata?\n\n"
                "La richiesta verrà inoltrata e sarai contattato quando una copia sarà disponibile.")
                .arg(QString::fromStdString(currentMedia->getTitolo())),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) { 
        // Mostra conferma invio richiesta
        QMessageBox::information(this, "Richiesta inviata", 
            QString("La richiesta per \"%1\" è stata inviata alle biblioteche affiliate.\n"
                    "Verrai contattato non appena una copia sarà disponibile.")
                    .arg(QString::fromStdString(currentMedia->getTitolo())));

        // Aggiorna aspetto del pulsante per indicare richiesta inviata
        requestButton->setText("Richiesta inviata!");
        requestButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #28a745;"
            "   color: white;"
            "   padding: 8px 10px;"
            "   font-size: 13px;"
            "   font-weight: bold;"
            "   border: none;"
            "   border-radius: 6px;"
            "}"
        );
    }
}