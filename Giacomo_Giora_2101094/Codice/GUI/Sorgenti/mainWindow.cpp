#include "../Headers/mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    // Inizialmente viene mostrata la pagina di login
    setupLoginPage();
    stackedWidget->setCurrentWidget(loginPage);

    setWindowTitle("BananoTECH-a");
    resize(900, 600);
}

// ========================================
// METODI DI INIZIALIZZAZIONE
// ========================================
void MainWindow::setupLoginPage() {
    loginPage = new LoginPage(this);
    stackedWidget->addWidget(loginPage);
    
    connect(loginPage, &LoginPage::loginAttempted, this, &MainWindow::onLoginButtonClicked);
}

void MainWindow::setupLibraryChoicePage() {
    libraryChoicePage = new LibraryChoicePage(this);
    stackedWidget->addWidget(libraryChoicePage);
    
    connect(libraryChoicePage, &LibraryChoicePage::libraryReady, this, &MainWindow::onLibraryReady);
    connect(libraryChoicePage, &LibraryChoicePage::goToLoginPage, this, &MainWindow::switchToLoginPage);
}

void MainWindow::setupMainPage(Biblioteca* biblio) {
    biblioteca = biblio;
    mainPage = new MainPage(this, biblioteca);
    
    // Configurazione stato biblioteca
    if (!isNewLibrary && !loadedFilePath.isEmpty()) {
        mainPage->setCurrentFile(loadedFilePath);
    }
    mainPage->setLibraryInfo(isNewLibrary, hasUnsavedChanges);
    
    stackedWidget->addWidget(mainPage);

    // Connessioni navigazione
    connect(mainPage, &MainPage::goToChoicePage, this, &MainWindow::switchToLibraryChoicePage);
    connect(mainPage, &MainPage::goToAddPage, this, &MainWindow::switchToAddPage);
    connect(mainPage, &MainPage::goToModifyPage, this, &MainWindow::switchToModifyPage);
    connect(mainPage, &MainPage::goToDetailsPage, this, &MainWindow::switchToDetailsPage);
    
    // Connessioni gestione media
    connect(mainPage, &MainPage::borrowMedia, this, &MainWindow::prendiInPrestitoMedia);
    connect(mainPage, &MainPage::returnMedia, this, &MainWindow::restituisciMedia);
    
    // Connessione stato modifiche
    connect(mainPage, &MainPage::unsavedChangesUpdated, this, &MainWindow::onUnsavedChangesUpdated);
}

void MainWindow::setupAddPage() {
    addPage = new AddPage(this);
    addPage->setBiblioteca(biblioteca);
    stackedWidget->addWidget(addPage);

    // Connessioni navigazione
    connect(addPage, &AddPage::goBackToMainPage, this, &MainWindow::switchToMainPage);
    
    // Connessioni aggiornamento dati
    connect(addPage, &AddPage::mediaCreated, mainPage, &MainPage::onMediaCreated);
    connect(addPage, &AddPage::mediaCopiesIncreased, mainPage, &MainPage::onMediaCopiesIncreased);
}

void MainWindow::setupModifyPage() {
    modifyPage = new ModifyPage(this);
    modifyPage->setBiblioteca(biblioteca);
    stackedWidget->addWidget(modifyPage);

    // Connessioni navigazione
    connect(modifyPage, &ModifyPage::goBackToMainPage, this, &MainWindow::switchToMainPage);
    
    // Connessioni aggiornamento dati
    connect(modifyPage, &ModifyPage::mediaModified, mainPage, &MainPage::onMediaModified);
    connect(modifyPage, &ModifyPage::mediaCopiesIncreased, mainPage, &MainPage::onMediaCopiesIncreased);
}

void MainWindow::setupDetailsPage() {
    detailsPage = new DetailsPage(this);
    stackedWidget->addWidget(detailsPage);

    // Connessioni navigazione
    connect(detailsPage, &DetailsPage::goBackToMainPage, this, &MainWindow::switchToMainPage);
    
    // Connessioni gestione media
    connect(detailsPage, &DetailsPage::mediaBorrowed, this, &MainWindow::prendiInPrestitoMedia);
    connect(detailsPage, &DetailsPage::mediaReturned, this, &MainWindow::restituisciMedia);
}

// ========================================
// SLOTS PER NAVIGAZIONE
// ========================================
void MainWindow::onLoginButtonClicked() {
    QString username = loginPage->getUsername();
    QString password = loginPage->getPassword();

    if (validateLogin(username, password)) {
        resetLibraryState();
        loginPage->clearErrorMessage();
        setupLibraryChoicePage();
        switchToLibraryChoicePage();
    } else {
        loginPage->showErrorMessage("Username o password errati. Riprova.");
    }
}

void MainWindow::onLibraryReady(Biblioteca* biblio, const QString& filePath, bool isNew) {
    // Salva le informazioni sulla biblioteca
    loadedFilePath = filePath;
    isNewLibrary = isNew;
    hasUnsavedChanges = false;
    
    // Setup di tutte le pagine
    setupMainPage(biblio);
    setupAddPage();
    setupDetailsPage();
    setupModifyPage();

    switchToMainPage();
}

void MainWindow::onUnsavedChangesUpdated(bool hasChanges) {
    hasUnsavedChanges = hasChanges;
}

// ========================================
// SLOTS PER SWITCHING PAGINE
// ========================================
void MainWindow::switchToLoginPage() {
    resetLibraryState();
    loginPage->resetToDefaultState();
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::switchToLibraryChoicePage() {
    if (!checkUnsavedChanges()) {
        return; // L'utente ha cancellato l'operazione
    }
    
    resetLibraryState();
    stackedWidget->setCurrentWidget(libraryChoicePage);
}

void MainWindow::switchToMainPage() {
    stackedWidget->setCurrentWidget(mainPage);
}

void MainWindow::switchToAddPage() {
    stackedWidget->setCurrentWidget(addPage);
}

void MainWindow::switchToDetailsPage(Media* media) {
    detailsPage->setMedia(media);
    stackedWidget->setCurrentWidget(detailsPage);
}

void MainWindow::switchToModifyPage(Media* media) {
    modifyPage->setMedia(media);
    stackedWidget->setCurrentWidget(modifyPage);
}

// ========================================
// SLOTS PER GESTIONE MEDIA
// ========================================
void MainWindow::prendiInPrestitoMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido.");
        return;
    }
    
    if (biblioteca->prendiInPrestito(media)) {
        QMessageBox::information(this, "Prestito effettuato", 
            QString("Hai preso in prestito '%1' con successo!")
            .arg(QString::fromStdString(media->getTitolo())));
        
        notifyLibraryChanged();
    } else {
        QMessageBox::warning(this, "Prestito non disponibile", 
            QString("Tutte le copie di '%1' sono già in prestito.")
            .arg(QString::fromStdString(media->getTitolo())));
    }
}

void MainWindow::restituisciMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido.");
        return;
    }

    // Validazioni
    if (!biblioteca->esisteMedia(media->getTitolo(), media->getAutore(), media->getAnno())) {
        QMessageBox::warning(this, "Errore", 
            QString("Il media '%1' non esiste nella biblioteca. Impossibile restituire.")
            .arg(QString::fromStdString(media->getTitolo())));
        return;
    }
    
    if (media->getInPrestito() <= 0) {
        QMessageBox::warning(this, "Restituzione non disponibile", 
            QString("Nessuna copia di '%1' risulta in prestito.")
            .arg(QString::fromStdString(media->getTitolo())));
        return;
    }
    
    // Esecuzione restituzione
    if (biblioteca->restituisci(media)) {
        QMessageBox::information(this, "Restituzione effettuata", 
            QString("Hai restituito '%1' con successo!")
            .arg(QString::fromStdString(media->getTitolo())));
        
        notifyLibraryChanged();
    } else {
        QMessageBox::warning(this, "Restituzione non riuscita", 
            QString("Si è verificato un errore durante la restituzione di '%1'.")
            .arg(QString::fromStdString(media->getTitolo())));
    }
}

// ========================================
// EVENT HANDLERS
// ========================================
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!checkUnsavedChanges()) {
        event->ignore();
        return;
    }
    
    event->accept();
}

// ========================================
// HELPER METHODS
// ========================================
bool MainWindow::validateLogin(const QString &username, const QString &password) {
    return (username == "admin" && password == "admin");
}

bool MainWindow::checkUnsavedChanges() {
    if (hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
            "Modifiche non salvate", 
            "Ci sono modifiche non salvate. Sei sicuro di voler uscire senza salvare?",
            QMessageBox::Yes | QMessageBox::No);
        
        return reply == QMessageBox::Yes;
    }
    
    return true;
}

void MainWindow::notifyLibraryChanged() {
    hasUnsavedChanges = true;
    if (mainPage) {
        mainPage->setHasUnsavedChanges(true);
    }
}

void MainWindow::resetLibraryState() {
    hasUnsavedChanges = false;
    isNewLibrary = false;
    loadedFilePath = "";
}