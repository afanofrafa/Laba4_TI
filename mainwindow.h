#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSpinBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
using namespace std;
class adapter_QByteArray : QByteArray {
public:
    adapter_QByteArray() : QByteArray() {};
    adapter_QByteArray(QByteArray byteArray) : QByteArray(byteArray) {};
    QByteArray getQByteArray() const {
        return *this;
    }
    void removeLast() {
        if (!QByteArray::isEmpty())
            QByteArray::remove(QByteArray::size() - 1, 1);
    }
    unsigned char at_uchar(int& i) {
        return static_cast<unsigned char>(QByteArray::at(i));
    }
    int at(bool isChar, int& i) {
        if (isChar)
            return QByteArray::at(i);
        else
            return static_cast<unsigned char>(QByteArray::at(i));
    }
    uint size() {
        return QByteArray::size();
    }
    int lastIndexOf(const char *str) {
        return QByteArray::lastIndexOf(str);
    }
    bool isDigit(const char &symbol) {
        return isdigit(symbol);
    }
    /*bool isDigit(const unsigned char &symbol) {
        return isdigit(symbol);
    }*/
    void push_back(const char *str) {
        QByteArray::push_back(str);
    }
    /*void push_back(const unsigned char *str) {
        QByteArray::push_back(str);
    }*/
    ~adapter_QByteArray() {};
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_choose_primitives_clicked();

    void on_pushButton_show_new_primitives_clicked();

    bool spinBox_value_changed(unsigned long long &v, QSpinBox *spinBox);

    void on_spinBox_p_valueChanged(int arg1);

    void on_spinBox_q_valueChanged(int arg1);

    void on_pushButton_apply_clicked();

    void on_comboBox_choose_primitive_e_currentIndexChanged(int index);

    void on_pushButton_choose_file_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
