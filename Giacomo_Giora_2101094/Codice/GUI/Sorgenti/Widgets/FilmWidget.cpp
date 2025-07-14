#include "../../Headers/Widgets/FilmWidget.h"
#include "../../../Modello logico/Headers/Film.h"
#include <QLabel>
#include <vector>

using std::vector;
using std::string;

FilmWidget::FilmWidget(QWidget *parent) : MediaWidget(parent), currentFilm(nullptr) {
    setupBaseUI("Scheda Film");
}

void FilmWidget::setCurrentMedia(Media* media) {
    // Controllo di validità
    if (!media) {
        QMessageBox::warning(this, "Errore", "Media non valido");
        return;
    }

    currentMedia = media;
    currentFilm = dynamic_cast<Film*>(media);

    if (!currentFilm) {
        QMessageBox::warning(this, "Errore", "Media non è un film");
        currentMedia = nullptr;
        return;
    }

    setCurrentValues();
}

void FilmWidget::addSpecificFields() {
    // Generi specifici per i film
    genreComboBox->addItems({"Animazione", "Azione", "Avventura", "Biografia", "Commedia", "Crime", "Documentario", "Drammatico", "Fantascienza", "Fantasy", "Horror", "Musical", "Mistero", "Romantico", "Thriller", "Western", "Altro"});

    // Campi specifici per film
    durationFilmEdit = new QSpinBox(scrollWidget);
    durationFilmEdit->setRange(1, 999);
    durationFilmEdit->setSuffix(" min");
    durationFilmEdit->setStyleSheet(getInputStyle());
    
    castEdit = new QLineEdit(scrollWidget);
    castEdit->setStyleSheet(getInputStyle());
    castEdit->setPlaceholderText("(separati da virgola)");
    
    QLabel *durationLbl = new QLabel("Durata:");
    durationLbl->setStyleSheet(getLabelStyle());
    QLabel *castLbl = new QLabel("Attori:");
    castLbl->setStyleSheet(getLabelStyle());

    formLayout->addRow(durationLbl, durationFilmEdit);
    formLayout->addRow(castLbl, castEdit);
}

void FilmWidget::setCurrentValues() {
    MediaWidget::setCurrentValues();
    
    if (currentFilm) {
        genreComboBox->setCurrentText(QString::fromStdString(currentFilm->getGenere()));
        durationFilmEdit->setValue(currentFilm->getDurata());
        
        QString castText;
        const std::vector<std::string>& cast = currentFilm->getCast();
        for (size_t i = 0; i < cast.size(); ++i) {
            castText += QString::fromStdString(cast[i]);
            if (i < cast.size() - 1) {
                castText += ", ";
            }
        }
        castEdit->setText(castText);
    }
}

bool FilmWidget::validateData() const {
    return MediaWidget::validateData() && 
           durationFilmEdit->value() > 0 && 
           !castEdit->text().isEmpty();
}

bool FilmWidget::applyChanges() {
    if (!validateData()) return false;

    MediaWidget::applyChanges();

    vector<string> castVector;
    QString castText = castEdit->text();
    QStringList castList = castText.split(",", Qt::SkipEmptyParts);
    for (const QString& actor : castList) {
        castVector.push_back(actor.trimmed().toStdString());
    }

    // Aggiorno i campi specifici del Film
    currentFilm->setDurata(durationFilmEdit->value());
    currentFilm->setCast(castVector);

    return true;
}

Media* FilmWidget::createMedia() {
    if(!validateData()) return nullptr;

    vector<string> cast;
    QString castString = castEdit->text();
    QStringList castList = castString.split(",", Qt::SkipEmptyParts);
    for (const QString &actor : castList) {
        cast.push_back(actor.trimmed().toStdString());
    }

    // Creazione e restituzione di un nuovo oggetto Film
    return new Film(
        titleEdit->text().toStdString(),
        authorEdit->text().toStdString(),
        genreComboBox->currentText().toStdString(),
        yearEdit->value(),
        languageEdit->text().toStdString(),
        "",  // immagine verrà impostata da AddPage
        true, // disponibilità predefinita
        1,    // copie predefinite
        durationFilmEdit->value(),
        cast,
        0,    // in prestito predefinito
        collocationEdit->text().toStdString(),
        static_cast<float>(ratingEdit->value())
    );
}

void FilmWidget::setReadOnly(bool readOnly) {
    MediaWidget::setReadOnly(readOnly);
    durationFilmEdit->setReadOnly(readOnly);
    castEdit->setReadOnly(readOnly);
}