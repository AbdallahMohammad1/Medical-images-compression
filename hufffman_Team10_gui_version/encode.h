#ifndef ENCODE_H
#define ENCODE_H

#include <QDialog>

namespace Ui {
class encode;
}

class encode : public QDialog
{
    Q_OBJECT

public:
    explicit encode(QWidget *parent = nullptr);
    ~encode();

private slots:
    void on_pushButton_clicked();



private:
    Ui::encode *ui;
};

#endif // ENCODE_H
