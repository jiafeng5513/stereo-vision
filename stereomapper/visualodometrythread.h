#ifndef VISUALODOMETRYTHREAD_H
#define VISUALODOMETRYTHREAD_H

#include <QThread>

#include <list>
#include "calibiokitti.h"
#include "stereoimage.h"
#include "../libviso2/src/viso_stereo.h"
#include "../libviso2/src/timer.h"

class VisualOdometryThread : public QThread
{
    Q_OBJECT

public:

    VisualOdometryThread(CalibIOKITTI *calib,QObject *parent = 0);
    ~VisualOdometryThread();
    void pushBack(StereoImage::simage &s,bool record_raw_odometry_=false);
    std::vector<Matcher::p_match> getMatches() { return visualOdomStereo->getMatches(); }
    std::vector<bool> getInliers() { return inliers; }
    StereoImage::simage* getStereoImage() { return simg; }
    Matrix getHomographyTotal() { return H_total; }
    float getGain() { return gain; }
    void resetHomographyTotal() { H_total.eye(); }
    void pickedUp() { picked = true; }

    Matrix H_total;

protected:

    void run();

private:

    float timeDiff(timeval a,timeval b)
    {
        return ((float)(a.tv_sec -b.tv_sec )) +
               ((float)(a.tv_usec-b.tv_usec))*1e-6;
    }

    VisualOdometryStereo           *visualOdomStereo;
    StereoImage::simage            *simg;
    CalibIOKITTI                   *calib;
    //Matcher                      *matcher;

    //std::vector<Matcher::p_match>   matches;
    std::vector<bool>               inliers;
    timeval                         time_prev,time_curr;
    bool                            record_raw_odometry;
    float                           gain;
    bool                            picked;

signals:

    void newHomographyArrived();

public slots:

};

#endif // VISUALODOMETRYTHREAD_H
