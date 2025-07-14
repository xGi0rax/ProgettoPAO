#include "../Headers/AddPage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QFileInfo>
#include <QDir>
#include <QFile>

AddPage::AddPage(QWidget *parent) : QWidget(parent), currentWidget(nullptr) {
    setupUI();
    showSelectionPage();
}

void AddPage::setBiblioteca(Biblioteca* biblio) {
    biblioteca = biblio;
}


// ====================================
// METODI DI INIZIALIZZAZIONE UI
// ====================================

void AddPage::setupUI() {
    QHBoxLayout *mainHLayout = new QHBoxLayout(this);
    mainHLayout->setContentsMargins(0, 0, 0, 0);
    mainHLayout->setSpacing(0);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setMinimumSize(300, 0);

    // Setup sidebar
    QWidget *sidebarWidget = new QWidget();
    sidebarWidget->setObjectName("sidebarWidget");
    setupSidebar(sidebarWidget);
    splitter->addWidget(sidebarWidget);

    // Setup main content area
    mainContentStack = new QStackedWidget();
    
    setupSelectionPage();
    mainContentStack->addWidget(selectionWidget);

    detailsStackedWidget = new QStackedWidget();
    mainContentStack->addWidget(detailsStackedWidget);

    splitter->addWidget(mainContentStack);
    
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 3);
    
    mainHLayout->addWidget(splitter);
    setLayout(mainHLayout);
}

void AddPage::setupSidebar(QWidget* sidebarWidget) {
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
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
    connect(backButton, &QPushButton::clicked, this, &AddPage::onBackButtonClicked);
    sidebarLayout->addWidget(backButton, 0, Qt::AlignTop | Qt::AlignLeft);

    setupImageSection(sidebarLayout);

    QPushButton *addButton = new QPushButton("CREA MEDIA");
    addButton->setMinimumSize(190, 40);
    addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(0, 128, 0);"
        "   color: white;"
        "   border: 2px solid rgb(119, 114, 114);"
        "   border-radius: 6px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(0, 100, 0);"
        "}"
    );
    connect(addButton, &QPushButton::clicked, this, &AddPage::onAddButtonClicked);
    sidebarLayout->addWidget(addButton, 0, Qt::AlignBottom | Qt::AlignCenter);
}

void AddPage::setupImageSection(QVBoxLayout* sidebarLayout) {
    imagePreview = new QLabel("Seleziona un'immagine");
    imagePreview->setMinimumSize(220, 220);
    imagePreview->setMaximumSize(300, 300);
    imagePreview->setScaledContents(true);
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setStyleSheet(
        "   border: 1px dashed #666666;"
        "   border-radius: 4px;"
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
    connect(uploadButton, &QPushButton::clicked, this, &AddPage::onUploadButtonClicked);
    
    QVBoxLayout *imageLayout = new QVBoxLayout();
    imageLayout->setContentsMargins(20, 20, 20, 20);
    imageLayout->addWidget(imagePreview, 1);
    imageLayout->addWidget(uploadButton, 0, Qt::AlignCenter);

    sidebarLayout->addLayout(imageLayout, 1);
}

void AddPage::setupSelectionPage() {
    selectionWidget = new QWidget(this);

    QVBoxLayout *selectionLayout = new QVBoxLayout(selectionWidget);
    selectionLayout->setContentsMargins(40, 40, 40, 40);
    selectionLayout->setSpacing(20);

    QLabel *selectionLabel = new QLabel("Seleziona il tipo di media", selectionWidget);
    selectionLabel->setAlignment(Qt::AlignCenter);
    selectionLabel->setStyleSheet(
        "background-color: #444444; "
        "font-size: 22px; "
        "font-weight: bold; "
        "color: white; "
        "margin-bottom: 10px; "
        "border: 2px solid black; "
        "border-radius: 8px; "
        "padding: 10px;"
    );
    selectionLayout->addWidget(selectionLabel);

    setupRadioButtons();

    QVBoxLayout *radioLayout = new QVBoxLayout();
    radioLayout->addWidget(filmRadio);
    radioLayout->addWidget(libroRadio);
    radioLayout->addWidget(vinileRadio);
    radioLayout->addWidget(rivistaRadio);
    radioLayout->addWidget(giocoRadio);

    selectionLayout->addLayout(radioLayout);
    selectionLayout->addSpacing(15);

    confirmTypeButton = new QPushButton("Conferma", selectionWidget);
    confirmTypeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(255, 208, 0);"
        "   color: black;"
        "   border: 2px solid rgb(119, 114, 114);"
        "   border-radius: 6px;"
        "   font-size: 18px;"
        "   min-width: 150px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(255, 170, 0);"
        "}"
    );
    connect(confirmTypeButton, &QPushButton::clicked, this, &AddPage::onConfirmTypeButtonClicked);

    selectionLayout->addWidget(confirmTypeButton, 0, Qt::AlignCenter);

    selectionLayout->addStretch(1);
    selectionLayout->insertStretch(0, 1);
}

void AddPage::setupRadioButtons() {
    mediaTypeGroup = new QButtonGroup(this);
    filmRadio = new QRadioButton("Film", selectionWidget);
    libroRadio = new QRadioButton("Libro", selectionWidget);
    vinileRadio = new QRadioButton("Vinile", selectionWidget);
    rivistaRadio = new QRadioButton("Rivista", selectionWidget);
    giocoRadio = new QRadioButton("Gioco da Tavolo", selectionWidget);

    QString radioStyle = getRadioButtonStyle();
    filmRadio->setStyleSheet(radioStyle);
    libroRadio->setStyleSheet(radioStyle);
    vinileRadio->setStyleSheet(radioStyle);
    rivistaRadio->setStyleSheet(radioStyle);
    giocoRadio->setStyleSheet(radioStyle);

    mediaTypeGroup->addButton(filmRadio, FILM);
    mediaTypeGroup->addButton(libroRadio, LIBRO);
    mediaTypeGroup->addButton(vinileRadio, VINILE);
    mediaTypeGroup->addButton(rivistaRadio, RIVISTA);
    mediaTypeGroup->addButton(giocoRadio, GIOCO_DA_TAVOLO);
}

// ================================
// SLOTS - GESTIONE UI
// ================================

void AddPage::onBackButtonClicked() {
    if (mainContentStack->currentWidget() == selectionWidget) {
        emit goBackToMainPage();
    } else if (mainContentStack->currentWidget() == detailsStackedWidget) {
        showSelectionPage();
    }
}

void AddPage::onConfirmTypeButtonClicked() {
    if (!mediaTypeGroup->checkedButton()) {
        QMessageBox::warning(this, "Errore", "Seleziona un tipo di media!");
        return;
    }

    int selectedId = mediaTypeGroup->checkedId();
    if (selectedId < 0) {
        QMessageBox::warning(this, "Errore", "ID non valido");
        return;
    }

    if (currentWidget) {
        detailsStackedWidget->removeWidget(currentWidget);
        delete currentWidget;
        currentWidget = nullptr;
    }
    
    MediaType mediaType = static_cast<MediaType>(selectedId);
    
    switch (mediaType) {
        case FILM:
            currentWidget = new FilmWidget();
            break;
        case LIBRO:
            currentWidget = new LibroWidget();
            break;
        case VINILE:
            currentWidget = new VinileWidget();
            break;
        case RIVISTA:
            currentWidget = new RivistaWidget();
            break;
        case GIOCO_DA_TAVOLO:
            currentWidget = new GiocoWidget();
            break;
        default:
            QMessageBox::warning(this, "Errore", "Tipo di media non valido!");
            return;
    }
    
    detailsStackedWidget->addWidget(currentWidget);
    detailsStackedWidget->setCurrentWidget(currentWidget);
    mainContentStack->setCurrentWidget(detailsStackedWidget);
}

void AddPage::onAddButtonClicked() {
    if (!currentWidget) {
        QMessageBox::warning(this, "Errore", "Nessun widget attivo trovato!");
        return;
    }

    if (!currentWidget->validateData()) {
        QMessageBox::warning(this, "Errore", "Tutti i campi sono obbligatori!");
        return;
    }
    
    Media* newMedia = currentWidget->createMedia();
    if (!newMedia) {
        QMessageBox::warning(this, "Errore", "Errore nella creazione del media!");
        return;
    }

    if (!selectedImagePath.isEmpty()) {
        newMedia->setImmagine(selectedImagePath.toStdString());
    }

    if (biblioteca && biblioteca->esisteMedia(newMedia->getTitolo(), newMedia->getAutore(), newMedia->getAnno())) {
        if (handleExistingMedia(newMedia)) {
            return;
        }
    }
    
    biblioteca->aggiungiMedia(newMedia);
    QMessageBox::information(this, "Successo", "Media aggiunto con successo");
    
    emit mediaCreated();
    showSelectionPage();
    emit goBackToMainPage();
}

void AddPage::onUploadButtonClicked() {
    QString imagePath = QFileDialog::getOpenFileName(this, 
        "Seleziona un'immagine", "", "Immagini (*.png *.jpg *.jpeg)");
    
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            imagePreview->setPixmap(pixmap);
            copyImageToProject(imagePath);
        } else {
            QMessageBox::warning(this, "Errore", "Impossibile caricare l'immagine selezionata.");
        }
    }
}

// ====================================
// METODI DI GESTIONE
// ====================================

void AddPage::showSelectionPage() {
    if (mediaTypeGroup->checkedButton()) {
        mediaTypeGroup->setExclusive(false);
        mediaTypeGroup->checkedButton()->setChecked(false);
        mediaTypeGroup->setExclusive(true);
    }
    
    mainContentStack->setCurrentWidget(selectionWidget);
}

bool AddPage::handleExistingMedia(Media* newMedia) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Media già esistente", 
        "Un media con lo stesso titolo, autore e anno esiste già nella biblioteca. "
        "Vuoi aumentare il numero di copie di questo media presenti in biblioteca?", 
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Media* mediaEsistente = biblioteca->cercaMediaDaT_A_A(
            newMedia->getTitolo(), newMedia->getAutore(), newMedia->getAnno());
        mediaEsistente->setNumeroCopie(mediaEsistente->getNumeroCopie() + 1);
        
        delete newMedia;
        QMessageBox::information(this, "Salvataggio", 
            "Numero copie del media aumentate con successo!");

        emit mediaCopiesIncreased();
        showSelectionPage();
        emit goBackToMainPage();
        return true;
    }
    
    if (reply == QMessageBox::No) {
        delete newMedia;
        return true;
    }
    
    return false;
}

void AddPage::copyImageToProject(const QString& imagePath) {
    QFileInfo fileInfo(imagePath);
    QString fileName = fileInfo.fileName();
    
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
        dir.mkpath(projectImagesDir);
    }
    
    QString destinationPath = projectImagesDir + "/" + fileName;
    
    if (QFile::copy(imagePath, destinationPath)) {
        selectedImagePath = fileName;
    } else {
        if (QFile::exists(destinationPath)) {
            selectedImagePath = fileName;
        } else {
            QMessageBox::warning(this, "Errore", 
                "Impossibile copiare l'immagine nella cartella del progetto: " + destinationPath);
            selectedImagePath = "";
        }
    }
}

QString AddPage::getRadioButtonStyle() const {
    return "QRadioButton {"
           "   font-size: 18px;"
           "   color: white;"
           "   padding: 12px;"
           "   background-color: rgb(60, 85, 140);"
           "   border-radius: 8px;"
           "   margin: 3px 0px;"
           "}"
           "QRadioButton:hover {"
           "   background-color: rgb(80, 105, 160);"
           "}"
           "QRadioButton:checked {"
           "   background-color: rgb(255, 208, 0);"
           "   border: 2px solid rgb(119, 114, 114);"
           "   color: black;"
           "}"
           "QRadioButton::indicator {"
           "   width: 20px;"
           "   height: 20px;"
           "}";
}