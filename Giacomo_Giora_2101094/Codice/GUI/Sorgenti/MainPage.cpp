#include <QFile> 
#include <QScrollBar>
#include <QModelIndex>
#include "../Headers/MainPage.h"
#include "../../Modello logico/Headers/Media.h"
#include "../../Modello logico/Headers/Libro.h"
#include "../../Modello logico/Headers/Film.h"
#include "../../Modello logico/Headers/Vinile.h"
#include "../../Modello logico/Headers/GiocoDaTavolo.h"
#include "../../Modello logico/Headers/Rivista.h"
#include "../../Modello logico/Headers/Biblioteca.h"
#include "../../Modello logico/Headers/IOStrategy.h"
#include "../../Modello logico/Headers/JsonIO.h"
#include "../../Modello logico/Headers/XmlIO.h"
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>

MainPage::MainPage(QWidget *parent, Biblioteca* biblio) : QWidget(parent) {
    biblioteca = biblio;

    // Inizializzazione stato biblioteca
    hasCurrentFile = false;
    currentFilePath = "";
    isNewLibrary = true;
    hasUnsavedChanges = false;

    setupUI();
}

// ======================================
// METODI DI INIZIALIZZAZIONE (Setup)
// ======================================
void MainPage::setupUI(){
    setupTopBar();
    setupFilters();
    setupMediaList();
    setupPreviewPanel();
    setupMainLayout();
    setupStyles();
}

void MainPage::setupTopBar() {
    // Pulsanti della barra superiore
    backButton = new QPushButton("Indietro");
    addMediaButton = new QPushButton("Aggiungi Media");
    saveButton = new QPushButton("Salva");
    saveAsButton = new QPushButton("Salva come");

    QList<QPushButton*> topButtons = {backButton, addMediaButton, saveButton, saveAsButton};
    for (QPushButton* btn : topButtons) {
        btn->setMinimumSize(100, 30);
    }
    saveAsButton->setMinimumWidth(120);
    
    saveButton->setObjectName("saveButton");
    saveAsButton->setObjectName("saveAsButton");

    // Connessioni
    connect(backButton, &QPushButton::clicked, this, &MainPage::onBackButtonClicked);
    connect(addMediaButton, &QPushButton::clicked, this, &MainPage::onAddMediaButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &MainPage::onSaveButtonClicked);
    connect(saveAsButton, &QPushButton::clicked, this, &MainPage::onSaveAsButtonClicked);

    // Layout
    topBarLayout = new QHBoxLayout();
    topBarLayout->addWidget(backButton, 1);
    topBarLayout->addWidget(addMediaButton, 5);
    topBarLayout->addWidget(saveButton, 1);
    topBarLayout->addWidget(saveAsButton, 1);
}

void MainPage::setupFilters(){
    // Selezione tipo del media
    mediaTypeComboBox = new QComboBox();
    mediaTypeComboBox->addItem("Qualsiasi");
    mediaTypeComboBox->addItem("Libro");
    mediaTypeComboBox->addItem("Film");
    mediaTypeComboBox->addItem("Vinile");
    mediaTypeComboBox->addItem("Rivista");
    mediaTypeComboBox->addItem("Gioco da tavolo");
    
    // Selezione genere del media, popolato dopo la selezione del tipo
    genreComboBox = new QComboBox();
    genreComboBox->addItem("Qualsiasi genere");
    genreComboBox->setEnabled(false); 
    genreComboBox->setToolTip("Seleziona prima un tipo di media specifico");
    updateGenreComboBox(); // Popola i generi in base al tipo selezionato

    // Connessione del cambiamento del tipo media alla funzione di aggiornamento dei generi visualizzati
    connect(mediaTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainPage::onMediaTypeChanged);

    // Campi di input per rating
    ratingMinLineEdit = new QLineEdit();
    ratingMinLineEdit->setPlaceholderText("0.0");
    ratingMaxLineEdit = new QLineEdit();
    ratingMaxLineEdit->setPlaceholderText("5.0");

    // Checkbox per disponibilità
    availableCheckBox = new QCheckBox("Disponibile");
    availableCheckBox->setChecked(true); // Selezionato di default

    // Campi di input per lingua
    languageLineEdit = new QLineEdit();
    languageLineEdit->setPlaceholderText("Lingua");

    // Campi di input per anno
    minYearLineEdit = new QLineEdit();
    minYearLineEdit->setPlaceholderText("anno min");
    maxYearLineEdit = new QLineEdit();
    maxYearLineEdit->setPlaceholderText("anno max");

    // Pulsanti per applicare ed eliminare i filtri
    applyFiltersButton = new QPushButton("Applica filtri");
    clearFiltersButton = new QPushButton("Cancella filtri");

    // Layout filtri
    filtersLayout = new QVBoxLayout();
    filtersLayout->addWidget(new QLabel("Tipo media:"));
    filtersLayout->addWidget(mediaTypeComboBox);
    filtersLayout->addWidget(new QLabel("Genere:")); 
    filtersLayout->addWidget(genreComboBox);
    filtersLayout->addWidget(new QLabel("Rating:"));
    filtersLayout->addWidget(ratingMinLineEdit);
    filtersLayout->addWidget(ratingMaxLineEdit);
    filtersLayout->addWidget(new QLabel("Disponibilità:"));
    filtersLayout->addWidget(availableCheckBox);
    filtersLayout->addWidget(new QLabel("Lingua:"));
    filtersLayout->addWidget(languageLineEdit);
    filtersLayout->addWidget(new QLabel("Anno minimo:"));
    filtersLayout->addWidget(minYearLineEdit);
    filtersLayout->addWidget(new QLabel("Anno massimo:"));
    filtersLayout->addWidget(maxYearLineEdit);
    filtersLayout->addStretch();
    filtersLayout->addWidget(applyFiltersButton);
    filtersLayout->addWidget(clearFiltersButton);

    filtersGroupBox = new QGroupBox("Filtri");
    filtersGroupBox->setLayout(filtersLayout);
    filtersGroupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Connessioni per i pulsanti dei filtri
    connect(applyFiltersButton, &QPushButton::clicked, this, &MainPage::onApplyFiltersClicked);
    connect(clearFiltersButton, &QPushButton::clicked, this, &MainPage::onClearFiltersClicked);
}

void MainPage::setupMediaList() {
    // Barra di ricerca
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca per titolo o autore...");
    connect(searchBar, &QLineEdit::textChanged, this, &MainPage::onSearchTextChanged);

    // Lista dei media
    mediaList = new QListWidget();
    mediaList->setViewMode(QListView::ListMode);
    mediaList->setResizeMode(QListView::Adjust);
    mediaList->setMovement(QListView::Static);
    mediaList->setSelectionMode(QAbstractItemView::SingleSelection);
    mediaList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mediaList->setFocusPolicy(Qt::NoFocus);
    mediaList->setIconSize(QSize(27, 27));

    // Popolamento della lista dei media
    vector<Media*> listaMedia = biblioteca->getListaMedia();
    updateMediaList(listaMedia);

    // Connessioni lista
    connect(mediaList, &QListWidget::itemClicked, this, &MainPage::onMediaSelected);
    connect(mediaList->verticalScrollBar(), &QScrollBar::valueChanged, this, &MainPage::onScrollChanged);
    
    // Layout centrale
    centerLayout = new QVBoxLayout();
    centerLayout->addWidget(searchBar);
    centerLayout->addWidget(mediaList);

    // Widget dei pulsanti da visualizzare quando un elemento è selezionato
    buttonsContainer = new QWidget(mediaList);
    buttonsContainer->hide(); // inizialmente nascosto
    
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsContainer);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(6);
    
    // Pulsante modifica
    listEditButton = new QPushButton();
    listEditButton->setToolTip("Modifica");
    listEditButton->setFixedSize(26, 26);
    QPixmap editIcon(":/Immagini/BottoneMatita.png");
    listEditButton->setIcon(QIcon(editIcon));
    listEditButton->setIconSize(QSize(16, 16));
    listEditButton->setStyleSheet(
        "QPushButton {"
    "   background-color: rgb(131, 187, 222);"
    "   color: black;"
    "   border: 2px solid rgb(92, 92, 93);"
    "   border-radius: 13px;"
    "   font-size: 12px;"
    "   font-weight: bold;"
    "   text-align: center;"
    "   padding: 2px;"
    "}"
        "QPushButton:hover {"
        "   background-color: rgb(87, 163, 210);"
        "}"
    );
    
    // Pulsante elimina
    listDeleteButton = new QPushButton();
    listDeleteButton->setToolTip("Rimuovi");
    listDeleteButton->setFixedSize(26, 26);
    QPixmap deleteIcon(":/Immagini/BottoneCestino.png");
    listDeleteButton->setIcon(QIcon(deleteIcon));
    listDeleteButton->setIconSize(QSize(20, 20));
    listDeleteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(240, 107, 70);"
        "   color: black;"
        "   border: 2px solid rgb(92, 92, 93);"
        "   border-radius: 13px;"
        "   font-size: 12px;"
        "   font-weight: bold;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(250, 81, 34);"
        "}"
    );
    
    // Layout pulsanti
    buttonsLayout->addWidget(listEditButton);
    buttonsLayout->addWidget(listDeleteButton);
    
    // Connessioni
    connect(listEditButton, &QPushButton::clicked, this, &MainPage::onEditButtonClicked);
    connect(listDeleteButton, &QPushButton::clicked, this, &MainPage::onDeleteButtonClicked);
    connect(mediaList, &QListWidget::currentRowChanged, this, &MainPage::hideActionButtons);
}

void MainPage::setupPreviewPanel() {
    previewGroupBox = new QGroupBox("Anteprima");
    
    // Label per l'immagine del media
    mediaImageLabel = new QLabel();
    mediaImageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    mediaImageLabel->setAlignment(Qt::AlignCenter);
    mediaImageLabel->setMinimumSize(150, 150);
    mediaImageLabel->setStyleSheet("border: 1px solid black; background-color: white; color: gray; font-size: 14px;");
    mediaImageLabel->setText("Nessuna immagine");

    // Label per le informazioni del media
    mediaTitleLabel = new QLabel();
    mediaTitleLabel->setMinimumWidth(150);
    mediaTitleLabel->setWordWrap(true);
    mediaTitleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    mediaAuthorLabel = new QLabel("Seleziona un media per vedere i dettagli");
    mediaAuthorLabel->setWordWrap(true);
    
    mediaYearLabel = new QLabel();
    mediaRatingLabel = new QLabel();

    // Pulsanti azioni
    borrowButton = new QPushButton("Prendi in prestito");
    returnButton = new QPushButton("Restituisci");
    detailsButton = new QPushButton("Approfondisci");
    editMediaButton = new QPushButton("Modifica media");

    // Connessioni pulsanti
    connect(borrowButton, &QPushButton::clicked, this, &MainPage::onBorrowButtonClicked);
    connect(returnButton, &QPushButton::clicked, this, &MainPage::onReturnButtonClicked);
    connect(detailsButton, &QPushButton::clicked, this, &MainPage::onDetailsButtonClicked);
    connect(editMediaButton, &QPushButton::clicked, this, &MainPage::onEditButtonClicked);

    // Layout per la sezione di anteprima
    previewLayout = new QVBoxLayout();
    previewLayout->addWidget(mediaImageLabel);
    previewLayout->addWidget(mediaTitleLabel);
    previewLayout->addWidget(mediaAuthorLabel);
    previewLayout->addWidget(mediaYearLabel);
    previewLayout->addWidget(mediaRatingLabel);
    previewLayout->addStretch();
    previewLayout->addWidget(borrowButton);
    previewLayout->addWidget(returnButton);
    previewLayout->addWidget(detailsButton);
    previewLayout->addWidget(editMediaButton);

    previewGroupBox->setLayout(previewLayout);
    previewGroupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    previewGroupBox->setMinimumWidth(200);
    previewGroupBox->setMaximumWidth(400);
    filtersGroupBox->setMinimumWidth(200);
    filtersGroupBox->setMaximumWidth(400);
}

void MainPage::setupMainLayout() {
    // Layout generale
    contentLayout = new QHBoxLayout();
    contentLayout->addWidget(filtersGroupBox, 1);
    contentLayout->addLayout(centerLayout, 2);
    contentLayout->addWidget(previewGroupBox, 1);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topBarLayout);
    mainLayout->addLayout(contentLayout);

    setLayout(mainLayout);
}

void MainPage::setupStyles() {
    // Stili pulsanti barra superiore
    backButton->setStyleSheet(getButtonStyle("rgb(0, 104, 201)", "rgb(11, 82, 189)"));
    saveButton->setStyleSheet(getButtonStyle("rgb(0, 153, 51)", "rgb(0, 128, 43)"));
    saveAsButton->setStyleSheet(getButtonStyle("rgb(1, 175, 191)", "rgb(3, 141, 154)"));
    
    // Stili pulsanti anteprima
    borrowButton->setStyleSheet(getButtonStyle("rgb(0, 104, 201)", "rgb(11, 82, 189)"));
    returnButton->setStyleSheet(getButtonStyle("rgb(0, 104, 201)", "rgb(11, 82, 189)"));
    
    // Stili pulsanti filtri
    applyFiltersButton->setStyleSheet(getButtonStyle("rgb(255, 208, 0)", "rgb(255, 170, 0)", "black"));
    clearFiltersButton->setStyleSheet(getButtonStyle("rgb(255, 69, 0)", "rgb(200, 50, 0)"));

    // Stile per la barra di ricerca
    searchBar->setStyleSheet(
        "QLineEdit { "
        "   background-color: rgb(255, 208, 0); "
        "   color: black; "
        "   border: 2px solid rgb(119, 114, 114); "
        "   padding: 5px; "
        "   border-radius: 4px; "
        "   font-size: 12px; "
        "}"
    );
    
    // Stile lista media
    mediaList->setStyleSheet(
        "QListWidget { "
        "   background-color: rgb(243, 238, 238); "
        "   border: 2px solid rgb(119, 114, 114); "
        "   border-radius: 4px; "
        "   font-size: 14px; "
        "}"
        "QListWidget::item { "
        "   border-bottom: 3px solid #ddd; "
        "   padding: 8px 4px; "
        "   color: black; "
        "}"
        "QListWidget::item:hover { "
        "   background-color: rgb(151, 168, 190); "
        "   color: white; "
        "}"
        "QListWidget::item:selected { "
        "   background-color: rgb(255, 208, 0); "
        "   color: black; "
        "}"
        "QListWidget::item:focus { outline: none; }"
    );
}

// ===================
// METODI PUBBLICI
// ===================
void MainPage::setCurrentFile(const QString& filePath) {
    currentFilePath = filePath;
    hasCurrentFile = !filePath.isEmpty();
}

void MainPage::setLibraryInfo(bool isNew, bool hasChanges) {
    isNewLibrary = isNew;
    hasUnsavedChanges = hasChanges;
    updateSaveButtonsState();
}

void MainPage::resetUnsavedChanges() {
    hasUnsavedChanges = false;
    emit unsavedChangesUpdated(false);
}

void MainPage::setHasUnsavedChanges(bool hasChanges) {
    hasUnsavedChanges = hasChanges;
    emit unsavedChangesUpdated(hasChanges);
    updateSaveButtonsState();
}

// ==========================
// GESTIONE EVENTI
// ==========================
void MainPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    int maxImageWidth = this->width() / 3;
    
    int groupBoxMargins = 5;
    int layoutMargins = previewLayout->contentsMargins().left() + previewLayout->contentsMargins().right();
    int totalMargins = groupBoxMargins + layoutMargins - 3; 
    
    int imageWidth = qMin(previewGroupBox->width() - totalMargins, maxImageWidth - totalMargins);
    int imageHeight = previewGroupBox->height() / 3;
    
    imageWidth = qMax(150, imageWidth);
    imageHeight = qMax(150, imageHeight);
    
    mediaImageLabel->setFixedSize(imageWidth, imageHeight); 

    updateImageSize();
    updateTextTruncation();
}


// ===================================
// SLOTS
// ===================================

// a. Slots per la barra superiore
void MainPage::onBackButtonClicked() {
    emit goToChoicePage();
}

void MainPage::onAddMediaButtonClicked() {
    emit goToAddPage();
}

void MainPage::onSaveButtonClicked() {
    if (isNewLibrary) {
        QMessageBox::information(this, "Nuova biblioteca", "Per una nuova biblioteca, usa 'Salva come' per creare il file.");
        return;
    }

    if (!hasUnsavedChanges) {
        QMessageBox::information(this, "Nessuna modifica", "Non ci sono modifiche da salvare, la biblioteca è già aggiornata.");
        return;
    }
    
    if (hasCurrentFile && !currentFilePath.isEmpty()) {
        // Salva direttamente nel file corrente
        saveToFile(currentFilePath);
        hasUnsavedChanges = false;
        updateSaveButtonsState();

        emit unsavedChangesUpdated(false); 
    } else {
        // Se non c'è un file corrente, chiama "Salva come"
        onSaveAsButtonClicked();
    }
}

void MainPage::onSaveAsButtonClicked() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setWindowTitle("Salva biblioteca come");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilter("File JSON (*.json);;File XML (*.xml)");
    fileDialog.setDefaultSuffix("json");
    
    // Se c'è un file corrente, impostalo come directory di partenza
    if (hasCurrentFile && !currentFilePath.isEmpty()) {
        QFileInfo fileInfo(currentFilePath);
        fileDialog.setDirectory(fileInfo.absolutePath());
        fileDialog.selectFile(fileInfo.fileName());
    } else {
        fileDialog.setDirectory(QDir::currentPath());
    }
    
    if (fileDialog.exec()) {
        QStringList selectedFiles = fileDialog.selectedFiles();
        if (!selectedFiles.isEmpty()) {
            QString filePath = selectedFiles.first();
        
            QString selectedFilter = fileDialog.selectedNameFilter();
            if (selectedFilter.contains("*.json") && !filePath.endsWith(".json", Qt::CaseInsensitive)) {
                filePath += ".json";
            } else if (selectedFilter.contains("*.xml") && !filePath.endsWith(".xml", Qt::CaseInsensitive)) {
                filePath += ".xml";
            }
            
            // Salva nel file selezionato
            saveToFile(filePath);
            
            // Aggiorna lo stato dopo salvataggio riuscito
            setCurrentFile(filePath);
            isNewLibrary = false; // Non è più una nuova biblioteca
            hasUnsavedChanges = false; // Reset del flag modifiche
            updateSaveButtonsState();
            
            // Notifica il MainWindow che non ci sono più modifiche non salvate
            emit unsavedChangesUpdated(false);
        }
    }
}

// b. Slots per i filtri
void MainPage::onMediaTypeChanged() {
    updateGenreComboBox();

    int selectedIndex = mediaTypeComboBox->currentIndex();
    
    if (selectedIndex == 0) { // "Qualsiasi" è selezionato
        genreComboBox->setEnabled(false);
        genreComboBox->setToolTip("Seleziona prima un tipo di media specifico");
    } else {
        genreComboBox->setEnabled(true);
        genreComboBox->setToolTip("");
    }
}

void MainPage::onApplyFiltersClicked() {
    string mediaType = mediaTypeComboBox->currentText().toStdString();
    if (mediaType == "Qualsiasi") {
        mediaType = "";
    }

    string genre = genreComboBox->currentText().toStdString();
    if (genre == "Qualsiasi genere") {
        genre = "";
    }

    double ratingMin = ratingMinLineEdit->text().toDouble();
    double ratingMax = ratingMaxLineEdit->text().toDouble();
    if (ratingMax == 0) {
        ratingMax = 5.0;
    }

    string language = languageLineEdit->text().toStdString();
    string languageLower = QString::fromStdString(language).toLower().toStdString();

    int minYear = minYearLineEdit->text().toInt();
    int maxYear = maxYearLineEdit->text().toInt();
    if (maxYear == 0) {
        maxYear = 3000;
    }
    bool available = availableCheckBox->isChecked();

    vector<Media*> listaFiltrata = biblioteca->filtra("", mediaType, genre, ratingMin, ratingMax, available, languageLower, minYear, maxYear);

    if (listaFiltrata.empty()) {
        QMessageBox::information(this, "Nessun risultato", "Nessun media trovato con i filtri selezionati.");
    } else{
        updateMediaList(listaFiltrata);
    }    
}

void MainPage::onClearFiltersClicked() {
    mediaTypeComboBox->setCurrentIndex(0);
    genreComboBox->setCurrentIndex(0);
    genreComboBox->setEnabled(false);
    genreComboBox->setToolTip("Seleziona prima un tipo di media specifico");
    ratingMinLineEdit->clear();
    ratingMaxLineEdit->clear();
    languageLineEdit->clear();
    minYearLineEdit->clear();
    maxYearLineEdit->clear();
    availableCheckBox->setChecked(true);
    searchBar->clear();

    vector<Media*> listaCompleta = biblioteca->getListaMedia();
    updateMediaList(listaCompleta);
}

void MainPage::onSearchTextChanged(const QString& searchText) {
    vector<Media*> listaCompleta = biblioteca->getListaMedia();
    
    if (searchText.isEmpty()) {
        updateMediaList(listaCompleta);
        return;
    }
    
    vector<Media*> listaFiltrata;
    QString searchedLower = searchText.toLower();
    
    for (Media* media : listaCompleta) {
        QString titoloMedia = QString::fromStdString(media->getTitolo()).toLower();
        QString autoreMedia = QString::fromStdString(media->getAutore()).toLower();
        
        if (titoloMedia.contains(searchedLower) || autoreMedia.contains(searchedLower)) {
            listaFiltrata.push_back(media);
        }
    }
    
    updateMediaList(listaFiltrata);

    mediaList->clearSelection();
    clearPreviewPanel();
    hideActionButtons();
}

// c. Slots per la lista media
void MainPage::onMediaSelected(QListWidgetItem *item) {
    if (!item) return;
    
    int row = mediaList->row(item);
    showActionButtons(row);
    
    Media* media = item->data(Qt::UserRole).value<Media*>();
    updatePreviewPanel(media);
}

void MainPage::onScrollChanged() {
    // Aggiorna la posizione dei pulsanti quando la lista viene scrollata
    if (buttonsContainer->isVisible() && currentSelectedRow >= 0) {
        updateButtonsPosition();
    }
}

// d. Slots per azioni sui media
void MainPage::onEditButtonClicked() {
    Media* selectedMedia = getSelectedMedia();
    if (!selectedMedia) {
        QMessageBox::warning(this, "Errore", "Media selezionato non valido.");
        return;
    }
    
    emit goToModifyPage(selectedMedia);
}

void MainPage::onDeleteButtonClicked() {
    QListWidgetItem* currentItem = mediaList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Errore", "Nessun media selezionato per la rimozione.");
        return;
    }
    
    Media* selectedMedia = currentItem->data(Qt::UserRole).value<Media*>();
    if (!selectedMedia) {
        QMessageBox::warning(this, "Errore", "Media selezionato non valido.");
        return;
    }
    
    QString message = QString("Sei sicuro di voler rimuovere '%1' dalla biblioteca?")
                      .arg(QString::fromStdString(selectedMedia->getTitolo()));
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Conferma eliminazione", message, QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (biblioteca && biblioteca->rimuoviMedia(selectedMedia)) {
            hideActionButtons();
            clearPreviewPanel();
            currentSelectedRow = -1;
            
            // Rimozione dell'elemento dalla UI 
            int row = mediaList->row(currentItem);
            QListWidgetItem* removedItem = mediaList->takeItem(row);
            if (removedItem) {
                delete removedItem;
            }
            
            // Aggiornamento stato modifiche
            hasUnsavedChanges = true;
            emit unsavedChangesUpdated(true);
            updateSaveButtonsState();
            
            if (mediaList->count() == 0) {
                mediaList->clearSelection();
            }            
        } else {
            QMessageBox::warning(this, "Errore", 
                "Impossibile rimuovere il media dalla biblioteca. Riprova.");
        }
    }
}

void MainPage::onBorrowButtonClicked() {
    Media* selectedMedia = getSelectedMedia();
    if (!selectedMedia) return;
    
    emit borrowMedia(selectedMedia);
}

void MainPage::onReturnButtonClicked() {
    Media* selectedMedia = getSelectedMedia();
    if (!selectedMedia) return;
    
    emit returnMedia(selectedMedia);
}

void MainPage::onDetailsButtonClicked() {
    Media* selectedMedia = getSelectedMedia();
    if (!selectedMedia) {
        QMessageBox::warning(this, "Errore", "Nessun elemento selezionato.");
        return;
    }
    
    emit goToDetailsPage(selectedMedia);
}

// e. Slots per aggiornamenti da altre pagine
void MainPage::onMediaCreated() {
    updateMediaList(biblioteca->getListaMedia());
    hasUnsavedChanges = true;
    emit unsavedChangesUpdated(true);
    updateSaveButtonsState();
}

void MainPage::onMediaModified() {
    updateMediaList(biblioteca->getListaMedia());
    hasUnsavedChanges = true;
    emit unsavedChangesUpdated(true);
    updateSaveButtonsState();
}

void MainPage::onMediaCopiesIncreased() {
    updateMediaList(biblioteca->getListaMedia());
    hasUnsavedChanges = true;
    emit unsavedChangesUpdated(true);
    updateSaveButtonsState();
}

// ===============================
// METODI DI AGGIORNAMENTO UI
// ===============================
void MainPage::updateMediaList(vector<Media*> listaFiltrata) {
    mediaList->clear();

    for (Media* media : listaFiltrata) {
        QString mediaInfo = media->mediaInfo();

        // Calcolo spazio disponibile per il testo
        int listWidth = mediaList->width();
        int buttonSpace = 70; // Spazio riservato per i pulsanti (modifica + elimina)
        int iconSpace = 35; // Spazio per l'icona + margini
        int scrollBarSpace = 20; // Spazio per la scrollbar
        int availableWidth = listWidth - buttonSpace - iconSpace - scrollBarSpace;

        // Troncamento testo se necessario
        QFontMetrics fontMetrics(mediaList->font());
        QString truncatedText = mediaInfo;
        
        if (fontMetrics.horizontalAdvance(mediaInfo) > availableWidth) {
            truncatedText = fontMetrics.elidedText(mediaInfo, Qt::ElideRight, availableWidth);
        }

        // Creazione elemento della lista
        QListWidgetItem *item = new QListWidgetItem(mediaList);
        item->setText(truncatedText);
        item->setToolTip(mediaInfo);

        // Impostazione icona in base al tipo di media
        QString iconPath;
        if (dynamic_cast<Libro*>(media)) {
            iconPath = ":/Immagini/LogoLibro1.png";
        } 
        else if (dynamic_cast<Film*>(media)) {
            iconPath = ":/Immagini/LogoFilm1.png";
        } 
        else if (dynamic_cast<Vinile*>(media)) {
            iconPath = ":/Immagini/LogoVinile1.png";
        } 
        else if (dynamic_cast<Rivista*>(media)) {
            iconPath = ":/Immagini/LogoRivista1.png";
        } 
        else if (dynamic_cast<GiocoDaTavolo*>(media)) {
            iconPath = ":/Immagini/LogoGioco1.png";
        }
        
        if (!iconPath.isEmpty()) {
            QIcon icon(iconPath);
            item->setIcon(icon);
        }
        
        item->setSizeHint(QSize(mediaList->width(), 48));
        item->setData(Qt::UserRole, QVariant::fromValue(media));
    }
}

void MainPage::updatePreviewPanel(Media* media) {
    if (!media) {
        clearPreviewPanel();
        return;
    }
    
    // Aggiorno l'anteprima con le informazioni del media selezionato
    mediaTitleLabel->setText(QString::fromStdString(media->getTitolo()));
    mediaTitleLabel->setStyleSheet("font-weight: bold; font-size: 20px;");
    mediaAuthorLabel->setText(QString::fromStdString(media->getAutore()));
    mediaAuthorLabel->setStyleSheet("font-size: 16px;");
    mediaYearLabel->setText(QString::number(media->getAnno()));
    mediaYearLabel->setStyleSheet("font-size: 14px;");

    double rating = media->getRating();
    QString stars = QString("Rating: %1 %2").arg(QString("★").repeated(static_cast<int>(rating))).arg(QString::number(rating, 'f', 1));
    mediaRatingLabel->setText(stars);
    mediaRatingLabel->setStyleSheet("font-size: 14px;");

    updateMediaImage(media);
   
    borrowButton->setEnabled(true);
    returnButton->setEnabled(true);
    detailsButton->setEnabled(true);
    editMediaButton->setEnabled(true);
}

void MainPage::updateMediaImage(Media* media) {
    QString imagePath = QString::fromStdString(media->getImmagine());
    
    QPixmap pixmap;
    QDir currentDir = QDir::current();
    
    // Prova di diverse possibili ubicazioni per i percorsi dell'immagine
    QStringList possiblePaths = {
        currentDir.absoluteFilePath("Immagini/" + imagePath),
        currentDir.absoluteFilePath("../Immagini/" + imagePath),
        currentDir.absoluteFilePath("../../Immagini/" + imagePath),
        currentDir.absoluteFilePath("GUI/Immagini/" + imagePath),
        currentDir.absoluteFilePath("../GUI/Immagini/" + imagePath)
    };
    
    for (const QString& path : possiblePaths) {
        if (QFile::exists(path)) {
            pixmap.load(path);
            if (!pixmap.isNull()) {
                break;
            }
        }
    }

    if (!pixmap.isNull()) {
        originalPixmap = pixmap;
        mediaImageLabel->setMinimumSize(150, 150);
        mediaImageLabel->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        updateImageSize();
    } else {
        mediaImageLabel->setText("Immagine non disponibile");
        mediaImageLabel->setStyleSheet(
            "border: 1px solid black;"
            "background-color: white;"
            "color: gray;"
            "padding: 5px;"
        );
    }
}

void MainPage::updateImageSize(){
    if (!originalPixmap.isNull()) {
        int maxImageWidth = this->width() / 3;
        int groupBoxMargins =  5;
        int layoutMargins = previewLayout->contentsMargins().left() + previewLayout->contentsMargins().right();
        int totalMargins = groupBoxMargins + layoutMargins - 3;
        
        int availableWidth = qMin(previewGroupBox->width() - totalMargins, maxImageWidth - totalMargins); 
        int availableHeight = previewGroupBox->height() / 3;
        
        availableWidth = qMax(150, availableWidth);
        availableHeight = qMax(150, availableHeight);
        
        QSize newSize = originalPixmap.size();
        newSize.scale(availableWidth, availableHeight, Qt::KeepAspectRatio);

        QPixmap background(availableWidth, availableHeight);
        background.fill(Qt::white);
        
        QPainter painter(&background);
        int x = (availableWidth - newSize.width()) / 2;
        int y = (availableHeight - newSize.height()) / 2;
        painter.drawPixmap(x, y, originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        mediaImageLabel->setPixmap(background);
        mediaImageLabel->setFixedSize(availableWidth, availableHeight);
    } 
}

void MainPage::updateGenreComboBox() {
    genreComboBox->clear();
    genreComboBox->addItem("Qualsiasi genere");
    
    int currentType = mediaTypeComboBox->currentIndex();
    
    switch(currentType) {
        case 1: // Libro
            genreComboBox->addItems({"Avventura", "Biografia", "Fantasy", "Fantascienza", "Giallo", "Horror", "Narrativa", "Poesia", "Romanzo", "Saggistica", "Storico", "Thriller", "Altro"});
            break;
        case 2: // Film
            genreComboBox->addItems({"Animazione", "Azione", "Avventura", "Biografica", "Commedia", "Crime", "Documentario", "Drammatico", "Fantascienza", "Fantasy", "Horror", "Musical", "Mistero", "Romantico", "Thriller", "Western", "Altro"});
            break;
        case 3: // Vinile
            genreComboBox->addItems({"Alternative", "Blues", "Classica", "Country", "Disco", "Elettronica", "Folk", "Funk", "Hip Hop", "Jazz", "Metal", "Pop", "Punk", "Reggae", "Rock", "Soul", "Altro"});
            break;
        case 4: // Rivista
            genreComboBox->addItems({"Attualità", "Arte", "Cucina","Culturale", "Economia", "Intrattenimento", "Informatica", "Moda", "Natura", "Salute","Scienza", "Sport", "Tecnologia", "Viaggi", "Altro"});
            break;
        case 5: // Gioco da tavolo
            genreComboBox->addItems({"Avventura", "Carte", "Cooperativo", "Deduzione", "Economico", "Fantasy", "Guerra", "Party game", "Strategia", "Altro"});
            break;
        default: // Qualsiasi o non specificato
            break;
    }
}

void MainPage::updateSaveButtonsState() {
    QPushButton* saveButton = findChild<QPushButton*>("saveButton");
    QPushButton* saveAsButton = findChild<QPushButton*>("saveAsButton");
    
    if (saveButton && saveAsButton) {
        if (isNewLibrary) {
            // Per nuove biblioteche: "Salva" disibalitato, "Salva come" abilitato
            saveButton->setEnabled(false);
            saveButton->setToolTip("Usa 'Salva come' per creare un nuovo file");
            saveAsButton->setEnabled(true);
        } else {
            saveButton->setEnabled(true);

            if (!hasCurrentFile) {
                saveButton->setToolTip("Nessun file corrente");
            } else if (!hasUnsavedChanges) {
                saveButton->setToolTip("Nessuna modifica da salvare");
            } else {
                saveButton->setToolTip("Salva nel file corrente");
            }
            
            saveAsButton->setEnabled(true);
        }
    }
}

void MainPage::updateTextTruncation() {
    // Aggiornamento troncamento del testo per tutti gli elementi visibili
    for (int i = 0; i < mediaList->count(); ++i) {
        QListWidgetItem* item = mediaList->item(i);
        if (!item) continue;
        
        Media* media = item->data(Qt::UserRole).value<Media*>();
        if (!media) continue;
    
        QString fullText = media->mediaInfo();
        
        int listWidth = mediaList->width();
        int buttonSpace = 70;
        int iconSpace = 35;
        int scrollBarSpace = 20;
        int availableWidth = listWidth - buttonSpace - iconSpace - scrollBarSpace;
        
        QFontMetrics fontMetrics(mediaList->font());
        QString truncatedText = fullText;
        
        if (fontMetrics.horizontalAdvance(fullText) > availableWidth) {
            truncatedText = fontMetrics.elidedText(fullText, Qt::ElideRight, availableWidth);
        }
     
        item->setText(truncatedText);
    }
    
    if (buttonsContainer->isVisible() && currentSelectedRow >= 0) {
        updateButtonsPosition();
    }
}

void MainPage::updateButtonsPosition() {
    if (currentSelectedRow < 0 || currentSelectedRow >= mediaList->count()) {
        hideActionButtons();
        return;
    }
    
    // Recupero dell'item e del suo rettangolo visivo
    QListWidgetItem* item = mediaList->item(currentSelectedRow);
    if (!item) {
        hideActionButtons();
        return;
    }
    
    QRect rect = mediaList->visualItemRect(item);

    
    // Calcolo larghezza dei pulsanti
    int buttonWidth = buttonsContainer->sizeHint().width();
    int buttonHeight = buttonsContainer->sizeHint().height();
    
    // Calcolo larghezza del testo
    QString itemText = item->text();
    QFontMetrics fontMetrics(mediaList->font());
    int iconWidth = 27;
    int iconMargin = 8;
    int textWidth = fontMetrics.horizontalAdvance(itemText);
    int totalContentWidth = iconWidth + iconMargin + textWidth;

    // Posizione X: dopo il contenuto + margine di sicurezza
    int marginAfterText = 15;
    int xPosition = rect.left() + totalContentWidth + marginAfterText;
    
    // In modo che i pulsanti non escano dal bordo destro
    int rightMargin = 10;
    int maxXPosition = rect.right() - buttonWidth - rightMargin;
    
    // Limitazione alla posizione massima se la posizione calcolata è troppo a destra
    if (xPosition > maxXPosition) {
        xPosition = maxXPosition;
    }
    
    if (xPosition < rect.left() + totalContentWidth + 5) {
        xPosition = rect.left() + (rect.width() * 0.75);
        
        if (xPosition + buttonWidth > rect.right() - rightMargin) {
            xPosition = rect.right() - buttonWidth - rightMargin;
        }
    }
    
    // Centramento verticale
    int yPosition = rect.top() + (rect.height() - buttonHeight) / 2;
    
    buttonsContainer->setGeometry(xPosition, yPosition, buttonWidth, buttonHeight);
    buttonsContainer->show();
}

// ========================================
// METODI DI PULIZIA UI
// ========================================
void MainPage::clearPreviewPanel() {
    mediaTitleLabel->setText("");
    mediaAuthorLabel->setText("Seleziona un media per vedere i dettagli");
    mediaYearLabel->setText("");
    mediaRatingLabel->setText("");
    mediaImageLabel->setText("Nessuna immagine");
    mediaImageLabel->setStyleSheet(
        "border: 1px solid black;"
        "background-color: white;"
        "color: gray;"
        "padding: 5px;"
    );
    
    borrowButton->setEnabled(false);
    returnButton->setEnabled(false);
    detailsButton->setEnabled(false);
    editMediaButton->setEnabled(false);
}

void MainPage::showActionButtons(int row) {
    // Nascondi eventuali pulsanti precedentemente mostrati
    hideActionButtons();
    
    if (row < 0 || row >= mediaList->count()) return;
    
    currentSelectedRow = row;
    updateButtonsPosition();
    buttonsContainer->show();
}

void MainPage::hideActionButtons() {
    buttonsContainer->hide();
    currentSelectedRow = -1; // Reset della riga selezionata
}

// =======================
// METODI DI UTILITA'
// =======================
Media* MainPage::getSelectedMedia() const {
    QListWidgetItem* currentItem = mediaList->currentItem();
    if (!currentItem) return nullptr;
    
    QVariant mediaData = currentItem->data(Qt::UserRole);
    if (!mediaData.isValid()) return nullptr;
    
    return mediaData.value<Media*>();
}

QString MainPage::getButtonStyle(const QString& bgColor, const QString& hoverColor, const QString& textColor) const {
    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: %2;"
        "   border: none;"
        "   border-radius: 4px;"
        "   font-size: 14px;"
        "   padding: 3px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %3;"
        "}"
    ).arg(bgColor, textColor, hoverColor);
}

// ============================
// METODI DI I/O
// ============================
void MainPage::saveToFile(const QString& filePath) {
    bool success = false;
    QString errorMessage = "";
    
    try {
        if (filePath.endsWith(".json", Qt::CaseInsensitive)) {
            JsonIO jsonSaver;
            success = jsonSaver.salvaSuFile(*biblioteca, filePath.toStdString());
            if (!success) {
                errorMessage = "Errore durante il salvataggio del file JSON.";
            }
        } else if (filePath.endsWith(".xml", Qt::CaseInsensitive)) {
            XmlIO xmlSaver;
            success = xmlSaver.salvaSuFile(*biblioteca, filePath.toStdString());
            if (!success) {
                errorMessage = "Errore durante il salvataggio del file XML.";
            }
        } else {
            errorMessage = "Formato file non supportato. Utilizzare .json o .xml";
        }
    } catch (const std::exception& e) {
        errorMessage = QString("Errore durante il salvataggio: %1").arg(e.what());
        success = false;
    }
    
    if (success) {
        QFile file(filePath);
        if (file.exists() && file.size() > 0) {
            QMessageBox::information(this, "Salvataggio completato", 
                QString("La biblioteca è stata salvata con successo in:\n%1").arg(filePath));
            
            hasUnsavedChanges = false;
            updateSaveButtonsState();
            emit unsavedChangesUpdated(false);
        } else {
            QMessageBox::warning(this, "Errore di salvataggio", 
                "Il file è stato creato ma potrebbe essere vuoto o corrotto.");
        }
    } else {
        QMessageBox::warning(this, "Errore di salvataggio", 
            errorMessage.isEmpty() ? 
            "Impossibile salvare la biblioteca nel file specificato." : 
            errorMessage);
    }
}