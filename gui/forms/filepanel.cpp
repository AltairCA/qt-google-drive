#include "filepanel.h"
#include "ui_filepanel.h"
#include "share/defs.h"
#include "share/debug.h"
#include  <QApplication>

FilePanel::FilePanel(int pn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel),
    panelNum(pn)
{
    ui->setupUi(this);
    init();
}

void FilePanel::init(void)
{
    accountsToolBar = new ToolBar;
    ui->verticalLayout->insertWidget(0, accountsToolBar);

    accountsComboBox = new ComboBox(26);
    accountsToolBar->addWidget(accountsComboBox);

    connect(accountsComboBox, SIGNAL(activated(const QString&)), SLOT(slotActivated(const QString&)));
}

FilePanel::~FilePanel()
{
    delete ui;
}

void FilePanel::slotActivated(const QString &text)
{
    int beginPos = text.indexOf(ACCOUNT_SEPARATOR_BEGIN) + 1;
    int length = text.lastIndexOf(ACCOUNT_SEPARATOR_END) - beginPos;

    emit signalAccountChanged(panelNum, text.mid(beginPos, length));
}

QTreeWidget* FilePanel::getFileView(void) const
{
    return ui->fileView;
}

QLabel* FilePanel::getPathLabel(void) const
{
    return ui->pathLabel;
}

ToolBar* FilePanel::getAccountsToolBar(void) const
{
    return accountsToolBar;
}

ComboBox* FilePanel::getAccountsComboBox(void) const
{
    return accountsComboBox;
}

void FilePanel::fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
    QStringList keys(accountsMap.keys());

    accountsComboBox->clear();

    for(int i = 0; i < keys.count(); ++i)
    {
        QString discLetter(keys[i]);

        discLetter = discLetter.rightJustified(2,' ');
        discLetter = discLetter.leftJustified(6, ' ');

        accountsComboBox->addItem(discLetter + ACCOUNT_SEPARATOR_BEGIN + accountsMap[keys[i]] + ACCOUNT_SEPARATOR_END);
        accountsComboBox->setItemIcon(i, QIcon(QApplication::style()->standardIcon(QStyle::QStyle::SP_DriveFDIcon)));

        if(currentAccount == accountsMap[keys[i]] && accountsComboBox->currentIndex() != i)
        {
          accountsComboBox->setCurrentIndex(i);
        }
    }  
}

