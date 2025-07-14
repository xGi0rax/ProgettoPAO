#include "../../Headers/Widgets/RivistaWidget.h"
#include "../../../Modello logico/Headers/Rivista.h"
#include <QLabel>

using std::string;

RivistaWidget::RivistaWidget(QWidget *parent) : MediaWidget(parent), currentRivista(nullptr) {
    setupBaseUI("Scheda Rivista");
}

void RivistaWidget::setCurrentMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido");
        return;
    }

    currentMedia = media;
    currentRivista = dynamic_cast<Rivista*>(media);

    if (!currentRivista) {
        QMessageBox::warning(this, "Errore", "Media non è una rivista");
        currentMedia = nullptr;
        return;
    }

    setCurrentValues();
}

void RivistaWidget::addSpecificFields() {
    // Generi specifici per le riviste
    genreComboBox->addItems({"Attualità", "Arte", "Cucina", "Cultura", "Economia", "Intrattenimento", "Informatica", "Moda", "Natura", "Salute", "Scienza", "Sport", "Tecnologia", "Viaggi", "Altro"});

    // Campi specifici per rivista
    editorEdit = new QLineEdit(scrollWidget);
    editorEdit->setStyleSheet(getInputStyle());

    pagesEdit = new QSpinBox(scrollWidget);
    pagesEdit->setRange(1, 999);
    pagesEdit->setStyleSheet(getInputStyle());
    
    publicationDateEdit = new QDateEdit(scrollWidget);
    publicationDateEdit->setDisplayFormat("dd/MM/yyyy");
    publicationDateEdit->setCalendarPopup(true);
    publicationDateEdit->setStyleSheet(getInputStyle());
    
    periodicityComboBox = new QComboBox(scrollWidget);
    periodicityComboBox->addItems({"Settimanale", "Bisettimanale", "Mensile", "Bimestrale", "Trimestrale", "Semestrale", "Annuale"});
    periodicityComboBox->setStyleSheet(getInputStyle());

    QLabel *editorLbl = new QLabel("Editore:");
    editorLbl->setStyleSheet(getLabelStyle());
    QLabel *pagesLbl = new QLabel("Pagine:");
    pagesLbl->setStyleSheet(getLabelStyle());
    QLabel *dateLbl = new QLabel("Data pubblicazione:");
    dateLbl->setStyleSheet(getLabelStyle());
    QLabel *periodicityLbl = new QLabel("Periodicità:");
    periodicityLbl->setStyleSheet(getLabelStyle());

    // Aggiungi i campi al form
    formLayout->addRow(editorLbl, editorEdit);
    formLayout->addRow(pagesLbl, pagesEdit);
    formLayout->addRow(dateLbl, publicationDateEdit);
    formLayout->addRow(periodicityLbl, periodicityComboBox);
}

void RivistaWidget::setCurrentValues() {
    MediaWidget::setCurrentValues();
    
    if (currentRivista) {
        genreComboBox->setCurrentText(QString::fromStdString(currentRivista->getGenere()));
        editorEdit->setText(QString::fromStdString(currentRivista->getEditore()));
        pagesEdit->setValue(currentRivista->getNPagine());
        publicationDateEdit->setDate(QDate::fromString(QString::fromStdString(currentRivista->getDataPubb()), "dd/MM/yyyy"));
        periodicityComboBox->setCurrentText(QString::fromStdString(currentRivista->getPeriodicita()));
    }
}

bool RivistaWidget::validateData() const {
    return MediaWidget::validateData() && 
            !editorEdit->text().isEmpty() && 
            pagesEdit->value() > 0 && 
            !publicationDateEdit->text().isEmpty() && 
            !periodicityComboBox->currentText().isEmpty();
}

bool RivistaWidget::applyChanges() {
    if (!validateData()) return false;

    // Aggiornamento i campi comuni
    MediaWidget::applyChanges();

    // Aggiornamento campi specifici della rivista
    currentRivista->setEditore(editorEdit->text().toStdString());
    currentRivista->setNPagine(pagesEdit->value());
    currentRivista->setDataPubb(publicationDateEdit->date().toString("dd/MM/yyyy").toStdString());
    currentRivista->setPeriodicita(periodicityComboBox->currentText().toStdString());

    return true;
}

Media* RivistaWidget::createMedia() {
    if(!validateData()) return nullptr;

    QString dateStr = publicationDateEdit->date().toString("dd/MM/yyyy");

    // Creazione e restituzione di un nuovo oggetto Rivista
    return new Rivista(
        titleEdit->text().toStdString(),
        authorEdit->text().toStdString(),
        genreComboBox->currentText().toStdString(),
        yearEdit->value(),
        languageEdit->text().toStdString(),
        "",  // immagine verrà impostata da AddPage
        true, // disponibilità predefinita
        1,    // copie predefinite
        editorEdit->text().toStdString(),
        pagesEdit->value(),
        dateStr.toStdString(),
        periodicityComboBox->currentText().toStdString(),
        0,    // in prestito predefinito
        collocationEdit->text().toStdString(),
        static_cast<float>(ratingEdit->value())
    );
}

void RivistaWidget::setReadOnly(bool readOnly) {
    MediaWidget::setReadOnly(readOnly);
    editorEdit->setReadOnly(readOnly);
    pagesEdit->setReadOnly(readOnly);
    publicationDateEdit->setReadOnly(readOnly);
    periodicityComboBox->setDisabled(readOnly);
}