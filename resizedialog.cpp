#include "resizedialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>

ResizeDialog::ResizeDialog(QWidget *parent, QSize *imageSize) : QDialog(parent)
{
    resultSize=imageSize;
    proportion = 1.0 * imageSize->width() / imageSize->height();

    wLabel = new QLabel(tr("&Width:"));
    hLabel = new QLabel(tr("&Height:"));

    QRegExp regExp("[1-9][0-9]{0,4}");

    wLineEdit = new QLineEdit(QString::number(imageSize->width()));
    wLineEdit->setValidator(new QRegExpValidator(regExp, this));
    wLineEdit->setFixedWidth(50);

    hLineEdit = new QLineEdit(QString::number(imageSize->height()));
    hLineEdit->setValidator(new QRegExpValidator(regExp, this));
    hLineEdit->setFixedWidth(50);

    wLabel->setBuddy(wLineEdit);
    hLabel->setBuddy(hLineEdit);

    checkBox = new QCheckBox(tr("Save proportions:"));

    okButton = new QPushButton(tr("Ok"));
    okButton->setDefault(true);
    okButton->setDisabled(true);

    cancelButton = new QPushButton(tr("Cancel"));

    connect(wLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateWidgets(const QString&)));
    connect(hLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateWidgets(const QString&)));
    connect(checkBox, SIGNAL(clicked()), this, SLOT(updateWidgets()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(cancelButton, SIGNAL(clicked()),this, SLOT(reject()));

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(wLabel);
    labelLayout->addWidget(hLabel);

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(wLineEdit);
    editLayout->addWidget(hLineEdit);

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addLayout(labelLayout);
    topLeftLayout->addLayout(editLayout);

    QVBoxLayout *leftLayot = new QVBoxLayout;
    leftLayot->addLayout(topLeftLayout);
    leftLayot->addWidget(checkBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(okButton);
    rightLayout->addWidget(cancelButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayot);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
    setModal(true);
    setWindowTitle(tr("Resize Image"));
    setFixedHeight(sizeHint().height());
}

void ResizeDialog::updateWidgets(const QString &text)
{
    okButton->setEnabled(!text.isEmpty());
    if(!checkBox->isChecked())
        return;
    if(this->sender()==hLineEdit)
    {
        double newW =proportion*hLineEdit->text().toInt();
        wLineEdit->setText(QString::number(static_cast<int>(newW)));
    }
    else
    {
        double newH = (1.0/proportion)*wLineEdit->text().toInt();
        hLineEdit->setText(QString::number(static_cast<int>(newH)));
    }
}

void ResizeDialog::okClicked()
{
    resultSize->setWidth(wLineEdit->text().toInt());
    resultSize->setHeight(hLineEdit->text().toInt());
    this->accept();
}

ResizeDialog::~ResizeDialog()
{

}
