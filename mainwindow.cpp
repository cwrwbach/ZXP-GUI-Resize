#include <QSettings>
#include <QByteArray>
#include <QDateTime>
#include <QDesktopServices>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QGroupBox>
#include <QKeySequence>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QResource>
#include <QString>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextStream>
#include <QtGlobal>
#include <QTimer>
#include <QVBoxLayout>
#include <QSvgWidget>
#include "mainwindow.h"

/* Qt Designer files */
#include "ui_mainwindow.h"

#include <complex.h>

#define MAX_FFT_SIZE 1024


MainWindow::MainWindow(const QString cfgfile, bool edit_conf, QWidget *parent) :
    QMainWindow(parent),
    configOk(true),
    mwui(new Ui::MainWindow),
    d_lnb_lo(0),
    d_hw_freq(0),
    d_fftAvg(0.25),
    d_have_audio(true)
{
    mwui->setupUi(this);

   
    /* Initialise default configuration directory */
    QByteArray xdg_dir = qgetenv("XDG_CONFIG_HOME");
    if (xdg_dir.isEmpty())
    {
        // Qt takes care of conversion to native separators
        m_cfg_dir = QString("%1/.config/gqrx").arg(QDir::homePath());
    }
    else
    {
        m_cfg_dir = QString("%1/gqrx").arg(xdg_dir.data());
    }

 //   setWindowTitle(QString("Gqrx %1").arg(VERSION));
//ok to start to here
    /* frequency control widget */
    mwui->freqCtrl->setup(0, 0, 9999e6, 1, FCTL_UNIT_NONE);
    mwui->freqCtrl->setFrequency(123456789);
 
    /* meter timer */
//    meter_timer = new QTimer(this);
//    connect(meter_timer, SIGNAL(timeout()), this, SLOT(meterTimeout()));

    /* FFT timer & data */
    iq_fft_timer = new QTimer(this);
    connect(iq_fft_timer, SIGNAL(timeout()), this, SLOT(iqFftTimeout()));

  iq_fft_timer->start(500); //FIXME 
  
  

//    d_fftData = new std::complex<float>[MAX_FFT_SIZE];
 //   d_realFftData = new float[MAX_FFT_SIZE];
//    d_iirFftData = new float[MAX_FFT_SIZE];
//    for (int i = 0; i < MAX_FFT_SIZE; i++)
  //      d_iirFftData[i] = -140.0;  // dBFS
  
  
//FIXME FINDME qqq just here for debug
//  iq_fft_timer->start(1000/uiDockFft->fftRate()); //FIXME 
  
  
#if 0


    /* connect signals and slots */
    connect(mwui->freqCtrl, SIGNAL(newFrequency(qint64)), this, SLOT(setNewFrequency(qint64)));
    connect(mwui->freqCtrl, SIGNAL(newFrequency(qint64)), remote, SLOT(setNewFrequency(qint64)));
    connect(mwui->freqCtrl, SIGNAL(newFrequency(qint64)), uiDockAudio, SLOT(setRxFrequency(qint64)));
    connect(mwui->freqCtrl, SIGNAL(newFrequency(qint64)), uiDockRxOpt, SLOT(setRxFreq(qint64)));
    connect(uiDockInputCtl, SIGNAL(lnbLoChanged(double)), this, SLOT(setLnbLo(double)));
    connect(uiDockInputCtl, SIGNAL(lnbLoChanged(double)), remote, SLOT(setLnbLo(double)));
    connect(uiDockInputCtl, SIGNAL(gainChanged(QString, double)), this, SLOT(setGain(QString,double)));
    connect(uiDockInputCtl, SIGNAL(gainChanged(QString, double)), remote, SLOT(setGain(QString,double)));
    connect(uiDockInputCtl, SIGNAL(autoGainChanged(bool)), this, SLOT(setAutoGain(bool)));
    connect(uiDockInputCtl, SIGNAL(freqCorrChanged(double)), this, SLOT(setFreqCorr(double)));
    connect(uiDockInputCtl, SIGNAL(iqSwapChanged(bool)), this, SLOT(setIqSwap(bool)));
    connect(uiDockInputCtl, SIGNAL(dcCancelChanged(bool)), this, SLOT(setDcCancel(bool)));
    connect(uiDockInputCtl, SIGNAL(iqBalanceChanged(bool)), this, SLOT(setIqBalance(bool)));
    connect(uiDockInputCtl, SIGNAL(ignoreLimitsChanged(bool)), this, SLOT(setIgnoreLimits(bool)));
    connect(uiDockInputCtl, SIGNAL(antennaSelected(QString)), this, SLOT(setAntenna(QString)));
    connect(uiDockInputCtl, SIGNAL(freqCtrlResetChanged(bool)), this, SLOT(setFreqCtrlReset(bool)));
    connect(uiDockRxOpt, SIGNAL(rxFreqChanged(qint64)), mwui->freqCtrl, SLOT(setFrequency(qint64)));
    connect(uiDockRxOpt, SIGNAL(filterOffsetChanged(qint64)), this, SLOT(setFilterOffset(qint64)));
    connect(uiDockRxOpt, SIGNAL(filterOffsetChanged(qint64)), remote, SLOT(setFilterOffset(qint64)));
    connect(uiDockRxOpt, SIGNAL(demodSelected(int)), this, SLOT(selectDemod(int)));
    connect(uiDockRxOpt, SIGNAL(demodSelected(int)), remote, SLOT(setMode(int)));
    connect(uiDockRxOpt, SIGNAL(fmMaxdevSelected(float)), this, SLOT(setFmMaxdev(float)));
    connect(uiDockRxOpt, SIGNAL(fmEmphSelected(double)), this, SLOT(setFmEmph(double)));
    connect(uiDockRxOpt, SIGNAL(amDcrToggled(bool)), this, SLOT(setAmDcr(bool)));
    connect(uiDockRxOpt, SIGNAL(cwOffsetChanged(int)), this, SLOT(setCwOffset(int)));
    connect(uiDockRxOpt, SIGNAL(agcToggled(bool)), this, SLOT(setAgcOn(bool)));
    connect(uiDockRxOpt, SIGNAL(agcHangToggled(bool)), this, SLOT(setAgcHang(bool)));
    connect(uiDockRxOpt, SIGNAL(agcThresholdChanged(int)), this, SLOT(setAgcThreshold(int)));
    connect(uiDockRxOpt, SIGNAL(agcSlopeChanged(int)), this, SLOT(setAgcSlope(int)));
    connect(uiDockRxOpt, SIGNAL(agcGainChanged(int)), this, SLOT(setAgcGain(int)));
    connect(uiDockRxOpt, SIGNAL(agcDecayChanged(int)), this, SLOT(setAgcDecay(int)));
    connect(uiDockRxOpt, SIGNAL(noiseBlankerChanged(int,bool,float)), this, SLOT(setNoiseBlanker(int,bool,float)));
    connect(uiDockRxOpt, SIGNAL(sqlLevelChanged(double)), this, SLOT(setSqlLevel(double)));
    connect(uiDockRxOpt, SIGNAL(sqlAutoClicked()), this, SLOT(setSqlLevelAuto()));
    connect(uiDockAudio, SIGNAL(audioGainChanged(float)), this, SLOT(setAudioGain(float)));
    connect(uiDockAudio, SIGNAL(audioStreamingStarted(QString,int,bool)), this, SLOT(startAudioStream(QString,int,bool)));
    connect(uiDockAudio, SIGNAL(audioStreamingStopped()), this, SLOT(stopAudioStreaming()));
    connect(uiDockAudio, SIGNAL(audioRecStarted(QString)), this, SLOT(startAudioRec(QString)));
    connect(uiDockAudio, SIGNAL(audioRecStarted(QString)), remote, SLOT(startAudioRecorder(QString)));
    connect(uiDockAudio, SIGNAL(audioRecStopped()), this, SLOT(stopAudioRec()));
    connect(uiDockAudio, SIGNAL(audioRecStopped()), remote, SLOT(stopAudioRecorder()));
    connect(uiDockAudio, SIGNAL(audioPlayStarted(QString)), this, SLOT(startAudioPlayback(QString)));
    connect(uiDockAudio, SIGNAL(audioPlayStopped()), this, SLOT(stopAudioPlayback()));
    connect(uiDockAudio, SIGNAL(fftRateChanged(int)), this, SLOT(setAudioFftRate(int)));
    connect(uiDockFft, SIGNAL(fftSizeChanged(int)), this, SLOT(setIqFftSize(int)));
    connect(uiDockFft, SIGNAL(fftRateChanged(int)), this, SLOT(setIqFftRate(int)));
    connect(uiDockFft, SIGNAL(fftWindowChanged(int)), this, SLOT(setIqFftWindow(int)));
    connect(uiDockFft, SIGNAL(wfSpanChanged(quint64)), this, SLOT(setWfTimeSpan(quint64)));
    connect(uiDockFft, SIGNAL(fftSplitChanged(int)), this, SLOT(setIqFftSplit(int)));
    connect(uiDockFft, SIGNAL(fftAvgChanged(float)), this, SLOT(setIqFftAvg(float)));
    connect(uiDockFft, SIGNAL(fftZoomChanged(float)), mwui->plotter, SLOT(zoomOnXAxis(float)));
    connect(uiDockFft, SIGNAL(resetFftZoom()), mwui->plotter, SLOT(resetHorizontalZoom()));
    connect(uiDockFft, SIGNAL(gotoFftCenter()), mwui->plotter, SLOT(moveToCenterFreq()));
    connect(uiDockFft, SIGNAL(gotoDemodFreq()), mwui->plotter, SLOT(moveToDemodFreq()));
    connect(uiDockFft, SIGNAL(wfColormapChanged(const QString)), mwui->plotter, SLOT(setWfColormap(const QString)));
    connect(uiDockFft, SIGNAL(wfColormapChanged(const QString)), uiDockAudio, SLOT(setWfColormap(const QString)));

    connect(uiDockFft, SIGNAL(pandapterRangeChanged(float,float)),
            mwui->plotter, SLOT(setPandapterRange(float,float)));
    connect(uiDockFft, SIGNAL(waterfallRangeChanged(float,float)),
            mwui->plotter, SLOT(setWaterfallRange(float,float)));
    connect(mwui->plotter, SIGNAL(pandapterRangeChanged(float,float)),
            uiDockFft, SLOT(setPandapterRange(float,float)));
    connect(mwui->plotter, SIGNAL(newZoomLevel(float)),
            uiDockFft, SLOT(setZoomLevel(float)));
    connect(mwui->plotter, SIGNAL(newSize()), this, SLOT(setWfSize()));

    connect(uiDockFft, SIGNAL(fftColorChanged(QColor)), this, SLOT(setFftColor(QColor)));
    connect(uiDockFft, SIGNAL(fftFillToggled(bool)), this, SLOT(setFftFill(bool)));
    connect(uiDockFft, SIGNAL(fftPeakHoldToggled(bool)), this, SLOT(setFftPeakHold(bool)));
    connect(uiDockFft, SIGNAL(peakDetectionToggled(bool)), this, SLOT(setPeakDetection(bool)));
    connect(uiDockRDS, SIGNAL(rdsDecoderToggled(bool)), this, SLOT(setRdsDecoder(bool)));

 
#endif 
}

//---

MainWindow::~MainWindow()
{
 //   on_actionDSP_triggered(false);
#if 0
    /* stop and delete timers */
    dec_timer->stop();
    delete dec_timer;

    meter_timer->stop();
    delete meter_timer;

    iq_fft_timer->stop();
    delete iq_fft_timer;

    audio_fft_timer->stop();
    delete audio_fft_timer;

    if (m_settings)
    {
        m_settings->setValue("configversion", 2);
        m_settings->setValue("crashed", false);

        // hide toolbar (default=false)
        if (mwui->mainToolBar->isHidden())
            m_settings->setValue("gui/hide_toolbar", true);
        else
            m_settings->remove("gui/hide_toolbar");

        m_settings->setValue("gui/geometry", saveGeometry());
        m_settings->setValue("gui/state", saveState());

        // save session
        storeSession();

        m_settings->sync();
        delete m_settings;
    }

    delete iq_tool;
    delete ui;
    delete uiDockRxOpt;
    delete uiDockAudio;
    delete uiDockBookmarks;
    delete uiDockFft;
    delete uiDockInputCtl;
    delete uiDockRDS;
    delete rx;
    delete remote;
    delete [] d_fftData;
    delete [] d_realFftData;
    delete [] d_iirFftData;
    delete qsvg_dummy;
    
#endif    
}


#if 0




/**
 * @brief Slot for receiving frequency change signals.
 * @param[in] freq The new frequency.
 *
 * This slot is connected to the CFreqCtrl::newFrequency() signal and is used
 * to set new receive frequency.
 */
void MainWindow::setNewFrequency(qint64 rx_freq)
{
    double hw_freq = (double)(rx_freq - d_lnb_lo) - rx->get_filter_offset();
    qint64 center_freq = rx_freq - (qint64)rx->get_filter_offset();

    d_hw_freq = (qint64)hw_freq;

    // set receiver frequency
    rx->set_rf_freq(hw_freq);

    // update widgets
    mwui->plotter->setCenterFreq(center_freq);
    uiDockRxOpt->setHwFreq(d_hw_freq);
    mwui->freqCtrl->setFrequency(rx_freq);
    uiDockBookmarks->setNewFrequency(rx_freq);
}


/** Reset lower digits of main frequency control widget */
void MainWindow::setFreqCtrlReset(bool enabled)
{
    mwui->freqCtrl->setResetLowerDigits(enabled);
}

/**
 * @brief Select new demodulator.
 * @param demod New demodulator.
 */
void MainWindow::selectDemod(QString strModulation)
{
    int iDemodIndex;

    iDemodIndex = DockRxOpt::GetEnumForModulationString(strModulation);
    qDebug() << "selectDemod(str):" << strModulation << "-> IDX:" << iDemodIndex;

    return selectDemod(iDemodIndex);
}


/**
 * @brief Audio gain changed.
 * @param value The new audio gain in dB.
 */
void MainWindow::setAudioGain(float value)
{
    rx->set_af_gain(value);
}

/
/** Signal strength meter timeout. */
void MainWindow::meterTimeout()
{
    float level;

    level = rx->get_signal_pwr(true);
    mwui->sMeter->setLevel(level);
    remote->setSignalLevel(level);
}



#endif

#define LOG2_10 3.321928094887362

/** Baseband FFT plot timeout. */
void MainWindow::iqFftTimeout()
{
    unsigned int    fftsize;
    unsigned int    i;
    float           pwr_scale;
    std::complex<float> pt;     /* a single FFT point used in calculations */



float temp[2048];
for(int i = 0; i< 2048 ;i++)
{
	temp[i] = (rand()/1e7)* -1;
	

}	
	fftsize = 2048;
  mwui->plotter->setNewFftData(temp, fftsize);


    // FIXME: fftsize is a reference
  //  rx->get_iq_fft_data(d_fftData, fftsize);


/*

    if (fftsize == 0)
    {
        // nothing to do, wait until next activation. 
        return;
    }

    // NB: without cast to float the multiplication will overflow at 64k
    // and pwr_scale will be inf
    pwr_scale = 1.0 / ((float)fftsize * (float)fftsize);

    // Normalize, calculate power and shift the FFT 
    volk_32fc_magnitude_squared_32f(d_realFftData, d_fftData + (fftsize/2), fftsize/2);
    volk_32fc_magnitude_squared_32f(d_realFftData + (fftsize/2), d_fftData, fftsize/2);
    volk_32f_s32f_multiply_32f(d_realFftData, d_realFftData, pwr_scale, fftsize);
    volk_32f_log2_32f(d_realFftData, d_realFftData, fftsize);
    volk_32f_s32f_multiply_32f(d_realFftData, d_realFftData, 10 / LOG2_10, fftsize);

    for (i = 0; i < fftsize; i++)
    {
        // FFT averaging 
        d_iirFftData[i] += d_fftAvg * (d_realFftData[i] - d_iirFftData[i]);
    }
*/
  //  mwui->plotter->setNewFftData(d_iirFftData, d_realFftData, fftsize);
}


#if 0
/** Audio FFT plot timeout. */
void MainWindow::audioFftTimeout()
{
    unsigned int    fftsize;
    unsigned int    i;
    float           pwr;
    float           pwr_scale;
    std::complex<float> pt;             /* a single FFT point used in calculations */

    if (!d_have_audio || !uiDockAudio->isVisible())
        return;

    rx->get_audio_fft_data(d_fftData, fftsize);

    if (fftsize == 0)
    {
        /* nothing to do, wait until next activation. */
        qDebug() << "No audio FFT data.";
        return;
    }

    pwr_scale = 1.0 / (fftsize * fftsize);

    /** FIXME: move post processing to rx_fft_f **/
    /* Normalize, calculcate power and shift the FFT */
    for (i = 0; i < fftsize; i++)
    {
        /* normalize and shift */
        if (i < fftsize/2)
        {
            pt = d_fftData[fftsize/2+i];
        }
        else
        {
            pt = d_fftData[i-fftsize/2];
        }

        /* calculate power in dBFS */
        pwr = pwr_scale * (pt.imag() * pt.imag() + pt.real() * pt.real());
        d_realFftData[i] = 10.0 * log10f(pwr + 1.0e-20);
    }

    uiDockAudio->setNewFftData(d_realFftData, fftsize);
}


void MainWindow::setIqFftWindow(int type)
{
    rx->set_iq_fft_window(type);
}

/** Waterfall time span has changed. */
void MainWindow::setWfTimeSpan(quint64 span_ms)
{
    // set new time span, then send back new resolution to be shown by GUI label
    mwui->plotter->setWaterfallSpan(span_ms);
    uiDockFft->setWfResolution(mwui->plotter->getWfTimeRes());
}

void MainWindow::setWfSize()
{
    uiDockFft->setWfResolution(mwui->plotter->getWfTimeRes());
}

/**
 * @brief Vertical split between waterfall and pandapter changed.
 * @param pct_pand The percentage of the waterfall.
 */
void MainWindow::setIqFftSplit(int pct_wf)
{
    if ((pct_wf >= 0) && (pct_wf <= 100))
        mwui->plotter->setPercent2DScreen(pct_wf);
}

void MainWindow::setIqFftAvg(float avg)
{
    if ((avg >= 0) && (avg <= 1.0))
        d_fftAvg = avg;
}



/** Set FFT plot color. */
void MainWindow::setFftColor(const QColor color)
{
    mwui->plotter->setFftPlotColor(color);
    uiDockAudio->setFftColor(color);
}

/** Enalbe/disable filling the aread below the FFT plot. */
void MainWindow::setFftFill(bool enable)
{
    mwui->plotter->setFftFill(enable);
    uiDockAudio->setFftFill(enable);
}

void MainWindow::setFftPeakHold(bool enable)
{
    mwui->plotter->setPeakHold(enable);
}

void MainWindow::setPeakDetection(bool enabled)
{
    mwui->plotter->setPeakDetection(enabled ,2);
}


/**
 * @brief Start/Stop DSP processing.
 * @param checked Flag indicating whether DSP processing should be ON or OFF.
 *
 * This slot is executed when the actionDSP is toggled by the user. This can
 * either be via the menu bar or the "power on" button in the main toolbar.
 */
void MainWindow::on_actionDSP_triggered(bool checked)
{
    remote->setReceiverStatus(checked);

    if (checked)
    {
        /* start receiver */
        rx->start();

        /* start GUI timers */
        meter_timer->start(100);

        if (uiDockFft->fftRate())
        {
            iq_fft_timer->start(1000/uiDockFft->fftRate());
            mwui->plotter->setRunningState(true);
        }
        else
        {
            iq_fft_timer->start(36e7); // 100 hours
            mwui->plotter->setRunningState(false);
        }

        audio_fft_timer->start(40);

        /* update menu text and button tooltip */
        mwui->actionDSP->setToolTip(tr("Stop DSP processing"));
        mwui->actionDSP->setText(tr("Stop DSP"));

        // reconfigure RX after 1s to counteract possible jerky streaming from rtl dongles
        //QTimer::singleShot(1000, this, SLOT(forceRxReconf()));
    }
    else
    {
        /* stop GUI timers */
        meter_timer->stop();
        iq_fft_timer->stop();
        audio_fft_timer->stop();
        rds_timer->stop();

        /* stop receiver */
        rx->stop();

        /* update menu text and button tooltip */
        mwui->actionDSP->setToolTip(tr("Start DSP processing"));
        mwui->actionDSP->setText(tr("Start DSP"));

        mwui->plotter->setRunningState(false);
    }
}

/


/* CPlotter::NewfilterFreq() is emitted or bookmark activated */
void MainWindow::on_plotter_newFilterFreq(int low, int high)
{
    receiver::status retcode;

    /* parameter correctness will be checked in receiver class */
    retcode = rx->set_filter((double) low, (double) high, d_filter_shape);

    /* Update filter range of plotter, in case this slot is triggered by
     * switching to a bookmark */
    mwui->plotter->setHiLowCutFrequencies(low, high);

    if (retcode == receiver::STATUS_OK)
        uiDockRxOpt->setFilterParam(low, high);
}

void MainWindow::on_plotter_newCenterFreq(qint64 f)
{
    rx->set_rf_freq(f);
    mwui->freqCtrl->setFrequency(f);
}




#define DATA_BUFFER_SIZE 48000




void MainWindow::setPassband(int bandwidth)
{
    /* Check if filter is symmetric or not by checking the presets */
    int mode = uiDockRxOpt->currentDemod();
    int preset = uiDockRxOpt->currentFilter();

    int lo, hi;
    uiDockRxOpt->getFilterPreset(mode, preset, &lo, &hi);

    if(lo + hi == 0)
    {
        lo = -bandwidth / 2;
        hi =  bandwidth / 2;
    }
    else if(lo >= 0 && hi >= 0)
    {
        hi = lo + bandwidth;
    }
    else if(lo <= 0 && hi <= 0)
    {
        lo = hi - bandwidth;
    }

    remote->setPassband(lo, hi);

    on_plotter_newFilterFreq(lo, hi);
}





#endif
