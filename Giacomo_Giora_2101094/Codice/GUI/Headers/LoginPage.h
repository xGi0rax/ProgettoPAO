#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

    // ========================================
    // METODI DI UTILITA'
    // ========================================
    QString getUsername() const;
    QString getPassword() const;
    void showErrorMessage(const QString &message);
    void clearErrorMessage();
    void resetToDefaultState();
    
signals:
    void loginAttempted();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onLoginButtonClicked();

private:
    // ========================================
    // WIDGET UI
    // ========================================
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QLabel *errorLabel;

    // ========================================
    // METODI DI INIZIALIZZAZIONE
    // ========================================
    void setupUI();
};

#endif // LOGINPAGE_H