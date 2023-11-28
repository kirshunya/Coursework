#include "audioplayer.h"
#include "ui_audioplayer.h"

AudioPlayer::AudioPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AudioPlayer)
{
    ui->setupUi(this); // Используйте ui для настройки интерфейса

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);

    player->setPlaylist(playlist);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setVolume(ui->volumeSlider->value());

    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->durationSlider->setRange(0, 0);
    ui->volumeSlider->setMinimum(0);
    ui->volumeSlider->setMaximum(100);
    ui->volumeSlider->setValue(30);

    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));


    connect(player, &QMediaPlayer::durationChanged, this, &AudioPlayer::onDurationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &AudioPlayer::onPositionChanged);
    connect(ui->durationSlider, &QSlider::sliderMoved, this, &AudioPlayer::onSliderMoved);
    connect(ui->openButton, &QPushButton::clicked, this, &AudioPlayer::openFile);
    connect(ui->nextButton, &QPushButton::clicked, this, &AudioPlayer::next);
    connect(ui->prevButton, &QPushButton::clicked, this, &AudioPlayer::prev);
    connect(ui->clearButton, &QPushButton::clicked, this, &AudioPlayer::clear_playlist);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &AudioPlayer::onItemDoubleClicked);
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &AudioPlayer::onCurrentIndexChanged);




}

AudioPlayer::~AudioPlayer()
{
    delete ui;
    delete player;
    delete playlist;
}

void AudioPlayer::onDurationChanged(qint64 duration)
{
    ui->durationSlider->setMaximum(duration);
    QTime totalTime = QTime(0, 0).addMSecs(duration);
    ui->timeLabel2->setText(totalTime.toString("mm:ss"));
    QMediaContent currentMedia = playlist->media(playlist->currentIndex());
    QUrl currentUrl = currentMedia.canonicalUrl();
    QString fileName = currentUrl.fileName();
    ui->nameLabel->setText(fileName);

}

void AudioPlayer::onPositionChanged(qint64 position)
{
    ui->durationSlider->setValue(position);
    ui->timeLabel1->setText(QTime(0, position / 60000, position / 1000 % 60, position % 1000).toString("mm:ss"));

}

void AudioPlayer::onSliderMoved(int position)
{
    player->setPosition(position);

}

void AudioPlayer::openFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select audio files", "/path/to/default", "Audio files (*.mp3 *.wav *.ogg)");

    foreach (QString file, files)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(file);
        ui->listWidget->addItem(item);
        QMediaContent content(QUrl::fromLocalFile(file));
        item->setText(QString("Название: %1\nИсполнители: %2\nАльбом: %3")
                          .arg(content.canonicalUrl().fileName())
                          .arg(player->metaData("Artist").toString())
                          .arg(player->metaData("Album").toString()));

        playlist->addMedia(QUrl::fromLocalFile(file));
    }
}

void AudioPlayer::clear_playlist()
{
    playlist->clear();
    ui->listWidget->clear();
    player->stop();
    ui->nameLabel->setText("");
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    IS_Play = false;
    player->pause();

}


void AudioPlayer::on_actionFullScreen_Mode_triggered()
{
    showMaximized();
}


void AudioPlayer::on_actionNormalScreen_triggered()
{
    showNormal();
}


void AudioPlayer::on_playButton_clicked()
{
    if(IS_Play == false)
    {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        IS_Play = true;
        player->play();
    }
    else
    {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        IS_Play = false;
        player->pause();
    }
}


void AudioPlayer::on_volumeSlider_valueChanged(int value)
{
    player->setVolume(value);
}

void AudioPlayer::onItemDoubleClicked(QListWidgetItem *item) {
    playlist->setCurrentIndex(ui->listWidget->row(item));
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    IS_Play = true;
    player->play();
}

void AudioPlayer::onCurrentIndexChanged(int index) {
    ui->listWidget->setCurrentRow(index);
}

void AudioPlayer::next()
{
    playlist->next();
}

void AudioPlayer::prev()
{
    playlist->previous();
}

void AudioPlayer::on_muteButton_clicked()
{
    if(IS_Muted == false)
    {
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        IS_Muted = true;
        player->setMuted(true);
    }
    else
    {
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        IS_Muted = false;
        player->setMuted(false);
    }
}

void AudioPlayer::on_actionAdd_background_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "/path/to/default", "Images (*.png *.jpg *.bmp)");

    if (!fileName.isEmpty()) {
        this->setStyleSheet(QString("background-image: url(%1);").arg(fileName));
    }
}


void AudioPlayer::on_actionLoop_triggered()
{
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    ui->playBackLabel->setText("PlaBackMode: Loop");
}


void AudioPlayer::on_actionRandom_triggered()
{
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    ui->playBackLabel->setText("PlaBackMode: Random");
}

