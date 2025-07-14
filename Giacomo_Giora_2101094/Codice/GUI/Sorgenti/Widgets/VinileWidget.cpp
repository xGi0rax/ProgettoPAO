#include "../../Headers/Widgets/VinileWidget.h"
#include "../../../Modello logico/Headers/Vinile.h"
#include <QLabel>

using std::string;

VinileWidget::VinileWidget(QWidget *parent) : MediaWidget(parent), currentVinile(nullptr) {
    setupBaseUI("Scheda Vinile");
}

void VinileWidget::setCurrentMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido");
        return;
    }

    currentMedia = media;
    currentVinile = dynamic_cast<Vinile*>(media);

    if (!currentVinile) {
        QMessageBox::warning(this, "Errore", "Media non è un vinile");
        currentMedia = nullptr;
        return;
    }

    setCurrentValues();
}

void VinileWidget::addSpecificFields() {
    // Imposta generi specifici per i vinili
    genreComboBox->addItems({"Alternative", "Blues", "Classica", "Country", "Disco", "Elettronica", "Folk", "Funk", "Hip Hop", "Jazz", "Metal", "Pop", "Punk", "Reggae", "Rock", "Soul", "Altro"});

    // Campi specifici per vinile
    trackCountEdit = new QSpinBox(scrollWidget);
    trackCountEdit->setRange(1, 99);
    trackCountEdit->setStyleSheet(getInputStyle());
    
    durationEdit = new QSpinBox(scrollWidget);
    durationEdit->setRange(1, 999);
    durationEdit->setSuffix(" min");
    durationEdit->setStyleSheet(getInputStyle());

    QLabel *trackCountLbl = new QLabel("Numero tracce:");
    trackCountLbl->setStyleSheet(getLabelStyle());
    QLabel *durationLbl = new QLabel("Durata totale:");
    durationLbl->setStyleSheet(getLabelStyle());

    // Aggiungi i campi al form
    formLayout->addRow(trackCountLbl, trackCountEdit);
    formLayout->addRow(durationLbl, durationEdit);
}

void VinileWidget::setCurrentValues() {
    MediaWidget::setCurrentValues();
    
    if (currentVinile) {
        genreComboBox->setCurrentText(QString::fromStdString(currentVinile->getGenere()));
        trackCountEdit->setValue(currentVinile->getNTracce());
        durationEdit->setValue(currentVinile->getDurata());
    }
}

bool VinileWidget::validateData() const {
    return MediaWidget::validateData() && 
            trackCountEdit->value() > 0 && 
            durationEdit->value() > 0;
}

bool VinileWidget::applyChanges() {
    if (!validateData()) return false;

    // Aggiornamento campi comuni
    MediaWidget::applyChanges();

    // Aggiornamento campi specifici del Vinile
    currentVinile->setNTracce(trackCountEdit->value());
    currentVinile->setDurata(durationEdit->value());

    return true;
}

Media* VinileWidget::createMedia() {
    if(!validateData()) return nullptr;

    // Creazione e restituzione di un nuovo oggetto Vinile
    return new Vinile(
        titleEdit->text().toStdString(),
        authorEdit->text().toStdString(),
        genreComboBox->currentText().toStdString(),
        yearEdit->value(),
        languageEdit->text().toStdString(),
        "",  // immagine verrà impostata da AddPage
        true, // disponibilità predefinita
        1,    // copie predefinite
        trackCountEdit->value(),
        durationEdit->value(),
        0,    // in prestito predefinito
        collocationEdit->text().toStdString(),
        static_cast<float>(ratingEdit->value())
    );
}

void VinileWidget::setReadOnly(bool readOnly) {
    MediaWidget::setReadOnly(readOnly);
    trackCountEdit->setReadOnly(readOnly);
    durationEdit->setReadOnly(readOnly);
}