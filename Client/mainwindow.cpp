#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>

QString ip = "localhost";
int port = 4200;
QString usernaname = "TEST";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_bMousePressed(false),
      m_bDragTop(false),
      m_bDragLeft(false),
      m_bDragRight(false),
      m_bDragBottom(false) {
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
  setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
  setMouseTracking(true);
  socket = new QTcpSocket(this);
  socket->connectToHost(ip, port);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::checkBorderDragging(QMouseEvent *event) {
  if (isMaximized()) {
    return;
  }

  QPoint globalMousePos = event->globalPos();
  if (m_bMousePressed) {
    QRect availGeometry = QApplication::desktop()->availableGeometry();
    int h = availGeometry.height();
    int w = availGeometry.width();
    if (QApplication::desktop()->isVirtualDesktop()) {
      QSize sz = QApplication::desktop()->size();
      h = sz.height();
      w = sz.width();
    }
    if (m_bDragTop && m_bDragRight) {
      int diff =
          globalMousePos.x() - (m_StartGeometry.x() + m_StartGeometry.width());
      int neww = m_StartGeometry.width() + diff;
      diff = globalMousePos.y() - m_StartGeometry.y();
      int newy = m_StartGeometry.y() + diff;
      if (neww > 0 && newy > 0 && newy < h - 50) {
        QRect newg = m_StartGeometry;
        newg.setWidth(neww);
        newg.setX(m_StartGeometry.x());
        newg.setY(newy);
        setGeometry(newg);
      }
    }
    else if (m_bDragTop && m_bDragLeft) {
      int diff = globalMousePos.y() - m_StartGeometry.y();
      int newy = m_StartGeometry.y() + diff;
      diff = globalMousePos.x() - m_StartGeometry.x();
      int newx = m_StartGeometry.x() + diff;
      if (newy > 0 && newx > 0) {
        QRect newg = m_StartGeometry;
        newg.setY(newy);
        newg.setX(newx);
        setGeometry(newg);
      }
    }
    else if (m_bDragBottom && m_bDragLeft) {
      int diff =
          globalMousePos.y() - (m_StartGeometry.y() + m_StartGeometry.height());
      int newh = m_StartGeometry.height() + diff;
      diff = globalMousePos.x() - m_StartGeometry.x();
      int newx = m_StartGeometry.x() + diff;
      if (newh > 0 && newx > 0) {
        QRect newg = m_StartGeometry;
        newg.setX(newx);
        newg.setHeight(newh);
        setGeometry(newg);
      }
    } else if (m_bDragTop) {
      int diff = globalMousePos.y() - m_StartGeometry.y();
      int newy = m_StartGeometry.y() + diff;
      if (newy > 0 && newy < h - 50) {
        QRect newg = m_StartGeometry;
        newg.setY(newy);
        setGeometry(newg);
      }
    } else if (m_bDragLeft) {
      int diff = globalMousePos.x() - m_StartGeometry.x();
      int newx = m_StartGeometry.x() + diff;
      if (newx > 0 && newx < w - 50) {
        QRect newg = m_StartGeometry;
        newg.setX(newx);
        setGeometry(newg);
      }
    } else if (m_bDragRight) {
      int diff =
          globalMousePos.x() - (m_StartGeometry.x() + m_StartGeometry.width());
      int neww = m_StartGeometry.width() + diff;
      if (neww > 0) {
        QRect newg = m_StartGeometry;
        newg.setWidth(neww);
        newg.setX(m_StartGeometry.x());
        setGeometry(newg);
      }
    } else if (m_bDragBottom) {
      int diff =
          globalMousePos.y() - (m_StartGeometry.y() + m_StartGeometry.height());
      int newh = m_StartGeometry.height() + diff;
      if (newh > 0) {
        QRect newg = m_StartGeometry;
        newg.setHeight(newh);
        newg.setY(m_StartGeometry.y());
        setGeometry(newg);
      }
    }
  } else {
    if (leftBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
      setCursor(Qt::SizeFDiagCursor);
    } else if (rightBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
      setCursor(Qt::SizeBDiagCursor);
    } else if (leftBorderHit(globalMousePos) &&
               bottomBorderHit(globalMousePos)) {
      setCursor(Qt::SizeBDiagCursor);
    } else {
      if (topBorderHit(globalMousePos)) {
        setCursor(Qt::SizeVerCursor);
      } else if (leftBorderHit(globalMousePos)) {
        setCursor(Qt::SizeHorCursor);
      } else if (rightBorderHit(globalMousePos)) {
        setCursor(Qt::SizeHorCursor);
      } else if (bottomBorderHit(globalMousePos)) {
        setCursor(Qt::SizeVerCursor);
      } else {
        m_bDragTop = false;
        m_bDragLeft = false;
        m_bDragRight = false;
        m_bDragBottom = false;
        setCursor(Qt::ArrowCursor);
      }
    }
  }
}


bool MainWindow::leftBorderHit(const QPoint &pos) {
  const QRect &rect = this->geometry();
  if (pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE) {
    return true;
  }
  return false;
}

bool MainWindow::rightBorderHit(const QPoint &pos) {
  const QRect &rect = this->geometry();
  int tmp = rect.x() + rect.width();
  if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
    return true;
  }
  return false;
}

bool MainWindow::topBorderHit(const QPoint &pos) {
  const QRect &rect = this->geometry();
  if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
    return true;
  }
  return false;
}

bool MainWindow::bottomBorderHit(const QPoint &pos) {
  const QRect &rect = this->geometry();
  int tmp = rect.y() + rect.height();
  if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
    return true;
  }
  return false;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (isMaximized()) {
    return QWidget::eventFilter(obj, event);
  }
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
    if (pMouse) {
      checkBorderDragging(pMouse);
    }
  }
  else if (event->type() == QEvent::MouseButtonPress && obj == this) {
    QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
    if (pMouse) {
      mousePressEvent(pMouse);
    }
  } else if (event->type() == QEvent::MouseButtonRelease) {
    if (m_bMousePressed) {
      QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
      if (pMouse) {
      }
    }
  }
  return QWidget::eventFilter(obj, event);
}

void MainWindow::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        QRegExp messageRegex("^([^:]+):(.*)$");
        QRegExp usersRegex("^/users:(.*)$");
        if(usersRegex.indexIn(line) != -1)
        {
            QStringList users = usersRegex.cap(1).split(",");
            ui->userListWidget->clear();
            foreach(QString user, users)
                new QListWidgetItem(QPixmap(":/user.png"), user, ui->userListWidget);
        }
        else if(messageRegex.indexIn(line) != -1)
        {
            QString user = messageRegex.cap(1);
            QString message = messageRegex.cap(2);
            ui->roomTextEdit->append("<b>" + user + "</b>: " + message);
        }



    }
}

void MainWindow::connected()
{
    socket->write(QString("/me:" + usernaname + "\n").toUtf8());
}

void MainWindow::on_sayButton_clicked()
{
    QString message = ui->sayLineEdit->text().trimmed();
    if(!message.isEmpty())
    {
        socket->write(QString(message + "\n").toUtf8());
    }
    ui->sayLineEdit->clear();
    ui->sayLineEdit->setFocus();
}
