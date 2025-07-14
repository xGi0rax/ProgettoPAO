#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QGroupBox>
#include <QResizeEvent>
#include <QPixmap>
#include <QSizePolicy>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QPainter>
#include "AddPage.h"
#include "../../Modello logico/Headers/Media.h"
#include "../../Modello logico/Headers/Biblioteca.h"

class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr, Biblioteca* biblioteca = nullptr);
    
    // Metodi pubblici per gestione stato
    void setCurrentFile(const QString& filePath); 
    void setLibraryInfo(bool isNew, bool hasChanges);  
    void resetUnsavedChanges();
    void setHasUnsavedChanges(bool hasChanges);

public slots:
    // Slots barra superiore
    void onBackButtonClicked();
    void onAddMediaButtonClicked();
    void onSaveButtonClicked();
    void onSaveAsButtonClicked();
    
    // Slots filtri
    void onApplyFiltersClicked();
    void onClearFiltersClicked();
    void onSearchTextChanged(const QString& searchText);
    
    // Slots lista media
    void onMediaSelected(QListWidgetItem *item);
    
    // Slots azioni sui media
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onDetailsButtonClicked();
    void onBorrowButtonClicked();
    void onReturnButtonClicked();
    
    // Slots aggiornamenti da altre pagine
    void onMediaCreated();
    void onMediaModified();
    void onMediaCopiesIncreased();

private slots:
    void onMediaTypeChanged();
    void onScrollChanged();

signals:
    void goToChoicePage();
    void goToAddPage();
    void goToModifyPage(Media* media);
    void goToDetailsPage(Media* media);
    void borrowMedia(Media* media);
    void returnMedia(Media* media);
    void unsavedChangesUpdated(bool hasChanges);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    // ======================================== 
    // WIDGET UI - Raggruppati per sezione
    // ========================================
    
    // Barra superiore
    QPushButton *backButton;
    QPushButton *addMediaButton;
    QPushButton *saveButton;
    QPushButton *saveAsButton;

    // Filtri (sezione sinistra)
    QComboBox *mediaTypeComboBox;
    QComboBox *genreComboBox;
    QLineEdit *minYearLineEdit;
    QLineEdit *maxYearLineEdit;
    QLineEdit *ratingMinLineEdit;
    QLineEdit *ratingMaxLineEdit;
    QLineEdit *languageLineEdit;
    QPushButton *applyFiltersButton;
    QPushButton *clearFiltersButton;
    QCheckBox *availableCheckBox;

    // Lista media (sezione centrale)
    QLineEdit *searchBar;
    QListWidget *mediaList;
    
    // Pulsanti flottanti nella lista
    QPushButton* listEditButton;
    QPushButton* listDeleteButton;
    QWidget* buttonsContainer;
    int currentSelectedRow = -1;

    // Anteprima media (sezione destra)
    QLabel *mediaImageLabel;
    QPixmap originalPixmap;
    QLabel *mediaTitleLabel;
    QLabel *mediaAuthorLabel;
    QLabel *mediaYearLabel;
    QLabel *mediaRatingLabel;
    QPushButton *borrowButton;
    QPushButton *returnButton;
    QPushButton *detailsButton;
    QPushButton *editMediaButton;

    // ========================================
    // LAYOUT
    // ========================================
    QVBoxLayout *mainLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *contentLayout;
    QVBoxLayout *filtersLayout;
    QVBoxLayout *centerLayout;
    QVBoxLayout *previewLayout;
    QGroupBox *filtersGroupBox;
    QGroupBox *previewGroupBox;

    // ========================================
    // DATI E STATO
    // ========================================
    Biblioteca *biblioteca;
    QString currentFilePath;
    bool hasCurrentFile;
    bool isNewLibrary;
    bool hasUnsavedChanges;

    // ========================================
    // METODI DI INIZIALIZZAZIONE
    // ========================================
    void setupUI();
    void setupTopBar();
    void setupFilters();
    void setupMediaList();
    void setupPreviewPanel();
    void setupMainLayout();
    void setupStyles();

    // ========================================
    // METODI DI AGGIORNAMENTO UI
    // ========================================
    void updateGenreComboBox();
    void updateMediaList(vector<Media*> listaFiltrata);
    void updatePreviewPanel(Media* media);
    void updateMediaImage(Media* media);
    void updateImageSize();
    void updateSaveButtonsState();
    void updateButtonsPosition();
    void updateTextTruncation();

    // ========================================
    // METODI DI GESTIONE UI
    // ========================================
    void clearPreviewPanel();
    void showActionButtons(int row);
    void hideActionButtons();

    // ========================================
    // METODI DI UTILITA'
    // ========================================
    Media* getSelectedMedia() const;
    QString getButtonStyle(const QString& bgColor, const QString& hoverColor, const QString& textColor = "white") const;

    // ========================================
    // METODI I/O
    // ========================================
    void saveToFile(const QString& filePath);
};

#endif // MAINPAGE_H