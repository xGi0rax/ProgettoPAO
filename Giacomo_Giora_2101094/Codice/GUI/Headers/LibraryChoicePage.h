#ifndef LIBRARYCHOICEPAGE_H
#define LIBRARYCHOICEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include "../../Modello logico/Headers/Biblioteca.h"

class LibraryChoicePage : public QWidget {
    Q_OBJECT

public:
    explicit LibraryChoicePage(QWidget *parent = nullptr);
    ~LibraryChoicePage();

signals:
    void libraryReady(Biblioteca* biblioteca, const QString& filePath, bool isNew);
    void goToLoginPage();

private slots:
    void onLoadFileButtonClicked();
    void onNewLibraryButtonClicked();

private:
    // ========================================
    // WIDGET UI
    // ========================================
    QLabel *descriptionLabel;
    QPushButton *loadFileButton;
    QPushButton *newLibraryButton;

    // ========================================
    // DATI
    // ========================================
    Biblioteca *biblioteca;

    // ========================================
    // METODI DI INIZIALIZZAZIONE
    // ========================================
    void setupUI();
    void setupStyles();
    void setupConnections();

    // ========================================
    // HELPER METHODS
    // ========================================
    void resetBiblioteca();
    bool loadFromFile(const QString& filePath);
    QString getPrimaryButtonStyle() const;
    QString getSecondaryButtonStyle() const;
};

#endif // LIBRARYCHOICEPAGE_H