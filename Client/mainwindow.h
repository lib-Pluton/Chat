#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
#include <QTcpSocket>
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = Q_NULLPTR);
  ~MainWindow();

protected:
 virtual void checkBorderDragging(QMouseEvent *event);
 virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_sayButton_clicked();
    void readyRead();
    void connected();

private:
 bool leftBorderHit(const QPoint &pos);
 bool rightBorderHit(const QPoint &pos);
 bool topBorderHit(const QPoint &pos);
 bool bottomBorderHit(const QPoint &pos);
 void styleWindow(bool bActive, bool bNoState);

 private:
  Ui::MainWindow *ui;
  QRect m_StartGeometry;
  const quint8 CONST_DRAG_BORDER_SIZE = 15;
  bool m_bMousePressed;
  bool m_bDragTop;
  bool m_bDragLeft;
  bool m_bDragRight;
  bool m_bDragBottom;
  QTcpSocket *socket;
};

#endif
