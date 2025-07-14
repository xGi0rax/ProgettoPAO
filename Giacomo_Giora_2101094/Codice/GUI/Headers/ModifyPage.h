#ifndef MODIFYPAGE_H
#define MODIFYPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QStackedWidget>
#include "Widgets/MediaWidget.h"
#include "Widgets/FilmWidget.h"
#include "Widgets/LibroWidget.h"
#include "Widgets/VinileWidget.h"
#include "Widgets/RivistaWidget.h"
#include "Widgets/GiocoWidget.h"
#include "../../Modello logico/Headers/Biblioteca.h"

class ModifyPage : public QWidget {
    Q_OBJECT

public:
    explicit ModifyPage(QWidget* parent = nullptr);
    void setMedia(Media* media);
    void setBiblioteca(Biblioteca* biblio);

signals:
    void goBackToMainPage(); 
    void mediaModified(); 
    void mediaCopiesIncreased();

private slots:
    void onBackButtonClicked();
    void onSaveButtonClicked();
    void onUploadButtonClicked();
    
private:
    // ========================================
    // DATI E STATO 
    // ========================================
    Biblioteca* biblioteca;
    Media* currentMedia;

    // Gestione immagini
    QString currentImagePath;
    QString newImagePath;
    bool hasNewImage;

    // ========================================
    // WIDGET SIDEBAR
    // ========================================
    QPushButton* backButton;
    QLabel* imagePreview;
    QPushButton* uploadButton;
    QPushButton* saveButton;
    
    // ========================================
    // WIDGET PRINCIPALE
    // ========================================
    QStackedWidget* detailsStackedWidget;
    MediaWidget* currentWidget;

    // ========================================
    // METODI DI INIZIALIZZAZIONE UI
    // ========================================
    void setupUI();
    
    // ========================================
    // METODI DI GESTIONE
    // ========================================
    void loadExistingImage(Media* media);  
    QPixmap loadImageFromPath(const QString& imagePath);  
};

#endif // MODIFYPAGE_H