#include "../Headers/ModifyPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>

ModifyPage::ModifyPage(QWidget* parent) : QWidget(parent), biblioteca(nullptr), currentMedia(nullptr), currentWidget(nullptr) {
    setupUI();
}

void ModifyPage::setMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido");
        return;
    }
    
    currentMedia = media;
    newImagePath = "";
    hasNewImage = false;
    
    if (currentWidget) {
        detailsStackedWidget->removeWidget(currentWidget);
        delete currentWidget;
        currentWidget = nullptr;
    }

    if (Film* film = dynamic_cast<Film*>(media)) {
        FilmWidget* filmWidget = new FilmWidget();
        filmWidget->setCurrentMedia(film);
        currentWidget = filmWidget;
    } 
    else if (Libro* libro = dynamic_cast<Libro*>(media)) {
        LibroWidget* libroWidget = new LibroWidget();
        libroWidget->setCurrentMedia(libro);
        currentWidget = libroWidget;
    } 
    else if (Vinile* vinile = dynamic_cast<Vinile*>(media)) {
        VinileWidget* vinileWidget = new VinileWidget();
        vinileWidget->setCurrentMedia(vinile);
        currentWidget = vinileWidget;
    } 
    else if (Rivista* rivista = dynamic_cast<Rivista*>(media)) {
        RivistaWidget* rivistaWidget = new RivistaWidget();
        rivistaWidget->setCurrentMedia(rivista);
        currentWidget = rivistaWidget;
    } 
    else if (GiocoDaTavolo* gioco = dynamic_cast<GiocoDaTavolo*>(media)) {
        GiocoWidget* giocoWidget = new GiocoWidget();
        giocoWidget->setCurrentMedia(gioco);
        currentWidget = giocoWidget;
    }
    else {
        QMessageBox::warning(this, "Errore", "Tipo di media non riconosciuto");
        return;
    }
    
    detailsStackedWidget->addWidget(currentWidget);
    detailsStackedWidget->setCurrentWidget(currentWidget);
    
    loadExistingImage(media);
}

void ModifyPage::setBiblioteca(Biblioteca* biblio) {
    biblioteca = biblio;
}

// ====================================
// METODI DI INIZIALIZZAZIONE UI
// ====================================

void ModifyPage::setupUI() {
    QHBoxLayout* mainHLayout = new QHBoxLayout(this);
    mainHLayout->setContentsMargins(0, 0, 0, 0);
    mainHLayout->setSpacing(0);

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->setMinimumSize(300, 0);

    QWidget* sidebarWidget = new QWidget();
    sidebarWidget->setObjectName("sidebarWidget");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(10, 10, 10, 10);
    sidebarLayout->setSpacing(15);

    backButton = new QPushButton("Indietro");
    backButton->setMinimumSize(100, 30);
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
    connect(backButton, &QPushButton::clicked, this, &ModifyPage::onBackButtonClicked);
    sidebarLayout->addWidget(backButton, 0, Qt::AlignTop | Qt::AlignLeft);

    imagePreview = new QLabel("Nessuna immagine");
    imagePreview->setMinimumSize(220, 220);
    imagePreview->setMaximumSize(300, 300);
    imagePreview->setScaledContents(true);
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setStyleSheet(
        "border: 1px dashed #666666;"
        "border-radius: 4px;"
    );

    uploadButton = new QPushButton("Carica immagine");
    uploadButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(255, 208, 0);"
        "   color: black;"
        "   border: 2px solid rgb(119, 114, 114);"
        "   border-radius: 4px;"
        "   font-size: 15px;"
        "   padding: 8px 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(255, 170, 0);"
        "}"
    );
    connect(uploadButton, &QPushButton::clicked, this, &ModifyPage::onUploadButtonClicked);
    
    QVBoxLayout* imageLayout = new QVBoxLayout();
    imageLayout->setContentsMargins(20, 20, 20, 20);
    imageLayout->addWidget(imagePreview, 1);
    imageLayout->addWidget(uploadButton, 0, Qt::AlignCenter);

    sidebarLayout->addLayout(imageLayout, 1);

    saveButton = new QPushButton("CONFERMA MODIFICHE");
    saveButton->setMinimumSize(190, 45);
    saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(0, 128, 0);"
        "   color: white;"
        "   border: 2px solid rgb(119, 114, 114);;"
        "   border-radius: 6px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(0, 100, 0);"
        "}"
    );
    connect(saveButton, &QPushButton::clicked, this, &ModifyPage::onSaveButtonClicked);
    sidebarLayout->addWidget(saveButton, 0, Qt::AlignBottom | Qt::AlignCenter);

    splitter->addWidget(sidebarWidget);

    detailsStackedWidget = new QStackedWidget();
    splitter->addWidget(detailsStackedWidget);

    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 3);
    
    mainHLayout->addWidget(splitter);
    setLayout(mainHLayout);

    currentImagePath = "";
}

// =============================
// SLOTS
// =============================

void ModifyPage::onBackButtonClicked() {
    emit goBackToMainPage();
}

void ModifyPage::onSaveButtonClicked() {
    if (!currentMedia || !currentWidget) {
        QMessageBox::warning(this, "Errore", "Nessun media da salvare");
        return;
    }
    
    if (!currentWidget->validateData()) {
        QMessageBox::warning(this, "Errore", "Dati non validi. Verifica tutti i campi.");
        return;
    }

    QString newTitle = currentWidget->getTitolo(); 
    QString newAuthor = currentWidget->getAutore();
    int newYear = currentWidget->getAnno();

    if (biblioteca && biblioteca->esisteMedia(newTitle.toStdString(), newAuthor.toStdString(), newYear)) {
        Media* mediaEsistente = biblioteca->cercaMediaDaT_A_A(newTitle.toStdString(), newAuthor.toStdString(), newYear);
        
        if (mediaEsistente && mediaEsistente != currentMedia) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Media già esistente", 
                "Un media con lo stesso titolo, autore e anno esiste già nella biblioteca. "
                "Vuoi aumentare il numero di copie di questo media invece di salvare le modifiche? "
                "\nIl media che stavi modificando verrà eliminato.", 
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                mediaEsistente->setNumeroCopie(mediaEsistente->getNumeroCopie() + 1);
                biblioteca->rimuoviMedia(currentMedia);
                currentMedia = nullptr;

                QMessageBox::information(this, "Salvataggio", 
                    "Numero copie del media aumentate con successo!");
                emit mediaCopiesIncreased();
                emit goBackToMainPage();
                return;
            } else if (reply == QMessageBox::No) {
                return;
            }
        }
    }
    
    if (currentWidget->applyChanges()) {
        if (hasNewImage && !newImagePath.isEmpty()) {
            currentMedia->setImmagine(newImagePath.toStdString());
        }
        
        QMessageBox::information(this, "Successo", "Modifiche salvate con successo");
        emit mediaModified();
        emit goBackToMainPage();
    } 
    else {
        QMessageBox::warning(this, "Errore", "Non è stato possibile applicare le modifiche");
    }
}

void ModifyPage::onUploadButtonClicked() {
    QString imagePath = QFileDialog::getOpenFileName(this, "Seleziona un'immagine", "", "Immagini (*.png *.jpg *.jpeg *.bmp)");
    
    if (imagePath.isEmpty()) {
        return;
    }
    
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Errore", "Impossibile caricare l'immagine selezionata");
        return;
    }
    
    QPixmap scaledPixmap = pixmap.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imagePreview->setPixmap(scaledPixmap);
    imagePreview->setScaledContents(false);
    
    QDir currentDir = QDir::current();
    QString projectImagesDir;
    
    QDir searchDir = currentDir;
    while (!searchDir.isRoot()) {
        if (searchDir.dirName() == "BananoTECH-a" || searchDir.exists("Immagini")) {
            projectImagesDir = searchDir.absoluteFilePath("Immagini");
            break;
        }
        if (!searchDir.cdUp()) {
            break;
        }
    }
    
    if (projectImagesDir.isEmpty()) {
        QStringList possiblePaths = {
            currentDir.absoluteFilePath("../Immagini"),
            currentDir.absoluteFilePath("../../Immagini"),
            currentDir.absoluteFilePath("Immagini"),
        };
        
        for (const QString& path : possiblePaths) {
            QDir testDir(path);
            if (testDir.exists() || QDir().mkpath(path)) {
                projectImagesDir = path;
                break;
            }
        }
    }
    
    if (projectImagesDir.isEmpty()) {
        projectImagesDir = currentDir.absoluteFilePath("../Immagini");
    }
    
    QDir dir;
    if (!dir.exists(projectImagesDir)) {
        if (!dir.mkpath(projectImagesDir)) {
            QMessageBox::warning(this, "Errore", 
                "Impossibile creare la cartella Immagini in: " + projectImagesDir);
            return;
        }
    }
    
    QFileInfo fileInfo(imagePath);
    QString fileName = fileInfo.fileName();
    QString destinationPath = projectImagesDir + "/" + fileName;
    
    if (QFile::copy(imagePath, destinationPath)) {
        newImagePath = fileName;
        hasNewImage = true;
    } else {
        // Se la copia fallisce, controlla se esiste già
        if (QFile::exists(destinationPath)) {
            newImagePath = fileName;
            hasNewImage = true;
        } else {
            QMessageBox::warning(this, "Errore", 
                "Impossibile copiare l'immagine nella cartella del progetto: " + destinationPath);
            hasNewImage = false;
        }
    }
}

// =====================================
// METODI DI GESTIONE IMMAGINI
// =====================================

void ModifyPage::loadExistingImage(Media* media) {
    std::string imagePath = media->getImmagine();
    if (imagePath.empty()) {
        imagePreview->setText("Nessuna immagine");
        currentImagePath = "";
        return;
    }

    currentImagePath = QString::fromStdString(imagePath);
    
    if (!hasNewImage) {
        QPixmap pixmap = loadImageFromPath(currentImagePath);
        
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imagePreview->setPixmap(scaledPixmap);
            imagePreview->setScaledContents(false);
        } else {
            imagePreview->setText("Immagine non disponibile");
        }
    }
}

QPixmap ModifyPage::loadImageFromPath(const QString& imagePath) {
    QString resourcePath = ":/Immagini/" + imagePath;
    QPixmap pixmap(resourcePath);
    
    if (pixmap.isNull() && !imagePath.isEmpty()) {
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
    
    return pixmap;
}