#include "vegetableprintwidget.h"
#include "ui_vegetableprintwidget.h"


VegetablePrintWidget::VegetablePrintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VegetablePrintWidget)
{
    ui->setupUi(this);
    InitUI();
}

VegetablePrintWidget::~VegetablePrintWidget()
{
    delete ui;
}

void VegetablePrintWidget::InitUI()
{
    // 初始化菜单名的按钮框
    InitVegeNameBtns();
    // 初始化tableview的表头
    InitTableViewForPrint();
    // 初始化按钮 控件
    InitBtns();

    // 初始化Label信息
    InitLabels();
}

void VegetablePrintWidget::InitVegeNameBtns()
{
    const vector<string> curVegetableNames = DataManager::getInstance()->getVegeNames();


    for(int i = 0, j = 0; j < curVegetableNames.size(); ++ j)
    {
        QPushButton *newButton  = new QPushButton(QString::fromLocal8Bit(curVegetableNames[j].data()));
        newButton->setObjectName(QString::fromLocal8Bit(curVegetableNames[j].data()));
        //QPushButton *newButton  = new QPushButton(curVegetableNames.at(j));
        //newButton->setObjectName(curVegetableNames.at(j));
        ui->gridLayout->addWidget(newButton, j / ROW_ELEMENTS, i);
        i = ((i + 1 == ROW_ELEMENTS) ? 0 : i + 1);

        connect(newButton, SIGNAL(clicked(bool)), this, SLOT(ClickVegeBtn()));
    }
}


void VegetablePrintWidget::InitTableViewForPrint()
{
    ui->tableWidget->setColumnCount(TABLEVIEWHEAD_COUNT);
    //ui->tableWidget->horizontalHeader()->set

    QStringList header;
    for(int i = 0; i < TABLEVIEWHEAD_COUNT; ++ i)
    {
        header << tr(TABLEVIEWHEAD[i].c_str());
        ui->tableWidget->horizontalHeader()->resizeSection(i,110);
    }
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowDeleteMenu(QPoint)));

}

void VegetablePrintWidget::InitBtns()
{
    connect(ui->prePrintBtn, SIGNAL(clicked(bool)), this, SLOT(ClickPreviewBtn()));
    connect(ui->printBtn, SIGNAL(clicked(bool)), this, SLOT(ClickPrintBtn()));
}

void VegetablePrintWidget::InitLabels()
{
    // 初始化日期
    QDateTime current_date_time =QDateTime::currentDateTime();
    //QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz ddd");
    QString current_date =current_date_time.toString("yyyy-MM-dd");
    ui->dataLabel->setText(current_date);

    // 初始化总价
    ui->priceLabel->setText(QString::number(0.0) + " 元");

    // 初始化总量
    ui->numberLabel->setText(QString::number(0));
}

void VegetablePrintWidget::ClickVegeBtn()
{
    //QMessageBox::information(this, "test widget", "hello");
    // 点击之后在listview中添加一行内容，作为当前买家的一个购买记录，该记录是可以删除的。
    int row_count = ui->tableWidget->rowCount();
    cout << "cur row: " << row_count << endl;
    ui->tableWidget->insertRow(row_count);

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setText(QObject::sender()->objectName());


    double vegePrice = DataManager::getInstance()->getVegePriceThroughName(string(QObject::sender()->objectName().toLocal8Bit()));
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setText(QString::number(vegePrice, 10, 2));


    ui->tableWidget->setItem(row_count, 0, item1);
    ui->tableWidget->setItem(row_count, 1, item2);

    // 每增加一行记录，则修改总量
    ui->numberLabel->setText(QString::number(ui->tableWidget->rowCount()));
}

void VegetablePrintWidget::ShowDeleteMenu(const QPoint pos)
{
    cout << "Show Delete Menu" << endl;
    QMenu *menu = new QMenu(ui->tableWidget);
    QAction *pDelete = new QAction("删除",ui->tableWidget);
    connect (pDelete, SIGNAL(triggered()), this, SLOT(ClickDelete()));

    menu->addAction(pDelete);

    menu->move(cursor().pos());
    menu->show();
    //获得鼠标点击的x，y坐标点
    int x = pos.x();
    int y = pos.y();
    QModelIndex index = ui->tableWidget->indexAt(QPoint(x,y));
    int row = index.row ();//获得QTableWidget列表点击的行数
}

void VegetablePrintWidget::ClickDelete()
{
    //QMessageBox::information(this, "delete test", "delete");
    cout << "Click  Delete" << endl;
    int currentSelectedRow = ui->tableWidget->currentRow();
    if(currentSelectedRow != -1)
    {
        ui->tableWidget->removeRow(currentSelectedRow);
        // 每删除一行记录，也要修改总量
        ui->numberLabel->setText(QString::number(ui->tableWidget->rowCount()));
    }



}

void VegetablePrintWidget::ClickPreviewBtn()
{
    cout << "click preview btn" << endl;

    QPrinter printer(QPrinter::HighResolution);
    //自定义纸张大小
    printer.setPageSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(ui->printArea->height(), ui->printArea->width()),
                               QPrinter::Point);
    QPrintPreviewDialog preview(&printer, this);
    preview.setMinimumSize(1000,600);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(PrintPreviewSlot(QPrinter*)));
    preview.exec();
}

void VegetablePrintWidget::ClickPrintBtn()
{
    cout <<  "click  print btn" << endl;

    // 创建打印机对象
    QPrinter printer;
    // 创建打印对话框
    QString printerName = printer.printerName();
    if( printerName.size() == 0)
        return;
    QPrintDialog dlg(&printer, this);

    // 如果在对话框中按下了打印按钮，则执行打印操作
    if (dlg.exec() == QDialog::Accepted)
    {
        cout << "start print" << endl;
        //ui->tableWidget->print(&printer);
    }
}

void VegetablePrintWidget::PrintPreviewSlot(QPrinter *printerPixmap)
{
    printerPixmap->setOrientation(QPrinter::Landscape);
    //获取界面的图片
    QPixmap pixmap = QPixmap::grabWidget(ui->printArea, ui->printArea->rect());
    QPainter painterPixmap(this);
    painterPixmap.begin(printerPixmap);
    QRect rect = painterPixmap.viewport();
    int x = rect.width() / pixmap.width();
    int y = rect.height() / pixmap.height();
    painterPixmap.scale(x, y);
    painterPixmap.drawPixmap(0, 0, pixmap);
    painterPixmap.end();
}
