#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include "LoginPage.h"
#include "LibraryChoicePage.h"
#include "MainPage.h"
#include "AddPage.h"
#include "DetailsPage.h"
#include "ModifyPage.h"
#include "../../Modello logico/Headers/Biblioteca.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Slots per navigazione
    void onLoginButtonClicked();
    void onLibraryReady(Biblioteca* biblio, const QString& filePath, bool isNew);
    void onUnsavedChangesUpdated(bool hasChanges);
    
    // Slots per switching pagine
    void switchToLoginPage();
    void switchToLibraryChoicePage();
    void switchToMainPage();
    void switchToAddPage();
    void switchToDetailsPage(Media* media);
    void switchToModifyPage(Media* media);
    
    // Slots per gestione media
    void prendiInPrestitoMedia(Media* media);
    void restituisciMedia(Media* media);

private:
    // ========================================
    // WIDGET UI
    // ========================================
    QStackedWidget *stackedWidget;
    LoginPage *loginPage = nullptr;
    LibraryChoicePage *libraryChoicePage = nullptr;
    MainPage *mainPage = nullptr;
    AddPage *addPage = nullptr;
    DetailsPage *detailsPage = nullptr;
    ModifyPage *modifyPage = nullptr;

    // ========================================
    // DATI E STATO
    // ========================================
    Biblioteca *biblioteca = nullptr;
    QString loadedFilePath;
    bool isNewLibrary = false;
    bool hasUnsavedChanges = false;

    // ========================================
    // METODI DI INIZIALIZZAZIONE
    // ========================================
    void setupLoginPage();
    void setupLibraryChoicePage();
    void setupMainPage(Biblioteca* biblio);
    void setupAddPage();
    void setupModifyPage();
    void setupDetailsPage();

    // ========================================
    // HELPER METHODS
    // ========================================
    bool validateLogin(const QString &username, const QString &password);
    bool checkUnsavedChanges();
    void notifyLibraryChanged(); 
    void resetLibraryState();  
};

#endif // MAINWINDOW_H