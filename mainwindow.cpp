#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QUrl>
#include <QDateTime>
#include <QRegularExpressionValidator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QUuid>
#include <QClipboard>
#include <QColorDialog>
#include <QColor>
#include <QFile>

#define APP_VERSION "1.1.0"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("MegaTools Developer Utilities v%1").arg(APP_VERSION));
    ui->statusbar->showMessage(QString("Version: %1").arg(APP_VERSION));

    // Set the application icon
    this->setWindowIcon(QIcon(":/appicon.ico"));

//    on_actionDark_Mode_triggered();

    // --- Number Base Validators ---
    decValidator = new QRegularExpressionValidator(QRegularExpression("[0-9]+"), this);
    ui->decLineEdit->setValidator(decValidator);
    hexValidator = new QRegularExpressionValidator(QRegularExpression("[0-9A-Fa-f]+"), this);
    ui->hexLineEdit->setValidator(hexValidator);
    binValidator = new QRegularExpressionValidator(QRegularExpression("[0-1]+"), this);
    ui->binLineEdit->setValidator(binValidator);
    octValidator = new QRegularExpressionValidator(QRegularExpression("[0-7]+"), this);
    ui->octLineEdit->setValidator(octValidator);    
    on_btnNow_clicked();// Set initial timestamp to current time
    on_btnGenerateUuid_clicked(); // Generate a UUID on startup
    ui->btnCopyUuid->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    // Color Converter
    hexColorValidator = new QRegularExpressionValidator(QRegularExpression("#?[0-9A-Fa-f]{6}"), this);
    ui->hexColorLineEdit->setValidator(hexColorValidator);
    QIntValidator* rgbIntValidator = new QIntValidator(0, 255, this);
    ui->redLineEdit->setValidator(rgbIntValidator);
    ui->greenLineEdit->setValidator(rgbIntValidator);
    ui->blueLineEdit->setValidator(rgbIntValidator);
    QIntValidator* hslIntValidator = new QIntValidator(0, 359, this); // H & S can be up to 359/255, L up to 255
    ui->hueLineEdit->setValidator(hslIntValidator);
    ui->satLineEdit->setValidator(new QIntValidator(0, 255, this));
    ui->lightLineEdit->setValidator(new QIntValidator(0, 255, this));
    updateColorFields(QColor(Qt::blue), nullptr); // Start with a default color

    ui->menubar->hide(); // until qss is fixed

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::applyStyleSheet(const QString& sheetName)
{
    QFile file(":/"+ sheetName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);
}

void MainWindow::on_actionLight_Mode_triggered()
{
    applyStyleSheet("light.qss");
}

void MainWindow::on_actionDark_Mode_triggered()
{
    applyStyleSheet("dark.qss");
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

    ui->md5Result->setText(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
    ui->sha1Result->setText(QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex());
    ui->sha256Result->setText(QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex());
    ui->sha512Result->setText(QCryptographicHash::hash(data, QCryptographicHash::Sha512).toHex());
}

//=============================================================================
//  NUMBER BASE CONVERSIONS TAB
//=============================================================================

void MainWindow::updateNumberBases(qint64 value, QObject* sender)
{
    if (m_isUpdatingNumberBases) return;
    m_isUpdatingNumberBases = true;

    if (sender != ui->decLineEdit) ui->decLineEdit->setText(QString::number(value));
    if (sender != ui->hexLineEdit) ui->hexLineEdit->setText(QString::number(value, 16).toUpper());
    if (sender != ui->binLineEdit) ui->binLineEdit->setText(QString::number(value, 2));
    if (sender != ui->octLineEdit) ui->octLineEdit->setText(QString::number(value, 8));

    m_isUpdatingNumberBases = false;
}

void MainWindow::on_decLineEdit_textChanged(const QString &text) { bool ok; qint64 val = text.toLongLong(&ok); if (ok) updateNumberBases(val, ui->decLineEdit); }
void MainWindow::on_hexLineEdit_textChanged(const QString &text) { bool ok; qint64 val = text.toLongLong(&ok, 16); if (ok) updateNumberBases(val, ui->hexLineEdit); }
void MainWindow::on_binLineEdit_textChanged(const QString &text) { bool ok; qint64 val = text.toLongLong(&ok, 2); if (ok) updateNumberBases(val, ui->binLineEdit); }
void MainWindow::on_octLineEdit_textChanged(const QString &text) { bool ok; qint64 val = text.toLongLong(&ok, 8); if (ok) updateNumberBases(val, ui->octLineEdit); }



//=============================================================================
//  UNIX TIMESTAMP TAB
//=============================================================================

void MainWindow::on_btnTimestampToDate_clicked()
{
    bool ok;
    qint64 timestamp = ui->timestampLineEdit->text().toLongLong(&ok);
    if(ok) {
        ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC));
    } else {
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
//=============================================================================
//  JSON FORMATTER TAB
//=============================================================================
void MainWindow::on_btnFormatJson_clicked()
{
    QString rawJson = ui->jsonInput->toPlainText();
    if(rawJson.trimmed().isEmpty()) {
        ui->jsonStatusLabel->setText("Status: Input is empty.");
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = QString("Status: <span style='color:red;'>Invalid JSON</span> - %1 at offset %2")
                           .arg(parseError.errorString())
                           .arg(parseError.offset);
        ui->jsonStatusLabel->setText(errorMsg);
        ui->jsonOutput->clear();
    } else {
        ui->jsonStatusLabel->setText("Status: <span style='color:green;'>Valid JSON</span>");
        ui->jsonOutput->setPlainText(doc.toJson(QJsonDocument::Indented));
    }
}


//=============================================================================
//  UUID GENERATOR TAB
//=============================================================================
void MainWindow::on_btnGenerateUuid_clicked()
{
    // Generate a new UUID v4 and display it without curly braces.
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    ui->uuidLineEdit->setText(uuid);
}

void MainWindow::on_btnCopyUuid_clicked()
{
    QApplication::clipboard()->setText(ui->uuidLineEdit->text());
}

//=============================================================================
//  LOREM IPSUM TAB
//=============================================================================
void MainWindow::on_btnGenerateLorem_clicked()
{
    const QStringList dictionary = {
        "lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit",
        "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore", "et", "dolore",
        "magna", "aliqua", "enim", "ad", "minim", "veniam", "quis", "nostrud",
        "exercitation", "ullamco", "laboris", "nisi", "aliquip", "ex", "ea", "commodo",
        "consequat", "duis", "aute", "irure", "in", "reprehenderit", "voluptate",
        "velit", "esse", "cillum", "eu", "fugiat", "nulla", "pariatur", "excepturi",
        "sint", "occaecat", "cupidatat", "non", "proident", "sunt", "culpa", "qui",
        "officia", "deserunt", "mollit", "anim", "id", "est", "laborum"
    };

    int count = ui->loremCountSpinBox->value();
    QString result;

    if (ui->loremWordsRadio->isChecked()) {
        for (int i = 0; i < count; ++i) {
            result += dictionary[rand() % dictionary.size()] + " ";
        }
    } else if (ui->loremSentencesRadio->isChecked()) {
        for (int i = 0; i < count; ++i) {
            int sentenceLength = 5 + (rand() % 15); // 5 to 20 words
            QString sentence;
            for (int j = 0; j < sentenceLength; ++j) {
                sentence += dictionary[rand() % dictionary.size()] + " ";
            }
            result += sentence.trimmed().replace(0, 1, sentence.at(0).toUpper()) + ". ";
        }
    } else { // Paragraphs
        for (int i = 0; i < count; ++i) {
            int paragraphLength = 3 + (rand() % 5); // 3 to 8 sentences
            for (int j = 0; j < paragraphLength; ++j) {
                int sentenceLength = 5 + (rand() % 15);
                QString sentence;
                for (int k = 0; k < sentenceLength; ++k) {
                    sentence += dictionary[rand() % dictionary.size()] + " ";
                }
                result += sentence.trimmed().replace(0, 1, sentence.at(0).toUpper()) + ". ";
            }
            result += "\n\n";
        }
    }
    ui->loremOutput->setPlainText(result.trimmed());
}

//=============================================================================
//  COLOR CONVERTER TAB
//=============================================================================
void MainWindow::updateColorFields(const QColor& color, QObject* sender) {
    if (m_isUpdatingColors || !color.isValid()) return;
    m_isUpdatingColors = true;

    // Update Hex field
    if (sender != ui->hexColorLineEdit) {
        ui->hexColorLineEdit->setText(color.name().toUpper());
    }

    // Update RGB fields
    if (sender != ui->redLineEdit && sender != ui->greenLineEdit && sender != ui->blueLineEdit) {
        ui->redLineEdit->setText(QString::number(color.red()));
        ui->greenLineEdit->setText(QString::number(color.green()));
        ui->blueLineEdit->setText(QString::number(color.blue()));
    }

    // Update HSL fields
    if (sender != ui->hueLineEdit && sender != ui->satLineEdit && sender != ui->lightLineEdit) {
        ui->hueLineEdit->setText(QString::number(color.hslHue() == -1 ? 0 : color.hslHue()));
        ui->satLineEdit->setText(QString::number(color.hslSaturation()));
        ui->lightLineEdit->setText(QString::number(color.lightness()));
    }

    // Update color preview
    ui->colorPreviewFrame->setStyleSheet(QString("background-color: %1;").arg(color.name()));

    m_isUpdatingColors = false;
}

void MainWindow::on_btnPickColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->colorPreviewFrame->palette().color(QWidget::backgroundRole()), this, "Select Color");
    if(color.isValid()){
        updateColorFields(color, nullptr);
    }
}

void MainWindow::on_hexColorLineEdit_textChanged(const QString &text)
{
    QString cleanText = text;
    if (!cleanText.startsWith("#")) {
        cleanText = "#" + cleanText;
    }
    QColor color(cleanText);
    if(color.isValid()) {
        updateColorFields(color, ui->hexColorLineEdit);
    }
}

void MainWindow::on_rgbColorLineEdit_textChanged(const QString &)
{
    QColor color(ui->redLineEdit->text().toInt(), ui->greenLineEdit->text().toInt(), ui->blueLineEdit->text().toInt());
    if(color.isValid()){
        updateColorFields(color, ui->redLineEdit); // Sender can be any of the RGB fields
    }
}

void MainWindow::on_hslColorLineEdit_textChanged(const QString &)
{
    QColor color;
    color.setHsl(ui->hueLineEdit->text().toInt(), ui->satLineEdit->text().toInt(), ui->lightLineEdit->text().toInt());
    if(color.isValid()){
       updateColorFields(color, ui->hueLineEdit); // Sender can be any of the HSL fields
    }
}


//=============================================================================
//  REGEX TESTER TAB
//=============================================================================
void MainWindow::on_regexInput_textChanged(const QString &)
{
    on_regexTestString_textChanged(); // Retest when regex changes
}

void MainWindow::on_regexTestString_textChanged()
{
    ui->regexMatchesList->clear();
    QString pattern = ui->regexInput->text();
    QString testString = ui->regexTestString->toPlainText();

    if(pattern.isEmpty()){
        ui->regexStatusLabel->setText("Status: Enter a regular expression.");
        return;
    }

    QRegularExpression regex(pattern);
    if (!regex.isValid()) {
        ui->regexStatusLabel->setText(QString("Status: <span style='color:red;'>Invalid Pattern</span> - %1").arg(regex.errorString()));
        return;
    }

    QRegularExpressionMatchIterator i = regex.globalMatch(testString);
    int count = 0;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString matchText = match.captured(0);
        new QListWidgetItem(QString("Match %1: '%2' at position %3").arg(count + 1).arg(matchText).arg(match.capturedStart()), ui->regexMatchesList);
        count++;
    }

    ui->regexStatusLabel->setText(QString("Status: <span style='color:green;'>Found %1 matches.</span>").arg(count));
}
