#ifndef RIVISTAWIDGET_H
#define RIVISTAWIDGET_H

#include "MediaWidget.h"
#include "../../../Modello logico/Headers/Rivista.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>

class RivistaWidget : public MediaWidget {
    Q_OBJECT

private:
    Rivista *currentRivista;
    
    QLineEdit* editorEdit;
    QSpinBox* pagesEdit;
    QDateEdit* publicationDateEdit;
    QComboBox* periodicityComboBox;

public:
    explicit RivistaWidget(QWidget *parent = nullptr);
    virtual ~RivistaWidget() = default;

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

#endif // RIVISTAWIDGET_H