#ifndef LIBROWIDGET_H
#define LIBROWIDGET_H

#include "MediaWidget.h"
#include "../../../Modello logico/Headers/Libro.h"
#include <QSpinBox>
#include <QLineEdit>

class LibroWidget : public MediaWidget {
    Q_OBJECT

private:
    Libro *currentLibro;
    QLineEdit* isbnEdit;
    QLineEdit* editorEdit;
    QSpinBox* pagesEdit;

public:
    explicit LibroWidget(QWidget *parent = nullptr);
    virtual ~LibroWidget() = default;

    // Metodi ereditati da MediaWidget
    virtual void setCurrentMedia(Media* media) override;
    virtual bool validateData() const override;
    virtual bool applyChanges() override; // (per modifyPage)
    virtual Media* createMedia() override; // (per AddPage)
    void setReadOnly(bool readOnly);

protected:
    void addSpecificFields() override;
    void setCurrentValues() override;
};

#endif // LIBROWIDGET_H