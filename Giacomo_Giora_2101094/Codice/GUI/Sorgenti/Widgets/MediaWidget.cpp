#include "../../Headers/Widgets/MediaWidget.h"

MediaWidget::MediaWidget(QWidget *parent) : QWidget(parent), currentMedia(nullptr) {
}

void MediaWidget::setupBaseUI(const QString &title) {
    // Layout verticale principale
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 30);
    
    // ScrollArea
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background: transparent;");
    
    // Widget per il contenuto scrollabile
    scrollWidget = new QWidget();
    formLayout = new QFormLayout(scrollWidget);
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    // Titolo della pagina di dettaglio
    titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    // Aggiunta campi
    addStandardFields();
    addSpecificFields();
    
    formLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea, 1);
}

void MediaWidget::addStandardFields() {
    titleEdit = new QLineEdit(scrollWidget);
    titleEdit->setStyleSheet(getInputStyle());
    
    authorEdit = new QLineEdit(scrollWidget);
    authorEdit->setStyleSheet(getInputStyle());
    
    genreComboBox = new QComboBox(scrollWidget);
    genreComboBox->setStyleSheet(getInputStyle());
    
    yearEdit = new QSpinBox(scrollWidget);
    yearEdit->setRange(1880, 2100);
    yearEdit->setValue(QDate::currentDate().year());
    yearEdit->setStyleSheet(getInputStyle());
    
    languageEdit = new QLineEdit(scrollWidget);
    languageEdit->setStyleSheet(getInputStyle());

    collocationEdit = new QLineEdit(scrollWidget);
    collocationEdit->setStyleSheet(getInputStyle());
    
    ratingEdit = new QDoubleSpinBox(scrollWidget);
    ratingEdit->setRange(1.0, 5.0);
    ratingEdit->setDecimals(1);
    ratingEdit->setSingleStep(0.1);
    ratingEdit->setStyleSheet(getInputStyle());

    QLabel *titleLbl = new QLabel("Titolo:");
    titleLbl->setStyleSheet(getLabelStyle());
    QLabel *authorLbl = new QLabel("Autore:");
    authorLbl->setStyleSheet(getLabelStyle());
    QLabel *genreLbl = new QLabel("Genere:");
    genreLbl->setStyleSheet(getLabelStyle());
    QLabel *yearLbl = new QLabel("Anno:");
    yearLbl->setStyleSheet(getLabelStyle());
    QLabel *languageLbl = new QLabel("Lingua:");
    languageLbl->setStyleSheet(getLabelStyle());
    QLabel *collocationLbl = new QLabel("Collocazione:");
    collocationLbl->setStyleSheet(getLabelStyle());
    QLabel *ratingLbl = new QLabel("Valutazione:");
    ratingLbl->setStyleSheet(getLabelStyle());

    // Aggiunta campi al form
    formLayout->addRow(titleLbl, titleEdit);
    formLayout->addRow(authorLbl, authorEdit);
    formLayout->addRow(genreLbl, genreComboBox);
    formLayout->addRow(yearLbl, yearEdit);
    formLayout->addRow(languageLbl, languageEdit);
    formLayout->addRow(collocationLbl, collocationEdit);
    formLayout->addRow(ratingLbl, ratingEdit);
}


void MediaWidget::setCurrentValues() {
    if (currentMedia) {
        titleEdit->setText(QString::fromStdString(currentMedia->getTitolo()));
        authorEdit->setText(QString::fromStdString(currentMedia->getAutore()));
        yearEdit->setValue(currentMedia->getAnno());
        languageEdit->setText(QString::fromStdString(currentMedia->getLingua()));
        collocationEdit->setText(QString::fromStdString(currentMedia->getCollocazione()));
        ratingEdit->setValue(currentMedia->getRating());
    }
}

bool MediaWidget::validateData() const{
    return !titleEdit->text().isEmpty() && 
           !authorEdit->text().isEmpty() && 
           !genreComboBox->currentText().isEmpty() &&
           yearEdit->value() > 0 && 
           !languageEdit->text().isEmpty() &&
           !collocationEdit->text().isEmpty() &&
           ratingEdit->value() > 0;
}

bool MediaWidget::applyChanges() {
    if (!validateData() || !currentMedia)
        return false;

    currentMedia->setTitolo(titleEdit->text().toStdString());
    currentMedia->setAutore(authorEdit->text().toStdString());
    currentMedia->setGenere(genreComboBox->currentText().toStdString());
    currentMedia->setAnno(yearEdit->value());
    currentMedia->setLingua(languageEdit->text().toStdString());
    currentMedia->setCollocazione(collocationEdit->text().toStdString());
    currentMedia->setRating(ratingEdit->value());

    return true;
}

void MediaWidget::setReadOnly(bool readOnly) {
    titleEdit->setReadOnly(readOnly);
    authorEdit->setReadOnly(readOnly);
    genreComboBox->setEnabled(!readOnly);
    yearEdit->setReadOnly(readOnly);
    languageEdit->setReadOnly(readOnly);
    collocationEdit->setReadOnly(readOnly);
    ratingEdit->setReadOnly(readOnly);
}

QString MediaWidget::getLabelStyle() const {
    return "QLabel { font-size: 14px; }";
}

QString MediaWidget::getInputStyle() const {
    return "QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit{ "
           "border: 1px solid #ccc; "
           "border-radius: 4px; "
           "min-height: 28px; "
           "padding: 6px; "
           "font-size: 14px; "
           "}"
           "QSpinBox::up-button, QDoubleSpinBox::up-button { width: 20px; }"
           "QSpinBox::down-button, QDoubleSpinBox::down-button { width: 20px; }";
}

QString MediaWidget::getTitolo() const {
    return titleEdit->text();
}

QString MediaWidget::getAutore() const {
    return authorEdit->text();
}

int MediaWidget::getAnno() const {
    return yearEdit->value();
}