#ifndef ADDPAGE_H
#define ADDPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QDateEdit>
#include <QScrollArea>
#include "Widgets/MediaWidget.h"
#include "Widgets/FilmWidget.h"
#include "Widgets/LibroWidget.h"
#include "Widgets/VinileWidget.h"
#include "Widgets/RivistaWidget.h"
#include "Widgets/GiocoWidget.h"
#include "../../Modello logico/Headers/Biblioteca.h"

class AddPage : public QWidget {
    Q_OBJECT

public:
    explicit AddPage(QWidget *parent = nullptr);
    void setBiblioteca(Biblioteca *biblio);

signals:
    void goBackToMainPage();
    void mediaCreated();
    void mediaCopiesIncreased();

private slots:
    void onBackButtonClicked();
    void onConfirmTypeButtonClicked();
    void onAddButtonClicked();
    void onUploadButtonClicked();

private:
    // ========================================
    // ENUM TIPI
    // ========================================
    enum MediaType {
        FILM = 0,
        LIBRO,
        VINILE,
        RIVISTA,
        GIOCO_DA_TAVOLO
    };

    // ========================================
    // DATI
    // ========================================
    Biblioteca* biblioteca;
    QString selectedImagePath;

    // ========================================
    // WIDGET UI PRINCIPALI
    // ========================================
    QStackedWidget *mainContentStack;
    QStackedWidget *detailsStackedWidget;
    MediaWidget *currentWidget;

    // ========================================
    // WIDGET UI - SIDEBAR
    // ========================================
    QPushButton *backButton;
    QLabel *imagePreview;
    QPushButton *uploadButton;

    // ========================================
    // WIDGET UI - PAGINA DI SELEZIONE
    // ========================================
    QWidget *selectionWidget;
    QButtonGroup *mediaTypeGroup;
    QRadioButton *filmRadio;
    QRadioButton *libroRadio;
    QRadioButton *vinileRadio;
    QRadioButton *rivistaRadio;
    QRadioButton *giocoRadio;
    QPushButton *confirmTypeButton;

    // ========================================
    // METODI DI INIZIALIZZAZIONE
    // ========================================
    void setupUI();
    void setupSidebar(QWidget* sidebarWidget);
    void setupImageSection(QVBoxLayout* sidebarLayout);
    void setupSelectionPage();
    void setupRadioButtons();

    // ========================================
    // METODI DI GESTIONE
    // ========================================
    void showSelectionPage();
    bool handleExistingMedia(Media* newMedia);
    void copyImageToProject(const QString& imagePath);
    QString getRadioButtonStyle() const;
};

#endif // ADDPAGE_H