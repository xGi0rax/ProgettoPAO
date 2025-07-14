#ifndef VINILEWIDGET_H
#define VINILEWIDGET_H

#include "MediaWidget.h"
#include "../../../Modello logico/Headers/Vinile.h"
#include <QSpinBox>

class VinileWidget : public MediaWidget {
    Q_OBJECT

private:
    Vinile *currentVinile;
    
    QSpinBox* trackCountEdit;
    QSpinBox* durationEdit;

public:
    explicit VinileWidget(QWidget *parent = nullptr);
    virtual ~VinileWidget() = default;

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

#endif // VINILEWIDGET_H