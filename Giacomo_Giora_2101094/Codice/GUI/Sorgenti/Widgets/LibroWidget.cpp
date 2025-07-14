#include "../../Headers/Widgets/LibroWidget.h"
#include "../../../Modello logico/Headers/Libro.h"
#include <QLabel>

using std::string;

LibroWidget::LibroWidget(QWidget *parent) : MediaWidget(parent), currentLibro(nullptr) {
    setupBaseUI("Scheda Libro");
}

void LibroWidget::setCurrentMedia(Media* media) {
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido");
        return;
    }

    currentMedia = media;
    currentLibro = dynamic_cast<Libro*>(media);

    if (!currentLibro) {
        QMessageBox::warning(this, "Errore", "Media non è un libro");
        currentMedia = nullptr;
        return;
    }

    setCurrentValues();
}

void LibroWidget::addSpecificFields() {
    // Generi specifici per i libri
    genreComboBox->addItems({"Avventura", "Biografia", "Fantasy", "Fantascienza", "Giallo", "Horror", "Narrativa", "Poesia", "Romanzo", "Saggistica", "Storico", "Thriller", "Altro"});

    // Campi specifici per libro
    isbnEdit = new QLineEdit(scrollWidget);
    isbnEdit->setStyleSheet(getInputStyle());

    editorEdit = new QLineEdit(scrollWidget);
    editorEdit->setStyleSheet(getInputStyle());

    pagesEdit = new QSpinBox(scrollWidget);
    pagesEdit->setRange(1, 9999);
    pagesEdit->setStyleSheet(getInputStyle());

    QLabel *isbnLbl = new QLabel("ISBN:");
    isbnLbl->setStyleSheet(getLabelStyle());
    QLabel *editorLbl = new QLabel("Editore:");
    editorLbl->setStyleSheet(getLabelStyle());
    QLabel *pagesLbl = new QLabel("Pagine:");
    pagesLbl->setStyleSheet(getLabelStyle());

    formLayout->addRow(isbnLbl, isbnEdit);
    formLayout->addRow(editorLbl, editorEdit);
    formLayout->addRow(pagesLbl, pagesEdit);
}

void LibroWidget::setCurrentValues() {
    MediaWidget::setCurrentValues();
    
    if (currentLibro) {
        genreComboBox->setCurrentText(QString::fromStdString(currentLibro->getGenere()));
        isbnEdit->setText(QString::fromStdString(currentLibro->getIsbn()));
        editorEdit->setText(QString::fromStdString(currentLibro->getEditore()));
        pagesEdit->setValue(currentLibro->getNPagine());
    }
}

bool LibroWidget::validateData() const {
    return MediaWidget::validateData() && 
            !isbnEdit->text().isEmpty() && 
            !editorEdit->text().isEmpty() && 
            pagesEdit->value() > 0;
}

bool LibroWidget::applyChanges() {
    if (!validateData()) return false;

    // Aggiornamento campi comuni
    MediaWidget::applyChanges();

    // Aggiornamento campi specifici del libro
    currentLibro->setIsbn(isbnEdit->text().toStdString());
    currentLibro->setEditore(editorEdit->text().toStdString());
    currentLibro->setNPagine(pagesEdit->value());

    return true;
}

Media* LibroWidget::createMedia() {
    if(!validateData()) return nullptr;

    // Creazione e restituzione di un nuovo oggetto Libro
    return new Libro(
        titleEdit->text().toStdString(),
        authorEdit->text().toStdString(),
        genreComboBox->currentText().toStdString(),
        yearEdit->value(),
        languageEdit->text().toStdString(),
        "",  // immagine verrà impostata da AddPage
        true, // disponibilità predefinita
        1,    // copie predefinite
        isbnEdit->text().toStdString(),
        editorEdit->text().toStdString(),
        pagesEdit->value(),
        0,    // in prestito predefinito
        collocationEdit->text().toStdString(),
        static_cast<float>(ratingEdit->value())
    );
}

void LibroWidget::setReadOnly(bool readOnly) {
    MediaWidget::setReadOnly(readOnly);
    isbnEdit->setReadOnly(readOnly);
    editorEdit->setReadOnly(readOnly);
    pagesEdit->setReadOnly(readOnly);
}