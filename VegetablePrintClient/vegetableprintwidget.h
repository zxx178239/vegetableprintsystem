#ifndef VEGETABLEPRINTWIDGET_H
#define VEGETABLEPRINTWIDGET_H

#include <QWidget>
#include "common.h"
#include <QStandardItemModel>

namespace Ui {
class VegetablePrintWidget;
}

class VegetablePrintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VegetablePrintWidget(QWidget *parent = 0);
    ~VegetablePrintWidget();

private:
    Ui::VegetablePrintWidget *ui;

    QStandardItemModel *model;

private:
    void InitUI();

    void InitVegeNameBtns();

    void InitTableViewForPrint();

    void InitBtns();

    void InitLabels();

public slots:
    void ClickVegeBtn();

    void ShowDeleteMenu(const QPoint);

    void ClickDelete();

    void ClickPreviewBtn();

    void ClickPrintBtn();

    void PrintPreviewSlot(QPrinter *);

    void ChangeTheValueAndTotalPrice(QModelIndex,QModelIndex,QVector<int>);
};

#endif // VEGETABLEPRINTWIDGET_H
