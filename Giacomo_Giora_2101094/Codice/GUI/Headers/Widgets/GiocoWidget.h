#ifndef GIOCOWIDGET_H
#define GIOCOWIDGET_H

#include "MediaWidget.h"
#include "../../../Modello logico/Headers/GiocoDaTavolo.h"
#include <QSpinBox>
#include <QLineEdit>

class GiocoWidget : public MediaWidget {
    Q_OBJECT

private:
    GiocoDaTavolo *currentGioco;
    QSpinBox* maxPlayersEdit;
    QSpinBox* playTimeEdit;
    QSpinBox* minAgeEdit;
    QLineEdit* editorEdit;

public:
    explicit GiocoWidget(QWidget *parent = nullptr);
    virtual ~GiocoWidget() = default;

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

#endif // GIOCOWIDGET_H