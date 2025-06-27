#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QUrl>
#include <QDateTime>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MegaConverter Developer Tools");

    // --- Setup Number Base Validators ---
    // This ensures users can only type valid characters in each field.
    decValidator = new QRegularExpressionValidator(QRegularExpression("[0-9]+"), this);
    ui->decLineEdit->setValidator(decValidator);

    hexValidator = new QRegularExpressionValidator(QRegularExpression("[0-9A-Fa-f]+"), this);
    ui->hexLineEdit->setValidator(hexValidator);
    
    binValidator = new QRegularExpressionValidator(QRegularExpression("[0-1]+"), this);
    ui->binLineEdit->setValidator(binValidator);
    
    octValidator = new QRegularExpressionValidator(QRegularExpression("[0-7]+"), this);
    ui->octLineEdit->setValidator(octValidator);

    // Set initial timestamp to current time
    on_btnNow_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//=============================================================================
//  TEXT CONVERSIONS TAB
//=============================================================================

void MainWindow::on_btnTextToHex_clicked()
{
    QString input = ui->textInput->toPlainText();
    QByteArray hex = input.toUtf8().toHex(' ');
    ui->textOutput->setPlainText(hex.toUpper());
}

void MainWindow::on_btnHexToText_clicked()
{
    // Remove spaces and newlines for robustness
    QString input = ui->textInput->toPlainText().remove(QChar(' ')).remove(QChar('\n'));
    QByteArray hex = QByteArray::fromHex(input.toLatin1());
    
    // Use a codec that supports a wide range of characters
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode(hex);
    
    ui->textOutput->setPlainText(text);
}

void MainWindow::on_btnTextToBase64_clicked()
{
    QString input = ui->textInput->toPlainText();
    QByteArray base64 = input.toUtf8().toBase64();
    ui->textOutput->setPlainText(base64);
}

void MainWindow::on_btnBase64ToText_clicked()
{
    QString input = ui->textInput->toPlainText();
    QByteArray text = QByteArray::fromBase64(input.toLatin1());
    ui->textOutput->setPlainText(text);
}

void MainWindow::on_btnUrlEncode_clicked()
{
    QString input = ui->textInput->toPlainText();
    QByteArray encoded = QUrl::toPercentEncoding(input);
    ui->textOutput->setPlainText(encoded);
}

void MainWindow::on_btnUrlDecode_clicked()
{
    QString input = ui->textInput->toPlainText();
    QString decoded = QUrl::fromPercentEncoding(input.toLatin1());
    ui->textOutput->setPlainText(decoded);
}


//=============================================================================
//  HASHING TAB
//=============================================================================

void MainWindow::on_hashInputText_textChanged()
{
    QString input = ui->hashInputText->toPlainText();
    QByteArray data = input.toUtf8();

    // Calculate MD5
    QByteArray md5Hash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
    ui->md5Result->setText(md5Hash);

    // Calculate SHA-1
    QByteArray sha1Hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
    ui->sha1Result->setText(sha1Hash);

    // Calculate SHA-256
    QByteArray sha256Hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
    ui->sha256Result->setText(sha256Hash);

    // Calculate SHA-512
    QByteArray sha512Hash = QCryptographicHash::hash(data, QCryptographicHash::Sha512).toHex();
    ui->sha512Result->setText(sha512Hash);
}

//=============================================================================
//  NUMBER BASE CONVERSIONS TAB
//=============================================================================

void MainWindow::updateNumberBases(qint64 value, QObject* sender)
{
    if (m_isUpdatingNumberBases) return; // Prevent recursive calls

    m_isUpdatingNumberBases = true;

    if (sender != ui->decLineEdit)
        ui->decLineEdit->setText(QString::number(value));
        
    if (sender != ui->hexLineEdit)
        ui->hexLineEdit->setText(QString::number(value, 16).toUpper());
        
    if (sender != ui->binLineEdit)
        ui->binLineEdit->setText(QString::number(value, 2));

    if (sender != ui->octLineEdit)
        ui->octLineEdit->setText(QString::number(value, 8));

    m_isUpdatingNumberBases = false;
}

void MainWindow::on_decLineEdit_textChanged(const QString &text)
{
    bool ok;
    qint64 val = text.toLongLong(&ok);
    if (ok) {
        updateNumberBases(val, ui->decLineEdit);
    }
}

void MainWindow::on_hexLineEdit_textChanged(const QString &text)
{
    bool ok;
    qint64 val = text.toLongLong(&ok, 16);
    if (ok) {
        updateNumberBases(val, ui->hexLineEdit);
    }
}

void MainWindow::on_binLineEdit_textChanged(const QString &text)
{
    bool ok;
    qint64 val = text.toLongLong(&ok, 2);
    if (ok) {
        updateNumberBases(val, ui->binLineEdit);
    }
}

void MainWindow::on_octLineEdit_textChanged(const QString &text)
{
    bool ok;
    qint64 val = text.toLongLong(&ok, 8);
    if (ok) {
        updateNumberBases(val, ui->octLineEdit);
    }
}


//=============================================================================
//  UNIX TIMESTAMP TAB
//=============================================================================

void MainWindow::on_btnTimestampToDate_clicked()
{
    bool ok;
    qint64 timestamp = ui->timestampLineEdit->text().toLongLong(&ok);
    if(ok)
    {
        // Set the datetime value, specifying UTC for universal time
        QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
        ui->dateTimeEdit->setDateTime(dt);
    }
    else
    {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid Unix timestamp (integer).");
    }
}

void MainWindow::on_btnDateToTimestamp_clicked()
{
    QDateTime dt = ui->dateTimeEdit->dateTime();
    qint64 timestamp = dt.toSecsSinceEpoch();
    ui->timestampLineEdit->setText(QString::number(timestamp));
}

void MainWindow::on_btnNow_clicked()
{
    // Set timestamp to current time
    qint64 now = QDateTime::currentSecsSinceEpoch();
    ui->timestampLineEdit->setText(QString::number(now));
    
    // Trigger the conversion to show the current human-readable time
    on_btnTimestampToDate_clicked();
}
