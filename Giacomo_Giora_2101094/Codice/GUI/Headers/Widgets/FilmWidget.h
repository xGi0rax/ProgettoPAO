#ifndef FILMWIDGET_H
#define FILMWIDGET_H

#include "MediaWidget.h"
#include "../../../Modello logico/Headers/Film.h"
#include <QSpinBox>
#include <QLineEdit>

class FilmWidget : public MediaWidget {
    Q_OBJECT

private:
    Film *currentFilm;
    QSpinBox *durationFilmEdit;
    QLineEdit *castEdit;

public:
    explicit FilmWidget(QWidget *parent = nullptr);
    virtual ~FilmWidget() = default;

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

#endif // FILMWIDGET_H