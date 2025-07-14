#include "../Headers/LoginPage.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>

LoginPage::LoginPage(QWidget *parent) : QWidget(parent) {
    setupUI();
}

// ========================================
// METODI DI INIZIALIZZAZIONE
// ========================================
void LoginPage::setupUI() {
    // Configurazione background pagina
    setObjectName("loginPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    setStyleSheet("#loginPage {"
                  "background-image: url(:/Immagini/SfondoLogin.jpg);"
                  "background-repeat: no-repeat;"
                  "background-position: center center;"
                  "background-attachment: fixed;"
                  "}");

    // Creazione frame principale
    QFrame *loginFrame = new QFrame(this);
    loginFrame->setFixedSize(450, 350);
    loginFrame->setStyleSheet("QFrame { "
                              "background-color: rgb(42, 68, 113); "
                              "border: 2px solid #000000; "
                              "border-radius: 10px; "
                              "}");

    // Label di benvenuto
    QLabel *welcomeLabel = new QLabel("Benvenuto nella BananoTECH-a", loginFrame);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("background-color: #333333; "
                                "font-size: 24px; "
                                "font-weight: bold; "
                                "color: white;");

    // Campo username
    usernameField = new QLineEdit(loginFrame);
    usernameField->setPlaceholderText("Username");
    usernameField->setFixedSize(330, 40);
    usernameField->setStyleSheet("QLineEdit { "
                                 "background-color: rgb(237, 236, 236); "
                                 "color: black; "
                                 "border: 1px solid #AAAAAA; "
                                 "border-radius: 5px; "
                                 "padding: 5px; "
                                 "font-size: 16px; "
                                 "selection-background-color: #4a90e2; "
                                 "}");
        
    // Campo password
    passwordField = new QLineEdit(loginFrame);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFixedSize(330, 40);
    passwordField->setStyleSheet("QLineEdit { "
                                 "background-color: rgb(237, 236, 236); "
                                 "color: black; "
                                 "border: 1px solid #AAAAAA; "
                                 "border-radius: 5px; "
                                 "padding: 5px; "
                                 "font-size: 16px; "
                                 "selection-background-color: #4a90e2; "
                                 "}");

    // Pulsante login
    loginButton = new QPushButton("Accedi", loginFrame);
    loginButton->setFixedSize(150, 45);
    loginButton->setStyleSheet("QPushButton { "
                               "background-color: rgb(225, 192, 37); "
                               "border: 1px solid #AAAAAA; "
                               "border-radius: 5px; "
                               "padding: 8px 16px; "
                               "font-size: 18px; "
                               "}"
                               "QPushButton:hover { "
                               "background-color: rgb(184, 174, 90); "
                               "}");
    
    // Connessione pulsante
    connect(loginButton, &QPushButton::clicked, this, &LoginPage::onLoginButtonClicked);

    // Label messaggi
    errorLabel = new QLabel(loginFrame);
    errorLabel->setStyleSheet("QLabel { "
                              "color: rgb(230, 230, 230); "
                              "font-size: 16px; "
                              "border: 0px; "
                              "}");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setText("Inserisci Username e Password per accedere.");

    // Layout interno frame
    QVBoxLayout *loginWidgetsLayout = new QVBoxLayout();
    loginWidgetsLayout->setContentsMargins(10, 10, 10, 10); 
    loginWidgetsLayout->setSpacing(10);
    loginWidgetsLayout->addWidget(welcomeLabel);
    loginWidgetsLayout->addWidget(usernameField, 0, Qt::AlignCenter);
    loginWidgetsLayout->addWidget(passwordField, 0, Qt::AlignCenter);
    loginWidgetsLayout->addWidget(loginButton, 0, Qt::AlignCenter);
    loginWidgetsLayout->addWidget(errorLabel);
    
    loginFrame->setLayout(loginWidgetsLayout);

    // Layout esterno per centratura
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addStretch();
    outerLayout->addWidget(loginFrame, 0, Qt::AlignCenter); 
    outerLayout->addStretch();
    
    setLayout(outerLayout);
}

// ========================================
// METODI DI UTILITA'
// ========================================
QString LoginPage::getUsername() const {
    return usernameField->text();
}

QString LoginPage::getPassword() const {
    return passwordField->text();
}

void LoginPage::showErrorMessage(const QString &message) {
    errorLabel->setText(message);
    errorLabel->setStyleSheet("QLabel { "
                              "color: rgb(255, 0, 0); "
                              "padding: 5px; "
                              "font-size: 16px; "
                              "}");
}

void LoginPage::clearErrorMessage() {
    errorLabel->clear();
    errorLabel->setStyleSheet("QLabel { "
                              "color: rgb(230, 230, 230); "
                              "padding: 5px; "
                              "font-size: 16px; "
                              "border: 0px; "
                              "}");
}

void LoginPage::resetToDefaultState() {
    usernameField->clear();
    passwordField->clear();
    errorLabel->setText("Inserisci Username e Password per accedere.");
    errorLabel->setStyleSheet("QLabel { "
                              "color: rgb(230, 230, 230); "
                              "padding: 5px; "
                              "font-size: 16px; "
                              "border: 0px; "
                              "}");
}

// ========================================
// EVENT HANDLERS
// ========================================
void LoginPage::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && 
        !usernameField->text().isEmpty() && 
        !passwordField->text().isEmpty()) {
        emit loginAttempted();
    } else {
        QWidget::keyPressEvent(event);
    }
}

// ========================================
// SLOTS
// ========================================
void LoginPage::onLoginButtonClicked() {
    clearErrorMessage();
    emit loginAttempted();
}