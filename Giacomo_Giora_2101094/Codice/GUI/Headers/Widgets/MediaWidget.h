#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QScrollArea>
#include <QMessageBox>
#include "../../../Modello logico/Headers/Media.h"

class MediaWidget : public QWidget {
    Q_OBJECT

public:
    explicit MediaWidget(QWidget* parent = nullptr);
    virtual ~MediaWidget() = default;

    // Metodi virtuali da implementare nelle classi derivate
    virtual bool validateData() const = 0;  // Validazione dei dati inseriti
    virtual bool applyChanges() = 0;  // Applicazione delle modifiche (per modifyPage)
    virtual Media* createMedia() = 0; // Creazione di un nuovo oggetto Media (per AddPage)
    virtual void setReadOnly(bool readOnly) = 0; // Imposta i campi come di sola lettura

    QString getTitolo() const;
    QString getAutore() const;
    int getAnno() const;

protected:
    Media* currentMedia;

    // Elementi UI comuni
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QFormLayout *formLayout;

    // Campi comuni
    QLabel *titleLabel;
    QLineEdit *titleEdit;
    QLineEdit *authorEdit;
    QComboBox *genreComboBox;
    QSpinBox *yearEdit;
    QLineEdit *languageEdit;
    QLineEdit *collocationEdit;
    QDoubleSpinBox *ratingEdit;

    // Metodi per setup
    void setupBaseUI(const QString &title);
    virtual void setCurrentMedia(Media* media) = 0; // Per impostare il media corrente
    virtual void addSpecificFields() = 0; // Per aggiungere i campi specifici del tipo del media
    virtual void setCurrentValues() = 0; // Per impostare i valori del media corrente nei campi
    void addStandardFields();

    // Stili comuni
    QString getLabelStyle() const;
    QString getInputStyle() const;
};

#endif // MEDIAWIDGET_H