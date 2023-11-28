#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMainWindow>
#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QUrl>
#include <QTime>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QMediaMetaData>
#include <QPalette>


QT_BEGIN_NAMESPACE
namespace Ui { class AudioPlayer; }
QT_END_NAMESPACE

class AudioPlayer : public QMainWindow
{
    Q_OBJECT

public:
    AudioPlayer(QWidget *parent = nullptr);
    ~AudioPlayer();

private slots:
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onSliderMoved(int position);
    void openFile();
    void clear_playlist();
    void on_actionFullScreen_Mode_triggered();
    void onItemDoubleClicked(QListWidgetItem *item);
    void onCurrentIndexChanged(int index);
    void on_actionNormalScreen_triggered();
    void on_playButton_clicked();
    void on_volumeSlider_valueChanged(int value);
    void next();
    void prev();
    void on_muteButton_clicked();
    void on_actionAdd_background_triggered();

    void on_actionLoop_triggered();

    void on_actionRandom_triggered();

private:
    Ui::AudioPlayer *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    bool IS_Muted = false;
    bool IS_Play = false;
};

#endif // AUDIOPLAYER_H
