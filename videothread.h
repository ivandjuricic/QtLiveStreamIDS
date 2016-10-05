#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>

class VideoThread : public QThread
{
    Q_OBJECT
public:
    VideoThread();
    virtual ~VideoThread();


    void stopVideo();

protected:


    virtual void run();

signals:
    void frame(void *data);

};

#endif // VIDEOTHREAD_H
