#ifndef WINNERLABEL_H
#define WINNERLABEL_H

#include <qapplication.h>
#include <qlabel.h>
#include <qdesktopwidget.h>
#include <stdlib.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

class WinnerLabel
  : public QLabel
{
  Q_OBJECT

public:
  WinnerLabel(QString s="Hello World!", QWidget *parent = 0)
    :on_(true)
  {
    QDesktopWidget *mydesk = QApplication::desktop();
    int wPos = mydesk->width()/2 - 300;
    int hPos = mydesk->height()/2 - 240;
    message_ = "<span style=\"font-size:48pt; font-weight:600; color:#aa0000;\">";
    message_ += s;
    message_ += "</span>";
    setTextFormat(Qt::RichText);
    setText(message_);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setGeometry(wPos,hPos,600,480);
    show();

    boost::scoped_ptr< boost::thread > thread1_;
    thread1_.reset( new boost::thread( boost::bind(&WinnerLabel::musicThread,this) ) );

    timerId_ = startTimer(250);
  }

protected:
  virtual void timerEvent(QTimerEvent *event)
  {
    if(event->timerId() == timerId_)
    {
      if(on_)
        setText("");
      else
        setText(message_);
      on_=!on_;
      return;
    }
    QWidget::timerEvent(event);
  }

private:
  void musicThread()
  {
    system("vlc /home/suttonpd/Desktop/Imperial_March.ogg");
  }

  int timerId_;
  bool on_;
  QString message_;
};

#endif // WINNERLABEL_H
