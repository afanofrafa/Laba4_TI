#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QByteArray>
#include <QFileDialog>
#include <QMessageBox>
unsigned long long last_primitive = 2;
unsigned long long P = 2;
unsigned long long Q = 2;
unsigned long long R = 4;
unsigned long long N = 1;
unsigned long long E = -1;
unsigned long long D = -1;
unsigned long long M1 = -1;
unsigned long long M2 = -1;
unsigned long long S = -1;
bool canApplyP = true;
bool canApplyQ = true;
QString filePath;
QString path = "";
adapter_QByteArray file_text;
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
// Реализация функции для нахождения НОД
int gcdExtended(int a, int b, int& x, int& y) {
    // Базовый случай: если одно из чисел равно 0
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    int x1, y1; // Для хранения результатов рекурсивного вызова
    int gcd = gcdExtended(b % a, a, x1, y1);

    // Обновляем x и y согласно формулам Безу
    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}
unsigned long long _pow_mod(unsigned long long value, unsigned long long degree, unsigned long long n_mod) {
    unsigned long long result = 1;
    while (degree) {
        if (degree & 0x01)
            result = (result * value) % n_mod;
        value = (value * value) % n_mod;
        degree >>= 1;
    }
    return result;
}

double _pow(double value, unsigned long long degree) {
    double result = 1;
    while (degree) {
        if (degree & 0x01)
            result *= value;
        value *= value;
        degree >>= 1;
    }
    return result;
}

bool fermat_test(const unsigned long long& n, int k) {
    for (int i = 0; i < k; i++) {
        // Генерируем случайное число a в интервале [2, n-1]
        unsigned long long a;
        if (n > 2)
            a = 2 + rand() % (n - 2);
        else {
            a = n;
            if (a == 2)
                return true;
        }
        // Вычисляем a^(n-1) по модулю n
        unsigned long long res = _pow_mod(a, n - 1, n);

        // Если результат не равен 1, то n составное
        if (res != 1)
            return false;
    }
    // Если число прошло все проверки, то оно вероятно простое
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton_p->setChecked(true);
    on_pushButton_show_new_primitives_clicked();
    ui->pushButton_choose_file->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
    ui->radioButton_true_file->setChecked(true);
    ui->radioButton_true_file->setEnabled(false);
    ui->radioButton_gotten_file->setEnabled(false);
    ui->radioButton_uchar->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_choose_primitives_clicked()
{
    QString value = ui->comboBox_choose_primitive->currentText();
    for (int i = 0; i < value.size(); i++) {
        if (value.at(i) == ' ') {
            i++;
            value.erase(value.begin(), value.begin() + i);
            if (ui->radioButton_p->isChecked()) {
                ui->spinBox_p->setValue(value.toUInt());
                ui->radioButton_q->setChecked(true);
                break;
            }
            else {
                if (ui->radioButton_q->isChecked()) {
                    ui->spinBox_q->setValue(value.toUInt());
                    ui->radioButton_p->setChecked(true);
                    break;
                }
            }
        }
    }
}


void MainWindow::on_pushButton_show_new_primitives_clicked()
{
    unsigned long long it = 0;
    if (last_primitive + 1 < INT_MAX)
        for (unsigned long long i = last_primitive; it == 0 || it % 100 != 0; i++) {
            if (fermat_test(i, 40)) {
                QString str = QString::number(ui->comboBox_choose_primitive->count() + 1) + ": " + QString::number(i);
                ui->comboBox_choose_primitive->addItem(str);
                it++;
            }
            if (last_primitive + 1 < INT_MAX)
                last_primitive = i + 1;
        }
}

bool MainWindow::spinBox_value_changed(unsigned long long &v, QSpinBox *spinBox)
{
    v = spinBox->value();
    if (fermat_test(v, 40)) {
        spinBox->setStyleSheet("QSpinBox { background-color: white; }");
        return true;
    }
    else {
        spinBox->setStyleSheet("QSpinBox { background-color: #ff6f71; }");
        ui->pushButton_apply->setEnabled(false);
        return false;
    }
}


void MainWindow::on_spinBox_p_valueChanged(int arg1)
{
    canApplyP = spinBox_value_changed(P, ui->spinBox_p);
    if (canApplyP && canApplyQ)
        ui->pushButton_apply->setEnabled(true);
    else
        ui->pushButton_apply->setEnabled(false);
}


void MainWindow::on_spinBox_q_valueChanged(int arg1)
{
    canApplyQ = spinBox_value_changed(Q, ui->spinBox_q);
    if (canApplyP && canApplyQ)
        ui->pushButton_apply->setEnabled(true);
    else
        ui->pushButton_apply->setEnabled(false);
}


void MainWindow::on_pushButton_apply_clicked()
{
    if (canApplyP && canApplyQ) {
        ui->label_r->setText("R = " + QString::number(R = P * Q));
        ui->label_n->setText("N = f(R) = " + QString::number( N = (P - 1) * (Q - 1) ));
        ui->comboBox_choose_primitive_e->clear();
        for (int i = 2; i < N; i++) {
            if (gcd(i, N) == 1)
                ui->comboBox_choose_primitive_e->addItem(QString::number(i));
        }
        if (ui->comboBox_choose_primitive_e->count()) {
            E = ui->comboBox_choose_primitive_e->currentText().toInt();
            int x, y;
            gcdExtended(N, E, x, y);
            if (y < 0)
                y += N;
            ui->label_d->setText("D = " + QString::number(D = y));
            ui->label_Ko->setText("Открытый ключ: (E, R) = (" + QString::number(E) + ", " + QString::number(R) + ")");
            ui->label_Kc->setText("Закрытый ключ: (D, R) = (" + QString::number(D) + ", " + QString::number(R) + ")");
            ui->pushButton_choose_file->setEnabled(true);
        }
        else {
            E = -1;
            D = -1;
            ui->label_d->setText("D = __");
            ui->label_Ko->setText("Открытый ключ: __");
            ui->label_Kc->setText("Закрытый ключ: __");
            ui->pushButton_choose_file->setEnabled(false);
        }
    }
    else {
        ui->pushButton_choose_file->setEnabled(false);
        ui->label_r->setText("R = __");
        ui->label_n->setText("N = f(R) = __");
        ui->label_d->setText("D = __");
        ui->label_Ko->setText("Открытый ключ: __");
        ui->label_Kc->setText("Закрытый ключ: __");
        E = -1;
        D = -1;
        ui->comboBox_choose_primitive_e->clear();
    }
}


void MainWindow::on_comboBox_choose_primitive_e_currentIndexChanged(int index)
{
    if (ui->comboBox_choose_primitive_e->count()) {
        E = ui->comboBox_choose_primitive_e->currentText().toInt();
        int x, y;
        gcdExtended(N, E, x, y);
        if (y < 0)
            y += N;
        ui->label_d->setText("D = " + QString::number(D = y));
        ui->label_Ko->setText("Открытый ключ: (E, R) = (" + QString::number(E) + ", " + QString::number(R) + ")");
        ui->label_Kc->setText("Закрытый ключ: (D, R) = (" + QString::number(D) + ", " + QString::number(R) + ")");
    }
    else {
        E = -1;
        D = -1;
        ui->label_d->setText("D = __");
        ui->label_Ko->setText("Открытый ключ: __");
        ui->label_Kc->setText("Закрытый ключ: __");
    }
}

void MainWindow::on_pushButton_choose_file_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "C:/Users/Archie/Documents/ТИ_файлы_лаба2", "Все файлы (*.*)");
    if (filePath.isEmpty()) {
        ui->label_file_path->setText("");
        QMessageBox::warning(this, "Предупреждение", "Файл не выбран.");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для чтения.");
        return;
    }
    ui->label_file_path->setText("Текущий путь к файлу: " + filePath);
    file_text = file.readAll(); // Чтение всех байтов файла в QByteArray
    float pr_bar_value = 0;
    unsigned long long size = (unsigned long long)(file_text.size());
    ui->textEdit_hash_count->clear();
    ui->progressBar->setVisible(true);
    ui->progressBar->setEnabled(true);
    int ind = -1;
    if (ui->radioButton_gotten_file->isChecked())
        ind = file_text.lastIndexOf("\n");
    else
        ind = file_text.size();
    if (ind == -1)
        ind = file_text.size();
    unsigned long long H = 100;
    bool isCorrect;
    string digit = "";
    for (int i = ind + 1; i < file_text.size(); i++) {
        digit += file_text.at(true, i);
    }
    int num = atoi(digit.c_str());
    if (num == 0 && digit != "0") {
        isCorrect = false;
    } else {
        isCorrect = true;
    }
    if (!isCorrect)
        ind = file_text.size();
    bool isChar = ui->radioButton_char->isChecked();
    for (int i = 0; i < ind; i++) {
        QString str1 = "H" + QString::number(i + 1) + " = " + "(" + "H" + QString::number(i) + " + M" + QString::number(i + 1) + ")^2 mod R = ";
        QString str2 = "(" + QString::number(H) + " + " + QString::number(file_text.at(isChar, i)) + ")^2 mod " + QString::number(R) + " = ";
        H = _pow_mod(H + file_text.at(isChar, i), 2, R);
        QString str3 = QString::number(H) + ";";
        ui->textEdit_hash_count->append(str1 + str2 + str3);
        pr_bar_value++;
        ui->progressBar->setValue(pr_bar_value / size * 100);
    }
    if (ui->radioButton_true_file->isChecked()) {
        ui->label_hash_true->setText("Хэш-образ исходного файла M1: " + QString::number(M1 = H));
        ui->label_s->setText("Подпись S: " + QString::number(S = _pow_mod(M1, D, R)));
        file_text.push_back('\n' + QByteArray::number(S));
        int sl = filePath.lastIndexOf('/');
        path = "";
        for (int i = 0; i <= sl && i < filePath.size(); i++)
            path += filePath.at(i);
        path += "New_";
        for (int i = sl + 1; i < filePath.size(); i++)
            path += filePath.at(i);
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
            return;
        }
        file.write(file_text.getQByteArray());
        file.close();
        M2 = -1;
        ui->label_hash_gotten->clear();
        ui->label_result->clear();
        ui->label_hash_counted->clear();
        ui->radioButton_gotten_file->setChecked(true);
        ui->radioButton_true_file->setEnabled(true);
        ui->radioButton_gotten_file->setEnabled(true);
    }
    else {
        ui->label_hash_gotten->setText("Хэш-образ получателя файла M2: " + QString::number(M2 = H));
        unsigned long long M;
        if (isCorrect) {
            char c = ' ';
            for (int j = file_text.size() - 1; j >= 0 && c != '\n'; j--) {
                c = file_text.at(true, j);
                file_text.removeLast();
            }
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
                return;
            }
            file.write(file_text.getQByteArray());
            file.close();
            S = num;
        }
        ui->label_hash_counted->setText("Рассчитанный правильный хэш: " + QString::number(M = _pow_mod(S, E, R)));
        if (M != M2) {
            ui->label_result->setStyleSheet("color: red;");
            ui->label_result->setText("Несоответствие!");
        } else {
            ui->label_result->setStyleSheet("color: green;");
            ui->label_result->setText("Соответствие!");
        }
        ui->radioButton_true_file->setChecked(true);
    }
    file.close();
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
}

